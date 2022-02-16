#include "ScriptNV.h"
#include "ScriptLib.h"

MSGHANDLER cScriptNV::cScriptNVInit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	InitVars();
	OnBeginScript(pMsg, pReply, eTrace);
	return 0;
}

long __stdcall cScriptNV::ReceiveMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	long iRet = 1;
	try
	{
		iRet = cScript::ReceiveMessage(pMsg, pReply, eTrace);
	}
	catch (std::exception& err)
	{
		DebugString("Exception caught: \n", err.what());
	}
	return iRet;
}
