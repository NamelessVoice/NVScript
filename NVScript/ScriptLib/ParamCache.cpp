#include "ScriptLib.h"
#include "ParamCache.h"
#include <lg/objects.h>

#include <cstring>
#include <cctype>

using namespace std;

extern const char* g_pszParamDelim;
extern const char* g_pszQuotes;

cParamCache::cParamCache(IScriptMan *pInterface)
{
	if (E_NOINTERFACE == pInterface->QueryInterface(IID_ITraitManager, reinterpret_cast<void**>(&m_pTraitMan)))
		throw no_interface("ITraitManager");

	sDispatchListenerDesc desc;
	desc.pIID_Listener = &sm_iid;
	desc.dwFlags = kSimStop;
	desc.pfnListener = SimListener;
	desc.pData = reinterpret_cast<void*>(this);
	SInterface<ISimManager> pSM(pInterface);
	pSM->Listen(&desc);

	SInterface<IPropertyManager> pPM(pInterface);
	IProperty* pProp = pPM->GetPropertyNamed(g_pszDesignNote);
	if (!pProp)
		throw null_pointer();
	if (E_NOINTERFACE == pProp->QueryInterface(IID_IStringProperty, reinterpret_cast<void**>(&m_pDNProp)))
		throw no_interface("IStringProperty");
	pProp->Release();
	m_hListenerHandle = m_pDNProp->Listen(kPropertyChange|kPropertyDelete, PropertyListener, reinterpret_cast<PropListenerData>(this));
	m_iUpdatingObj = 0;
#ifdef DEBUG_PARAMCACHE	
	m_Printf = NULL;
#endif
}

cParamCache::~cParamCache()
{
	ISimManager* pSM;
	m_pTraitMan->QueryInterface(IID_ISimManager, reinterpret_cast<void**>(&pSM));
	pSM->Unlisten(&sm_iid);
	pSM->Release();
	m_pDNProp->Unlisten(m_hListenerHandle);
	m_pDNProp->Release();
	m_pTraitMan->Release();
}

const GUID cParamCache::sm_iid = {0x74ea0cd2,0x2ad1,0x42c0,{0xba,0x29,0x4c,0x54,0xbf,0x35,0x63,0x25}};

int __cdecl cParamCache::SimListener(const sDispatchMsg* pSimMsg, const sDispatchListenerDesc* pData)
{
	if (pSimMsg->dwEventId == kSimStop)
	{
		reinterpret_cast<cParamCache*>(pData->pData)->Reset();
	}
	return 0;
}

void __stdcall cParamCache::PropertyListener(sPropertyListenMsg* pPropMsg, PropListenerData pData)
{
	if (!(pPropMsg->event & 8))
	{
		reinterpret_cast<cParamCache*>(pData)->Touch(pPropMsg->iObjId);
	}
}

string cParamCache::Unparse(tParamEntryMap& cache)
{
	string sDN;
	size_t len = 0;
	for (tParamEntryMap::const_iterator param = cache.begin();
	     param != cache.end(); ++param)
	{
		// first="second" + extra if escapes are needed
		len += param->first.size() + param->second.size() + 5;
	}
	sDN.reserve(len);
	for (tParamEntryMap::const_iterator param = cache.begin();
	     param != cache.end(); ++param)
	{
		const string& value = param->second;
		sDN.append(param->first);
		sDN.append(1, '=');
		if (value.size() == 1 && value.at(0) == '\0')
		{
			// out-of-bound data trick
			sDN.append(1, '!');
		}
		else if (value.size() == 0)
		{
			sDN.append(2, '"');
		}
		else
		{
			if (string::npos == value.find_first_of(";\"'\\ !"))
			{
				sDN.append(value);
			}
			else
			{
				if (string::npos == value.find('"'))
				{
					sDN.append(1, '"');
					size_t start = 0;
					size_t pos;
					while (string::npos != (pos = value.find_first_of("\"\\", start)))
					{
						sDN.append(value, start, start-pos);
						sDN.append(1, '\\');
						sDN.append(value, pos, 1);
						start = pos + 1;
					}
					sDN.append(value, start, string::npos);
					sDN.append(1, '"');
				}
				else
				{
					sDN.append(1, '\'');
					size_t start = 0;
					size_t pos;
					while (string::npos != (pos = value.find_first_of("\'\\", start)))
					{
						sDN.append(value, start, start-pos);
						sDN.append(1, '\\');
						sDN.append(value, pos, 1);
						start = pos + 1;
					}
					sDN.append(value, start, string::npos);
					sDN.append(1, '\'');
				}
			}
		}
		sDN.append(1, ';');
	}
	return sDN;
}

void cParamCache::Parse(tParamEntryMap& cache, const char* pszDN)
{
	if (!pszDN)
		return;

	//char* pszString = new char[strlen(pszDN) + 1];
#ifndef _MSC_VER
	char szString[strlen(pszDN) + 1];
#else
	char *szString = (char*)alloca(strlen(pszDN) + 1);
#endif
	strcpy(szString, pszDN);
	char* pszSep, *pszToken;
	for (pszSep = szString, pszToken = strqsep(&pszSep, g_pszParamDelim, g_pszQuotes); 
	     pszToken; pszToken = strqsep(&pszSep, g_pszParamDelim, g_pszQuotes))
	{
		while (isspace(*pszToken)) ++pszToken;
		if (*pszToken == '\0' || *pszToken == '=')
			continue;
		char* pszStart = strchr(pszToken, '=');
		if (pszStart)
		{
			*pszStart++ = '\0';
			if (*pszStart == '!')
			{
				// out-of-bound data trick
				cache.insert(make_pair(string(pszToken),string(1,0)));
			}
			else
			{
				while (isspace(*pszStart)) ++pszStart;
				char* pszEnd;
				if (*pszStart == '\"' || *pszStart == '\'')
				{
					char* pszChar = pszStart + 1;
					pszEnd = pszChar;
					while (*pszChar)
					{
						if (*pszChar == '\\')
						{
							if (*(pszChar+1) == *pszStart
							 || *(pszChar+1) == '\\')
							{
								++pszChar;
							}
						}
						else if (*pszChar == *pszStart)
							break;
						*pszEnd++ = *pszChar++;
					}
					*pszEnd = '\0';
					++pszStart;
				}
				else
					pszEnd = pszStart + strlen(pszStart);
				/*
				char* pszEnd = pszStart + strlen(pszStart);
				if (*pszStart == '\"' || *pszStart == '\'')
				{
					if (*(pszEnd-1) == *pszStart)
						*(--pszEnd) = '\0';
					++pszStart;
				}
				*/
				cache.insert(make_pair(string(pszToken),string(pszStart,pszEnd)));
			}
		}
		else
		{
			cache.insert(make_pair(string(pszToken),string()));
		}
	}
	//delete[] pszString;
}

cParamCache::tParamEntryMap* cParamCache::Read(int iObj)
{
	if (m_pDNProp->IsSimplyRelevant(iObj))
	{
		const char* pszDesignNote;
		m_pDNProp->GetSimple(iObj, &pszDesignNote);

		tParamEntryMap& entry = m_mapParamCache[iObj];
		Parse(entry, pszDesignNote);
#ifdef DEBUG_PARAMCACHE
		m_mapParamDebug[iObj].read++;
#endif
		return &entry;
	}
	return NULL;
}

void cParamCache::Write(int iObj)
{
	m_iUpdatingObj = iObj;
	try
	{
		tParamCacheMap::iterator entry = m_mapParamCache.find(iObj);
		if (entry != m_mapParamCache.end())
		{
			string sDN = Unparse(entry->second);
			m_pDNProp->Set(iObj, sDN.c_str());
		}
	}
	catch (exception& e)
	{
		m_iUpdatingObj = 0;
		throw e;
	}
	m_iUpdatingObj = 0;
}

const string* cParamCache::Retrieve(int iObj, const string& sParamName)
{
	const string* psParam = NULL;
	{
		tParamCacheMap::const_iterator cached_entry = m_mapParamCache.find(iObj);
		if (cached_entry != m_mapParamCache.end())
		{
			tParamEntryMap::const_iterator param_entry = cached_entry->second.find(sParamName);
			if (param_entry != cached_entry->second.end())
			{
				psParam = &(param_entry->second);
			}
		}
		else
		{
			tParamEntryMap* entry = Read(iObj);
			if (entry)
			{
#ifdef DEBUG_PARAMCACHE
			m_mapParamDebug[iObj].counter[sParamName].reparse++;
#endif
				tParamEntryMap::const_iterator param_entry = entry->find(sParamName);
				if (param_entry != entry->end())
				{
					psParam = &(param_entry->second);
				}
			}
		}
	}
#ifdef DEBUG_PARAMCACHE
	if (psParam)
		m_mapParamDebug[iObj].counter[sParamName].hit++;
	else
		m_mapParamDebug[iObj].counter[sParamName].miss++;
#endif

	if (!psParam)
	{
		// We could ask the TraitManager to query the entire tree, but
		// to get the block-inheritance feature to work, we have to recurse
		// at each level... slightly more time consuming, but them's the breaks
		IObjectQuery* pInheritance = m_pTraitMan->Query(iObj, kTraitQueryMetaProps);
		if (!pInheritance)
			return NULL;
		while (! pInheritance->Done())
		{
			psParam = Retrieve(pInheritance->Object(), sParamName);
			if (psParam)
				break;
			pInheritance->Next();
		}
		pInheritance->Release();
	}

	if (psParam)
	{
		// Check for the block-inheritance sentinal.
		if (psParam->size() == 1 && psParam->at(0) == '\0')
			return NULL;
	}
	return psParam;
}

const string* cParamCache::RetrieveSingle(int iObj, const string& sParamName)
{
	const string* psParam = NULL;
	{
		tParamCacheMap::const_iterator cached_entry = m_mapParamCache.find(iObj);
		if (cached_entry != m_mapParamCache.end())
		{
			tParamEntryMap::const_iterator param_entry = cached_entry->second.find(sParamName);
			if (param_entry != cached_entry->second.end())
			{
				psParam = &(param_entry->second);
			}
		}
		else
		{
			tParamEntryMap* entry = Read(iObj);
			if (entry)
			{
#ifdef DEBUG_PARAMCACHE
			m_mapParamDebug[iObj].counter[sParamName].reparse++;
#endif
				tParamEntryMap::const_iterator param_entry = entry->find(sParamName);
				if (param_entry != entry->end())
				{
					psParam = &(param_entry->second);
				}
			}
		}
	}
#ifdef DEBUG_PARAMCACHE
	if (psParam)
		m_mapParamDebug[iObj].counter[sParamName].hit++;
	else
		m_mapParamDebug[iObj].counter[sParamName].miss++;
#endif

	if (psParam)
	{
		// Check for the block-inheritance sentinal.
		if (psParam->size() == 1 && psParam->at(0) == '\0')
			return NULL;
	}
	return psParam;
}

void cParamCache::Update(int iObj, const string& sParamName, const string* sParamValue)
{
	tParamEntryMap* cache;
	tParamCacheMap::iterator cached_entry = m_mapParamCache.find(iObj);
	if (cached_entry != m_mapParamCache.end())
	{
		cache = &(cached_entry->second);
	}
	else
	{
		cache = &(m_mapParamCache[iObj]);
		Read(iObj);
	}
	
	(*cache)[sParamName] = (sParamValue != NULL) ? *sParamValue : string(1,0);
	Write(iObj);
}

void cParamCache::Remove(int iObj, const string& sParamName)
{
	tParamEntryMap* cache;
	tParamCacheMap::iterator cached_entry = m_mapParamCache.find(iObj);
	if (cached_entry != m_mapParamCache.end())
	{
		cache = &(cached_entry->second);
	}
	else
	{
		cache = &(m_mapParamCache[iObj]);
		Read(iObj);
	}
	
	cache->erase(sParamName);
	Write(iObj);
}

void cParamCache::Touch(int iObj)
{
	if (iObj != m_iUpdatingObj)
	{
		tParamCacheMap::iterator cached_entry = m_mapParamCache.find(iObj);
		if (cached_entry != m_mapParamCache.end())
		{
			m_mapParamCache.erase(cached_entry);
#ifdef DEBUG_PARAMCACHE
		m_mapParamDebug[iObj].touch++;
#endif
		}
	}
}

void cParamCache::Reset(void)
{
	m_mapParamCache.clear();
#ifdef DEBUG_PARAMCACHE
	PrintDebug();
	m_mapParamDebug.clear();
#endif
}

#ifdef DEBUG_PARAMCACHE
void cParamCache::SetDebug(int (__cdecl *pfnMPrintf)(const char*,...))
{
	m_Printf = pfnMPrintf;
}
void cParamCache::PrintDebug(void)
{
	if (!m_Printf) return;
	m_Printf("***** Begin ParamCache stats *****\n");
	map<int, tParamDebug>::const_iterator iObj = m_mapParamDebug.begin();
	for (; iObj != m_mapParamDebug.end(); ++iObj)
	{
		if (iObj->second.read > 0) // usually archetypes with no DesignNote
		{
			m_Printf("#%d: reads(%lu) touches(%lu)\n",
				iObj->first, iObj->second.read, iObj->second.touch);
			map<string, tParamDebugCounter, stringless_nocase>::const_iterator iParam
				= iObj->second.counter.begin();
			for (; iParam != iObj->second.counter.end(); ++iParam)
			{
				m_Printf("\t%s: %lu/%lu/%lu\n", iParam->first.c_str(),
					iParam->second.hit, iParam->second.miss, iParam->second.reparse);
			}
		}
	}
	m_Printf("***** End ParamCache stats *****\n\n");
}
#endif
