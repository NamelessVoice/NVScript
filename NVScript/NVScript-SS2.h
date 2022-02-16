/****************************************
 * Scripts by Nameless Voice
 *
 *
 * For use in System Shock 2 only
 */

#if _DARKGAME == 3
#ifndef NVSCRIPTSS_H
#define NVSCRIPTSS_H

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
 * NVComestible
 * (System Shock 2 only)
 *
  *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVComestible,cScriptNV)
{
public:
	DECLARE_FACTORY(NVComestible,CustomScript);
	cScr_NVComestible(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobInvEnd);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVComestible::OnFrobInvEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVComestible);


/**************************
 * NVPutIntoContainers
 *
 * This script allows the player to put items with it into containers.
 *
 * Messages: FrobToolEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVPutIntoContainers,cScriptNV)
{
public:
	DECLARE_FACTORY(NVPutIntoContainers,CustomScript);
	cScr_NVPutIntoContainers(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobToolEnd);
	void Startup(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVPutIntoContainers::OnFrobToolEnd);
	}

}
END_DECLARE_SCRIPT(cScr_NVPutIntoContainers);


/**************************
 * NVEndTrap
 * (NVTrap)
 *
 * Ends the game. System Shock 2 only.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVEndTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVEndTrap,NVGenericTrap);
	cScr_NVEndTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVEndTrap);


/**************************
 * NVDeathCutscene
 *
 * Displays a cutscene when the player dies. System Shock 2 only.
 *
 * Messages: Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVDeathCutscene,cScriptNV)
{
public:
	DECLARE_FACTORY(NVDeathCutscene,CustomScript);
	cScr_NVDeathCutscene(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVDeathCutscene::OnTimer);
	}

}
END_DECLARE_SCRIPT(cScr_NVDeathCutscene);


/**************************
 * NVRecalcTrap
 * (NVTrap)
 *
 * Recalcs the player's stats. System Shock 2 only.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVRecalcTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVRecalcTrap,NVGenericTrap);
	cScr_NVRecalcTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVRecalcTrap);


/**************************
 * NVMultiplayerHack
 * (System Shock 2 only)
 *
 * Can be used to 'hack' multiplayer support into a fan mission that doesn't not support it.
 *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMultiplayerHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVMultiplayerHack,CustomScript);
	cScr_NVMultiplayerHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVMultiplayerHack::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVMultiplayerHack);

/**************************
 * NVMultiplayerTrap
 * (NVTrap)
 *
 * Triggers a multiplayer-enabling level transition.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMultiplayerTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVMultiplayerTrap,NVGenericTrap);
	cScr_NVMultiplayerTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVMultiplayerTrap);


/**************************
 * NVLightTrap
 * (NVTrap)
 *
 * Controls lights. System Shock 2 only.
 * DOES NOT WORK
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVLightTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVLightTrap,NVGenericTrap);
	cScr_NVLightTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVLightTrap);


/**************************
 * NVStatBasedProjectile
 *
 * Adds a damage modifier to a projectile based on one of the firer's stats.
 * The amount specified by NVStatBasedProjectileScale is multiplied by the stat,
 * has 1.00 added to it, and is used as the projectile's source scale.
 *
 * Stats are:
 *	0 - STR
 *	1 - END
 *	2 - PSI
 *	3 - AGI
 *	4 - CYB
 *
 * Messages: FrobInvEnd
 * Parameters: NVStatBasedProjectileStat, NVStatBasedProjectileScale = 0.10
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVStatBasedProjectile,cScriptNV)
{
public:
	DECLARE_FACTORY(NVStatBasedProjectile,CustomScript);
	cScr_NVStatBasedProjectile(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnThrow);
	DECLARE_MSGHANDLER(OnCalculate);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("PhysMadePhysical", cScr_NVStatBasedProjectile::OnThrow);
		REGISTER_MSGHANDLER("Create", cScr_NVStatBasedProjectile::OnThrow);
		REGISTER_MSGHANDLER("Calculate", cScr_NVStatBasedProjectile::OnCalculate);
	}
}
END_DECLARE_SCRIPT(cScr_NVStatBasedProjectile);


/**************************
 * NVRegenPsi
 * (System Shock 2 only)
 *
 * Allows the player to regenerate psi points.
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVRegenPsi,cScriptNV)
{
public:
	DECLARE_FACTORY(NVRegenPsi,CustomScript);
	cScr_NVRegenPsi(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:

	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVRegenPsi::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVRegenPsi);


/**************************
 * NVAmmoScript
 * (System Shock 2 only)
 *
 * Modified ammo script. Stops you from reloading a gun that isn't equipped.
 *
 * Messages: FrobToolEnd, FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVAmmoScript,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAmmoScript,CustomScript);
	cScr_NVAmmoScript(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobToolEnd);
	DECLARE_MSGHANDLER(OnFrobInvEnd);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVAmmoScript::OnFrobToolEnd);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVAmmoScript::OnFrobInvEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVAmmoScript);


/**************************
 * NVImplant
 * (System Shock 2 only)
 *
 * Modifiable implant script
  *
 * Messages: BeginScript, Timer
 * Parameters: NVImplantMeta, NVImplantSpeed, NVImplantOffMeta, NVImplantOffSelfMeta
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVImplant,cScriptNV)
{
public:
	DECLARE_FACTORY(NVImplant,CustomScript);
	cScr_NVImplant(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_iUsage)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnTurnOn);
	DECLARE_MSGHANDLER(OnTurnOff);
	DECLARE_MSGHANDLER(OnRecharge);
	DECLARE_MSGHANDLER(OnFrobToolEnd);
	DECLARE_MSGHANDLER(OnTimer);
	void StartDrainTimer(void);
	void StartUse(void);
	void StopUse(void);
	bool IsEquipped(void);
	void DoEffect(bool bOn);
	void DoOffEffect(bool bOn);


protected:
	void ProcessDrainTimer(void);

	script_handle<tScrTimer> m_hTimerHandle;
	script_int m_iUsage;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_iUsage.Init();
		return;
	}
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVImplant::OnTimer);
		REGISTER_MSGHANDLER("TurnOn", cScr_NVImplant::OnTurnOn);
		REGISTER_MSGHANDLER("TurnOff", cScr_NVImplant::OnTurnOff);
		REGISTER_MSGHANDLER("Recharge", cScr_NVImplant::OnRecharge);
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVImplant::OnFrobToolEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVImplant);


/**************************
 * NVGillsImplant
 * (System Shock 2 only)
 *
 * Modifiable implant script
  *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVGillsImplant,cScr_NVImplant)
{
public:
	DECLARE_FACTORY(NVGillsImplant,NVImplant);
	cScr_NVGillsImplant(const char* pszName, int iHostObjId)
		: cScr_NVImplant(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle2)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnImplantOn);
	DECLARE_MSGHANDLER(OnImplantOff);


protected:
	script_handle<tScrTimer> m_hTimerHandle2;

	virtual void InitVars(void)
	{
		cScr_NVImplant::InitVars();
		m_hTimerHandle2.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScr_NVImplant::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVGillsImplant::OnTimer);
		REGISTER_MSGHANDLER("ImplantOn", cScr_NVGillsImplant::OnImplantOn);
		REGISTER_MSGHANDLER("ImplantOff", cScr_NVGillsImplant::OnImplantOff);
	}
}
END_DECLARE_SCRIPT(cScr_NVGillsImplant);

/**************************
 * NVPlayerScript
 * (System Shock 2 only)
 *
 * Allows a certain level of modification (hacking ) and addition to existing traits (O/S Upgrades).
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVPlayerScript,cScriptNV)
{
public:
	DECLARE_FACTORY(NVPlayerScript,CustomScript);
	cScr_NVPlayerScript(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{
		m_hListenHandle = NULL;
		m_pProp = NULL;
	}

	virtual ~cScr_NVPlayerScript();

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	static void __stdcall ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData);
	PropListenerHandle m_hListenHandle;
	IGenericProperty* m_pProp;
	void StartListener();
	void StopListener();
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnBurnout);


protected:

	virtual void InitVars(void)
	{
		StartListener();
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVPlayerScript::OnEndScript);
		REGISTER_MSGHANDLER("Timer", cScr_NVPlayerScript::OnTimer);
		REGISTER_MSGHANDLER("OverloadDamage", cScr_NVPlayerScript::OnBurnout);
	}
}
END_DECLARE_SCRIPT(cScr_NVPlayerScript);


/**************************
 * NVSecurityComputer
 * (System Shock 2 only)
 *
 * Makes it no longer possible to simply turn off alarms by frobbing security computers, unless the player has
 * the "Security Expert" trait.
 *
 * Messages: FrobWorldEnd, HackSuccess, NetHackSuccess, HackCritfail, NetHackCritfail
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSecurityComputer,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSecurityComputer,CustomScript);
	cScr_NVSecurityComputer(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	DECLARE_MSGHANDLER(OnHackSuccess);
	DECLARE_MSGHANDLER(OnNetHackSuccess);
	DECLARE_MSGHANDLER(OnHackCritfail);
	// DECLARE_MSGHANDLER(OnNetHackCritfail);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVSecurityComputer::OnFrob);
		REGISTER_MSGHANDLER("HackSuccess", cScr_NVSecurityComputer::OnHackSuccess);
		REGISTER_MSGHANDLER("NetHackSuccess", cScr_NVSecurityComputer::OnNetHackSuccess);
		REGISTER_MSGHANDLER("HackCritfail", cScr_NVSecurityComputer::OnHackCritfail);
		// REGISTER_MSGHANDLER("NetHackCritfail", cScr_NVSecurityComputer::OnNetHackCritfail);
	}
}
END_DECLARE_SCRIPT(cScr_NVSecurityComputer);


/**************************
 * NVUpgradeCheatStopper
 * (System Shock 2 only)
 *
 * Stops various cheats that involve keeping an upgrade panel open while using a newly acquired skill, and then undoing the change.
 *
 * Messages: FrobWorldEnd, HackSuccess, NetHackSuccess, HackCritfail, NetHackCritfail
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVUpgradeCheatStopper,cScriptNV)
{
public:
	DECLARE_FACTORY(NVUpgradeCheatStopper,CustomScript);
	cScr_NVUpgradeCheatStopper(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnCloseTrainer);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Research", cScr_NVUpgradeCheatStopper::OnCloseTrainer);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVUpgradeCheatStopper::OnCloseTrainer);
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVUpgradeCheatStopper::OnCloseTrainer);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVUpgradeCheatStopper::OnCloseTrainer);
	}
}
END_DECLARE_SCRIPT(cScr_NVUpgradeCheatStopper);


/**************************
 * NVPlayerAccuracy
 * (System Shock 2 only)
 *
 * Messages: Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVPlayerAccuracy,cScriptNV)
{
public:
	DECLARE_FACTORY(NVPlayerAccuracy,CustomScript);
	cScr_NVPlayerAccuracy(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);

	float m_fSpeed;

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVPlayerAccuracy::OnTimer);
	}

	virtual void InitVars(void)
	{
		m_fSpeed = 0.00f;
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVPlayerAccuracy);


/**************************
 * NVArmour
 * (System Shock 2 only)
 *
 * Messages: Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVArmour,cScriptNV)
{
public:
	DECLARE_FACTORY(NVArmour,CustomScript);
	cScr_NVArmour(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTurnOn);
	DECLARE_MSGHANDLER(OnTurnOff);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	script_handle<tScrTimer> m_hTimerHandle;

	int m_iStrength;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("TurnOn", cScr_NVArmour::OnTurnOn);
		REGISTER_MSGHANDLER("TurnOff", cScr_NVArmour::OnTurnOff);
		REGISTER_MSGHANDLER("Timer", cScr_NVArmour::OnTimer);
	}

}
END_DECLARE_SCRIPT(cScr_NVArmour);



/**************************
 * NVBattery
 * (System Shock 2 only)
 *
 * Replacement for bugged battery script
 *
 * Messages: FrobTooldEnd, FrobInvEnd, Consume
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVBattery,cScriptNV)
{
public:
	DECLARE_FACTORY(NVBattery,CustomScript);
	cScr_NVBattery(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobToolEnd);
	DECLARE_MSGHANDLER(OnConsume);
	DECLARE_MSGHANDLER(OnFrobInvEnd);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVBattery::OnFrobToolEnd);
		REGISTER_MSGHANDLER("Consume", cScr_NVBattery::OnConsume);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVBattery::OnFrobInvEnd);
	}

}
END_DECLARE_SCRIPT(cScr_NVBattery);


/**************************
 * NVRecycler
 * (System Shock 2 only)
 *
 * Replacement for bugged Recycler script
 *
 * Messages: FrobTooldEnd, FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVRecycler,cScriptNV)
{
public:
	DECLARE_FACTORY(NVRecycler,CustomScript);
	cScr_NVRecycler(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobToolEnd);
	DECLARE_MSGHANDLER(OnFrobInvEnd);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVRecycler::OnFrobToolEnd);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVRecycler::OnFrobInvEnd);
	}

}
END_DECLARE_SCRIPT(cScr_NVRecycler);

/**************************
 * NVAvatarWeapons
 * (System Shock 2 only)
 *
 * Allows non-hard coded weapons to have proper avatar weapons.
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVAvatarWeapons,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAvatarWeapons,CustomScript);
	cScr_NVAvatarWeapons(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:

	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVAvatarWeapons::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVAvatarWeapons);


/**************************
 * NVGuaranteedLoot
 * (System Shock 2 only)
 *
 * Allows more than one item of guaranteed loot.
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVGuaranteedLoot,cScriptNV)
{
public:
	DECLARE_FACTORY(NVGuaranteedLoot,CustomScript);
	cScr_NVGuaranteedLoot(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSlain);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Slain", cScr_NVGuaranteedLoot::OnSlain);
	}
}
END_DECLARE_SCRIPT(cScr_NVGuaranteedLoot);

/**************************
 * NVCrateHack
 * (System Shock 2 only)
 *
 * A crate that can be opened without interfaces
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVCrateHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVCrateHack,CustomScript);
	cScr_NVCrateHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVCrateHack::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVCrateHack);

/**************************
 * NVRefreshInvTrap
 * (NVTrap)
 * (System Shock 2 Only)
 *
 * Refreshes the player's inventory on TurnOn
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVRefreshInvTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVRefreshInvTrap,NVGenericTrap);
	cScr_NVRefreshInvTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:
		virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVRefreshInvTrap::OnTimer);
	}

}
END_DECLARE_SCRIPT(cScr_NVRefreshInvTrap);

/**************************
 * NVObjState
 * (System Shock 2 only)
 *
 * Sends messages when an object's state changes
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVObjState,cScriptNV)
{
public:
	DECLARE_FACTORY(NVObjState,CustomScript);
	cScr_NVObjState(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{
			m_hListenHandle = NULL;
			m_pProp = NULL;
	}

	virtual ~cScr_NVObjState();

private:
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	static void __stdcall ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData);
	IGenericProperty* m_pProp;
	void StopListener();

protected:
	PropListenerHandle m_hListenHandle;
	void StartListener();

	script_handle<tScrTimer> m_hTimerHandle;

	void CheckState(int iState);

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();

		StartListener();

		return;
	}
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVObjState::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVObjState);

/**************************
 * NVSimpleRepair
 * (NVObjState)
 * (System Shock 2 only)
 *
 * Shows repair overlay when frobbed if broken.
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSimpleRepair,cScr_NVObjState)
{
public:
	DECLARE_FACTORY(NVSimpleRepair,CustomScript);
	cScr_NVSimpleRepair(const char* pszName, int iHostObjId)
		: cScr_NVObjState(pszName,iHostObjId)
	{	}

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScr_NVObjState::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVSimpleRepair::OnFrob);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVSimpleRepair::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVSimpleRepair);

/**************************
 * NVJumpyGrubEgg
 * (NVTrap)
 * (System Shock 2 Only)
 *
 * Makes grubs jump. Out of eggs. For some reason.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVJumpyGrubEgg, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVJumpyGrubEgg,NVGenericTrap);
	cScr_NVJumpyGrubEgg(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing
}
END_DECLARE_SCRIPT(cScr_NVJumpyGrubEgg);

/**************************
 * NVBriefingTrap
 * (NVTrap)
 * (System Shock 2 Only)
 *
 * Triggers SS2 briefings
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVBriefingTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVBriefingTrap,NVGenericTrap);
	cScr_NVBriefingTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing
}
END_DECLARE_SCRIPT(cScr_NVBriefingTrap);

/**************************
 * NVIrradiator
 * (System Shock 2 only)
 *
 * Attaches a rad glow object to the player when irradiated
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVIrradiator,cScriptNV)
{
public:
	DECLARE_FACTORY(NVIrradiator,CustomScript);
	cScr_NVIrradiator(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVIrradiator::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVIrradiator);

/**************************
 * NVCopyModify
 * (System Shock 2 only)
 *
 * Copies props from another object on Modify messsages
 *
 * Messages: Modify1, Modify2
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVCopyModify,cScriptNV)
{
public:
	DECLARE_FACTORY(NVCopyModify,CustomScript);
	cScr_NVCopyModify(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnModify1);
	DECLARE_MSGHANDLER(OnModify2);
	void DoModification(int iMod);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Modify1", cScr_NVCopyModify::OnModify1);
		REGISTER_MSGHANDLER("Modify2", cScr_NVCopyModify::OnModify2);
	}
}
END_DECLARE_SCRIPT(cScr_NVCopyModify);

/**************************
 * NVResurrectionToggle
 * (NVTrap)
 * (System Shock 2 Only)
 *
 * "Cyborg conversion cancelled. Standard station restoration procedure online."
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVResurrectionToggle, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVResurrectionToggle,NVGenericTrap);
	cScr_NVResurrectionToggle(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing
}
END_DECLARE_SCRIPT(cScr_NVResurrectionToggle);

/**************************
 * NVInstantCloseDoor
 * (System Shock 2 only)
 *
 * Allows its door to be instantly closed
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVInstantCloseDoor,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInstantCloseDoor,CustomScript);
	cScr_NVInstantCloseDoor(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTurnOff);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("TurnOff", cScr_NVInstantCloseDoor::OnTurnOff);
	}
}
END_DECLARE_SCRIPT(cScr_NVInstantCloseDoor);

/**************************
 * NVOverlayWatcher
 * (NVTrap)
 * (System Shock 2 Only)
 *
 * Watches for an overlay to close
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVOverlayWatcher, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVOverlayWatcher,NVGenericTrap);
	cScr_NVOverlayWatcher(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hBaseTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	script_handle<tScrTimer> m_hBaseTimerHandle;

	virtual void InitVars(void)
	{
		m_hBaseTimerHandle.Init();
		return;
	}
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVOverlayWatcher::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVOverlayWatcher);

/**************************
 * NVBaseOverlay
 * (NVTrap)
 * (System Shock 2 Only)
 *
 * Handles cam-attached overlays
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVBaseOverlay, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVBaseOverlay,NVGenericTrap);
	cScr_NVBaseOverlay(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hBaseTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);

protected:

	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	// Main function to start drawing overlay(s), to be overriden by child scripts
	virtual void StartOverlay(void);
	// Create object to cam attach at offset, and return object ID
	int AddOverlay(float x = 0.00f, float y = 0.00f, float z = 0.00f, object oArc = -1, const char *pszShape = NULL);
	// Destroy any and all attachments
	virtual void StopOverlay(void);

	virtual bool OverlayIsDistanceControlled() { return true; }

	void StartTimer(void);
	void KillTimer(void);

	virtual void ProcessBaseTimer();

	script_handle<tScrTimer> m_hBaseTimerHandle;

	virtual void InitVars(void)
	{
		m_hBaseTimerHandle.Init();
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVBaseOverlay::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVBaseOverlay);


/**************************
 * NVWiringPuzzle
 * (System Shock 2 only)
 *
 * SS1 wiring puzzle
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVWiringPuzzle,cScr_NVBaseOverlay), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVWiringPuzzle,NVBaseOverlay);
	cScr_NVWiringPuzzle(const char* pszName, int iHostObjId)
		: cScr_NVBaseOverlay(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{
		m_iSelection = -1;
	}

private:
		DECLARE_MSGHANDLER(OnScrewbox);
		DECLARE_MSGHANDLER(OnFrob);
		MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	virtual void StartOverlay(void);
	void DrawWires(void);
	void DrawProgress(void);

	int m_iSelection;

	virtual void InitScriptData()
	{
		cScr_NVBaseOverlay::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVWiringPuzzle::OnFrob);
		REGISTER_MSGHANDLER("Screwbox", cScr_NVWiringPuzzle::OnScrewbox);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVWiringPuzzle::OnResetTriggerCount);
	}
}
END_DECLARE_SCRIPT(cScr_NVWiringPuzzle);

/**************************
 * NVWiringPuzzleHelper
 * (System Shock 2 only)
 *
 * Allows its door to be instantly closed
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVWiringPuzzleHelper,cScriptNV)
{
public:
	DECLARE_FACTORY(NVWiringPuzzleHelper,CustomScript);
	cScr_NVWiringPuzzleHelper(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVWiringPuzzleHelper::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVWiringPuzzleHelper);

/**************************
 * NVModify
 * (System Shock 2 only)
 *
 * New Modify system
 *
 * Messages: BeginScript, Timer
 * Parameters: NVModify%iIcon [required], NVModify%iSkill, NVModify%iDescription, NVModify%iProp%i, NVModify%iProp%i NVModify%iSubProp%i, NVModify%iOperator%i, NVModify%iOperand%i
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVModify,cScr_NVBaseOverlay), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVModify,NVBaseOverlay);
	cScr_NVModify(const char* pszName, int iHostObjId)
		: cScr_NVBaseOverlay(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{
		m_iSelection = -1;
	}

private:
		DECLARE_MSGHANDLER(OnModifyIcon);
		DECLARE_MSGHANDLER(OnModify);
		DECLARE_MSGHANDLER(OnFrob);
		DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	virtual void StartOverlay(void);
	virtual void StopOverlay(void);

	virtual bool OverlayIsDistanceControlled() { return false; }

	int m_iSelection;

	virtual void InitScriptData()
	{
		cScr_NVBaseOverlay::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVModify::OnFrob);
		REGISTER_MSGHANDLER("ModifyIcon", cScr_NVModify::OnModifyIcon);
		REGISTER_MSGHANDLER("Modify1", cScr_NVModify::OnModify);
		REGISTER_MSGHANDLER("Modify2", cScr_NVModify::OnModify);
		REGISTER_MSGHANDLER("Timer", cScr_NVModify::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVModify);

/**************************
 * NVModifyHelper
 * (System Shock 2 only)
 *
 * Helper script  for overlays for NVModify
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVModifyHelper,cScriptNV)
{
public:
	DECLARE_FACTORY(NVModifyHelper,CustomScript);
	cScr_NVModifyHelper(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVModifyHelper::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVModifyHelper);

/**************************
 * NVStackConsumable
 * (System Shock 2 only)
 *
 * Base script for consumable stackable objects
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVStackConsumable,cScriptNV)
{
public:
	DECLARE_FACTORY(NVStackConsumable,CustomScript);
	cScr_NVStackConsumable(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

protected:
	void ConsumeStack(int iDestroy);
}
END_DECLARE_SCRIPT(cScr_NVStackConsumable);

/**************************
 * NVMedPatchScript
  *
 * New replacement Med Hypo script
 *
 * Messages: ?
 * Parameters:
 *		%sHealTotal ( = 10 )			Total amount healed
 *		%sPsiTotal ( = 10 )			Total amount of psi restored
 *		%sTrait ( = 2 )				Trait that gives modifier (default: 2 = Pharmo Friendly)
 *		%sTraitBonus ( = 1.20 )		Modifier for trait (e.g. Pharmo Friendly)
*		%sEasyBonus ( = 1.50 )			Modifier for 'Easy' difficulty
*		%sHealInc ( = 2 )				HP healed per 1500ms
*		%sPsilInc ( = 2 )				Psi restored per 1500ms
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVMedPatchScript,cScr_NVStackConsumable)
{
public:
	DECLARE_FACTORY(NVMedPatchScript,NVStackConsumable);
	cScr_NVMedPatchScript(const char* pszName, int iHostObjId)
		: cScr_NVStackConsumable(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_fHealAmt),
		SCRIPT_VAROBJ(m_fPsiAmt),
		SCRIPT_VAROBJ(m_iPatient),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{	}

private:
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnFrob);
	DECLARE_MSGHANDLER(OnEndScript);

protected:
	virtual float BaseHealTotal();
	virtual float BasePsiTotal();
	virtual float BaseHealInc();
	virtual float BasePsiInc();
	virtual float HealTotal(int iFrobber, bool bPsi = false);
//	virtual float PsiTotal(int iFrobber);
	virtual float HealInc(bool bPsi = false);
	virtual void DoHeal(float fHealAmount, float fPsiAmount);

	script_float m_fHealAmt;
	script_float m_fPsiAmt;
	script_int m_iPatient;
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_fHealAmt.Init();
		m_fPsiAmt.Init();
		m_iPatient.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVMedPatchScript::OnTimer);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVMedPatchScript::OnFrob);
		REGISTER_MSGHANDLER("EndScript", cScr_NVMedPatchScript::OnEndScript);
	}

}
END_DECLARE_SCRIPT(cScr_NVMedPatchScript);

/**************************
 * NVPsiKitScript
 *
 * Variant of NVMedPatchScript
 *
 * Defaults to healing 0hp, 20psi in 2pp increments
 *
 * Messages: Same as NVMedPatchScript
 * Parameters:  Same as NVMedPatchScript
 * Links: Same as NVMedPatchScript
 */


DECLARE_SCRIPT(cScr_NVPsiKitScript,cScr_NVMedPatchScript)
{
public:
	DECLARE_FACTORY(NVPsiKitScript,NVMedPatchScript);
	cScr_NVPsiKitScript(const char* pszName, int iHostObjId)
		: cScr_NVMedPatchScript(pszName,iHostObjId)
	{	}

private:
	// Nothing private

protected:
	virtual float BaseHealTotal();
	virtual float BasePsiTotal();
	virtual float BasePsiInc();
}
END_DECLARE_SCRIPT(cScr_NVPsiKitScript);


/**************************
 * NVMedKitScript
 *
 * Variant of NVMedPatchScript
 *
 * Defaults to healing 200 in 5hp increments.
 *
 * Messages: Same as NVMedPatchScript
 * Parameters:  Same as NVMedPatchScript
 * Links: Same as NVMedPatchScript
 */


DECLARE_SCRIPT(cScr_NVMedKitScript,cScr_NVMedPatchScript)
{
public:
	DECLARE_FACTORY(NVMedKitScript,NVMedPatchScript);
	cScr_NVMedKitScript(const char* pszName, int iHostObjId)
		: cScr_NVMedPatchScript(pszName,iHostObjId)
	{	}

private:
	// Nothing private

protected:
	virtual float BaseHealTotal();
	virtual float BaseHealInc();
}
END_DECLARE_SCRIPT(cScr_NVMedKitScript);

/**************************
 * NVHealingGland
 *
 * Variant of NVMedPatchScript
 *
 * Defaults to healing 200 in 5hp increments.
 *
 * Messages: Same as NVMedPatchScript
 * Parameters:  Same as NVMedPatchScript
 * Links: Same as NVMedPatchScript
 */


DECLARE_SCRIPT(cScr_NVHealingGland,cScr_NVMedPatchScript)
{
public:
	DECLARE_FACTORY(NVHealingGland,NVMedPatchScript);
	cScr_NVHealingGland(const char* pszName, int iHostObjId)
		: cScr_NVMedPatchScript(pszName,iHostObjId)
	{	}

private:
	// Nothing private

protected:
	virtual float BaseHealTotal();
	virtual float BaseHealInc();
}
END_DECLARE_SCRIPT(cScr_NVHealingGland);

/**************************
 * NVVisibilityModifierHack
 *
 * Removes objects with DS12
 *
 * Messages: None
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVisibilityModifierHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVVisibilityModifierHack,CustomScript);
	cScr_NVVisibilityModifierHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), m_pProp(NULL)
	{ }

	virtual ~cScr_NVVisibilityModifierHack()
	{
		StopListener();
		if ( m_pProp )
		{
			m_pProp->Release();
		}
	}

private:
	static void __stdcall ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData);
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	IGenericProperty* m_pProp;
	void StopListener();
	void DoUpdate();
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnDeactivatePsi);

	int m_iState;

protected:
	PropListenerHandle m_hListenHandle;
	void StartListener();

	virtual void InitVars(void)
	{
		m_hListenHandle = NULL;
		DoUpdate();
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVVisibilityModifierHack::OnTimer);
		REGISTER_MSGHANDLER("DeactivatePsi", cScr_NVVisibilityModifierHack::OnDeactivatePsi);
	}

}
END_DECLARE_SCRIPT(cScr_NVVisibilityModifierHack);

/**************************
 * NVFreeModify
 * (System Shock 2 only)
 *
 * New French-Ep script, for use with NVModify
 * Always applies modification #1
 *
 * Messages: FrobToolEnd, FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVFreeModify,cScriptNV)
{
public:
	DECLARE_FACTORY(NVFreeModify,CustomScript);
	cScr_NVFreeModify(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobToolEnd);
	DECLARE_MSGHANDLER(OnFrobInvEnd);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVFreeModify::OnFrobToolEnd);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVFreeModify::OnFrobInvEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVFreeModify);

/**************************
 * NVDetoxTrap
 * (NVTrap)
 *
 * Detoxes the player. System Shock 2 only.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVDetoxTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVDetoxTrap,NVGenericTrap);
	cScr_NVDetoxTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVDetoxTrap);

/**************************
 * NVTranslucentTextTrap
 * (NVTrap)
 *
 * Displays the specified text in a window at the centre of the screen.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVTranslucentTextTrapText, NVTranslucentTextFile, NVTranslucentTextNumber, NVTranslucentTextTime
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVTranslucentTextTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVTranslucentTextTrap,NVGenericTrap);
	cScr_NVTranslucentTextTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnEndScript);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVTranslucentTextTrap::OnTimer);
		REGISTER_MSGHANDLER("EndScript", cScr_NVTranslucentTextTrap::OnEndScript);
	}

}
END_DECLARE_SCRIPT(cScr_NVTranslucentTextTrap);


/**************************
 * NVVaporizeInventory
 * (NVTrap)
 *
 * Vaporises the player's inventory and takes away all his nanites.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVaporizeInventory, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVVaporizeInventory,NVGenericTrap);
	cScr_NVVaporizeInventory(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVVaporizeInventory);

#endif
#endif
