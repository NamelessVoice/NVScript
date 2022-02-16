/************s****************************
 * Scripts by Nameless Voice
 *
 *
 * For use in System Shock 2 only
 */

#if _DARKGAME == 3
#ifndef NVSCRIPTSSTS_H
#define NVSCRIPTSSTS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/scrservices.h>
#include <lg/properties.h>

#include "scriptvars.h"
#include "ScriptNV.h"
#include "NVGenericScript.h"
#include "NVGenericTrigger.h"

/**************************
 * NVVisibilityGem
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVisibilityGem,cScriptNV)
{
public:
	DECLARE_FACTORY(NVVisibilityGem,CustomScript);
	cScr_NVVisibilityGem(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnCreateObjs);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVVisibilityGem::OnTimer);
		REGISTER_MSGHANDLER("CreateObjs", cScr_NVVisibilityGem::OnCreateObjs);
	}
}
END_DECLARE_SCRIPT(cScr_NVVisibilityGem);


/**************************
 * NVFrobHilightHack
 * (System Shock 2 only)
 *
  *
 * Messages: WorldFocus, WorldDefocus
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVFrobHilightHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVFrobHilightHack,CustomScript);
	cScr_NVFrobHilightHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iPrev)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);

protected:

	script_int m_iPrev;

	virtual void InitVars(void)
	{
		m_iPrev.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVFrobHilightHack::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVFrobHilightHack);

/**************************
 * NVKnockoutable
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVKnockoutable,cScriptNV)
{
public:
	DECLARE_FACTORY(NVKnockoutable,CustomScript);
	cScr_NVKnockoutable(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnKnockout);
	DECLARE_MSGHANDLER(OnKnockedout);
	DECLARE_MSGHANDLER(OnTimer);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("KnockMeOut", cScr_NVKnockoutable::OnKnockedout);
		REGISTER_MSGHANDLER("KnockoutStimulus", cScr_NVKnockoutable::OnKnockout);
		REGISTER_MSGHANDLER("Timer", cScr_NVKnockoutable::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVKnockoutable);

/**************************
 * NVArrowHack
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVArrowHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVArrowHack,CustomScript);
	cScr_NVArrowHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iTime),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnSlay);
	DECLARE_MSGHANDLER(OnTimer);

protected:

	script_int m_iTime;
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_iTime.Init();
		m_hTimerHandle.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Slain", cScr_NVArrowHack::OnSlay);
		REGISTER_MSGHANDLER("Timer", cScr_NVArrowHack::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVArrowHack);

/**************************
 * NVExtinguishable
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVExtinguishable,cScriptNV)
{
public:
	DECLARE_FACTORY(NVExtinguishable,CustomScript);
	cScr_NVExtinguishable(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnLight);
	DECLARE_MSGHANDLER(OnDouse);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FireStimStimulus", cScr_NVExtinguishable::OnLight);
		REGISTER_MSGHANDLER("WaterStimStimulus", cScr_NVExtinguishable::OnDouse);
		REGISTER_MSGHANDLER("TurnOn", cScr_NVExtinguishable::OnLight);
		REGISTER_MSGHANDLER("TurnOff", cScr_NVExtinguishable::OnDouse);
	}
}
END_DECLARE_SCRIPT(cScr_NVExtinguishable);

/**************************
 * NVMossLump
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMossLump,cScriptNV)
{
public:
	DECLARE_FACTORY(NVMossLump,CustomScript);
	cScr_NVMossLump(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnPhysFellAsleep);
	DECLARE_MSGHANDLER(OnEndScript);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("PhysFellAsleep", cScr_NVMossLump::OnPhysFellAsleep);
	}
}
END_DECLARE_SCRIPT(cScr_NVMossLump);

/**************************
 * NVGong
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, EndScript, PhysCollision, FrobWorldEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVGong,cScriptNV)
{
public:
	DECLARE_FACTORY(NVGong,CustomScript);
	cScr_NVGong(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnPhysCollision);
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnFrob);
	void RingGong(void);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVGong::OnPhysCollision);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVGong::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVGong);

/**************************
 * NVCollisionStick
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, EndScript, PhysCollision, FrobWorldEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVCollisionStick,cScriptNV)
{
public:
	DECLARE_FACTORY(NVCollisionStick,CustomScript);
	cScr_NVCollisionStick(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnPhysCollision);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnEndScript);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVCollisionStick::OnTimer);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVCollisionStick::OnPhysCollision);
	}
}
END_DECLARE_SCRIPT(cScr_NVCollisionStick);

/**************************
 * NVZombieRegen
 * (System Shock 2 only)
 *
  *
 * Messages: BeginScript, EndScript, PhysCollision, Damage, Slain
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVZombieRegen,cScriptNV)
{
public:
	DECLARE_FACTORY(NVZombieRegen,CustomScript);
	cScr_NVZombieRegen(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnPhysCollision);
	DECLARE_MSGHANDLER(OnDamage);
	DECLARE_MSGHANDLER(OnSlain);
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnResurrect);
	void Resurrect(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Damage", cScr_NVZombieRegen::OnDamage);
		REGISTER_MSGHANDLER("Slain", cScr_NVZombieRegen::OnSlain);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVZombieRegen::OnPhysCollision);
		REGISTER_MSGHANDLER("Ressurect", cScr_NVZombieRegen::OnResurrect);
	}
}
END_DECLARE_SCRIPT(cScr_NVZombieRegen);

/**************************
 * NVLever
 * (NVTrap)
 *
 * Lever controller
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVLever, cScrNVGeneric), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVLever,NVGenericTrap);
	cScr_NVLever(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),  cScrNVTrigger(pszName, iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}
	DECLARE_MSGHANDLER(OnTweqComplete);
	DECLARE_MSGHANDLER(OnFrob);
	void DoTweq(bool bOn, int iStateObj = 0);


protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		cScrNVTrigger::InitScriptData();

		REGISTER_MSGHANDLER("TweqComplete", cScr_NVLever::OnTweqComplete);
		REGISTER_MSGHANDLER("GoForward", cScr_NVLever::OnActivate);
		REGISTER_MSGHANDLER("GoBackward", cScr_NVLever::OnDeactivate);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVLever::OnFrob);
		REGISTER_MSGHANDLER("Toggle", cScr_NVLever::OnFrob);
		REGISTER_MSGHANDLER("ToggleNoSE", cScr_NVLever::OnFrob);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVLever::OnResetTriggerCount);
	}

}
END_DECLARE_SCRIPT(cScr_NVLever);

#endif
#endif
