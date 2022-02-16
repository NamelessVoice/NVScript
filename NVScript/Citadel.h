/****************************************
 * Scripts for the Citadel Conversion by Nameless Voice
 *
 */

#ifndef CITADEL_H
#define CITADEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "lg/scrservices.h"
#include "lg/properties.h"

#include "scriptvars.h"
#include "ScriptNV.h"
#include "NVGenericScript.h"
#include "NVGenericTrigger.h"

DECLARE_MODULE(Citadel)


/**************************
 * CitadelSecurityReporter
 *
 * Security reporting script.  CitadelPlayer inherits from this.
 *
 *
 * Messages: Recharge
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_CitadelSecurityReporter,cScriptNV)
{
public:
	DECLARE_FACTORY(CitadelSecurityReporter,CustomScript);
	cScr_CitadelSecurityReporter(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSecurityUpdate);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("SecurityUpdate", cScr_CitadelSecurityReporter::OnSecurityUpdate);
	}
};


/**************************
 * CitadelPlayer
 *
 * The main 'Player' script for the Citadel Conversion.
 *
 * Note: In SS1, the player's maximum energy is ~250.
 *
 * Messages: Recharge
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_CitadelPlayer,cScr_CitadelSecurityReporter)
{
public:
	DECLARE_FACTORY(CitadelPlayer,CitadelSecurityReporter);
	cScr_CitadelPlayer(const char* pszName, int iHostObjId)
		: cScr_CitadelSecurityReporter(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_hBoosterTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnCreateObjs);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnRecalcEnergy);
	DECLARE_MSGHANDLER(OnHardwareOn);
	DECLARE_MSGHANDLER(OnHardwareOff);
//	DECLARE_MSGHANDLER(OnSecurityUpdate);
	void RecalcEnergy(void);
	void HardwareOff(int iHardware);
	void NightSightOn(void);
	void NightSightOff(void);
	float GetEnergyUsage(void);
	virtual long OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("CreateObjs", cScr_CitadelPlayer::OnCreateObjs);
		REGISTER_MSGHANDLER("Timer", cScr_CitadelPlayer::OnTimer);
		REGISTER_MSGHANDLER("RecalcEnergy", cScr_CitadelPlayer::OnRecalcEnergy);
		REGISTER_MSGHANDLER("HardwareOn", cScr_CitadelPlayer::OnHardwareOn);
		REGISTER_MSGHANDLER("HardwareOff", cScr_CitadelPlayer::OnHardwareOff);
//		REGISTER_MSGHANDLER("SecurityUpdate", cScr_CitadelPlayer::OnSecurityUpdate);
	}
	
	script_handle<tScrTimer> m_hTimerHandle;
	script_handle<tScrTimer> m_hBoosterTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_hBoosterTimerHandle.Init();
		return;
	}
};

/**************************
 * CitadelHardware
  *
 * A base script, inherited by others.
 *
 * Messages: Contained, FrobInvEnd
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_CitadelHardware,cScriptNV)
{
public:
	DECLARE_FACTORY(CitadelHardware,CustomScript);
	cScr_CitadelHardware(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), m_pProp(NULL),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{
		m_hListenHandle = NULL;
	}
	//
	virtual ~cScr_CitadelHardware();
	//

private:
	DECLARE_MSGHANDLER(OnContained);
	DECLARE_MSGHANDLER(OnFrobInvEnd); // Temp
	//
	DECLARE_MSGHANDLER(OnStateChange); // Temp
	//
	//
	DECLARE_MSGHANDLER(OnTimer);
	static void __stdcall ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData);
	PropListenerHandle m_hListenHandle;
	IGenericProperty* m_pProp;
	void StartListener();
	DECLARE_MSGHANDLER(OnEndScript);
	virtual long OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	//
	
protected:
	virtual int ToggleHardware(int iFrobber, int iHardware, int iSetting);

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Contained", cScr_CitadelHardware::OnContained);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_CitadelHardware::OnFrobInvEnd);
		//
		REGISTER_MSGHANDLER("StateChange", cScr_CitadelHardware::OnStateChange);
		//
		//
		REGISTER_MSGHANDLER("Timer", cScr_CitadelHardware::OnTimer);
		REGISTER_MSGHANDLER("EndScript", cScr_CitadelHardware::OnEndScript);
		//
	}
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_pProp = NULL;
		m_hTimerHandle.Init();
		return;
	}
};

/**************************
 * CitadelRecharge
 *
 * Base script for batteries and recharging stations.
 *
 * Messages: None
 * Parameters: CitadelBatterySize
 * Links: None
 */
DECLARE_SCRIPT(cScr_CitadelRecharge,cScriptNV)
{
public:
	DECLARE_FACTORY(CitadelRecharge,CustomScript);
	cScr_CitadelRecharge(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	// Nothing private
	
protected:
	virtual int Recharge(int iFrobber);
};

/**************************
 * CitadelBattery
 * (CitadelRecharge)
 *
 * Used for Batter Packs to allow the player to recharge his 'battery'
 *
 * Messages: FrobInvEnd
 * Parameters: None [inherits CitadelBatterySize]
 * Links: None
 */
DECLARE_SCRIPT(cScr_CitadelBattery,cScr_CitadelRecharge)
{
public:
	DECLARE_FACTORY(CitadelBattery,CustomScript);
	cScr_CitadelBattery(const char* pszName, int iHostObjId)
		: cScr_CitadelRecharge(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_CitadelBattery::OnFrob);
	}
};

/**************************
 * CitadelChargingStation
 * (CitadelRecharge)
 *
 * Used for recharging stations
 *
 * Messages: FrobWorldEnd
 * Parameters: CitadelChargingStationInterval, [inherits CitadelBatterySize]
 * Links: None
 */
DECLARE_SCRIPT(cScr_CitadelChargingStation,cScr_CitadelRecharge)
{
public:
	DECLARE_FACTORY(CitadelChargingStation,CustomScript);
	cScr_CitadelChargingStation(const char* pszName, int iHostObjId)
		: cScr_CitadelRecharge(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	DECLARE_MSGHANDLER(OnTimer);
	
protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_CitadelChargingStation::OnFrob);
		REGISTER_MSGHANDLER("Timer", cScr_CitadelChargingStation::OnTimer);
	}
	
	virtual void InitVars(void)
	{
		return;
	}
};

/**************************
 * CitadelEnergyWeapon
 *
 * Used for Energy Weapons, to get their ammo to synch with the player's 'battery'
 *
 * Messages: EnergyUpdate
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_CitadelEnergyWeapon,cScriptNV)
{
public:
	DECLARE_FACTORY(CitadelEnergyWeapon,CustomScript);
	cScr_CitadelEnergyWeapon(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEnergyUpdate);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EnergyUpdate", cScr_CitadelEnergyWeapon::OnEnergyUpdate);
	}
};


/**************************
 * CitadelEnergyProjectile
 *
 * Used to have energy weapons draw from the player's 'battery'
 *
 * Messages: Create, Timer, Destroy
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_CitadelEnergyProjectile,cScriptNV)
{
public:
	DECLARE_FACTORY(CitadelEnergyProjectile,CustomScript);
	cScr_CitadelEnergyProjectile(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnCreate);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnDestroy);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Create", cScr_CitadelEnergyProjectile::OnCreate);
		REGISTER_MSGHANDLER("Timer", cScr_CitadelEnergyProjectile::OnTimer);
		REGISTER_MSGHANDLER("Destroy", cScr_CitadelEnergyProjectile::OnDestroy);
	}
};



/**************************
 * CitadelSecurity
 *
 * Used for Security Cameras / Nodes to affect the Security %.
 *
 * Messages: FrobInvEnd
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_CitadelSecurity,cScriptNV)
{
public:
	DECLARE_FACTORY(CitadelSecurity,CustomScript);
	cScr_CitadelSecurity(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSlay);
	void CalculateSecurityLevel(int iCurrent, int iTotal);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Slain", cScr_CitadelSecurity::OnSlay);
	}
	virtual long OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
};

/**************************
 * NVOverlayTest
 *
 * A test script. Don't use.
 *
 * Messages: FrobInvEnd
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_NVOverlayTest,cScriptNV)
{
public:
	DECLARE_FACTORY(NVOverlayTest,CustomScript);
	cScr_NVOverlayTest(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iOverlay)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	DECLARE_MSGHANDLER(OnTurnOn);
	DECLARE_MSGHANDLER(OnTurnOff);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVOverlayTest::OnFrob);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVOverlayTest::OnFrob);
		REGISTER_MSGHANDLER("TurnOn", cScr_NVOverlayTest::OnTurnOn);
		REGISTER_MSGHANDLER("TurnOff", cScr_NVOverlayTest::OnTurnOff);
	}
	
	script_int m_iOverlay;

	virtual void InitVars(void)
	{
		m_iOverlay.Init();
		return;
	}
};

/**************************
 * NVOverlayIncrease
 *
 * Used for a test.
 *
 * Messages: TurnOn
 * Parameters: Not Specified
 * Links: Not Specified
 */
DECLARE_SCRIPT(cScr_NVOverlayIncrease,cScriptNV)
{
public:
	DECLARE_FACTORY(NVOverlayIncrease,CustomScript);
	cScr_NVOverlayIncrease(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTurnOn);
	
protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("TurnOn", cScr_NVOverlayIncrease::OnTurnOn);
	}
};

/**************************
 * NVSecurityBlock
 * (System Shock 2 only)
 *
 * Blocks access to something unless the level's Security % is under a certain value
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSecurityBlock,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSecurityBlock,CustomScript);
	cScr_NVSecurityBlock(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual long OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnFrob);

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
		REGISTER_MSGHANDLER("Timer", cScr_NVSecurityBlock::OnTimer);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVSecurityBlock::OnFrob);
	}
};

#endif
