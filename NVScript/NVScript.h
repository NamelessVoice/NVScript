/****************************************
 * Scripts by Nameless Voice
 *
 */

#ifndef NVSCRIPT_H
#define NVSCRIPT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/scrservices.h>
#include <lg/properties.h>
#include <set>

#include "scriptvars.h"
#include "ScriptNV.h"
#include "NVGenericScript.h"
#include "NVGenericTrigger.h"

DECLARE_MODULE(NVScript)


#if (_DARKGAME != 3) // SS2 handles the inventory completely differently
/**************************
 * NVEmitWhileSelected
 *
 * This script copies the CfgTweqEmit property from its object
 * to the Player object, and then turns the emitter on via InvSelect
 * messages, and off via InvDeselect.
 *
 * The emitter properties to copy/activate are specified via
 * the [NVTweqNum] paramater.
 *
 * Messages: InvSelect, InvDeselect
 * Parameters: NVTweqNum, IgnoreDeFocus, NVLanternHack
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVEmitWhileSelected,cScriptNV)
{
public:
	DECLARE_FACTORY(NVEmitWhileSelected,CustomScript);
	cScr_NVEmitWhileSelected(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnInventory);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("InvSelect", cScr_NVEmitWhileSelected::OnInventory);
		REGISTER_MSGHANDLER("InvFocus", cScr_NVEmitWhileSelected::OnInventory);
		REGISTER_MSGHANDLER("InvDeselect", cScr_NVEmitWhileSelected::OnInventory);
		REGISTER_MSGHANDLER("InvDeFocus", cScr_NVEmitWhileSelected::OnInventory);
	}
}
END_DECLARE_SCRIPT(cScr_NVEmitWhileSelected);
#endif // Non-SS2


/**************************
 * NVLinkBuilder
 * (NVTrap)
 *
 * When activated, this script creates a link with the
 * flavour specified by [NVLinkType], from the object
 * specified in [NVLinkSource] to the object specified in
 * [NVLinkDest].
 * You can specify data for the link by using [NVLinkData#Field] and
 * [NVLink#DataValue] (up to 10, maximum).
 * Use 0 or 1 to specify Booleans.
 * If a link has no fields, then [NVLinkData#Field] MUST be omitted.
 *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: NVLinkSource, NVLinkDest, NVLinkType, NVLinkData#Field, NVLinkData#Value
 * Links: Whatever the user specifies. They get created via the script.
 */
DECLARE_SCRIPT(cScr_NVLinkBuilder, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVLinkBuilder,NVGenericTrap);
	cScr_NVLinkBuilder(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:

	virtual long DoLink(bool bOn, sScrMsg* pMsg);
	virtual MSGHANDLER BuildLink(int iSource, int iDest, int iFlavour);
	virtual MSGHANDLER RemoveLink(int iSource, int iDest, int iFlavour);
}
END_DECLARE_SCRIPT(cScr_NVLinkBuilder);

/**************************
 * NVLinkBuilder2
 * (NVLinkBuilder)
 *
 */

DECLARE_SCRIPT(cScr_NVLinkBuilder2, cScr_NVLinkBuilder)
{
public:
	DECLARE_FACTORY(NVLinkBuilder2,NVLinkBuilder);
	cScr_NVLinkBuilder2(const char* pszName, int iHostObjId)
		: cScr_NVLinkBuilder(pszName,iHostObjId)
	{ }

}
END_DECLARE_SCRIPT(cScr_NVLinkBuilder2);

/**************************
 * NVLinkBuilder3
 * (NVLinkBuilder)
 *
 */

DECLARE_SCRIPT(cScr_NVLinkBuilder3, cScr_NVLinkBuilder)
{
public:
	DECLARE_FACTORY(NVLinkBuilder3,NVLinkBuilder);
	cScr_NVLinkBuilder3(const char* pszName, int iHostObjId)
		: cScr_NVLinkBuilder(pszName,iHostObjId)
	{ }

}
END_DECLARE_SCRIPT(cScr_NVLinkBuilder3);

/**************************
 * NVLinkBuilder4
 * (NVLinkBuilder)
 *
 */

DECLARE_SCRIPT(cScr_NVLinkBuilder4, cScr_NVLinkBuilder)
{
public:
	DECLARE_FACTORY(NVLinkBuilder4,NVLinkBuilder);
	cScr_NVLinkBuilder4(const char* pszName, int iHostObjId)
		: cScr_NVLinkBuilder(pszName,iHostObjId)
	{ }

}
END_DECLARE_SCRIPT(cScr_NVLinkBuilder4);


/**************************
 * NVCreateAndLink
 * (NVTrap->NVLinkBuilder)
 *
 * When activated, this script creates the object specified by [NVCreate].
 * It then creates a link with the flavour specified by [NVLinkType].
 * You can specify data for the link by using [NVLinkData#Field] and
 * [NVLinkData#Value] (up to 10, maximum).
 *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: NVCreate, NVLinkType, NVLinkData#Field, NVLinkData#Value
 * Links: Whatever the user specifies. They get created via the script.
 */
DECLARE_SCRIPT(cScr_NVCreateAndLink, cScr_NVLinkBuilder)
{
public:
	DECLARE_FACTORY(NVCreateAndLink,NVLinkBuilder);
	cScr_NVCreateAndLink(const char* pszName, int iHostObjId)
		: cScr_NVLinkBuilder(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnEndScript);

	void DestroyLinkedObject(sScrMsg* pMsg);

protected:

virtual void InitScriptData()
	{
		cScr_NVLinkBuilder::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVCreateAndLink::OnEndScript);
	}

	int m_iArc;
	virtual void InitVars(void)
	{
		m_iArc = 0;
		cScr_NVLinkBuilder::InitVars();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink);

/**************************
 * NVCreateAndLink2
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink2, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink2,NVCreateAndLink);
	cScr_NVCreateAndLink2(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink2);

/**************************
 * NVCreateAndLink3
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink3, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink3,NVCreateAndLink);
	cScr_NVCreateAndLink3(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink3);

/**************************
 * NVCreateAndLink4
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink4, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink4,NVCreateAndLink);
	cScr_NVCreateAndLink4(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink4);

/**************************
 * NVCreateAndLink5
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink5, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink5,NVCreateAndLink);
	cScr_NVCreateAndLink5(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink5);

/**************************
 * NVCreateAndLink6
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink6, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink6,NVCreateAndLink);
	cScr_NVCreateAndLink6(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink6);

/**************************
 * NVCreateAndLink7
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink7, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink7,NVCreateAndLink);
	cScr_NVCreateAndLink7(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink7);

/**************************
 * NVCreateAndLink8
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink8, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink8,NVCreateAndLink);
	cScr_NVCreateAndLink8(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink8);

/**************************
 * NVCreateAndLink9
 * (NVCreateAndLink)
 *
  */

DECLARE_SCRIPT(cScr_NVCreateAndLink9, cScr_NVCreateAndLink)
{
public:
	DECLARE_FACTORY(NVCreateAndLink9,NVCreateAndLink);
	cScr_NVCreateAndLink9(const char* pszName, int iHostObjId)
		: cScr_NVCreateAndLink(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVCreateAndLink9);

/**************************
 * NVNameOnCreation
 *
 * This script changes the symbolic name of the object which
 * has it when it is created, or the sim starts.
 * The name is specified by the [NVSymName] parameter.
 * Nothing happens if an object with that name already exists,
 * unless [NVSymNameIncrement]=1; this will cause a number
 * to be appended to each object to give it a unique name
 * (up to 99).
 *
 * Messages: Sim, Create
 * Parameters: NVSymName, NVSymNameIncrement
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVNameOnCreation,cScriptNV)
{
public:
	DECLARE_FACTORY(NVNameOnCreation,CustomScript);
	cScr_NVNameOnCreation(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	void Startup(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Sim", cScr_NVNameOnCreation::OnSim);
		REGISTER_MSGHANDLER("Create", cScr_NVNameOnCreation::OnCreate);
	}
}
END_DECLARE_SCRIPT(cScr_NVNameOnCreation);

/**************************
 * NVFrobToggle
 * (NVTrigger)
 *
 * This script sends alternating TurnOn / TurnOff
 * messages each time it's frobbed.
 * The first message will be TurnOn, unless you use
 * [NVFrobToggleStart]=1 - in which case it will be TurnOff.
 *
 * Messages: TurnOn, TurnOff
 * Parameters: NVFrobToggleStart
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVFrobToggle,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVFrobToggle,CustomScript);
	cScr_NVFrobToggle(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVFrobToggle::OnFrob);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVFrobToggle::OnFrob);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVFrobToggle::OnResetTriggerCount);
	}
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle);

/**************************
 * NVFrobToggle2
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle2, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle2,NVFrobToggle);
	cScr_NVFrobToggle2(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle2);

/**************************
 * NVFrobToggle3
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle3, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle3,NVFrobToggle);
	cScr_NVFrobToggle3(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle3);

/**************************
 * NVFrobToggle4
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle4, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle4,NVFrobToggle);
	cScr_NVFrobToggle4(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle4);

/**************************
 * NVFrobToggle5
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle5, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle5,NVFrobToggle);
	cScr_NVFrobToggle5(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle5);

/**************************
 * NVFrobToggle6
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle6, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle6,NVFrobToggle);
	cScr_NVFrobToggle6(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle6);

/**************************
 * NVFrobToggle7
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle7, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle7,NVFrobToggle);
	cScr_NVFrobToggle7(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle7);

/**************************
 * NVFrobToggle8
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle8, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle8,NVFrobToggle);
	cScr_NVFrobToggle8(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle8);

/**************************
 * NVFrobToggle9
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle9, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle9,NVFrobToggle);
	cScr_NVFrobToggle9(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle9);

/**************************
 * NVFrobToggle10
 * (NVFrobToggle)
 *
  */

DECLARE_SCRIPT(cScr_NVFrobToggle10, cScr_NVFrobToggle)
{
public:
	DECLARE_FACTORY(NVFrobToggle10,NVFrobToggle);
	cScr_NVFrobToggle10(const char* pszName, int iHostObjId) : cScr_NVFrobToggle(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVFrobToggle10);

/**************************
 * NVGhostingMessages
 *
 * This script displays the message "Ghosting Failed" when
 * an AI with the script reaches Alert 2 or 3, engages in
 * combat, or is damaged, slain, or knocked out.
 *
 * The message is only ever displayed ONCE, unless
 * the mission-scope QVar 'NVGhostingMessages' is
 * 1, or [NVGhostingMessages]=1
 *
 * The script will do nothing if the AI has the
 * [NVGhostingIgnore]=1 parameter set.
 *
 * Messages: Alertness
 * Parameters: NVGhostingMessages, NVGhostingIgnore
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVGhostingMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVGhostingMessages,CustomScript);
	cScr_NVGhostingMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iIgnore)
	{ }

private:
	DECLARE_MSGHANDLER(OnAlertness);
	DECLARE_MSGHANDLER(OnAIModeChange);
	DECLARE_MSGHANDLER(OnDamage);
	DECLARE_MSGHANDLER(OnEndScript);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Alertness", cScr_NVGhostingMessages::OnAlertness);
		REGISTER_MSGHANDLER("AIModeChange", cScr_NVGhostingMessages::OnAIModeChange);
		REGISTER_MSGHANDLER("Damage", cScr_NVGhostingMessages::OnDamage);
		REGISTER_MSGHANDLER("Slain", cScr_NVGhostingMessages::OnDamage);
		REGISTER_MSGHANDLER("EndScript", cScr_NVGhostingMessages::OnEndScript);
	}

	void FailGhosting(void);
	script_int m_iIgnore;

	virtual void InitVars(void)
	{
		m_iIgnore.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVGhostingMessages);

/**************************
 * NVScaleDims
 * (NVTrap)
 *
 * Upon receiving TurnOn, this script resizes the
 * physical dimensions of the object to match
 * its current scale. If a Tweq->Scale is currently active,
 * then a timer is started which fixes the dimensions every
 * 100ms.
 *
 * (In truth, the timer is always started, but is stopped
 *  after the first 100ms if the Tweq->Scale is inactive...)
 *
 * Messages: TurnOn, TurnOff (specifiable); TweqComplete. Also Sim and Create.
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVScaleDims, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVScaleDims,NVGenericTrap);
	cScr_NVScaleDims(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_iPhysType),
		SCRIPT_VAROBJ(m_vSize),
		SCRIPT_VAROBJ(m_vOffset),
		SCRIPT_VAROBJ(m_fBaseLine)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	void KillTimer(void);
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnTweqComplete);
	void Startup(void);

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("Timer",cScr_NVScaleDims::OnTimer);
		REGISTER_MSGHANDLER("Sim",cScr_NVScaleDims::OnSim);
		REGISTER_MSGHANDLER("Create",cScr_NVScaleDims::OnCreate);
		REGISTER_MSGHANDLER("TweqComplete",cScr_NVScaleDims::OnTweqComplete);
	}

	script_handle<tScrTimer> m_hTimerHandle;
	script_int m_iPhysType;
	script_vec m_vSize;
	script_vec m_vOffset;
	script_float m_fBaseLine;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_iPhysType.Init(3);
		m_vSize.Init();
		m_vOffset.Init();
		m_fBaseLine.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVScaleDims);

/**************************
 * NVFakeWater
 *
 * This script simulates water.
 * It is meant to be applied to a BoundsTrigger-type object.
 * (That is to say, an OBB physics model with the 'Is Edge Trigger' flag.)
 * When the player is inside the edges of the bounding box,
 * then various effects are applied to him to give the impression
 * of being in/under water.
 * Other objects entering / exiting the bounding box will have their
 * velocity and gravity reduced.
 *
 * This script will not work properly if its object is rotated.
 *
 * Can be turned on/off for performance reason via the 
 * NVActivate / NVDeactivate messages; see NVActivatorRoom
 *
 *
 * Messages: PhysEnter, PhysExit, Timer, EndScript, BeginScript (used to susbscribe to physics messages)
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVFakeWater,cScriptNV)
{
public:
	DECLARE_FACTORY(NVFakeWater,CustomScript);
	cScr_NVFakeWater(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_iInWater),
		SCRIPT_VAROBJ(m_iSlowed),
		SCRIPT_VAROBJ(m_iGrav),
		SCRIPT_VAROBJ(m_iTimerCount),
		SCRIPT_VAROBJ(m_fRecoverRate)
	{ }

private:
	DECLARE_MSGHANDLER(OnPhysEnter);
	DECLARE_MSGHANDLER(OnPhysExit);
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnFakeWaterOn);
	DECLARE_MSGHANDLER(OnFakeWaterOff);
	void KillTimer(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVFakeWater::OnEndScript);
		REGISTER_MSGHANDLER("PhysEnter", cScr_NVFakeWater::OnPhysEnter);
		REGISTER_MSGHANDLER("PhysExit", cScr_NVFakeWater::OnPhysExit);
		REGISTER_MSGHANDLER("Timer",cScr_NVFakeWater::OnTimer);
		REGISTER_MSGHANDLER("NVActivate",cScr_NVFakeWater::OnFakeWaterOn);
		REGISTER_MSGHANDLER("NVDeactivate",cScr_NVFakeWater::OnFakeWaterOff);

	}

	script_handle<tScrTimer> m_hTimerHandle;
	script_int m_iInWater;
	script_int m_iSlowed;
	script_int m_iGrav;
	script_int m_iTimerCount;
	script_float m_fRecoverRate;

	virtual MSGHANDLER InWater(void);
	virtual MSGHANDLER OutWater(void);

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_iInWater.Init();
		m_fRecoverRate.Init();
		m_iSlowed.Init();
		m_iGrav.Init();
		m_iTimerCount.Init();
		return;
	}
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

}
END_DECLARE_SCRIPT(cScr_NVFakeWater);

/**************************
 * NVLookAtNoises
 *
 * This script is used for AIs.
 * It is currently incomplete. (Lacking motions)
 *
 * Messages: Alertness
 * Parameters: None (Will eventually have a % chance )
 * Links: AIAlertness
 */
DECLARE_SCRIPT(cScr_NVLookAtNoises,cScriptNV)
{
public:
	DECLARE_FACTORY(NVLookAtNoises,CustomScript);
	cScr_NVLookAtNoises(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_vLocation)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnAlertness);
	static int AwarenessIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);
	void KillTimer(void);
	void DoCheck(void);


protected:
	script_handle<tScrTimer> m_hTimerHandle;
	int m_iLastTime;
	script_vec m_vLocation;

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Alertness", cScr_NVLookAtNoises::OnAlertness);
		REGISTER_MSGHANDLER("Timer",cScr_NVLookAtNoises::OnTimer);
	}

	virtual void InitVars(void) {
		m_hTimerHandle.Init();
		m_iLastTime = 0;
		m_vLocation.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVLookAtNoises);

/**************************
 * NVIncapacitatedMessages
 *
 * This script sends an 'Incapacitated' message to an AI when it is
 * knocked out or slain.
 *
 * Messages: Damage, AIModeChange
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVIncapacitatedMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVIncapacitatedMessages,CustomScript);
	cScr_NVIncapacitatedMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iIncap)
	{ }

private:
	DECLARE_MSGHANDLER(OnDamage);

protected:

	script_int m_iIncap;
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("AIModeChange", cScr_NVIncapacitatedMessages::OnDamage);
	}

	virtual void InitVars(void) {
		m_iIncap.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVIncapacitatedMessages);

/**************************
 * NVConvEnhancer
 *
 * Adds some new pseudo-script actions for use with conversations, responses, etc.
 *
 * DelayedFrob: Frob the object specified in Argument 2 after the number of
 * milliseconds specified in Argument 3. Continues immediately.
 * The object will be frobbed regardless of what happens to the AI.
 *
 * GiveItem: Gives the object (or a copy of an archetype) specified in Argument 2
 * to the object specified in Argument 3 (or the Player if none is specified).
 * It can be delayed. Given objects will stack.
 *
 * SendMessage: Sends the message specified by Argument 3 to the object
 * specified in Argument 2. Can be delayed.
 *
 * Follow: Sets the AI to start following the object specified in Argument 2.
 * StopFollow: Self-explanatory. Stops the AI from following.
 *
 * CheckLock: Checks if the AI can open the specified locked object, and aborts
 * the pseodoscript if they can. (Not available in SS2)
 *
 *
 * Messages: DelayedFrob, GiveItem, SendMessage, Follow, StopFollowing
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVConvEnhancer,cScriptNV)
{
public:
	DECLARE_FACTORY(NVConvEnhancer,CustomScript);
	cScr_NVConvEnhancer(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iFollowing)
	{ }

private:
	DECLARE_MSGHANDLER(OnDelayedFrob);
	DECLARE_MSGHANDLER(OnGiveItem);
	DECLARE_MSGHANDLER(OnSendMessage);
	DECLARE_MSGHANDLER(OnSetQVar);
	#if ( _DARKGAME != 3 )
		DECLARE_MSGHANDLER(OnCheckLock);
	#endif
	DECLARE_MSGHANDLER(OnFollow);
	DECLARE_MSGHANDLER(OnFollowRecalculate);
	DECLARE_MSGHANDLER(OnStopFollowing);
	DECLARE_MSGHANDLER(OnTimer);
	static int FollowRemoveWatchObjIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("DelayedFrob", cScr_NVConvEnhancer::OnDelayedFrob);
		REGISTER_MSGHANDLER("GiveItem", cScr_NVConvEnhancer::OnGiveItem);
		REGISTER_MSGHANDLER("SendMessage", cScr_NVConvEnhancer::OnSendMessage);
		REGISTER_MSGHANDLER("SetQVar", cScr_NVConvEnhancer::OnSetQVar);
		#if ( _DARKGAME != 3 )
			REGISTER_MSGHANDLER("CheckLock", cScr_NVConvEnhancer::OnCheckLock);
		#endif
		REGISTER_MSGHANDLER("Follow", cScr_NVConvEnhancer::OnFollow);
		REGISTER_MSGHANDLER("FollowRecalculate", cScr_NVConvEnhancer::OnFollowRecalculate);
		REGISTER_MSGHANDLER("StopFollowing", cScr_NVConvEnhancer::OnStopFollowing);
		REGISTER_MSGHANDLER("Timer",cScr_NVConvEnhancer::OnTimer);
	}

	script_int m_iFollowing;


	virtual void InitVars(void)
	{
		m_iFollowing.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVConvEnhancer);

#if (_DARKGAME == 2)
/**************************
 * NVSuspiciousTrap
 * (NVTrap)
 *
 * Makes the object suspicious on TurnOn, and unsuspicious on TurnOff.
 * Any ~AISuspiciousLink or related ~AIWatchObj links are removed from
 * the object upon TurnOff.
 *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: None
 * Links: AISuspiciousLink, AIWatchObj
 */

DECLARE_SCRIPT(cScr_NVSuspiciousTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSuspiciousTrap,NVGenericTrap);
	cScr_NVSuspiciousTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	static int SuspiciousIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);
	static int WatchObjIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
	}
}
END_DECLARE_SCRIPT(cScr_NVSuspiciousTrap);
#endif // Not availiable outside Thief2


/**************************
 * NVHPTrigger
 * (NVTrigger)
 *
 * This script sends TurnOn when an object's Game->Damage Model->Hit Points
 * falls below a certain value, and TurnOff when the hit points rise above it.
 *
 * Messages: Damage
 * Parameters: NVHitPoints
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVHPTrigger,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVHPTrigger,CustomScript);
	cScr_NVHPTrigger(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId),
		SCRIPT_VAROBJ(m_iState)
	{ }

private:
	DECLARE_MSGHANDLER(OnDamage);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}


protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("Damage", cScr_NVHPTrigger::OnDamage);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVHPTrigger::OnResetTriggerCount);
	}

	script_int m_iState;

	virtual void InitVars(void)
	{
		m_iState.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVHPTrigger);

/**************************
 * NVRelayTrap
 * (NVTrap) (NVTrigger)
 *
 * This script relays TurnOn and TurnOff messages.
 * Almost the same as the standard Relay trap, except that
 * it supports the added functionality of NVGenericScript
 * and NVGenericTrigger.
 *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: None
 * Links: ControlDevice (specifiable)
 */
DECLARE_SCRIPT(cScr_NVRelayTrap, cScrNVGeneric), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVRelayTrap,NVGenericTrap);
	cScr_NVRelayTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName, iHostObjId), cScrNVTrigger(pszName, iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_iRepeat),
		SCRIPT_VAROBJ(m_bRepeatingOn)
	{
		overflowWatch = 0;
	}

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER Relay(bool bOn, sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}
	DECLARE_MSGHANDLER(OnTimer);
	void KillTimer(void);
	int cScr_NVRelayTrap::GetInterval(bool bOn);

	int overflowWatch;

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("Timer",cScr_NVRelayTrap::OnTimer);

		// This should really only be registered IF a counter is enabled, but...
		 REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVRelayTrap::OnResetTriggerCount);
	}

	script_handle<tScrTimer> m_hTimerHandle;
	script_int m_iRepeat;
	script_int m_bRepeatingOn;

	virtual void InitVars(void)
	{
		m_iRepeat.Init();
		m_bRepeatingOn.Init();
		m_hTimerHandle.Init();
		return;
	}

	void TriggerRelayTrap(bool bOn, sScrMsg* pMsg);

}
END_DECLARE_SCRIPT(cScr_NVRelayTrap);

/**************************
 * NVRelayTrap2
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap2, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap2,NVRelayTrap);
	cScr_NVRelayTrap2(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap2);

/**************************
 * NVRelayTrap3
 * (NVRelayTrap)
 *
 */

DECLARE_SCRIPT(cScr_NVRelayTrap3, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap3,NVRelayTrap);
	cScr_NVRelayTrap3(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap3);

/**************************
 * NVRelayTrap4
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap4, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap4,NVRelayTrap);
	cScr_NVRelayTrap4(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap4);

/**************************
 * NVRelayTrap5
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap5, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap5,NVRelayTrap);
	cScr_NVRelayTrap5(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap5);

/**************************
 * NVRelayTrap6
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap6, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap6,NVRelayTrap);
	cScr_NVRelayTrap6(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap6);

/**************************
 * NVRelayTrap7
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap7, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap7,NVRelayTrap);
	cScr_NVRelayTrap7(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap7);

/**************************
 * NVRelayTrap8
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap8, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap8,NVRelayTrap);
	cScr_NVRelayTrap8(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap8);

/**************************
 * NVRelayTrap9
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap9, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap9,NVRelayTrap);
	cScr_NVRelayTrap9(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap9);

/**************************
 * NVRelayTrap10
 * (NVRelayTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRelayTrap10, cScr_NVRelayTrap)
{
public:
	DECLARE_FACTORY(NVRelayTrap10,NVRelayTrap);
	cScr_NVRelayTrap10(const char* pszName, int iHostObjId) : cScr_NVRelayTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRelayTrap10);

/**************************
 * NVTweqMessages
 *
 * Compiles and sends a specific 'Tweq completed' message to its object
 * when it recieves TweqComplete.
 *
 * Messages: TweqComplete
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVTweqMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVTweqMessages,CustomScript);
	cScr_NVTweqMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTweqComplete);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("TweqComplete", cScr_NVTweqMessages::OnTweqComplete);
	}
}
END_DECLARE_SCRIPT(cScr_NVTweqMessages);


/**************************
 * NVVOTrap
 * (NVTrap)
 *
 * Plays a different VoiceOver depending on the state of the quest variable
 * specified via [NVVOQVar]; schemas a linked via ScriptParams links, and
 * the one with link data matching the value of the qvar will be played.
 * The schema linked via a SoundDescription link will be played if no matching
 * ScriptParams link exists, the QVar doesn't exist, or the [NVVOQVar] param
 * was not specified. Only ever plays ONE VoiceOver.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVVOQVar, NVPoliteVO
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVOTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVVOTrap,NVGenericTrap);
	cScr_NVVOTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVVOTrap);

/**************************
 * NVOnscreenText
 * (NVTrap)
 *
 * Loads a book string and displays both the text and the quest  variable
 * that it specifies onscreen.
 * This behaves slightly differently from a standard book;
 * Rather than simply displaying the value of the string "page_0", it
 * displays first the contents string "page_0", then the value of the
 * quest variable specified in the string "QVar", and finally the
 * contents of "page_1".
 *
 * So, for example (assuming that the quest variable TestQVar is 54),
 * the string file:
 *
 * Page_0: "At present, the qvar is "
 * QVar: "TestQVar"
 * Page_1: ". Thank you for asking."
 *
 * would be displayed as "At present, the qvar is 54. Thank you for asking."
 *
 *
 * Messages: WorldSelect, WorldDeselect, InvSelect, InvDeselect; TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVOnscreenText, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVOnscreenText,NVGenericTrap);
	cScr_NVOnscreenText(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnFocus);
	DECLARE_MSGHANDLER(OnDefocus);
	void ShowText();
	void ClearText();

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVOnscreenText::OnPreActivate);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVOnscreenText::OnPreActivate);
		REGISTER_MSGHANDLER("WorldSelect", cScr_NVOnscreenText::OnFocus);
		REGISTER_MSGHANDLER("InvSelect", cScr_NVOnscreenText::OnFocus);
		REGISTER_MSGHANDLER("WorldDeSelect", cScr_NVOnscreenText::OnDefocus);
		REGISTER_MSGHANDLER("InvDeSelect", cScr_NVOnscreenText::OnDefocus);
  }

}
END_DECLARE_SCRIPT(cScr_NVOnscreenText);

/**************************
 * NVExclusiveObject
 *
 * This script destorys any other objects of the same type
 * when it is created.
 *
 * Messages: Sim, Create
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVExclusiveObject,cScriptNV)
{
public:
	DECLARE_FACTORY(NVExclusiveObject,CustomScript);
	cScr_NVExclusiveObject(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	void Startup(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Sim", cScr_NVExclusiveObject::OnSim);
		REGISTER_MSGHANDLER("Create", cScr_NVExclusiveObject::OnCreate);
	}
}
END_DECLARE_SCRIPT(cScr_NVExclusiveObject);

/**************************
 * NVAttachMyObj
 *
 * On Sim start or creation, this script crawls back through the inheritance of
 * the object looking for the first ScriptParams link with a data of 'Attach'.
 * It then creates the object linked to and creates a
 * CreatureAttachment link from the script's object to the newly created
 * object.
 *
 * Messages: Sim, Create
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVAttachMyObj,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAttachMyObj,CustomScript);
	cScr_NVAttachMyObj(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	DECLARE_MSGHANDLER(OnEnd);
	void Startup(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Sim", cScr_NVAttachMyObj::OnSim);
		REGISTER_MSGHANDLER("Create", cScr_NVAttachMyObj::OnCreate);
		REGISTER_MSGHANDLER("Destroy", cScr_NVAttachMyObj::OnEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVAttachMyObj);

/**************************
 * NVSafeDoor
 *
 * As seen the The Haunting, but without the twenty-odd objects.
 *
 * You can have an AIWatchObj link added to the object specified by the
 * NVSafeOpenAction parameter when the safe is opened by an AI.
 * You have to remove this link yourself, possibly via "Kill Link: after trigger")
 * Use NVSafeOpenAction="[Source]" to add the link to/from the AI.
 *
 * You should probably be able to get away with simply hiding a marker inside the
 * safe and giving that an AIWatchObj link from the AI with the Line of Sight
 * requirement.
 *
 * Messages: FrobWorldBegin, FrobWorldEnd, WorldDeSelect, Timer, Sim, Create.
 * Parameters: NVSafeOpenAction
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVSafeDoor,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSafeDoor,CustomScript);
	cScr_NVSafeDoor(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iCodePosition),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobWorldBegin);
	DECLARE_MSGHANDLER(OnFrobWorldEnd);
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	DECLARE_MSGHANDLER(OnTimer);
	void Startup(void);
	int IsCorrect(bool bSnapJoint = true);
	static int DestroyLinksIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);


protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldBegin", cScr_NVSafeDoor::OnFrobWorldBegin);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVSafeDoor::OnFrobWorldEnd);
		REGISTER_MSGHANDLER("WorldDeSelect", cScr_NVSafeDoor::OnFrobWorldEnd);
		REGISTER_MSGHANDLER("Sim", cScr_NVSafeDoor::OnSim);
		REGISTER_MSGHANDLER("Create", cScr_NVSafeDoor::OnSim);
		REGISTER_MSGHANDLER("Timer",cScr_NVSafeDoor::OnTimer);
	}

	script_int m_iCodePosition;
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_iCodePosition.Init();
		m_hTimerHandle.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVSafeDoor);

#if (_DARKGAME != 3) // Not needed for SS2, plus the inventory system is completely different.
/**************************
 * NVInventoryMemory
 *
 * If placed upon the Player object (starting point, Avatar archetype, etc.), this script will
 * restore the player's inventory selections when a saved game is resumed.
 *
 * Messages: BeginScript, EndScript, Timer.
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVInventoryMemory,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInventoryMemory,CustomScript);
	cScr_NVInventoryMemory(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iCurrWeapon),
		SCRIPT_VAROBJ(m_iCurrItem)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnTimer);
	script_int m_iCurrWeapon;
	script_int m_iCurrItem;

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVInventoryMemory::OnEndScript);
		REGISTER_MSGHANDLER("Timer", cScr_NVInventoryMemory::OnTimer);
	}

	virtual void InitVars(void)
	{
		m_iCurrWeapon.Init();
		m_iCurrItem.Init();
		return;
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVInventoryMemory);
#endif // Non-SS2

/**************************
 * NVInvTransform
 *
 * When frobbed in inventory, this object creates another object and
 * gives it to the player. Or something.
 *
 * Messages: TurnOn (specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVInvTransform,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInvTransform,CustomScript);
	cScr_NVInvTransform(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobInvEnd);

protected:
	virtual long Transform(int iFrobber);
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVInvTransform::OnFrobInvEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVInvTransform);

/**************************
 * NVFirerHack
 *
 * This script allows the player to hit himself with his own arrows.
 *
 * Messages: Create, PhysMadePhysical
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVFirerHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVFirerHack,CustomScript);
	cScr_NVFirerHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnThrow);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("PhysMadePhysical", cScr_NVFirerHack::OnThrow);
		REGISTER_MSGHANDLER("Create", cScr_NVFirerHack::OnThrow);
		REGISTER_MSGHANDLER("Timer",cScr_NVFirerHack::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVFirerHack);

/**************************
 * NVNewWeapon
 *
 * This script allows the creation of new weapons.
 * It adds the metaproperty specified by NVWeaponMeta to the player arm object
 * which is created when the weapon is equipped.
 * It also send the 'InitArm' message to the player arm, after the metaprop has been added.
 * It can also attach various objects to the arm via the Renderer->Mesh Attach property.
 *
 * Never try to attach an object without first setting up the Renderer->Mesh Attach property!
 *
 * Now works in SS2, and has a variant, NVGun, for sending gun-related messages!
 *
 * Messages: InvSelect, InvDeselect, Timer
 * Parameters: NVWeaponMeta, NVWeaponAttach1, NVWeaponAttach2, NVWeaponAttach3, NVWeaponAttach4
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVNewWeapon,cScriptNV)
{
public:
	DECLARE_FACTORY(NVNewWeapon,CustomScript);
#if (_DARKGAME != 	3)
	cScr_NVNewWeapon(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
#else
	cScr_NVNewWeapon(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iEquipped)
#endif
	{
		m_iListenerHandle = -1;
		m_iListenFor = 0;
	}

	virtual ~cScr_NVNewWeapon();

private:
	DECLARE_MSGHANDLER(OnForceInit);
	#if (_DARKGAME == 3)
		DECLARE_MSGHANDLER(OnContained);
	#endif

	static int DestroyAttachedIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);

	static void __cdecl ObjListener(int iObj, unsigned long uEvent, void* pData);

	int m_iListenerHandle;
	int m_iListenFor;

protected:
	DECLARE_MSGHANDLER(OnInvSelect);
	DECLARE_MSGHANDLER(OnInvDeselect);

	virtual void InitArm(object oWeap);
	virtual void ClearArm(object oWeap);
	virtual void ForceInit(void);
	virtual void ForcePutAway(void);

	#if (_DARKGAME == 3)
	script_int m_iEquipped;
	#endif

	void StartListener(void);

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		#if (_DARKGAME != 3)
			REGISTER_MSGHANDLER("InvSelect", cScr_NVNewWeapon::OnInvSelect);
			REGISTER_MSGHANDLER("InvDeselect", cScr_NVNewWeapon::OnInvDeselect);
		#else
			REGISTER_MSGHANDLER("Contained", cScr_NVNewWeapon::OnContained);
			REGISTER_MSGHANDLER("Equipped", cScr_NVNewWeapon::OnForceInit);
			REGISTER_MSGHANDLER("Unequipped", cScr_NVNewWeapon::OnInvDeselect);
		#endif
		REGISTER_MSGHANDLER("ForceInit", cScr_NVNewWeapon::OnForceInit);
	}
	#if (_DARKGAME == 3)
	virtual void InitVars(void)
	{
		m_iEquipped.Init();
	}
	#endif

}
END_DECLARE_SCRIPT(cScr_NVNewWeapon);

#if (_DARKGAME == 3)
/**************************
 * NVGun
 * (NVNewWeapon)
 * (System Shock 2 only)
 *
 * Sends gun-related messages:
 * Equipped, Unequipped, AmmoUp, AmmoDown, ConditionUp, ConditionDown,
 * GunBreak [ condition becomes 0.00 ], SettingChange...
 * and a generic "GunStateChange" message that is triggered by all of the above
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVGun,cScr_NVNewWeapon)
{
public:
	DECLARE_FACTORY(NVGun,CustomScript);
	cScr_NVGun(const char* pszName, int iHostObjId)
		: cScr_NVNewWeapon(pszName, iHostObjId),
		m_pProp(NULL),
		SCRIPT_VAROBJ(m_iAmmo),
		SCRIPT_VAROBJ(m_fCondition),
		SCRIPT_VAROBJ(m_iSetting),
		SCRIPT_VAROBJ(m_iModification),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{
		m_hListenHandle = NULL;
	}

	virtual ~cScr_NVGun();

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	static void __stdcall ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData);
	PropListenerHandle m_hListenHandle;
	IGenericProperty* m_pProp;
	void StartGunStateListener();
	void StopGunStateListener();
	virtual void InitArm(object oWeap);
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnGunStateChange);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnEquipped);
	DECLARE_MSGHANDLER(OnUnequipped);
	DECLARE_MSGHANDLER(OnGetGunInfo);
	DECLARE_MSGHANDLER(OnModify);
	void FinishCooldown(void);
	void SettingChanged(int iSetting);

protected:

	script_int m_iAmmo;
	script_float m_fCondition;
	script_int m_iSetting;
	script_int m_iModification;
	script_handle<tScrTimer> m_hTimerHandle;

	int m_iStrength;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_iAmmo.Init();
		m_fCondition.Init();
		m_iSetting.Init();
		m_iModification.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScr_NVNewWeapon::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVGun::OnEndScript);
		REGISTER_MSGHANDLER("GunStateChange", cScr_NVGun::OnGunStateChange);
		REGISTER_MSGHANDLER("Timer", cScr_NVGun::OnTimer);
		REGISTER_MSGHANDLER("Equipped", cScr_NVGun::OnEquipped);
		REGISTER_MSGHANDLER("Unequipped", cScr_NVGun::OnUnequipped);
		REGISTER_MSGHANDLER("GetGunInfo", cScr_NVGun::OnGetGunInfo);
		REGISTER_MSGHANDLER("NVModify1", cScr_NVGun::OnModify);
		REGISTER_MSGHANDLER("NVModify2", cScr_NVGun::OnModify);
	}
}
END_DECLARE_SCRIPT(cScr_NVGun);
#endif

/**************************
 * NVMoveOBB
 *
 * This script allows the player to pick up OBB objects and then
 * put them down or throw them without them floating away.
 *
 * Problem: They stay where they are put, not matter what.
 * Even if, for example, an object they should be balancing
 * on is (re)moved.
 *
 * I'll keep this script for now, anyway, but won't document it.
 *
 * Messages: PhysFellAsleep, Contained
 * Parameters: None
 * Links: Firer
 */
DECLARE_SCRIPT(cScr_NVMoveOBB,cScriptNV)
{
public:
	DECLARE_FACTORY(NVMoveOBB,CustomScript);
	cScr_NVMoveOBB(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnDrop);
	DECLARE_MSGHANDLER(OnGrab);
	DECLARE_MSGHANDLER(OnEndScript);
	MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVMoveOBB::OnEndScript);
		REGISTER_MSGHANDLER("PhysFellAsleep", cScr_NVMoveOBB::OnDrop);
		REGISTER_MSGHANDLER("Contained", cScr_NVMoveOBB::OnGrab);
	}
}
END_DECLARE_SCRIPT(cScr_NVMoveOBB);


/**************************
 * NVThrowingKnife
 * (NVInvTransform)
 *
 * Used in conjunction with the standard 'Blackjack' script to make throwable
 * weapons. Basically, it copies the Tweq->Emit property from the object to
 * the player and activates it when the weapon is 'swung'.
 * It also decreases the stack count and changes the arm model to
 * 'Unarmed'.
 *
 * You can specify which set of Tweq properties to use via the
 * NVKnifeTweqNum parameter.
 *
 * Messages: PhysMadePhysical, FrobInvEnd, InvSelect, Timer
 * Parameters: NVKnifeTweqNum
 * Links: None
 */

 DECLARE_SCRIPT(cScr_NVThrowingKnife,cScr_NVInvTransform)
{
public:
	DECLARE_FACTORY(NVThrowingKnife,NVInvTransform);
	cScr_NVThrowingKnife(const char* pszName, int iHostObjId)
		: cScr_NVInvTransform(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnDeselect);
	DECLARE_MSGHANDLER(OnSelect);
	DECLARE_MSGHANDLER(OnThrow);
	#if ( _DARKGAME != 3 )
	DECLARE_MSGHANDLER(OnFrobInvEnd);
	#endif
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScr_NVInvTransform::InitScriptData();
		REGISTER_MSGHANDLER("PhysMadePhysical", cScr_NVThrowingKnife::OnThrow);
		#if ( _DARKGAME != 3 )
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVThrowingKnife::OnFrobInvEnd);
		#endif
		REGISTER_MSGHANDLER("InvSelect", cScr_NVThrowingKnife::OnSelect);
		REGISTER_MSGHANDLER("InvDeselect", cScr_NVThrowingKnife::OnDeselect);
		REGISTER_MSGHANDLER("Timer",cScr_NVThrowingKnife::OnTimer);
	}

	script_handle<tScrTimer> m_hTimerHandle;
	char m_cThrowing;

	virtual void InitVars(void)
	{
		m_cThrowing = 0;
		m_hTimerHandle.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVThrowingKnife);

#if (_DARKGAME != 3) // Not needed for SS2
/**************************
 * NVKeyringKey
 *
 * Handles the combination of keys into a keyring object.
 *
 * Messages: Contained
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVKeyringKey,cScriptNV)
{
public:
	DECLARE_FACTORY(NVKeyringKey,CustomScript);
	cScr_NVKeyringKey(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnGrab);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Contained", cScr_NVKeyringKey::OnGrab);
		REGISTER_MSGHANDLER("Timer",cScr_NVKeyringKey::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVKeyringKey);
#endif // Non-SS2

#if (_BUILD != 1 )
/**************************
 * NVMirrorHack
 *
 * Don't ask.
 *
 * Messages: Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMirrorHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVMirrorHack,CustomScript);
	cScr_NVMirrorHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnMirrorImageMotionEnd);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVMirrorHack::OnTimer);
		REGISTER_MSGHANDLER("Sim", cScr_NVMirrorHack::OnSim);
		REGISTER_MSGHANDLER("MirrorImageMotionEnd", cScr_NVMirrorHack::OnMirrorImageMotionEnd);
	}

	script_handle<tScrTimer> m_hTimerHandle;
	int m_iMotion;
	int  m_iWeapon;

	virtual void InitVars(void)
	{
		m_iMotion = 0;
		m_iWeapon = 0;
		m_hTimerHandle.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVMirrorHack);

/**************************
 * NVMirrorImage
 *
 * A part of NVMirrorHack
 *
 * Messages: MotionEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMirrorImage,cScriptNV)
{
public:
	DECLARE_FACTORY(NVMirrorImage,CustomScript);
	cScr_NVMirrorImage(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnMotionEnd);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("MotionEnd", cScr_NVMirrorImage::OnMotionEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVMirrorImage);
#endif // _BUILD != 1

/**************************
 * NVItemGiver
 * (NVTrap)
 *
 * When frobbed in inventory, this object creates another object and
 * gives it to the player.
 *
 * Messages: TurnOn (specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVItemGiver, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVItemGiver,NVGenericTrap);
	cScr_NVItemGiver(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVItemGiver);



/**************************
 * NVInvAssembly
 *
 * Used to assemble objects from multiple parts in inventory.
 *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVInvAssembly,cScrNVGeneric), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVInvAssembly,NVGenericTrap);
	cScr_NVInvAssembly(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobInvEnd);

	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}


	void Assemble(int iFrobber, sScrMsg* pMsg);

protected:
	virtual void InitScriptData()
	{
		cScrNVTrigger::InitScriptData();
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVInvAssembly::OnFrobInvEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVInvAssembly);

/**************************
 * NVInvAssembly2
 *
 * Used to assemble objects from multiple parts in inventory.
 *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVInvAssembly2,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInvAssembly2,CustomScript);
	cScr_NVInvAssembly2(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnToolFrob);


protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobToolEnd", cScr_NVInvAssembly2::OnToolFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVInvAssembly2);


/**************************
 * NVHolyH2OHack
 * (NVInvTransform)
 *
 * Allows the player to throw his HolyH2O flasks if he has no water arrows.
 *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVHolyH2OHack,cScr_NVInvTransform)
{
public:
	DECLARE_FACTORY(NVHolyH2OHack,NVInvTransform);
	cScr_NVHolyH2OHack(const char* pszName, int iHostObjId)
		: cScr_NVInvTransform(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobInvEnd);

protected:
	virtual void InitScriptData()
	{
		cScr_NVInvTransform::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVHolyH2OHack::OnFrobInvEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVHolyH2OHack);

/**************************
 * NVSpy
 *
 * Just a quick Spy script to include in the NVScript module.
 *
 * Messages: All
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVSpy,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSpy,CustomScript);
	cScr_NVSpy(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	STDMETHOD_(long,ReceiveMessage)(sScrMsg*,sMultiParm*,eScrTraceAction);
	DECLARE_MSGHANDLER(OnEndScript);


protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVSpy::OnEndScript);
	}
}
END_DECLARE_SCRIPT(cScr_NVSpy);


/**************************
 * NVTrapConverse
 * (NVTrap)
 *
 * TrapConverse. Only it's an NVTrap.
 * Useful, eh?
 * (In truth, this is mainly of use for SS2)
 *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: None
 * Links: None (AIConversationActor links are used by conversations, though...)
 */
DECLARE_SCRIPT(cScr_NVTrapConverse, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVTrapConverse,NVGenericTrap);
	cScr_NVTrapConverse(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing
}
END_DECLARE_SCRIPT(cScr_NVTrapConverse);

/**************************
 * NVAirlock
 * (NVTrap)
 *
 * This script controls two airlock doors, both linked from it with ControlDevice
 * (or SwitchLink) links.
 * First, it sends TurnOn down ScriptParams links with data of "Alarm";
 * then it sends closes both its doors.
 * Once the doors are closed, it sends TurnOn down ScriptParams links with
 * data of "Vent", waits for the number of milliseconds specified by [NVAirlockDelay]
 * (The defaults is 5000ms), turns off the vent and then opens the door that was
 * originally closed. Once this door is open, TurnOff is sent to the Alarm.
 *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: NVAirlockDelay
 * Links: ControlDevice, ScriptParams ("Alarm"), ScriptParams ("Vent")
 */
DECLARE_SCRIPT(cScr_NVAirlock, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVAirlock,NVGenericTrap);
	cScr_NVAirlock(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_iDoorOne),
		SCRIPT_VAROBJ(m_iDoorTwo),
		SCRIPT_VAROBJ(m_iNextDoor)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	void KillTimer(void);
	DECLARE_MSGHANDLER(OnTimer);
	static int LinkIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVAirlock::OnTimer);
	}

	script_handle<tScrTimer> m_hTimerHandle;
	script_int m_iDoorOne;
	script_int m_iDoorTwo;
	script_int m_iNextDoor;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_iDoorOne.Init();
		m_iDoorTwo.Init();
		m_iNextDoor.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVAirlock);

/**************************
 * NVDoorStartsOpen
 *
 * This script causes doors to open VERY QUICKLY when the
 * Sim starts (or when they are created ingame)
 *
 * Messages: Sim, Create
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVDoorStartsOpen,cScriptNV)
{
public:
	DECLARE_FACTORY(NVDoorStartsOpen,CustomScript);
	cScr_NVDoorStartsOpen(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_fDoorSpeed),
		SCRIPT_VAROBJ(m_iDoorOpened)
	{ }

private:
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	DECLARE_MSGHANDLER(OnDoorOpen);
	void Startup(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Sim", cScr_NVDoorStartsOpen::OnSim);
		REGISTER_MSGHANDLER("Create", cScr_NVDoorStartsOpen::OnCreate);
		REGISTER_MSGHANDLER("DoorOpen", cScr_NVDoorStartsOpen::OnDoorOpen);
	}

	script_float m_fDoorSpeed;
	script_int m_iDoorOpened;

	virtual void InitVars(void)
	{
		m_fDoorSpeed.Init();
		m_iDoorOpened.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVDoorStartsOpen);

#if (_DARKGAME != 3) // SS2 handles the inventory completely differently
/**************************
 * NVCursedObj
 *
 * This script prevents the player from un-equiping its object.
 *
 * Messages: InvDeselect, InvDeFocus, Contained, Timer
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVCursedObj,cScriptNV)
{
public:
	DECLARE_FACTORY(NVCursedObj,CustomScript);
	cScr_NVCursedObj(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnInvDeselect);
	DECLARE_MSGHANDLER(OnGrab);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnInvDeFocus);
	void Curse(void);
	void CurseMessage(void);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("InvDeSelect", cScr_NVCursedObj::OnInvDeselect);
		REGISTER_MSGHANDLER("InvDeFocus", cScr_NVCursedObj::OnInvDeFocus);
		REGISTER_MSGHANDLER("Contained", cScr_NVCursedObj::OnGrab);
		REGISTER_MSGHANDLER("Timer", cScr_NVCursedObj::OnTimer);
	}
	script_handle<tScrTimer> m_hTimerHandle;
	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVCursedObj);
#endif // Non-SS2

/**************************
 * NVTrapSetQVar
 * (NVTrap)
 *
 * A replacement for the standard TrapSetQVar script.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVTrapSetQVar, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVTrapSetQVar,NVGenericTrap);
	cScr_NVTrapSetQVar(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVTrapSetQVar);

/**************************
 * NVTrigQVar
 * (NVTrigger)
 *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters:
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVTrigQVar,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVTrigQVar,CustomScript);
	cScr_NVTrigQVar(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId),
		SCRIPT_VAROBJ(m_iPrev),
		SCRIPT_VAROBJ(m_iPrevResult)
	{
		m_szQVar = NULL;
	}

	virtual ~cScr_NVTrigQVar()
	{
		if ( m_szQVar != NULL)
		{
			delete[] m_szQVar;
		}
	}

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnQuestChange);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	char* m_szQVar;

	virtual void InitVars(void)
	{
		GetQVarName();
		m_iPrev.Init(-1);
		m_iPrevResult.Init(-1);
		return;
	}

	void GetQVarName();

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVTrigQVar::OnEndScript);
		REGISTER_MSGHANDLER("QuestChange", cScr_NVTrigQVar::OnQuestChange);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVTrigQVar::OnResetTriggerCount);
	}

	void CheckQVar(sScrMsg* pMsg);

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	script_int m_iPrev;
	script_int m_iPrevResult;
}
END_DECLARE_SCRIPT(cScr_NVTrigQVar);


/**************************
 * NVTweqFirstDoor
 *
 * By request from Mortal Monkey
 *
 * Messages: FrobWorldEnd
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVTweqFirstDoor,cScriptNV)
{
public:
	DECLARE_FACTORY(NVTweqFirstDoor,CustomScript);
	cScr_NVTweqFirstDoor(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	DECLARE_MSGHANDLER(OnClosed);
	DECLARE_MSGHANDLER(OnTweqComplete);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVTweqFirstDoor::OnFrob);
		REGISTER_MSGHANDLER("DoorClose", cScr_NVTweqFirstDoor::OnClosed);
		REGISTER_MSGHANDLER("TweqComplete", cScr_NVTweqFirstDoor::OnTweqComplete);
	}
}
END_DECLARE_SCRIPT(cScr_NVTweqFirstDoor);

/**************************
 * NVActivatorRoom
 *
 * Used to activate / deactivate various effects based on
 * which room the player is in.
 *
 * The idea here is to turn off complicated scripts when they are
 * not in use.
 *
 * Messages: PlayerRoomEnter, PlayerRoomExit
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVActivatorRoom,cScriptNV)
{
public:
	DECLARE_FACTORY(NVActivatorRoom,CustomScript);
	cScr_NVActivatorRoom(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnPlayerRoomEnter);
	DECLARE_MSGHANDLER(OnPlayerRoomExit);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("PlayerRoomEnter", cScr_NVActivatorRoom::OnPlayerRoomEnter);
		REGISTER_MSGHANDLER("PlayerRoomExit", cScr_NVActivatorRoom::OnPlayerRoomExit);
	}
}
END_DECLARE_SCRIPT(cScr_NVActivatorRoom);

/**************************
 * NVTrigRoomPlayer
 * (NVTrigger)
  *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVTrigRoomPlayer,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVTrigRoomPlayer,CustomScript);
	cScr_NVTrigRoomPlayer(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnPlayerRoomEnter);
	DECLARE_MSGHANDLER(OnPlayerRoomExit);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();

		REGISTER_MSGHANDLER("PlayerRoomEnter", cScr_NVTrigRoomPlayer::OnPlayerRoomEnter);
		REGISTER_MSGHANDLER("PlayerRoomExit", cScr_NVTrigRoomPlayer::OnPlayerRoomExit);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVTrigRoomPlayer::OnResetTriggerCount);
	}
}
END_DECLARE_SCRIPT(cScr_NVTrigRoomPlayer);

/**************************
 * NVObjectWatcher
 *
 * A script to make an object constantly rotate so
 * that it is always facing another object.
 *
 * Messages: NVActivate, NVDeactivate
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVObjectWatcher,cScriptNV)
{
public:
	DECLARE_FACTORY(NVObjectWatcher,CustomScript);
	cScr_NVObjectWatcher(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnActivated);
	DECLARE_MSGHANDLER(OnDeactivated);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars()
	{
		m_hTimerHandle.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("NVActivate", cScr_NVObjectWatcher::OnActivated);
		REGISTER_MSGHANDLER("NVDeactivate", cScr_NVObjectWatcher::OnDeactivated);
		REGISTER_MSGHANDLER("Timer",cScr_NVObjectWatcher::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVObjectWatcher);


/**************************
 * NVMetaTrap
 * (NVTrap)
 *
 * Adds/removes a metaproperty on TurnOn/TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMetaTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVMetaTrap,NVGenericTrap);
	cScr_NVMetaTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	void DoMeta(bool bOn, sScrMsg* pMsg);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	 virtual void InitScriptData()
	 {
		 cScrNVGeneric::InitScriptData();
		 REGISTER_MSGHANDLER("Timer", cScr_NVMetaTrap::OnTimer);
	 }

}
END_DECLARE_SCRIPT(cScr_NVMetaTrap);

/**************************
 * NVMetaTrap2
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap2, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap2,NVMetaTrap);
	cScr_NVMetaTrap2(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap2);


/**************************
 * NVMetaTrap3
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap3, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap3,NVMetaTrap);
	cScr_NVMetaTrap3(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap3);


/**************************
 * NVMetaTrap4
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap4, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap4,NVMetaTrap);
	cScr_NVMetaTrap4(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap4);

/**************************
 * NVMetaTrap5
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap5, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap5,NVMetaTrap);
	cScr_NVMetaTrap5(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap5);

/**************************
 * NVMetaTrap6
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap6, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap6,NVMetaTrap);
	cScr_NVMetaTrap6(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap6);

/**************************
 * NVMetaTrap7
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap7, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap7,NVMetaTrap);
	cScr_NVMetaTrap7(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap7);

/**************************
 * NVMetaTrap8
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap8, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap8,NVMetaTrap);
	cScr_NVMetaTrap8(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap8);

/**************************
 * NVMetaTrap9
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap9, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap9,NVMetaTrap);
	cScr_NVMetaTrap9(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap9);

/**************************
 * NVMetaTrap10
 * (NVMetaTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMetaTrap10, cScr_NVMetaTrap)
{
public:
	DECLARE_FACTORY(NVMetaTrap10,NVMetaTrap);
	cScr_NVMetaTrap10(const char* pszName, int iHostObjId)
		: cScr_NVMetaTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVMetaTrap10);


/**************************
 * NVParticleGroup
 * (NVTrap)
 *
 * Turns particles on/off on TurnOn/TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVParticleGroup, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVParticleGroup,NVGenericTrap);
	cScr_NVParticleGroup(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVParticleGroup);

/**************************
 * NVUnusableUnlessQVar
  *
 * Does fire damage
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVUnusableUnlessQVar,cScriptNV)
{
public:
	DECLARE_FACTORY(NVUnusableUnlessQVar,CustomScript);
	cScr_NVUnusableUnlessQVar(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{
		m_szQVar = NULL;
	}

	virtual ~cScr_NVUnusableUnlessQVar()
	{
		if ( m_szQVar )
		{
			delete[] m_szQVar;
		}
	}

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnQuestChange);

protected:
	char* m_szQVar;

	virtual void InitVars(void)
	{
		GetQVarName();
	}

	void GetQVarName();

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVUnusableUnlessQVar::OnEndScript);
		REGISTER_MSGHANDLER("QuestChange", cScr_NVUnusableUnlessQVar::OnQuestChange);
		REGISTER_MSGHANDLER("InitNVUnusableUnlessQVar", cScr_NVUnusableUnlessQVar::OnQuestChange);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVUnusableUnlessQVar);

/**************************
 * NVTweqDevice
 * (NVTrap)
 *
 * Turns an objects tweqs on/off on TurnOn/TurnOff
 * Also plays schemas with tags Event Activate on TurnOn
 * and Event Deactivate on TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVTweqDevice, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVTweqDevice,NVGenericTrap);
	cScr_NVTweqDevice(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing
	virtual void InitScriptData()
	 {
		 cScrNVGeneric::InitScriptData();
	 }

}
END_DECLARE_SCRIPT(cScr_NVTweqDevice);

/**************************
 * NVTrigOBB
 * (NVTrigger)
 *
 * Replacement boundstrigger script.
 *
 *
 * Messages: PhysEnter, PhysExit, EndScript, BeginScript (used to susbscribe to physics messages), TurnOn, TurnOff (specifiable)
 * Parameters: NVTriggOBBPlayer
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVTrigOBB,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVTrigOBB,CustomScript);
	cScr_NVTrigOBB(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnPhysEnter);
	DECLARE_MSGHANDLER(OnPhysExit);
	DECLARE_MSGHANDLER(OnEndScript);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVTrigOBB::OnEndScript);
		REGISTER_MSGHANDLER("PhysEnter", cScr_NVTrigOBB::OnPhysEnter);
		REGISTER_MSGHANDLER("PhysExit", cScr_NVTrigOBB::OnPhysExit);
		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVTrigOBB::OnResetTriggerCount);

	}

	bool isMatchingObject(object oTransObj);

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

}
END_DECLARE_SCRIPT(cScr_NVTrigOBB);


/**************************
 * NVMachineSlot
 *
 * Used on input slots for machines
 *
 * Messages: BeginScript, EndScript, PhysContactCreate
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVMachineSlot,cScriptNV)
{
public:
	DECLARE_FACTORY(NVMachineSlot,CustomScript);
	cScr_NVMachineSlot(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysContactCreate);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVMachineSlot::OnEndScript);
		REGISTER_MSGHANDLER("PhysContactCreate", cScr_NVMachineSlot::OnPhysContactCreate);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVMachineSlot);

/**************************
 * NVMachine
 * (NVTrap)
 *
 * Used for manufacturing items.
  *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMachine, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVMachine,NVGenericTrap);
	cScr_NVMachine(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVMachine);

/**************************
 * NVDifficultyRelay
 * (NVTrap)
 *
 * Used to relay TurnOn and TurnOff along ScriptParams links depending on the difficulty
 *
 * Relays TurnOn or TurnOff messages along ScriptParams links depending on data.
 * Messages will only be sent along links that have the data "Difficulty#", where #
 * matches the currently set difficulty.
 * Not technically an NVTrigger, but supports message customisation like one.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVDifficultyRelayTOn, NVDifficultyRelayTOff
 * Links: ScriptParams
 */

DECLARE_SCRIPT(cScr_NVDifficultyRelay, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVDifficultyRelay,NVGenericTrap);
	cScr_NVDifficultyRelay(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	void InterateDifficultyLinks(bool bOn);

protected:
	// Nothing
}
END_DECLARE_SCRIPT(cScr_NVDifficultyRelay);


/**************************
 * NVLaserBeam
 *
 * Used to stretch an object between two points
 *
 * Messages: BeginScript, EndScript, PhysContactCreate
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVLaserBeam,cScriptNV)
{
public:
	DECLARE_FACTORY(NVLaserBeam,CustomScript);
	cScr_NVLaserBeam(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVLaserBeam::OnEndScript);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVLaserBeam::OnPhysCollision);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVLaserBeam);


/**************************
 * NVReflector
 *
 * Used on input slots for machines
 *
 * Messages: BeginScript, EndScript, PhysContactCreate
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVReflector,cScriptNV)
{
public:
	DECLARE_FACTORY(NVReflector,CustomScript);
	cScr_NVReflector(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer",cScr_NVReflector::OnTimer);
		REGISTER_MSGHANDLER("EndScript", cScr_NVReflector::OnEndScript);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVReflector::OnPhysCollision);
	}

	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return ;
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVReflector);

/**************************
 * NVAmbientSoundTrap
 * (NVTrap)
 *
 * Plays a sound as an ambient sound.
 * Can be turned on / off unlike the default AmbientSounds script.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: SoundDescription
 */

DECLARE_SCRIPT(cScr_NVAmbientSoundTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVAmbientSoundTrap,NVGenericTrap);
	cScr_NVAmbientSoundTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVAmbientSoundTrap);

/**************************
 * NVAttachOnCollide
 *
 * Creates and ParticleAttaches an object to its object
 * when it is hit by another object.
 *
 * Messages: BeginScript, EndScript, PhysContactCreate
 * Parameters: NVAttachOnCollide1, NVAttachOnCollide2, NVAttachOnCollider
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVAttachOnCollide,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAttachOnCollide,CustomScript);
	cScr_NVAttachOnCollide(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVAttachOnCollide::OnEndScript);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVAttachOnCollide::OnPhysCollision);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVAttachOnCollide);


/**************************
 * NVSortPuzzle
 *
 * Sort puzzle script.
 * Should be ControlDevice linked to a RequireAllTrap that
 * is placed in the one empty space.
 *
 *
 * Messages: FrobWorldEnd... TurnOn, TurnOff
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSortPuzzle,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSortPuzzle,CustomScript);
	cScr_NVSortPuzzle(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_vDest)
	{ }

private:
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnFrob);
	void Startup(void);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Sim", cScr_NVSortPuzzle::OnSim);
		REGISTER_MSGHANDLER("Create", cScr_NVSortPuzzle::OnCreate);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVSortPuzzle::OnFrob);
		REGISTER_MSGHANDLER("Timer", cScr_NVSortPuzzle::OnTimer);
	}

	script_handle<tScrTimer> m_hTimerHandle;
	script_vec m_vDest;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_vDest.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVSortPuzzle);


/**************************
 * NVRotatable
 *
 * Allows its object to be rotated.
 *
 *
 * Messages: FrobWorldBegin, FrobWorldEnd, WorldDeFocus
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVRotatable,cScriptNV)
{
public:
	DECLARE_FACTORY(NVRotatable,CustomScript);
	cScr_NVRotatable(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_hSpeedTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnStart);
	DECLARE_MSGHANDLER(OnStop);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	script_handle<tScrTimer> m_hTimerHandle;
	script_handle<tScrTimer> m_hSpeedTimerHandle;

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVRotatable::OnTimer);
		REGISTER_MSGHANDLER("FrobWorldBegin", cScr_NVRotatable::OnStart);
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVRotatable::OnStop);
		REGISTER_MSGHANDLER("WorldDeSelect", cScr_NVRotatable::OnStop);
	}

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_hSpeedTimerHandle.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVRotatable);

/**************************
 * NVStackTrap
 * (NVTrap)
 *
 * Adds 1 to the stack count of the object on TurnOn; removes 1 on Turnoff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVStackTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVStackTrap,NVGenericTrap);
	cScr_NVStackTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVStackTrap);

/**************************
 * NVSpikePitRoom
 *
 *
 *
 * Messages: PlayerRoomEnter, PlayerRoomExit
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVSpikePitRoom,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVSpikePitRoom,CustomScript);
	cScr_NVSpikePitRoom(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnPlayerRoomEnter);
	DECLARE_MSGHANDLER(OnPlayerRoomExit);
	DECLARE_MSGHANDLER(OnTimer);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();

		REGISTER_MSGHANDLER("PlayerRoomEnter", cScr_NVSpikePitRoom::OnPlayerRoomEnter);
		REGISTER_MSGHANDLER("PlayerRoomExit", cScr_NVSpikePitRoom::OnPlayerRoomExit);
		REGISTER_MSGHANDLER("Timer", cScr_NVSpikePitRoom::OnTimer);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVSpikePitRoom::OnResetTriggerCount);
	}

	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVSpikePitRoom);

/**************************
 * NVParticleTrailProjectile
 *
 *
 * Messages: BeginScript, EndScript, PhysContactCreate
 * Parameters:
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVParticleTrailProjectile,cScriptNV)
{
public:
	DECLARE_FACTORY(NVParticleTrailProjectile,CustomScript);
	cScr_NVParticleTrailProjectile(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVParticleTrailProjectile::OnEndScript);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVParticleTrailProjectile::OnPhysCollision);
		REGISTER_MSGHANDLER("Timer",cScr_NVParticleTrailProjectile::OnTimer);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVParticleTrailProjectile);


/**************************
 * NVCloneContactFrob
 *
 * Copy of CloneContactFrob
 *
 *
 * Messages: FrobWorldEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVCloneContactFrob,cScriptNV)
{
public:
	DECLARE_FACTORY(NVCloneContactFrob,CustomScript);
	cScr_NVCloneContactFrob(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVCloneContactFrob::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVCloneContactFrob);


/**************************
 * NVPhantomTrap
 * (NVTrap)
 *
 * Fades the object in and out on TurnOn and TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVPhantomAlphaMax, NVPhantomAlphaMin, NVPhantomFadeOn, NVPhantomFadeOff
 * Links: None
 */
/*
DECLARE_SCRIPT(cScr_NVPhantomTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVPhantomTrap,NVGenericTrap);
	cScr_NVPhantomTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	void DoFade(bool bOn, int messageSource);

protected:
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVPhantomTrap::OnTimer);
	}
	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVPhantomTrap);
*/

/**************************
 * NVTwoSpeedDoor
 *
 * This script allows a door to have a different opening speed to its closing speed.
 *
 *
 * Messages: DoorOpening, DoorClosing
 * Parameters: NVDoorOpeningSpeed, NVDoorClosingSpeed
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVTwoSpeedDoor,cScriptNV)
{
public:
	DECLARE_FACTORY(NVTwoSpeedDoor,CustomScript);
	cScr_NVTwoSpeedDoor(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnDoorOpening);
	DECLARE_MSGHANDLER(OnDoorClosing);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("DoorOpening", cScr_NVTwoSpeedDoor::OnDoorOpening);
		REGISTER_MSGHANDLER("DoorClosing", cScr_NVTwoSpeedDoor::OnDoorClosing);
	}

}
END_DECLARE_SCRIPT(cScr_NVTwoSpeedDoor);


/**************************
 * NVTextureTrap
 * (NVTrap)
 *
 * Multiple texture-change trap gadget
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVTextureTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVTextureTrap,NVGenericTrap);
	cScr_NVTextureTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_iStage)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	void DoTexture(void);

protected:
	script_handle<tScrTimer> m_hTimerHandle;
	script_int m_iStage;

	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("Timer",cScr_NVTextureTrap::OnTimer);
	}
	virtual void InitVars(void)
	{
		m_iStage.Init();
		m_hTimerHandle.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVTextureTrap);

/**************************
 * NVSimpleSchemaPlayerTrap
 * (NVTrap)
 *
 * Plays a schema
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVSchema
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSimpleSchemaPlayerTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSimpleSchemaPlayerTrap,NVGenericTrap);
	cScr_NVSimpleSchemaPlayerTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVSimpleSchemaPlayerTrap);


/**************************
 * NVDiminishingParticles
 * (NVTrap)
 *
 * Particles diminish over time, then finally slay.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVDiminishingParticles, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVDiminishingParticles,NVGenericTrap);
	cScr_NVDiminishingParticles(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	virtual void InitScriptData()
	{
		REGISTER_MSGHANDLER("Timer",cScr_NVDiminishingParticles::OnTimer);
		cScrNVGeneric::InitScriptData();
	}

}
END_DECLARE_SCRIPT(cScr_NVDiminishingParticles);


/**************************
 * NVDeleteTrap
 * (NVTrap)
 *
 * Destroys all CD linked objects on TurnOn.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVDeleteTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVDeleteTrap,NVGenericTrap);
	cScr_NVDeleteTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVDeleteTrap);


/**************************
 * NVSetEmissionRateHack
 *
 * Hcak to set the emission rate of an object while another object is being frobbed.
 *
 *
 * Messages: FrobWorldBegin, FrobWorldEnd, WorldDeFocus
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSetEmissionRateHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSetEmissionRateHack,CustomScript);
	cScr_NVSetEmissionRateHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnStart);
	DECLARE_MSGHANDLER(OnStop);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("StartRotate", cScr_NVSetEmissionRateHack::OnStart);
		REGISTER_MSGHANDLER("StopRotate", cScr_NVSetEmissionRateHack::OnStop);
	}
}
END_DECLARE_SCRIPT(cScr_NVSetEmissionRateHack);

/**************************
 * NVTurningSquare
 *
 * Has anyone ever played the very old Atari ST (also DOS) game 'Bloodwych'?
 *
 * Messages: PhysEnter, PhysExit, EndScript, BeginScript (used to susbscribe to physics messages)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVTurningSquare,cScriptNV)
{
public:
	DECLARE_FACTORY(NVTurningSquare,CustomScript);
	cScr_NVTurningSquare(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_vSize)
	{ }

private:
	DECLARE_MSGHANDLER(OnSim);
	DECLARE_MSGHANDLER(OnCreate);
	DECLARE_MSGHANDLER(OnPhysEnter);
	DECLARE_MSGHANDLER(OnPhysExit);
	DECLARE_MSGHANDLER(OnEndScript);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Sim", cScr_NVTurningSquare::OnSim);
		REGISTER_MSGHANDLER("Create", cScr_NVTurningSquare::OnCreate);
		REGISTER_MSGHANDLER("EndScript", cScr_NVTurningSquare::OnEndScript);
		REGISTER_MSGHANDLER("PhysEnter", cScr_NVTurningSquare::OnPhysEnter);
		REGISTER_MSGHANDLER("PhysExit", cScr_NVTurningSquare::OnPhysExit);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	void Startup(void);

	script_vec m_vSize;

	virtual void InitVars(void)
	{
		m_vSize.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVTurningSquare);

/**************************
 * NVFakeDeathTrap
 * (NVTrap)
 *
 * Makes the player seem to die.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: ControlDevice
 */
/*
DECLARE_SCRIPT(cScr_NVFakeDeathTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVFakeDeathTrap,NVGenericTrap);
	cScr_NVFakeDeathTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVFakeDeathTrap);
*/

/**************************
 * NVVineableDoor
 *
 * Door script - Allows rope/vine arrows to be attached to it.
 *
 * Messages: FrobWorldEnd, Collision
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVineableDoor,cScriptNV)
{
public:
	DECLARE_FACTORY(NVVineableDoor,CustomScript);
	cScr_NVVineableDoor(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVVineableDoor::OnFrob);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVVineableDoor::OnPhysCollision);
		REGISTER_MSGHANDLER("EndScript", cScr_NVVineableDoor::OnEndScript);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

}
END_DECLARE_SCRIPT(cScr_NVVineableDoor);

/**************************
 * NVVineableDoorHelper
 *
 * Helper script for NVVineableDoor
 *
 * Messages: FrobWorldEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVineableDoorHelper,cScriptNV)
{
public:
	DECLARE_FACTORY(NVVineableDoorHelper,CustomScript);
	cScr_NVVineableDoorHelper(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	static int DestroyLinksIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVVineableDoorHelper::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVVineableDoorHelper);


/**************************
 * NVTrigContained
 *
 * Sends TurnOn when the player contains the item, and TurnOff when the player no longer contains the item.
 *
 * Messages: PhysFellAsleep, Contained
 * Parameters: None
 * Links: Firer
 */
DECLARE_SCRIPT(cScr_NVTrigContained,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVTrigContained,CustomScript);
	cScr_NVTrigContained(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnContained);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();
		REGISTER_MSGHANDLER("Contained", cScr_NVTrigContained::OnContained);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVTrigContained::OnResetTriggerCount);
	}
}
END_DECLARE_SCRIPT(cScr_NVTrigContained);

#if (_DARKGAME != 3)
/**************************
 * NVSwapSword
 *
 * When the object with this script is frobbed in the world, it will remove the sword
 * from the player's inventory and place it at its object's location, then add itself to the
 * player's inventory, effectively swapping itself with the player's sword.
 * In order to use this script, the sword must have World Action of its
 * Engine Features->FrobInfo set to Script.
 * Note that the player will be able to pick up the sword he was previously carrying
 * so you need to either give it this script or set it's World Action to None to stop
 * the player from taking it back up.
 *
 * Messages: FrobWorldEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSwapSword,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSwapSword,CustomScript);
	cScr_NVSwapSword(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVSwapSword::OnFrob);
	}

}
END_DECLARE_SCRIPT(cScr_NVSwapSword);

/**************************
 * NVSwapSwordHelper
 *
 * Helper script for NSwapSword
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSwapSwordHelper,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSwapSwordHelper,CustomScript);
	cScr_NVSwapSwordHelper(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVSwapSwordHelper::OnTimer);
	}
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVSwapSwordHelper);
#endif

/**************************
 * NVInvSound
 *
 * Makes a sound when frobbed in inv
 *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVInvSound,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInvSound,CustomScript);
	cScr_NVInvSound(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVInvSound::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVInvSound);

#if (_BUILD != 1 )
/**************************
 * NVDirectionalControl
 *
 * Allows the player to use his movement controls to control something else.
 *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVDirectionalControl,cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVDirectionalControl,NVGenericTrap);
	cScr_NVDirectionalControl(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void RegisterMovement(int iDirection);
	virtual void Activate(void);
	virtual void Deactivate(void);

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer",cScr_NVDirectionalControl::OnTimer);
	}

	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVDirectionalControl);

/**************************
 * These should really be in another file...
 */
class sConversationOption {
public:
	int Object;
	int Priority;

	sConversationOption::sConversationOption(int iObj, int iPrior)
	{
		Object = iObj;
		Priority = iPrior;
	}
};

 struct mConversationOptionSorter
{
	bool operator()(sConversationOption* s1, sConversationOption* s2) const
	{
		return (s1->Priority < s2->Priority);
	}
};
/* Back to the scripts... */

/**************************
 * NVConversation
 * (NVDirectionalControl)
 *
 * Advanced interactive conversation interface.
 *
 * Messages:
 * Parameters:
 * Links:
 */
DECLARE_SCRIPT(cScr_NVConversation, cScr_NVDirectionalControl)
{
public:
	DECLARE_FACTORY(NVConversation,NVDirectionalControl);
	cScr_NVConversation(const char* pszName, int iHostObjId)
		: cScr_NVDirectionalControl(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iState)
	{ }

private:
	DECLARE_MSGHANDLER(OnConverse);

protected:
	virtual void RegisterMovement(int iDirection);
	virtual void Activate(void);
	virtual void Deactivate(void);
	void DisplayInterface(int iMovement);

		virtual void InitScriptData()
	{
		cScr_NVDirectionalControl::InitScriptData();
		REGISTER_MSGHANDLER("Converse",cScr_NVConversation::OnConverse);
	}

	std::multiset<sConversationOption*, mConversationOptionSorter> s;

	script_int m_iState;

	virtual void InitVars(void)
	{
		m_iState.Init();
		cScr_NVDirectionalControl::InitVars();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVConversation);

/**************************
 * NVConversationHelper
 *
 *
 *
 * Messages: FrobInvEnd
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVConversationHelper,cScriptNV)
{
public:
	DECLARE_FACTORY(NVConversationHelper,CustomScript);
	cScr_NVConversationHelper(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVConversationHelper::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVConversationHelper);
#endif // _BUILD != 1

/**************************
 * NVSimpleDoor
 *
 * Simple Door script - Can be opened/closed by frobbing or TurnOn / TurnOff
 * Completely ignores locks or any other fancy door functionality.
 *
 * Messages: FrobWorldEnd, Collision
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSimpleDoor,cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSimpleDoor,NVGenericTrap);
	cScr_NVSimpleDoor(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnFrob);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVSimpleDoor::OnFrob);
	}
}
END_DECLARE_SCRIPT(cScr_NVSimpleDoor);

/**************************
 * NVTriggeringRoom
 * (NVTrigger)
  *
 * Messages: TurnOn, TurnOff (specifiable)
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVTriggeringRoom,cScriptNV), cScrNVTrigger
{
public:
	DECLARE_FACTORY(NVTriggeringRoom,CustomScript);
	cScr_NVTriggeringRoom(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), cScrNVTrigger(pszName, iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnObjectRoomEnter);
	DECLARE_MSGHANDLER(OnObjectRoomExit);
	MSGHANDLER OnResetTriggerCount(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {ResetTriggerCount(); return 0;}

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		cScrNVTrigger::InitScriptData();

		REGISTER_MSGHANDLER("ObjectRoomEnter", cScr_NVTriggeringRoom::OnObjectRoomEnter);
		REGISTER_MSGHANDLER("ObjectRoomExit", cScr_NVTriggeringRoom::OnObjectRoomExit);

		// This should really only be registered IF a counter is enabled, but...
		REGISTER_MSGHANDLER("ResetTriggerCount", cScr_NVTriggeringRoom::OnResetTriggerCount);
	}
}
END_DECLARE_SCRIPT(cScr_NVTriggeringRoom);

/**************************
 * NVStimBasedAlpha
 *
 *
 *
 * Messages: AlphaStimStimulus
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVStimBasedAlpha,cScriptNV)
{
public:
	DECLARE_FACTORY(NVStimBasedAlpha,CustomScript);
	cScr_NVStimBasedAlpha(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnAlpha);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnSim);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("AlphaStimStimulus", cScr_NVStimBasedAlpha::OnAlpha);
		REGISTER_MSGHANDLER("Timer", cScr_NVStimBasedAlpha::OnTimer);
		REGISTER_MSGHANDLER("Sim", cScr_NVStimBasedAlpha::OnSim);
	}

	float m_fIntensity;
	float m_fTarget;

	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_fIntensity = 0.00f;
		m_fTarget = 0.50f;
		m_hTimerHandle.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVStimBasedAlpha);

/**************************
 * NVPatrolRouter
 *
 * Patrol Route Parser
 * Used to make an AI chose a path from multiple patrols linked
 * to the same patrol point.
 *
 *
 * Messages: PatrolPoint, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVPatrolRouter,cScriptNV)
{
public:
	DECLARE_FACTORY(NVPatrolRouter,CustomScript);
	cScr_NVPatrolRouter(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnPatrolPoint);
	DECLARE_MSGHANDLER(OnTimer);
	static int DestroyLinksIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("PatrolPoint", cScr_NVPatrolRouter::OnPatrolPoint);
		REGISTER_MSGHANDLER("Timer", cScr_NVPatrolRouter::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVPatrolRouter);

/**************************
 * NVBookHack
 *
 * High-resolution book art hack
 *
 * Messages: FrobWorldEnd, FrobInvEnd
 * Parameters: NVInvBook
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVBookHack,cScriptNV)
{
public:
	DECLARE_FACTORY(NVBookHack,CustomScript);
	cScr_NVBookHack(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrobWorldEnd);
	DECLARE_MSGHANDLER(OnFrobInvEnd);
	void ShowBook(void);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVBookHack::OnFrobWorldEnd);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVBookHack::OnFrobInvEnd);
	}
}
END_DECLARE_SCRIPT(cScr_NVBookHack);

/**************************
 * NVArmTrap
 * (NVTrap)
 *
 * Displays the player arm on TurnOn; hides it on TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVArmTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVArmTrap,NVGenericTrap);
	cScr_NVArmTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVArmTrap);

/**************************
 * NVVisibilityGemHider
 *
 * Hides the visibility gem
 *
 * Messages: BeginScript, EndScript
 * Parameters: NVInvBook
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVisibilityGemHider,cScriptNV)
{
public:
	DECLARE_FACTORY(NVVisibilityGemHider,CustomScript);
	cScr_NVVisibilityGemHider(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVVisibilityGemHider::OnEndScript);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVVisibilityGemHider);

/**************************
 * NVRepulsorLift
 * (NVTrap)
 *
 * SS1-style Grav lift
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVRepulsorLift, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVRepulsorLift,NVGenericTrap);
	cScr_NVRepulsorLift(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iState),
		SCRIPT_VAROBJ(m_hTimerHandle)

	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	DECLARE_MSGHANDLER(OnObjectRoomEnter);
	DECLARE_MSGHANDLER(OnObjectRoomExit);
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	void DoUpdate(void);

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();

		REGISTER_MSGHANDLER("ObjectRoomEnter", cScr_NVRepulsorLift::OnObjectRoomEnter);
		REGISTER_MSGHANDLER("ObjectRoomExit", cScr_NVRepulsorLift::OnObjectRoomExit);
		REGISTER_MSGHANDLER("PlayerRoomEnter", cScr_NVRepulsorLift::OnObjectRoomEnter);
		REGISTER_MSGHANDLER("PlayerRoomExit", cScr_NVRepulsorLift::OnObjectRoomExit);
		REGISTER_MSGHANDLER("Timer",cScr_NVRepulsorLift::OnTimer);
	}


	script_int m_iState;
	script_handle<tScrTimer> m_hTimerHandle;


	virtual void InitVars(void)
	{
		m_iState.Init(-1);
		m_hTimerHandle.Init();
		return;
	}



}
END_DECLARE_SCRIPT(cScr_NVRepulsorLift);

/**************************
 * NVAccelerator
 *
 * Accelerator script
 *
 * Messages: BeginScript, Timer
 * Parameters: NVAcceleratorRate, NVAcceleratorCoefficient
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVAccelerator,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAccelerator,CustomScript);
	cScr_NVAccelerator(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_fSpeed)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVAccelerator::OnTimer);
	}

	script_handle<tScrTimer> m_hTimerHandle;
	script_float m_fSpeed;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_fSpeed.Init();
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVAccelerator);

/**************************
 * NVSRContactTrap
 * (NVTrap)
 *
 * Initiates S&R contact on its own object when triggered.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: ControlDevice
 */

DECLARE_SCRIPT(cScr_NVSRContactTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSRContactTrap,NVGenericTrap);
	cScr_NVSRContactTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVSRContactTrap);

/**************************
 * NVSlowSpell
 * Slows creatures down when they get hit by SlowStim
 *
  *
 * Messages: SlowStimStimulus, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSlowSpell,cScriptNV)
{
public:
	DECLARE_FACTORY(NVSlowSpell,CustomScript);
	cScr_NVSlowSpell(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_fWarp),
		SCRIPT_VAROBJ(m_fTarget),
		SCRIPT_VAROBJ(m_fSpeed),
		SCRIPT_VAROBJ(m_iSlowTime)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnSlow);

protected:

	script_handle<tScrTimer> m_hTimerHandle;
	script_float m_fWarp;
	script_float m_fTarget;
	script_float m_fSpeed;
	script_int m_iSlowTime;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_fWarp.Init();
		m_fTarget.Init();
		m_fSpeed.Init();
		m_iSlowTime.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("SlowStimStimulus", cScr_NVSlowSpell::OnSlow);
		REGISTER_MSGHANDLER("Timer", cScr_NVSlowSpell::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVSlowSpell);


#if (_DARKGAME != 1)
/**************************
 * NVTFMadeMeDoIt
 *
 * Used for projeciltes that stimulate their target with a source originating from their firer
 *
 * Messages: BeginScript, EndScript, PhysCollision
 * Parameters: NVTFMadeMeDoItStimulus, NVTFMadeMeDoItIntensity
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVTFMadeMeDoIt,cScriptNV)
{
public:
	DECLARE_FACTORY(NVTFMadeMeDoIt,CustomScript);
	cScr_NVTFMadeMeDoIt(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVTFMadeMeDoIt::OnEndScript);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVTFMadeMeDoIt::OnPhysCollision);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVTFMadeMeDoIt);
#endif


/**************************
 * NVSlayMeTrap
 * (NVTrap)
 *
 * Slays its object when activated.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSlayMeTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSlayMeTrap,NVGenericTrap);
	cScr_NVSlayMeTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing!

}
END_DECLARE_SCRIPT(cScr_NVSlayMeTrap);


/**************************
 * NVDistanceBasedAlpha
 *
 *
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVDistanceBasedAlpha,cScriptNV)
{
public:
	DECLARE_FACTORY(NVDistanceBasedAlpha,CustomScript);
	cScr_NVDistanceBasedAlpha(const char* pszName, int iHostObjId)
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
		REGISTER_MSGHANDLER("Timer", cScr_NVDistanceBasedAlpha::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVDistanceBasedAlpha);

/**************************
 * NVAITeamSetter
 *
 * Allows an AI to use a team other than the standard 6.
 *
 * Messages: BeginScript
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVAITeamSetter,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAITeamSetter,CustomScript);
	cScr_NVAITeamSetter(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	// Nothing private

protected:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVAITeamSetter);

/**************************
 * NVCombineTypeSetter
 *
 * Sets an object's combine type based on a paramn on BeginScript.
 *
 * Messages: BeginScript
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVCombineTypeSetter,cScriptNV)
{
public:
	DECLARE_FACTORY(NVCombineTypeSetter,CustomScript);
	cScr_NVCombineTypeSetter(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	// Nothing private

protected:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVCombineTypeSetter);

/**************************
 * NVSetPropertyTrap
 * (NVTrap)
 *
 * Sets a property to a random value within range when triggered.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSetPropertyTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSetPropertyTrap,NVGenericTrap);
	cScr_NVSetPropertyTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing!

}
END_DECLARE_SCRIPT(cScr_NVSetPropertyTrap);


/**************************
 * NVRandomPropertyTrap
 * (NVSetPropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRandomPropertyTrap, cScr_NVSetPropertyTrap)
{
public:
	DECLARE_FACTORY(NVRandomPropertyTrap,NVSetPropertyTrap);
	cScr_NVRandomPropertyTrap(const char* pszName, int iHostObjId) : cScr_NVSetPropertyTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVRandomPropertyTrap);


/**************************
 * NVSetPropertyTrap2
 * (NVSetPropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVSetPropertyTrap2, cScr_NVSetPropertyTrap)
{
public:
	DECLARE_FACTORY(NVSetPropertyTrap2,NVSetPropertyTrap);
	cScr_NVSetPropertyTrap2(const char* pszName, int iHostObjId) : cScr_NVSetPropertyTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVSetPropertyTrap2);


/**************************
 * NVSetPropertyTrap3
 * (NVSetPropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVSetPropertyTrap3, cScr_NVSetPropertyTrap)
{
public:
	DECLARE_FACTORY(NVSetPropertyTrap3,NVSetPropertyTrap);
	cScr_NVSetPropertyTrap3(const char* pszName, int iHostObjId) : cScr_NVSetPropertyTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVSetPropertyTrap3);


/**************************
 * NVSetPropertyTrap4
 * (NVSetPropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVSetPropertyTrap4, cScr_NVSetPropertyTrap)
{
public:
	DECLARE_FACTORY(NVSetPropertyTrap4,NVSetPropertyTrap);
	cScr_NVSetPropertyTrap4(const char* pszName, int iHostObjId) : cScr_NVSetPropertyTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVSetPropertyTrap4);


/**************************
 * NVSetPropertyTrap5
 * (NVSetPropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVSetPropertyTrap5, cScr_NVSetPropertyTrap)
{
public:
	DECLARE_FACTORY(NVSetPropertyTrap5,NVSetPropertyTrap);
	cScr_NVSetPropertyTrap5(const char* pszName, int iHostObjId) : cScr_NVSetPropertyTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVSetPropertyTrap5);



/**************************
 * NVWhoKilledMe
 *
 * Shows messages about who killed an AI.
 *
 * Messages: BeginScript
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVWhoKilledMe,cScriptNV)
{
public:
	DECLARE_FACTORY(NVWhoKilledMe,CustomScript);
	cScr_NVWhoKilledMe(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnSlain);

protected:
		virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Slain", cScr_NVWhoKilledMe::OnSlain);
	}
}
END_DECLARE_SCRIPT(cScr_NVWhoKilledMe);

/**************************
 * NVStackOrDropTrap
 * (NVTrap)
 *
 * Sets a property to a random value within range when triggered.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVStackOrDropTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVStackOrDropTrap,NVGenericTrap);
	cScr_NVStackOrDropTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing!

}
END_DECLARE_SCRIPT(cScr_NVStackOrDropTrap);

/**************************
 * NVConsumeMessages
 *
 * Shows messages when an objects stack count decreases
 *
 * Messages: BeginScript
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVConsumeMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVConsumeMessages,CustomScript);
	cScr_NVConsumeMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iStack)
	{ }

private:
	DECLARE_MSGHANDLER(OnFrob);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("FrobWorldEnd", cScr_NVConsumeMessages::OnFrob);
		REGISTER_MSGHANDLER("FrobInvEnd", cScr_NVConsumeMessages::OnFrob);
		REGISTER_MSGHANDLER("Timer", cScr_NVConsumeMessages::OnTimer);
	}

	script_int m_iStack;
	virtual void InitVars(void)
	{
		m_iStack.Init();
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVConsumeMessages);

/**************************
 * NVContainedMessages
 *
 * More detailed 'Contained' messages
 *
 * Messages: Contained
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVContainedMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVContainedMessages,CustomScript);
	cScr_NVContainedMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnContained);

protected:
		virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Contained", cScr_NVContainedMessages::OnContained);
	}
}
END_DECLARE_SCRIPT(cScr_NVContainedMessages);

/**************************
 * NVPickyProjectile
 *
 * Projectile will ignore collisions with objects that inherit from [NVPickyProjectileIgnore],
 * and be slain upon collision with any other object.
 *
 * Messages: BeginScript, EndScript, PhysCollission
 * Parameters: NVPickyProjectileIgnore
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVPickyProjectile,cScriptNV)
{
public:
	DECLARE_FACTORY(NVPickyProjectile,CustomScript);
	cScr_NVPickyProjectile(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVPickyProjectile::OnEndScript);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVPickyProjectile::OnPhysCollision);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVPickyProjectile);

/**************************
 * NVGibTrap
 * (NVTrap)
 *
 * Sends the object's contents flying and destroys it.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVGibTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVGibTrap,NVGenericTrap);
	cScr_NVGibTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	void DoGib(sScrMsg* pMsg);

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("Timer",cScr_NVGibTrap::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVGibTrap);


/**************************
 * NVDoorSpeedTrap
 * (NVTrap)
 *
 * Variable speed door controller.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVDoorSpeedTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVDoorSpeedTrap,NVGenericTrap);
	cScr_NVDoorSpeedTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_vLocation),
		SCRIPT_VAROBJ(m_vLocation2),
		SCRIPT_VAROBJ(m_vRotation),
		SCRIPT_VAROBJ(m_vRotation2),
		SCRIPT_VAROBJ(m_iInit)
	{ }

private:
	DECLARE_MSGHANDLER(OnDoorOpen);
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	void DoDoor(float fSpeed);

	script_vec m_vLocation;
	script_vec m_vLocation2;
	script_vec m_vRotation;
	script_vec m_vRotation2;
	script_int m_iInit;

protected:
	virtual void InitVars(void)
	{
		m_vLocation.Init();
		m_vLocation2.Init();
		m_vRotation.Init();
		m_vRotation2.Init();
		m_iInit.Init();
	}

	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("DoorOpen",cScr_NVDoorSpeedTrap::OnDoorOpen);
	}
}
END_DECLARE_SCRIPT(cScr_NVDoorSpeedTrap);

/**************************
 * NVTimeDilationPhysics
  *
 * Tilme dilation 1/10th script
 *
 * Messages: ?
 * Parameters:
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVTimeDilationPhysics,cScriptNV)
{
public:
	DECLARE_FACTORY(NVTimeDilationPhysics,CustomScript);
	cScr_NVTimeDilationPhysics(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iDilate)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnQuestChange);
	DECLARE_MSGHANDLER(OnCheck);

	void DoTimeDilation(void);

protected:

	script_int m_iDilate;

	virtual void InitVars(void)
	{
		m_iDilate.Init();
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVTimeDilationPhysics::OnEndScript);
		REGISTER_MSGHANDLER("Timer", cScr_NVTimeDilationPhysics::OnTimer);
		REGISTER_MSGHANDLER("QuestChange", cScr_NVTimeDilationPhysics::OnQuestChange);
		REGISTER_MSGHANDLER("PhysMadePhysical", cScr_NVTimeDilationPhysics::OnCheck);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

}
END_DECLARE_SCRIPT(cScr_NVTimeDilationPhysics);

/**************************
 * NVTimeDilationTweq
 * (NVTrap)
 *
 * Tilme dilation 1/10th script
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVTimeDilationTweq, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVTimeDilationTweq,NVGenericTrap);
	cScr_NVTimeDilationTweq(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iDilate)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	void DoDilate(float fScale);

protected:
	script_int m_iDilate;

	virtual void InitVars(void)
	{
		m_iDilate.Init();
	}
}
END_DECLARE_SCRIPT(cScr_NVTimeDilationTweq);


/**************************
 * NVGetOrigin
  *
 * Gets the origin and stores it in a param on BeginScript
 *
 * Messages: BeginScript
 * Parameters:  None
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVGetOrigin,cScriptNV)
{
public:
	DECLARE_FACTORY(NVGetOrigin,CustomScript);
	cScr_NVGetOrigin(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{	}

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	//Nothing
}
END_DECLARE_SCRIPT(cScr_NVGetOrigin);

/**************************
 * NVStartUnrotated
  *
 * Sets the object's rotation to 0,0,0 on BeginScript
 *
 * Messages: ?
 * Parameters:
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVStartUnrotated,cScriptNV)
{
public:
	DECLARE_FACTORY(NVStartUnrotated,CustomScript);
	cScr_NVStartUnrotated(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{	}

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVStartUnrotated::OnTimer);
	}
	//Nothing
}
END_DECLARE_SCRIPT(cScr_NVStartUnrotated);

/**************************
 * NVLeaveTheDeadAlone
  *
 * Makes AIs stop attacking this object if it dies
 *
 * Messages: ?
 * Parameters:
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVLeaveTheDeadAlone,cScriptNV)
{
public:
	DECLARE_FACTORY(NVLeaveTheDeadAlone,CustomScript);
	cScr_NVLeaveTheDeadAlone(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{	}

private:
	DECLARE_MSGHANDLER(OnSlain);


protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Slain", cScr_NVLeaveTheDeadAlone::OnSlain);
	}

}
END_DECLARE_SCRIPT(cScr_NVLeaveTheDeadAlone);

/**************************
 * NVCameraTrap
 * (NVTrap)
 *
 * Attaches the camera to this object on TurnOn.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVCameraTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVCameraTrap,NVGenericTrap);
	cScr_NVCameraTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnEndScript);
	void deactivateAttach(void);

protected:
	script_handle<tScrTimer> m_hTimerHandle;
	boolean m_bWasEscPressed;

	virtual void InitVars(void)
	{
		m_bWasEscPressed = false;
		m_hTimerHandle.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVCameraTrap::OnEndScript);
		REGISTER_MSGHANDLER("Timer", cScr_NVCameraTrap::OnTimer);
	}

}
END_DECLARE_SCRIPT(cScr_NVCameraTrap);

/**************************
 * NVMapTrap
 * (NVTrap)
 *
 * Marks the specified map location as visited
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVMapTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVMapTrap,NVGenericTrap);
	cScr_NVMapTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing!

}
END_DECLARE_SCRIPT(cScr_NVMapTrap);

/**************************
 * NVMapTrap2
 * (NVMapTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMapTrap2, cScr_NVMapTrap)
{
public:
	DECLARE_FACTORY(NVMapTrap2,NVMapTrap);
	cScr_NVMapTrap2(const char* pszName, int iHostObjId) : cScr_NVMapTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVMapTrap2);

/**************************
 * NVMapTrap3
 * (NVMapTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMapTrap3, cScr_NVMapTrap)
{
public:
	DECLARE_FACTORY(NVMapTrap3,NVMapTrap);
	cScr_NVMapTrap3(const char* pszName, int iHostObjId) : cScr_NVMapTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVMapTrap3);

/**************************
 * NVMapTrap4
 * (NVMapTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMapTrap4, cScr_NVMapTrap)
{
public:
	DECLARE_FACTORY(NVMapTrap4,NVMapTrap);
	cScr_NVMapTrap4(const char* pszName, int iHostObjId) : cScr_NVMapTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVMapTrap4);

/**************************
 * NVMapTrap5
 * (NVMapTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVMapTrap5, cScr_NVMapTrap)
{
public:
	DECLARE_FACTORY(NVMapTrap5,NVMapTrap);
	cScr_NVMapTrap5(const char* pszName, int iHostObjId) : cScr_NVMapTrap(pszName,iHostObjId)	{ }
}
END_DECLARE_SCRIPT(cScr_NVMapTrap5);


/**************************
 * NVDeathStageFix
 *
 * Removes objects with DS12
 *
 * Messages: None
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVDeathStageFix,cScriptNV)
{
public:
	DECLARE_FACTORY(NVDeathStageFix,CustomScript);
	cScr_NVDeathStageFix(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId), m_pProp(NULL)
	{	}

	virtual ~cScr_NVDeathStageFix()
	{
		StopListener();
		if ( m_pProp )
		{
			m_pProp->Release();
		}
	}

private:
	static void __stdcall ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData);
	IGenericProperty* m_pProp;
	void StopListener();

	int m_iState;

protected:

	virtual void InitVars(void)
	{
		m_hListenHandle = NULL;
		StartListener();
	}

	PropListenerHandle m_hListenHandle;
	void StartListener();
}
END_DECLARE_SCRIPT(cScr_NVDeathStageFix);

/**************************
 * NVQVarName
  *
  * Sets an object's inventory name / label based on a qvar
  *
 * Messages:
 * Parameters:
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVQVarName,cScriptNV)
{
public:
	DECLARE_FACTORY(NVQVarName,CustomScript);
	cScr_NVQVarName(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{
		m_szQVar = NULL;
		GetQVarName();
	}

	virtual ~cScr_NVQVarName()
	{
		if ( m_szQVar )
		{
			delete[] m_szQVar;
		}
	}

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnQuestChange);

protected:
	char* m_szQVar;

	void GetQVarName();

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVQVarName::OnEndScript);
		REGISTER_MSGHANDLER("QuestChange", cScr_NVQVarName::OnQuestChange);
		REGISTER_MSGHANDLER("InitNVQvarName", cScr_NVQVarName::OnQuestChange);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	virtual void InitVars(void)
	{
		return;
	}

}
END_DECLARE_SCRIPT(cScr_NVQVarName);

/**************************
 * NVSelectTrap
 * (NVTrap)
 *
 * Selects an [inventory] object.  By name.  When activated.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVSelectTrapItem
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVSelectTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSelectTrap,NVGenericTrap);
	cScr_NVSelectTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVSelectTrap);


/**************************
 * NVAttackMessages
 *
 * Sends more specific attack messages:
 * StartAttackMelee - AI is starting a mêlée attack
 * StartAttackRanged - AI is starting a ranged attack
 * EndAttackMelee - AI has finished a mêlée attack
 * EndAttackRanged - AI has finished a ranged attack
 * StartWindupMelee -  AI Starts winding up for a mêlée attack
 * StartWindupRanged - AI Starts winding up for a ranged attack
 *
 * Messages: TweqComplete
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVAttackMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAttackMessages,CustomScript);
	cScr_NVAttackMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnStartAttack);
	DECLARE_MSGHANDLER(OnEndAttack);
	DECLARE_MSGHANDLER(OnStartWindup);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("StartAttack", cScr_NVAttackMessages::OnStartAttack);
		REGISTER_MSGHANDLER("EndAttack", cScr_NVAttackMessages::OnEndAttack);
		REGISTER_MSGHANDLER("StartWindup", cScr_NVAttackMessages::OnStartWindup);
	}
}
END_DECLARE_SCRIPT(cScr_NVAttackMessages);

/**************************
 * NVDifficulty
 *
 * Sets properties based on the current difficulty
 *
 * Messages: BeginScript
 * Parameters:  None
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVDifficulty,cScriptNV)
{
public:
	DECLARE_FACTORY(NVDifficulty,CustomScript);
	cScr_NVDifficulty(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:

protected:

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

}
END_DECLARE_SCRIPT(cScr_NVDifficulty);

/**************************
 * NVPsiProjectileThief
 *
 * Steals enemies' projectiles
 *
 * Messages: BeginScript, EndScript, PhysCollission
 * Parameters: NVPickyProjectileIgnore
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVPsiProjectileThief,cScriptNV)
{
public:
	DECLARE_FACTORY(NVPsiProjectileThief,CustomScript);
	cScr_NVPsiProjectileThief(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnEndScript);
	DECLARE_MSGHANDLER(OnPhysCollision);

protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVPsiProjectileThief	::OnEndScript);
		REGISTER_MSGHANDLER("PhysCollision", cScr_NVPsiProjectileThief::OnPhysCollision);
	}

	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
}
END_DECLARE_SCRIPT(cScr_NVPsiProjectileThief);

/**************************
 * NVCutsceneTrap
 * (NVTrap)
 *
 * Plays a cutscene.  Uses NVCutscene paramter.
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVSelectTrapItem
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVCutsceneTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVCutsceneTrap,NVGenericTrap);
	cScr_NVCutsceneTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVCutsceneTrap);


/**************************
 * NVShakePlayer
  *
 * Allows the player to be shaken by ShakeStim.  As in SS2.
 *
 * Messages: BeginScript
 * Parameters:  None
 * Links: None
 */


DECLARE_SCRIPT(cScr_NVShakePlayer,cScriptNV)
{
public:
	DECLARE_FACTORY(NVShakePlayer,CustomScript);
	cScr_NVShakePlayer(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{
		m_fShakeSize = 1.0f;
		m_iShakeCount = 5;
	}

private:
	DECLARE_MSGHANDLER(OnTimer);
	DECLARE_MSGHANDLER(OnShakeStim);

	void ShakeMeGood(float intensity);
	void PlayerShake(float intensity);

	float m_fShakeSize;
	int m_iShakeCount;

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer",cScr_NVShakePlayer::OnTimer);
		REGISTER_MSGHANDLER("ShakeStimStimulus",cScr_NVShakePlayer::OnShakeStim);
	}
}
END_DECLARE_SCRIPT(cScr_NVShakePlayer);


/**************************
 * NVPhysMessages
 *
 * Just a quick script to register interest in Phys messages
 *
 * Messages: All
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVPhysMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVPhysMessages,CustomScript);
	cScr_NVPhysMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnEndScript);


protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("EndScript", cScr_NVPhysMessages::OnEndScript);
	}
}
END_DECLARE_SCRIPT(cScr_NVPhysMessages);


/**************************
 * NVVisibilityToQVar
 *
 *
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVVisibilityToQVar,cScriptNV)
{
public:
	DECLARE_FACTORY(NVVisibilityToQVar,CustomScript);
	cScr_NVVisibilityToQVar(const char* pszName, int iHostObjId)
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
		REGISTER_MSGHANDLER("Timer", cScr_NVVisibilityToQVar::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVVisibilityToQVar);


/**************************
 * NVFreePatrol
 *
 *
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVFreePatrol,cScriptNV)
{
public:
	DECLARE_FACTORY(NVFreePatrol,CustomScript);
	cScr_NVFreePatrol(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

	static int RemoveWatchObjIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);

protected:

	script_handle<tScrTimer> m_hTimerHandle;

	void GoToPoint(mxs_vector vDest);

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVFreePatrol::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVFreePatrol);


/**************************
 * NVKeyboardController
 * (NVTrap)
 *
 *
 * Messages: BeginScript, Timer
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVKeyboardController,cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVKeyboardController,NVGenericTrap);
	cScr_NVKeyboardController(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnTimer);

protected:

	script_handle<tScrTimer> m_hTimerHandle;

	char m_keys[256];
	virtual void InitVars(void)
	{
		for ( int i = 0; i < 256; ++i)
		{
			m_keys[i] = 0;
		}
		m_hTimerHandle.Init();
		return;
	}

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVKeyboardController::OnTimer);
	}
}
END_DECLARE_SCRIPT(cScr_NVKeyboardController);

/**************************
 * NVTweqEmit
 * (NVTrap)
 *
 * Activates emitters
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVTweqEmitTweqNum
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVTweqEmit, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVTweqEmit,NVGenericTrap);
	cScr_NVTweqEmit(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	void SetEmitter(bool bOn);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVTweqEmit);

/**************************
 * NVHangGliderTrap
 * (NVTrap)
 *
 * Activates a hang glider ?!
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters:
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVHangGliderTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVHangGliderTrap,NVGenericTrap);
	cScr_NVHangGliderTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
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
		REGISTER_MSGHANDLER("Timer", cScr_NVHangGliderTrap::OnTimer);
	}

}
END_DECLARE_SCRIPT(cScr_NVHangGliderTrap);

/**************************
 * NVWatchMe
 *
 * Configurable version of the WatchMe script
 *
 * Messages: All
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVWatchMe,cScriptNV)
{
public:
	DECLARE_FACTORY(NVWatchMe,CustomScript);
	cScr_NVWatchMe(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnCreate);


protected:
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Create", cScr_NVWatchMe::OnCreate);
	}
}
END_DECLARE_SCRIPT(cScr_NVWatchMe);

/**************************
 * NVWatchMeTrap
 *
 * Configurable trap version of the WatchMe script
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVWatchMeTrap,cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVWatchMeTrap,NVGenericTrap);
	cScr_NVWatchMeTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
	}
}
END_DECLARE_SCRIPT(cScr_NVWatchMeTrap);


/**************************
 * NVGenericScalarTrap
 * (NVTrap)
 *
 * Fades some value in the object's properties "on" and "off" on TurnOn and TurnOff
 * Base script for NVPhantomTrap and NVGlowTrap
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: ?????
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVGenericScalarTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVGenericScalarTrap,NVGenericTrap);
	cScr_NVGenericScalarTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual void DoFade(bool bOn, sScrMsg* pMsg);

protected:
	/* This is such a messy hack */;
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVGenericScalarTrap::OnTimer);
	}
	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}

	virtual script_handle<tScrTimer> GetTimerHandle() { return m_hTimerHandle; }
	virtual void SetTimerHandle(tScrTimer t) { m_hTimerHandle = t; }
	/* End of horrible hack */

	virtual float GetMax();
	virtual float GetMin();
	virtual float GetIncrement();
	virtual float GetDefaultValue();

	virtual int GetTimerOn();
	virtual int GetTimerOff();

	virtual const char* GetPropName();
	virtual const char* GetSubPropName();

	virtual const char* GetOnMessage();
	virtual const char* GetOffMessage();

	virtual bool GetRemovePropOnZero();

	virtual void DoExtraTasks(int obj, float fCurrent);
}
END_DECLARE_SCRIPT(cScr_NVGenericScalarTrap);

/**************************
 * NVGlowTrap
 * (NVGenericScalarTrap)
 *
 * Fades the object's light on and off on TurnOn and TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVGlobMax, NVGlowMin, NVGlowFadeOn, NVGlowFadeOff
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVGlowTrap, cScr_NVGenericScalarTrap)
{
public:
	DECLARE_FACTORY(NVGlowTrap,NVGenericTrap);
	cScr_NVGlowTrap(const char* pszName, int iHostObjId)
		: cScr_NVGenericScalarTrap(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }


protected:
	/* This is such a messy hack */;
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}

	virtual script_handle<tScrTimer> GetTimerHandle() { return m_hTimerHandle; }
	virtual void SetTimerHandle(tScrTimer t) { m_hTimerHandle = t; }
	/* End of horrible hack */

	virtual float GetMax();
	virtual float GetMin();
	virtual float GetIncrement();
	virtual float GetDefaultValue();

	virtual int GetTimerOn();
	virtual int GetTimerOff();

	virtual const char* GetPropName();
	virtual const char* GetSubPropName();

	virtual const char* GetOnMessage();
	virtual const char* GetOffMessage();

	virtual bool GetRemovePropOnZero();

	virtual void DoExtraTasks(int obj, float fCurrent);
}
END_DECLARE_SCRIPT(cScr_NVGlowTrap);

/**************************
 * NVPhantomTrap
 * (NVTrap)
 *
 * Fades the object in and out on TurnOn and TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVPhantomAlphaMax, NVPhantomAlphaMin, NVPhantomFadeOn, NVPhantomFadeOff
 * Links: None
 */

 DECLARE_SCRIPT(cScr_NVPhantomTrap, cScr_NVGenericScalarTrap)
{
public:
	DECLARE_FACTORY(NVPhantomTrap,NVGenericTrap);
	cScr_NVPhantomTrap(const char* pszName, int iHostObjId)
		: cScr_NVGenericScalarTrap(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle)
	{ }


protected:
	/* This is such a messy hack */;
	script_handle<tScrTimer> m_hTimerHandle;

	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		return;
	}

	virtual script_handle<tScrTimer> GetTimerHandle() { return m_hTimerHandle; }
	virtual void SetTimerHandle(tScrTimer t) { m_hTimerHandle = t; }
	/* End of horrible hack */

	virtual float GetMax();
	virtual float GetMin();
	virtual float GetIncrement();
	virtual float GetDefaultValue();

	virtual int GetTimerOn();
	virtual int GetTimerOff();

	virtual const char* GetPropName();
	virtual const char* GetSubPropName();

	virtual const char* GetOnMessage();
	virtual const char* GetOffMessage();

	virtual bool GetRemovePropOnZero();

	virtual void DoExtraTasks(int obj, float fCurrent);
}
END_DECLARE_SCRIPT(cScr_NVPhantomTrap);


/**************************
 * NVFogTrap
 * (NVTrap)
 *
 * Fades the mission's fog distance "on" and "off" on TurnOn and TurnOff
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: ?????
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVFogTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVFogTrap,NVGenericTrap);
	cScr_NVFogTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_hTimerHandle),
		SCRIPT_VAROBJ(m_iCurrent)
	{ }

private:
	DECLARE_MSGHANDLER(OnTimer);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual void DoFade(bool bOn, sScrMsg* pMsg);

protected:
	script_handle<tScrTimer> m_hTimerHandle;
	script_int m_iCurrent;

	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
		REGISTER_MSGHANDLER("Timer", cScr_NVFogTrap::OnTimer);
	}
	virtual void InitVars(void)
	{
		m_hTimerHandle.Init();
		m_iCurrent.Init(-1);
		return;
	}
}
END_DECLARE_SCRIPT(cScr_NVFogTrap);

/**************************
 * NVJointControl
 *
 * A script for manually controlling multiple joints
 *
 * Messages: All
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVJointControl,cScriptNV)
{
public:
	DECLARE_FACTORY(NVJointControl,CustomScript);
	cScr_NVJointControl(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	STDMETHOD_(long,ReceiveMessage)(sScrMsg*,sMultiParm*,eScrTraceAction);
	DECLARE_MSGHANDLER(OnTweqComplete);
/*	virtual MSGHANDLER OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	DECLARE_MSGHANDLER(OnEndScript);*/


protected:

	virtual void SetupPrimaryJoint();
	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("TweqComplete",cScr_NVJointControl::OnTweqComplete);
	}

}
END_DECLARE_SCRIPT(cScr_NVJointControl);

/**************************
 * NVAlertMessages
 *
 * More detailed 'Alertness' messages
 *
 * Messages: Alertness; Alert0, Alert1, Alert2, Alert3, Alert0To1, Alert0To2, etc.
 * Parameters: None
 * Links: None
 */
DECLARE_SCRIPT(cScr_NVAlertMessages,cScriptNV)
{
public:
	DECLARE_FACTORY(NVAlertMessages,CustomScript);
	cScr_NVAlertMessages(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnAlertness);

protected:
		virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Alertness", cScr_NVAlertMessages::OnAlertness);
	}
}
END_DECLARE_SCRIPT(cScr_NVAlertMessages);

/**************************
 * NVWakePhysicsTrap
 * (NVTrap)
 *
 * Wakes the physics on its object when activated
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: None
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVWakePhysicsTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVWakePhysicsTrap,NVGenericTrap);
	cScr_NVWakePhysicsTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVWakePhysicsTrap);

/**************************
 * NVRemovePropertyTrap
 * (NVTrap)
 *
 * Removes a property from itself (or the Target object(s)).
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVRemovePropertyTrapProp, NVRemovePropertyTrapReAdd, NVPropertyTrapTarget
 * Links: None
 */

DECLARE_SCRIPT(cScr_NVRemovePropertyTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVRemovePropertyTrap,NVGenericTrap);
	cScr_NVRemovePropertyTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	// Nothing

}
END_DECLARE_SCRIPT(cScr_NVRemovePropertyTrap);

/**************************
 * NVRemovePropertyTrap2
 * (NVRemovePropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRemovePropertyTrap2, cScr_NVRemovePropertyTrap)
{
public:
	DECLARE_FACTORY(NVRemovePropertyTrap2,NVRemovePropertyTrap);
	cScr_NVRemovePropertyTrap2(const char* pszName, int iHostObjId)
		: cScr_NVRemovePropertyTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVRemovePropertyTrap2);

/**************************
 * NVRemovePropertyTrap3
 * (NVRemovePropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRemovePropertyTrap3, cScr_NVRemovePropertyTrap)
{
public:
	DECLARE_FACTORY(NVRemovePropertyTrap3,NVRemovePropertyTrap);
	cScr_NVRemovePropertyTrap3(const char* pszName, int iHostObjId)
		: cScr_NVRemovePropertyTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVRemovePropertyTrap3);

/**************************
 * NVRemovePropertyTrap4
 * (NVRemovePropertyTrap)
 *
  */

DECLARE_SCRIPT(cScr_NVRemovePropertyTrap4, cScr_NVRemovePropertyTrap)
{
public:
	DECLARE_FACTORY(NVRemovePropertyTrap4,NVRemovePropertyTrap);
	cScr_NVRemovePropertyTrap4(const char* pszName, int iHostObjId)
		: cScr_NVRemovePropertyTrap(pszName,iHostObjId)
	{ }
}
END_DECLARE_SCRIPT(cScr_NVRemovePropertyTrap4);

// End of scripts

#endif
