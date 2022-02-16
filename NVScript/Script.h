
#ifndef SCRIPT_H
#define SCRIPT_H

#include <map>
#include <list>
#include "strless_nocase.h"

#include "lg/objstd.h"
#include "lg/script.h"
#include "ScriptModule.h"

#undef DispatchMessage

#ifdef GENOSM
# define DECLARE_MODULE(osmName)			DECLARE_MODULE(osmName)
# define DECLARE_SCRIPT(clName,clBase)		DECLARE_SCRIPT(clName,clBase)
# define DECLARE_FACTORY(scrName,scrBase)	DECLARE_FACTORY(scrName,scrBase)
# define DECLARE_MSGHANDLER(fnName)			DECLARE_MSGHANDLER(fnName)
# define END_DECLARE_SCRIPT(clName)			END_DECLARE_SCRIPT(clName)
# define REGISTER_MSGHANDLER(msgName,fnHandler)	REGISTER_MSGHANDLER(msgName,fnHandler)
#else
# define DECLARE_MODULE(osmName)
# define DECLARE_SCRIPT(clName,clBase)  class clName : public clBase
# define DECLARE_FACTORY(scrName,scrBase) \
	private: \
	static IScript* __cdecl Make(const char* pszName, int iHostObjId) \
	{ \
		cScr_##scrName* pscrRet = new(std::nothrow) cScr_##scrName(pszName, iHostObjId); \
		if (pscrRet) \
		{ \
			pscrRet->AddRef(); \
			pscrRet->InitScriptData(); \
		} \
		return static_cast<IScript*>(pscrRet); \
	} \
	public: \
	static void InitScriptDesc(sNewClassDesc * pDesc) \
	{ \
		pDesc->pszModule = NULL; \
		pDesc->pszClass = #scrName; \
		pDesc->pszBaseClass = #scrBase; \
		pDesc->pfnFactory = cScriptModule::ScriptFactory; \
		pDesc->pfnRealFactory = Make; \
	}

#ifdef INITOSM
#define END_DECLARE_SCRIPT(clName) ; \
	struct _ScriptAutoRegister##clName \
	{ \
		_ScriptAutoRegister##clName() { RegisterScriptClass(clName::InitScriptDesc); } \
	} __ScriptAutoRegister##clName
#else
#define END_DECLARE_SCRIPT(clName)
#endif

# define MSGHANDLER  long __cdecl
# define DECLARE_MSGHANDLER(fnName)  MSGHANDLER fnName(sScrMsg*, sMultiParm*, eScrTraceAction)
# define REGISTER_MSGHANDLER(msgName,fnHandler)  \
	RegisterMessageHandler(msgName, static_cast<MessageHandlerFunc>(&fnHandler))
#endif


struct sNewClassDesc : public sScrClassDesc
{
	ScriptFactoryProc pfnRealFactory;
};


class cScript : public IScript
{
protected:
	typedef long (__cdecl cScript::*MessageHandlerFunc)(sScrMsg*, sMultiParm*, eScrTraceAction);

private:
	typedef std::map<const char*, std::list<MessageHandlerFunc>, strless_nocase>  tDispatchMap;

public:
	// IUnknown
	STDMETHOD(QueryInterface)(REFIID,void**);
	STDMETHOD_(ULONG,AddRef)(void);
	STDMETHOD_(ULONG,Release)(void);
	// IScript
	STDMETHOD_(const char*,GetClassName)(void);
	STDMETHOD_(long,ReceiveMessage)(sScrMsg*,sMultiParm*,eScrTraceAction);

private:
	long int m_iRef;

public:
	virtual ~cScript();
	cScript(const char* pszName, int iHostObjId);

	STDMETHOD_(void*,GetThisPointer)()
	{
		return reinterpret_cast<void*>(this);
	}
	void DebugString(const char* pszMsg1, const char* pszMsg2 = "");

protected:
	char* m_szName;
	int m_iObjId;
	bool m_bSim;

	tDispatchMap m_mapHandlers;

	bool RegisterMessageHandler(const char* pszMsgName, MessageHandlerFunc pmfMsgHandler);
	bool UnregisterMessageHandler(const char* pszMsgName, MessageHandlerFunc pmfMsgHandler);
	long DispatchMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual void InitScriptData();

private:

	MSGHANDLER ScriptPtrQuery(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

};


#ifdef INITOSM
void RegisterScriptClass(void (*)(sNewClassDesc*));
#endif


#endif // SCRIPT_H
