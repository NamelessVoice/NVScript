#ifndef SCRIPTMODULE_H
#define SCRIPTMODULE_H

#include <map>

#include <objbase.h>

#include "lg/script.h"
#include "lg/scrmanagers.h"

#include "ScrInstance.h"

#define SCRIPTMODULE2 1


class cScriptModule : public IScriptModule
{
private:
	typedef std::map<ScrInstance,IScript*> InstanceList;

public:
	// IUnknown
	STDMETHOD(QueryInterface)(REFIID,void**);
	STDMETHOD_(ULONG,AddRef)(void);
	STDMETHOD_(ULONG,Release)(void);
	// IScriptModule
	STDMETHOD_(const char*,GetName)(void);
	STDMETHOD_(const sScrClassDesc*,GetFirstClass)(tScrIter*);
	STDMETHOD_(const sScrClassDesc*,GetNextClass)(tScrIter*);
	STDMETHOD_(void,EndClassIter)(tScrIter*);

private:
	long int m_iRef;

public:
	virtual ~cScriptModule();
	cScriptModule();
	cScriptModule(const char* pszName);

	void SetName(const char* pszName);

	static IScript* __cdecl ScriptFactory(const char* pszName, int iHostObjId);

	void InitScriptDescs();
	void DestroyScriptDescs();

protected:
	static void DeleteScriptInstance(const char* pszName, int iHostObjId, IScript* pScript);

	friend class cScript;

private:
	const sScrClassDesc* GetScript(unsigned int i);
	
	char* m_pszName;

	static InstanceList m_InstanceList;
};

extern cScriptModule  g_ScriptModule;


#endif
