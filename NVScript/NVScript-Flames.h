/****************************************
 * Scripts by Nameless Voice
 *
 *
 * For use with The Circle of Flames
 */

#ifndef NVSCRIPTFLAMES_H
#define NVSCRIPTFLAMES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "lg/scrservices.h"
#include "lg/properties.h"

#include "scriptvars.h"
#include "ScriptNV.h"

/**************************
 * NVInflammable
 *
 * This script allows things to be set on fire.
 *
 * Messages: FlameStimStimulus
 * Parameters: None
 * Links: Firer
 */
DECLARE_SCRIPT(cScr_NVInflammable,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInflammable,CustomScript);
	cScr_NVInflammable(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iBurning)
	{ }

private:
	DECLARE_MSGHANDLER(OnBurn);
	DECLARE_MSGHANDLER(OnDouse);
	static int DestroyAttachedIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);
	script_int m_iBurning;

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FireStimStimulus", cScr_NVInflammable::OnBurn);
		REGISTER_MSGHANDLER("FlameStimStimulus", cScr_NVInflammable::OnBurn);
		REGISTER_MSGHANDLER("WaterStimStimulus", cScr_NVInflammable::OnDouse);
	}

	virtual void InitVars(void)
	{
		m_iBurning.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVInflammable);


#endif
