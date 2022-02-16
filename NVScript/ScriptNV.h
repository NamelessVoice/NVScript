
#ifndef NVSCRIPTNV_H
#define NVSCRIPTNV_H

#include "Script.h"


/*
 * Generic Script
 *
 */
class cScriptNV : public cScript
{
public:
	cScriptNV(const char* pszName, int iHostObjId)
		: cScript(pszName, iHostObjId)
	{ }

	STDMETHOD_(long,ReceiveMessage)(sScrMsg*,sMultiParm*,eScrTraceAction);

private:
	DECLARE_MSGHANDLER(cScriptNVInit);

protected:
	virtual void InitScriptData()
	{
		cScript::InitScriptData();
		REGISTER_MSGHANDLER("BeginScript",cScriptNV::cScriptNVInit);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
	{
		return 0;
	}
	virtual void InitVars(void)
	{
		return;
	}
};

#endif // NVSCRIPTNV_H
