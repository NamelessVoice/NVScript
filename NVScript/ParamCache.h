#ifndef PARAMCACHE_H
#define PARAMCACHE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/interface.h>
#include <lg/scrmanagers.h>
#include <lg/properties.h>

#include <map>
#include <string>
#include "strless_nocase.h"


class cParamCache
{
public:
	cParamCache(IScriptMan *pInterface);
	~cParamCache();

private:
	typedef std::map<std::string, std::string, stringless_nocase>	tParamEntryMap;
	typedef std::map<int, tParamEntryMap >	tParamCacheMap;

#ifdef DEBUG_PARAMCACHE
	struct tParamDebugCounter {
		unsigned long hit;
		unsigned long miss;
		unsigned long reparse;
		tParamDebugCounter() :
			hit(0),
			miss(0),
			reparse(0)
		{ };
	};
	struct tParamDebug {
		std::map<std::string, tParamDebugCounter, stringless_nocase> counter;
		unsigned long read;
		unsigned long touch;
		tParamDebug() :
			read(0),
			touch(0)
		{ };
	};
	std::map<int, tParamDebug> m_mapParamDebug;
	int (__cdecl *m_Printf)(const char*,...);
public:
	void SetDebug(int (__cdecl *pfnMPrintf)(const char*,...));
	void PrintDebug(void);
#endif
private:
	
	tParamCacheMap m_mapParamCache;

	IStringProperty *m_pDNProp;
	ITraitManager *m_pTraitMan;

	PropListenerHandle m_hListenerHandle;
	int m_iUpdatingObj;

	static void __stdcall PropertyListener(sPropertyListenMsg* pPropMsg, PropListenerData pData);
	static int __cdecl SimListener(const sDispatchMsg* pSimMsg, const sDispatchListenerDesc* pData);
	static const GUID sm_iid;

	void Parse(tParamEntryMap& cache, const char* pszDN);
	std::string Unparse(tParamEntryMap& cache);
	tParamEntryMap* Read(int iObj);
	void Write(int iObj);

public:
	const std::string* Retrieve(int iObj, const std::string& sParamName);
	const std::string* RetrieveSingle(int iObj, const std::string& sParamName);
	void Update(int iObj, const std::string& sParamName, const std::string* sParamValue);
	void Remove(int iObj, const std::string& sParamName);
	void Touch(int iObj);
	void Reset(void);
};

#endif // PARAMCACHE_H
