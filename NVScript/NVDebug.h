/****************************************
 * Scripts by Nameless Voice
 *
 */

#ifndef NVDEBUGSCRIPT_H
#define NVDEBUGSCRIPT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "lg/scrservices.h"
#include "lg/properties.h"

#include "scriptvars.h"
//#include "Script.h"
#include "ScriptNV.h"


DECLARE_MODULE(NVDebug)

/***************************************************
 * Debug / playtesting scripts.
 * Maybe these should be moved to a new module?
 **************************************************/



/**************************
 * NVDifficultyDebug
 *
 * A script designed solely for use in DromEd game mode!?
 *
 * Messages: Sim
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVDifficultyDebug,cScriptNV)
{
public:
	DECLARE_FACTORY(NVDifficultyDebug,CustomScript);
	cScr_NVDifficultyDebug(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSim);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Sim", cScr_NVDifficultyDebug::OnSim);
	}
};

/**************************
 * NVObjDumper
 *
 * Dumps information about its object on BeginScript
 *
 * Messages: BeginScript
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVObjDumper,cScriptNV)
{
public:
	DECLARE_FACTORY(NVObjDumper,CustomScript);
	cScr_NVObjDumper(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	// nothing

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
};

#endif
