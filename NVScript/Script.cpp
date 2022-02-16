
#include <map>
#include <list>
#include <algorithm>
#include <cstring>

#include "lg/scrmanagers.h"
#include "lg/scrservices.h"

#include "Script.h"
#include "ScriptModule.h"
#include "ScriptLib.h"

using namespace std;

extern cScriptModule g_ScriptModule;
extern int (__cdecl *g_pfnMPrintf)(const char*, ...);

cScript::~cScript()
{
#ifdef SCRIPTMODULE2
	g_ScriptModule.DeleteScriptInstance(m_szName, m_iObjId, this);
#endif
	if (m_szName)
		delete[] m_szName;
}

cScript::cScript(const char* pszName, int iHostObjId)
	    : m_iRef(0), m_iObjId(iHostObjId), m_bSim(false)
{
	m_szName = new(nothrow) char[strlen(pszName)+1];
	if (m_szName)
		strcpy(m_szName, pszName);
	//InitScriptData();
}

HRESULT __stdcall cScript::QueryInterface(REFIID riid, void** ppout)
{
	if (riid == IID_IUnknown)
		*ppout = this;
	else if (riid == IID_IScript)
		*ppout = static_cast<IScript*>(this);
	else
		return E_NOINTERFACE;
	static_cast<IUnknown*>(*ppout)->AddRef();
	return S_OK;
}

ULONG __stdcall cScript::AddRef(void)
{
	return InterlockedIncrement(&m_iRef);
}

ULONG __stdcall cScript::Release(void)
{
	ULONG uRefCnt = InterlockedDecrement(&m_iRef);
	if (uRefCnt == 0)
		delete this;
	return uRefCnt;
}

const char* __stdcall cScript::GetClassName(void)
{
	// Name MUST match the one in the list.
	// Still, we don't want to toss a NULL pointer around, do we?
	return (m_szName) ? m_szName : "cScript";
}

void cScript::InitScriptData()
{
	RegisterMessageHandler("ScriptPtrQuery", static_cast<MessageHandlerFunc>(&cScript::ScriptPtrQuery));
	SInterface<ISimManager> pSim(g_pScriptManager);
	m_bSim = pSim->LastMsg() & (kSimStart|kSimResume);
}

void cScript::DebugString(const char* pszMsg1, const char* pszMsg2)
{
	if (g_pfnMPrintf)
		g_pfnMPrintf("%s #%d: %s%s\n", m_szName, m_iObjId, pszMsg1, pszMsg2);
#ifdef DEBUG
	IDarkUISrv* pDUI = static_cast<IDarkUISrv*>(g_pScriptManager->GetService(IID_IDarkUIScriptService));
	if (!pDUI)
		return;
	int iTextLen = strlen(pszMsg1) + 1;
	char* pszText;
	if (pszMsg2)
	{
		iTextLen += strlen(pszMsg2) + 1;
		pszText = reinterpret_cast<char*>(g_pMalloc->Alloc(iTextLen));
		sprintf(pszText, "%s %s", pszMsg1, pszMsg2);
	}
	else
	{
		pszText = reinterpret_cast<char*>(g_pMalloc->Alloc(iTextLen));
		strcpy(pszText, pszMsg1);
	}
	pDUI->TextMessage(pszText, RGB(255,255,255), 3000);
	pDUI->Release();
	g_pMalloc->Free(reinterpret_cast<void*>(pszText));
#endif
	return;
}

bool cScript::RegisterMessageHandler(const char* pszMsgName, MessageHandlerFunc pmfMsgHandler)
{
	try
	{
		const char* psz = (!pszMsgName) ? "" : pszMsgName;
		list<MessageHandlerFunc>& vHandlerList = m_mapHandlers[psz];
		list<MessageHandlerFunc>::const_iterator nSearch = 
			find(vHandlerList.begin(), vHandlerList.end(), pmfMsgHandler);
		if (nSearch == vHandlerList.end())
		{
			vHandlerList.insert(vHandlerList.begin(), pmfMsgHandler);
			return true;
		}
		DebugString("Message handler already registered: ", pszMsgName);
		return true;
	}
	catch (exception& err)
	{
		DebugString("Failed to register message: ", pszMsgName);
	}
	return false;
}

bool cScript::UnregisterMessageHandler(const char* pszMsgName, MessageHandlerFunc pmfMsgHandler)
{
	try
	{
		const char* psz = (!pszMsgName) ? "" : pszMsgName;
		tDispatchMap::iterator pNode = m_mapHandlers.find(psz);
		if (pNode != m_mapHandlers.end())
		{
			list<MessageHandlerFunc>& vHandlerList = pNode->second;
			list<MessageHandlerFunc>::iterator nSearch = 
				find(vHandlerList.begin(), vHandlerList.end(), pmfMsgHandler);
			if (nSearch != vHandlerList.end())
			{
				vHandlerList.erase(nSearch);
				if (vHandlerList.empty())
				{
					m_mapHandlers.erase(pNode);
				}
			}
		}
		return true;
	}
	catch (exception& err)
	{
		DebugString("Failed to unregister message: ", pszMsgName);
	}
	return false;
}

long __stdcall cScript::ReceiveMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	/*
	 * This is the central hub for messages to our scripts.
	 * Derived scripts should, preferrably, not override this.
	 * Instead, register the messages you want to respond to in
	 * a hash with a pointer to member function (PMF). Then this
	 * function will call your function (in reverse order of 
	 * registration) when it receives a message.
	 *
	 * The idea is that this function catches any exceptions so 
	 * they don't fall back into the host application. (Which, 
	 * if you've been paying attention, would be a Bad Thing (tm).)
	 *
	 * Of course, you can override this if you want to; perhaps 
	 * if you want to respond to all messages. But be sure to
	 *   a) catch any exceptions before you return, and
	 *   b) call the parent ReceiveMessage.
	 */
	long iRet = 1;
	try
	{
		iRet = DispatchMessage(pMsg, pReply, eTrace);
		if (!stricmp(pMsg->message, "Sim"))
		{
			m_bSim = static_cast<sSimMsg*>(pMsg)->fStarting;
			iRet = 0;
		} else if (!m_bSim && !stricmp(pMsg->message, "BeginScript"))
		{
			m_bSim = true;
		}
	}
	catch (exception& err)
	{
		DebugString("Exception caught: \n", err.what());
	}
	return iRet;
}

long cScript::DispatchMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	long iRet = 1;
	tDispatchMap::const_iterator pNode = m_mapHandlers.find(pMsg->message);
	if (pNode != m_mapHandlers.end())
	{
		list<MessageHandlerFunc>::const_iterator vHandler, vEnd;
		for (vHandler = pNode->second.begin(), vEnd = pNode->second.end(); 
			 vHandler != vEnd; ++vHandler)
		{
			if (!(iRet = (this->*(*vHandler))(pMsg, pReply, eTrace)))
				break;
		}
	}
	if (iRet)
	{
		pNode = m_mapHandlers.find("");
		if (pNode != m_mapHandlers.end())
		{
			list<MessageHandlerFunc>::const_iterator vHandler, vEnd;
			for (vHandler = pNode->second.begin(), vEnd = pNode->second.end(); 
				 vHandler != vEnd; ++vHandler)
			{
				if (!(iRet = (this->*(*vHandler))(pMsg, pReply, eTrace)))
					break;
			}
		}
	}
	return iRet;
}

MSGHANDLER cScript::ScriptPtrQuery(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPtrQueryMsg* pPtrQ = static_cast<sPtrQueryMsg*>(pMsg);
	// Check class name 
	if (!stricmp(pPtrQ->pszDestClass, GetClassName()))
	{
		*(pPtrQ->pScriptReceptacle) = GetThisPointer();
		return 0;
	}
	return 1;
}


