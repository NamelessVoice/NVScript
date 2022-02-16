/****************************************
 * Scripts by Nameless Voice
 *
 *
 * For use with The Circle of Stone and Shadow
 */
#if _DARKGAME == 2
#ifndef NVSCRIPTCOSAS_H
#define NVSCRIPTCOSAS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "lg/scrservices.h"
#include "lg/properties.h"

#include "scriptvars.h"
#include "ScriptNV.h"
#include "NVScript.h"

/**************************
 * NVManaRegen
 *
 *  Slowly regenerates the player's mana at a specified rate
 *
 * Messages: BeginScript, Timer
 * Parameters: NVManaRegenRate, NVManaRegenInitial, NVManaRegenMax
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVManaRegen,cScriptNV)
{
public:
	DECLARE_FACTORY(NVManaRegen,CustomScript);
	cScr_NVManaRegen(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnQuestChange);

	void UpdateManaDisplay(int iMana);

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
		REGISTER_MSGHANDLER("QuestChange", cScr_NVManaRegen::OnQuestChange);
		REGISTER_MSGHANDLER("Timer", cScr_NVManaRegen::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVManaRegen);


/**************************
 * NVSpellBase
 *
 * Base script for spells
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSpellBase,cScr_NVNewWeapon)
{
public:
	DECLARE_FACTORY(NVSpellBase,CustomScript);
	cScr_NVSpellBase(const char* pszName, int iHostObjId)
		: cScr_NVNewWeapon(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hDrainTimerHandle),
		SCRIPT_VAROBJ(m_hWindupTimerHandle)
	{
		m_iWindingUp = 0;
	}

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnFrobBegin);
	DECLARE_MSGHANDLER(OnFrobEnd);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnInvSelect);
	DECLARE_MSGHANDLER(OnInvDeSelect);

	int m_iWindingUp;

protected:
	script_handle<tScrTimer> m_hDrainTimerHandle;
	script_handle<tScrTimer> m_hWindupTimerHandle;

	virtual void InitVars(void)
	{
		m_hDrainTimerHandle.Init();
		m_hWindupTimerHandle.Init();
		return;
	}
	virtual void InitScriptData()
	{
		cScr_NVNewWeapon::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVSpellBase::OnTimer);
		REGISTER_MSGHANDLER("FrobInvBegin", cScr_NVSpellBase::OnFrobBegin);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVSpellBase::OnFrobEnd);
		REGISTER_MSGHANDLER("InvSelect", cScr_NVSpellBase::OnInvSelect);
		REGISTER_MSGHANDLER("InvDeSelect", cScr_NVSpellBase::OnInvDeSelect);
	}

	bool UseMana(int iCost);
	void SetCastingSpell(bool bOn);

	void StartDrain();
	void StopDrain();

	virtual void BeginWindup();
	virtual void AbortSpell();
	virtual void FinishWindup();
	virtual void ReleaseSpell();

	virtual void StartSpellEffect();
	virtual void EndSpellEffect();
}
END_DECLARE_SCRIPT(cScr_NVSpellBase);


/**************************
 * NVSpellInvisibility
 *
 * Base script for spells
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSpellInvisibility,cScr_NVSpellBase)
{
public:
	DECLARE_FACTORY(NVSpellInvisibility,CustomScript);
	cScr_NVSpellInvisibility(const char* pszName, int iHostObjId)
		: cScr_NVSpellBase(pszName,iHostObjId)
	{ }

private:

protected:
	virtual void BeginWindup();
	virtual void AbortSpell();
	virtual void FinishWindup();
	virtual void ReleaseSpell();

	virtual void StartSpellEffect();
	virtual void EndSpellEffect();
}
END_DECLARE_SCRIPT(cScr_NVSpellInvisibility);

/**************************
 * NVSpellProjectile
 *
 * Projectile-hurling spell
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSpellProjectile,cScr_NVSpellBase)
{
public:
	DECLARE_FACTORY(NVSpellProjectile,CustomScript);
	cScr_NVSpellProjectile(const char* pszName, int iHostObjId)
		: cScr_NVSpellBase(pszName,iHostObjId)
	{ }

private:

protected:
	virtual void BeginWindup();
	virtual void AbortSpell();
	virtual void FinishWindup();
	virtual void ReleaseSpell();

	virtual void StartSpellEffect();
	virtual void EndSpellEffect();
}
END_DECLARE_SCRIPT(cScr_NVSpellProjectile);

/**************************
 * NVSpellEtherealForm
 *
 * Ethereal Form spell
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSpellEtherealForm,cScr_NVSpellBase)
{
public:
	DECLARE_FACTORY(NVSpellEtherealForm,CustomScript);
	cScr_NVSpellEtherealForm(const char* pszName, int iHostObjId)
		: cScr_NVSpellBase(pszName,iHostObjId)
	{ }

private:

protected:
	virtual void BeginWindup();
	virtual void AbortSpell();
	virtual void FinishWindup();
	virtual void ReleaseSpell();

	virtual void StartSpellEffect();
	virtual void EndSpellEffect();
}
END_DECLARE_SCRIPT(cScr_NVSpellEtherealForm);


/**************************
 * NVForgetTrap
 * (NVTrap)
 *
 * Causes AIs to forget
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVSelectTrapItem
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVForgetTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVForgetTrap,NVGenericTrap);
	cScr_NVForgetTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	void SetEmitter(bool bOn);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVForgetTrap);

#endif
#endif // Thief 2
