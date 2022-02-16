
#include <cstring>
#include <new>

#include <algorithm>
#include <functional>

#include <windows.h>
#include <objbase.h>

#ifdef _DEBUG
#include <assert.h>
#endif

#include "lg/malloc.h"
#include "lg/scrmanagers.h"
#include "lg/scrservices.h"

#include "ScriptLib.h"
#include "ScriptModule.h"
#include "Script.h"

typedef int (__cdecl *MPrintfProc)(const char*, ...);
extern MPrintfProc g_pfnMPrintf;

using namespace std;


//


static sNewClassDesc *sm_ScriptsArray = NULL;
static unsigned int sm_ScriptsArraySize = 0;


//

#define MAX_SCRIPT_CLASSES 1024

typedef void (INITSCRIPTDESC)(sNewClassDesc*);

static int g_iRegisteredScriptCount = 0;
static INITSCRIPTDESC *g_ppInitScriptDescFuncs[MAX_SCRIPT_CLASSES];


void RegisterScriptClass(INITSCRIPTDESC *pInitScriptDesc)
{
	if (g_iRegisteredScriptCount+1 > MAX_SCRIPT_CLASSES)
	{
		// MAX_SCRIPT_CLASSES should be large enough for this never to happen
#ifdef _DEBUG
		assert(0);
#endif
		return;
	}

	g_ppInitScriptDescFuncs[g_iRegisteredScriptCount] = pInitScriptDesc;

	g_iRegisteredScriptCount++;
}


void cScriptModule::InitScriptDescs()
{
	DestroyScriptDescs();

	sm_ScriptsArray = new sNewClassDesc[g_iRegisteredScriptCount];
	sm_ScriptsArraySize = g_iRegisteredScriptCount;

	for (int i=0; i<g_iRegisteredScriptCount; i++)
	{
		(*g_ppInitScriptDescFuncs[i])(sm_ScriptsArray+i);

		sm_ScriptsArray[i].pszModule = g_ScriptModule.GetName();
	}
}

void cScriptModule::DestroyScriptDescs()
{
	if (sm_ScriptsArray)
	{
		delete[] sm_ScriptsArray;
		sm_ScriptsArray = NULL;

		sm_ScriptsArraySize = 0;
	}
}


//


cScriptModule::~cScriptModule()
{
	if (m_pszName)
		delete[] m_pszName;
}

cScriptModule::cScriptModule()
	    : m_iRef(0), m_pszName(NULL)
{
}

cScriptModule::cScriptModule(const char* pszName)
	    : m_iRef(0), m_pszName(NULL)
{
	SetName(pszName);
}

HRESULT __stdcall cScriptModule::QueryInterface(REFIID riid, void** ppout)
{
	try
	{
		if (riid == IID_IUnknown)
			*ppout = this;
		else if (riid == IID_IScriptModule)
			*ppout = static_cast<IScriptModule*>(this);
		else
			return E_NOINTERFACE;
		reinterpret_cast<IUnknown*>(*ppout)->AddRef();
		return S_OK;
	}
	catch (...)
	{
		return E_FAIL;
	}
}

ULONG __stdcall cScriptModule::AddRef(void)
{
	if (m_iRef == 0)
	{
#ifndef NO_PARAMCACHE
		InitParamCache();
#endif
	}
	return InterlockedIncrement(&m_iRef);
}

ULONG __stdcall cScriptModule::Release(void)
{
	// global COM objects?  First time I've ever heard of THAT
	ULONG uRefCnt = InterlockedDecrement(&m_iRef);
	if (uRefCnt == 0)
	{
#ifndef NO_PARAMCACHE
			ReleaseParamCache();
#endif
			m_InstanceList.clear();
			DestroyScriptDescs();
	}
	return uRefCnt;
}
	
void cScriptModule::SetName(const char* pszName)
{
	if (m_pszName)
	{
		delete[] m_pszName;
		m_pszName = NULL;
	}
	if (pszName)
	{
		char* psz = new(nothrow) char[::strlen(pszName)+1];
		if (psz)
		{
			::strcpy(psz, pszName);
			m_pszName = psz;
		}
	}
	for (unsigned int n = 0; n < sm_ScriptsArraySize; ++n)
	{
		sm_ScriptsArray[n].pszModule = m_pszName;
	}
}

const char* __stdcall cScriptModule::GetName(void)
{
	return m_pszName;
}

const sScrClassDesc* __stdcall cScriptModule::GetFirstClass(tScrIter* pIterParam)
{
	*reinterpret_cast<unsigned int*>(pIterParam) = 0;
	return GetScript(0);
}

const sScrClassDesc* __stdcall cScriptModule::GetNextClass(tScrIter* pIterParam) 
{
	unsigned int iCurIndex = *reinterpret_cast<unsigned int*>(pIterParam) + 1;
	*reinterpret_cast<unsigned int*>(pIterParam) = iCurIndex;
	return GetScript(iCurIndex);
}

void __stdcall cScriptModule::EndClassIter(tScrIter*)
{
	// Nothing to do.
}

const sScrClassDesc* cScriptModule::GetScript(unsigned int i)
{
	if (i < sm_ScriptsArraySize)
	{
		return sm_ScriptsArray + i;
	}
	return NULL;
}

struct ScrClassDesc_IsClass : public binary_function<const char*,sScrClassDesc,bool>
{
	bool operator() (const char* __a, const sScrClassDesc& __b) const
	{
		return ::stricmp(__a, __b.pszClass) == 0;
	}
};

IScript* __cdecl cScriptModule::ScriptFactory(const char* pszName, int iHostObjId)
{
	IScript* pRet = NULL;
	ScrInstance inst(pszName, iHostObjId);
	InstanceList::const_iterator i = m_InstanceList.find(inst);
	if (i != m_InstanceList.end())
	{
		pRet = i->second;
		pRet->AddRef();
		return pRet;
	}

	const sNewClassDesc* ScriptsArrayStart = sm_ScriptsArray;
	const sNewClassDesc* ScriptsArrayEnd = sm_ScriptsArray + sm_ScriptsArraySize;
	const sNewClassDesc* pDesc = find_if(ScriptsArrayStart, ScriptsArrayEnd, 
				bind1st(ScrClassDesc_IsClass(), pszName) );
	if (pDesc == ScriptsArrayEnd)
		return NULL;

	pRet = pDesc->pfnRealFactory(pszName, iHostObjId);
	if (pRet)
	{
		m_InstanceList.insert(make_pair(inst,pRet));
	}
	return pRet;
}

void cScriptModule::DeleteScriptInstance(const char* pszName, int iHostObjId, IScript* pScript)
{
	ScrInstance inst(pszName, iHostObjId);
	InstanceList::iterator i = m_InstanceList.find(inst);
	if (i != m_InstanceList.end() && i->second == pScript)
	{
		m_InstanceList.erase(i);
	}
}

cScriptModule::InstanceList cScriptModule::m_InstanceList;
cScriptModule  g_ScriptModule;
MPrintfProc    g_pfnMPrintf = NULL;

extern "C" __declspec(dllexport)
int __stdcall ScriptModuleInit (char* pszName, IScriptMan* pScriptMan, MPrintfProc pfnMPrintf, IMalloc* pMalloc, IScriptModule** pOutInterface)
{
	*pOutInterface = NULL;

	// Do we have any guarantee that these pointers will always be valid?
	// Well, I guess that scripts work at all says we can assume a little bit.
	g_pScriptManager = pScriptMan;
#ifdef DEBUG
	pMalloc->QueryInterface(IID_IDebugMalloc, reinterpret_cast<void**>(&g_pMalloc));
	if (!g_pMalloc)
		g_pMalloc = pMalloc;
#else
	g_pMalloc = pMalloc;
#endif

	g_pfnMPrintf = pfnMPrintf;

	// Just in case, let's bail if we don't have what we want.
	if (!g_pScriptManager || !g_pMalloc)
		return 0;

	// If NewDark then the IVersionSrv is available that can be used to determine game type
	// and make sure the OSM supports it.
	IVersionSrv* pVS = static_cast<IVersionSrv*>(g_pScriptManager->GetService(IID_IVersionScriptService));
	IUnknown* pTemp = NULL;
	if (pVS && E_NOINTERFACE == pVS->QueryInterface(IID_INullScriptService, reinterpret_cast<void**>(&pTemp)))
	{
		SService<IVersionSrv> vs(pVS);
		pVS->Release();

		cVarScrStr sGame;
		pVS->GetGame(sGame);

#if (_DARKGAME == 3)
		if ( stricmp(sGame, "shock") )
		{
			// Not SS2
			SService<IDebugScrSrv> pDSS(g_pScriptManager);
			cScrStr nil;
			pDSS->Log("OSM \"", pszName, "\" is not made for Thief\n", nil, nil, nil, nil, nil);
			pDSS->MPrint("OSM \"", pszName, "\" is not made for Thief", nil, nil, nil, nil, nil);
			return 0;
		}
#endif
#if (_DARKGAME <= 2)
		if ( stricmp(sGame, "dark") )
		{
			// Not T1/2
			SService<IDebugScrSrv> pDSS(g_pScriptManager);
			cScrStr nil;
			pDSS->Log("OSM \"", pszName, "\" is not made for Shock\n", nil, nil, nil, nil, nil);
			pDSS->MPrint("OSM \"", pszName, "\" is not made for Shock", nil, nil, nil, nil, nil);
			return 0;
		}

		// Is NewDark in Thief1 or Thief2 mode?
		SService<IEngineSrv> pES(g_pScriptManager);
#if (_DARKGAME == 1)
		if ( !pES->ConfigIsDefined("dark1") )
		{
			// Not T1
			SService<IDebugScrSrv> pDSS(g_pScriptManager);
			cScrStr nil;
			pDSS->Log("OSM \"", pszName, "\" is not made for Thief2\n", nil, nil, nil, nil, nil);
			pDSS->MPrint("OSM \"", pszName, "\" is not made for Thief2", nil, nil, nil, nil, nil);
			return 0;
		}
#else
		if ( pES->ConfigIsDefined("dark1") )
		{
			// Not T2
			SService<IDebugScrSrv> pDSS(g_pScriptManager);
			cScrStr nil;
			pDSS->Log("OSM \"", pszName, "\" is not made for Thief1\n", nil, nil, nil, nil, nil);
			pDSS->MPrint("OSM \"", pszName, "\" is not made for Thief1", nil, nil, nil, nil, nil);
			return 0;
		}
#endif
#endif
	}
	else if (pTemp)
	{
		pTemp->Release();
	}

	//g_pScriptModule = new cScriptModule(pszName);
	g_ScriptModule.SetName(pszName);
	g_ScriptModule.InitScriptDescs();
	g_ScriptModule.QueryInterface(IID_IScriptModule, reinterpret_cast<void**>(pOutInterface));

	return 1;
}

#if 0
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}
#endif

