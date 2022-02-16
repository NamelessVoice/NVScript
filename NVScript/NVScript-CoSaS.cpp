#if _DARKGAME == 2

// #include <cstdlib>
#include <cmath>

#include "lg/interface.h"
#include "lg/scrservices.h"
#include "lg/scrmanagers.h"
#include "lg/scrmsgs.h"
#include "lg/objects.h"
#include "ScriptLib.h"
#include "NVScriptLib.h"
#include "NVScript-CoSaS.h"

#include "lg/iids.h"

#define null NULL

#if (_DARKGAME != 1)
	#define SOUND_NET  ,kSoundNetwork0
#else
	#define SOUND_NET
#endif



/***********************
 * NVManaRegen
 */
 
MSGHANDLER cScr_NVManaRegen::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{ 
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		int iRegenRate = ParamGetInt(m_iObjId, "NVManaRegenRate", 6000);
	
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVManaRegen", iRegenRate, kSTM_Periodic, g_mpUndef);
	}
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	pQSrv->SubscribeMsg(m_iObjId, "Mana", 2);
		
	return 0;
}

MSGHANDLER cScr_NVManaRegen::OnQuestChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	if (pQSrv->Exists("Mana"))
	{
		int iCurrent = pQSrv->Get("Mana");	// Get QVar value
		UpdateManaDisplay(iCurrent);
	}
	return 0;
}

void cScr_NVManaRegen::UpdateManaDisplay(int iMana)
{
	DisplayPrintf("Mana is now %i", iMana);
}

MSGHANDLER cScr_NVManaRegen::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVManaRegen"))
		return 0; // Abort if it's not the right timer.
		
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	
	if (pQSrv->Exists("CastingSpell"))
	{
		if ( pQSrv->Get("CastingSpell") > 0 )
		{
			// Don't allow the player to regenerate mana when a spell is currently being cast
			return 0;
		}
	}
		
	int iMaximum = ParamGetInt(m_iObjId, "NVManaRegenMax", 10);
	int iCurrent;
	
	if (pQSrv->Exists("Mana"))
	{
		iCurrent = pQSrv->Get("Mana");	// Get QVar value
	} else {
		// Default starting mana (if the qvar does not exist)
		iCurrent = ParamGetInt(m_iObjId, "NVManaRegenInitial", 5);
	}
	
	if ( iCurrent < iMaximum )
	{
		++iCurrent;
		UpdateManaDisplay(iCurrent);
		
		pQSrv->Set("Mana", iCurrent, 0);
	}
	
	return 0;
}

/***********************
 * NVSpellBase
 */
 
MSGHANDLER cScr_NVSpellBase::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	EndSpellEffect();
	
	return 0;
}
 
MSGHANDLER cScr_NVSpellBase::OnInvSelect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	cScr_NVNewWeapon::OnInvSelect(pMsg, pReply, eTrace);
	
	SService<IWeaponSrv> pWSrv(g_pScriptManager);
	pWSrv->Equip(m_iObjId, 1);
	return 0;
}
 
MSGHANDLER cScr_NVSpellBase::OnInvDeSelect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	cScr_NVNewWeapon::OnInvDeselect(pMsg, pReply, eTrace);

	SetCastingSpell(false);
	AbortSpell();
	
	cScr_NVNewWeapon::OnInvDeselect(pMsg, pReply, eTrace);
	SService<IWeaponSrv> pWSrv(g_pScriptManager);
	pWSrv->UnEquip(m_iObjId);
	return 0;
}
 
MSGHANDLER cScr_NVSpellBase::OnFrobBegin(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oArm =  0;
	sLink slOLink;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
	{
		oArm = slOLink.dest;
	}
	
	if ( oArm != 0 )
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);
		
		int iCurrent = 0;	// Default value if the QVar is not set
		
		if (pQSrv->Exists("Mana"))
		{
			iCurrent = pQSrv->Get("Mana");	// Get QVar value
		}
		
		// Don't let the player even to cast the spell if they don't have enough mana.
		// Uses [SpellName]ManaRequired if present, or [SpellName]InitialCost if not.
		int iManaCost = NVParamGetInt(m_iObjId, m_szName, "ManaRequired", NVParamGetInt(m_iObjId, m_szName, "InitialCost", 1));
		
		if ( iManaCost > iCurrent )
		{
			DisplayPrintf("Not enough mana.");
			return 0;
		}
		
		SService<IWeaponSrv> pWSrv(g_pScriptManager);
		pWSrv->StartAttack(m_iObjId, m_iObjId);
		
		m_iWindingUp = 1;
		SetCastingSpell(true);
		BeginWindup();
		
		if (m_hWindupTimerHandle == NULL)
		{ // Start a timer.
			int iDrainRate = NVParamGetInt(m_iObjId, m_szName, "WindupTime", 2500);
		
			m_hWindupTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVSpellWindup", iDrainRate, kSTM_OneShot, g_mpUndef);
		}
		
		SimplePost(0, oArm, "SpellStartWindup");
	}
	return 0;
}

MSGHANDLER cScr_NVSpellBase::OnFrobEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_iWindingUp == 2 )
	{
		ReleaseSpell();
	} else if ( m_iWindingUp == 1 ) {
		AbortSpell();
	}
	
	SService<IWeaponSrv> pWSrv(g_pScriptManager);
	pWSrv->FinishAttack(m_iObjId, m_iObjId);
	
	return 0;
}

void cScr_NVSpellBase::StartDrain()
{
	if (m_hDrainTimerHandle == NULL)
	{ // Start a timer.
		int iDrainRate = NVParamGetInt(m_iObjId, m_szName, "DrainRate", 6000);
	
		m_hDrainTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVManaDrain", iDrainRate, kSTM_Periodic, g_mpUndef);
	}
}

void cScr_NVSpellBase::StopDrain()
{
	if (m_hDrainTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hDrainTimerHandle);
		m_hDrainTimerHandle = NULL;
	}
}


MSGHANDLER cScr_NVSpellBase::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVManaDrain"))
	{
		
		if ( UseMana(1) == false)
		{
			// Cannot drani mana - pool empty!
			StopDrain();
			AbortSpell();
		}
	} else if (!stricmp(pszMsgName, "NVSpellWindup"))
	{
		m_hWindupTimerHandle = NULL;
		int iManaCost = NVParamGetInt(m_iObjId, m_szName, "InitialCost", 1);
		if ( UseMana(iManaCost) )
		{
			FinishWindup();
		} else {
			AbortSpell();
		}
	}
	
	return 0;
}

void cScr_NVSpellBase::SetCastingSpell(bool bOn)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	if ( bOn )
		pQSrv->Set("CastingSpell", 1, 0);
	else
		pQSrv->Set("CastingSpell", 0, 0);
}

bool cScr_NVSpellBase::UseMana(int iCost)
{
		SService<IQuestSrv> pQSrv(g_pScriptManager);
			
		int iCurrent = 0;	// Default value if the QVar is not set
		
		if (pQSrv->Exists("Mana"))
		{
			iCurrent = pQSrv->Get("Mana");	// Get QVar value
		}
		
		if ( iCurrent >= iCost )
		{
			iCurrent -= iCost;
			pQSrv->Set("Mana", iCurrent, 0);
			
			DisplayPrintf("Mana is now %i", iCurrent);
			return true;
		} else {
			return false;
		}
}

void cScr_NVSpellBase::BeginWindup()
{
//	DisplayPrintf("Winding up spell...");
}

void cScr_NVSpellBase::FinishWindup()
{
//	DisplayPrintf("Windup complete.");
	
	m_iWindingUp = 2;
	object oArm =  0;
	sLink slOLink;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
	{
		object oArm = slOLink.dest;
		SimplePost(0, oArm, "SpellFinishWindup");
		SimplePost(0, oArm, "SpellCleanupWindup");
	}
}

void cScr_NVSpellBase::AbortSpell()
{
//	DisplayPrintf("Spell aborted!");
	
	EndSpellEffect();
	
	m_iWindingUp = 0;
	SetCastingSpell(false);
	
	object oArm =  0;
	sLink slOLink;
	long linkID;
	linkID = GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1);
	
	if ( linkID )
	{
		object oArm = slOLink.dest;
		SimplePost(0, oArm, "SpellAbort");
		SimplePost(0, oArm, "SpellCleanupWindup");
		SimplePost(0, oArm, "SpellCleanupHold");
		
		SService<ILinkSrv> pLS(g_pScriptManager);
		pLS->Destroy(linkID);
	}
	
	if (m_hWindupTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hWindupTimerHandle);
		m_hWindupTimerHandle = NULL;
	}
	
	SService<IWeaponSrv> pWSrv(g_pScriptManager);
	pWSrv->UnEquip(m_iObjId);
	
	pWSrv->Equip(m_iObjId, 1);
	
	cScr_NVNewWeapon::StartListener();
}

void cScr_NVSpellBase::ReleaseSpell()
{
//	DisplayPrintf("Spell released!");
	
	SetCastingSpell(false);
	
	object oArm =  0;
	sLink slOLink;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
	{
		object oArm = slOLink.dest;
		SimplePost(0, oArm, "SpellRelease");
		SimplePost(0, oArm, "SpellCleanupHold");
	}
}

void cScr_NVSpellBase::StartSpellEffect()
{
//	DisplayPrintf("Starting spell effect...");
	
	object oSchema = NVParamGetNVObject(m_iObjId, m_szName, "StartEffectSchema", 0);
	if ( oSchema != 0 )
	{
		SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
		true_bool bReturn;
		pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, oSchema SOUND_NET);
	}
	
	object oArm =  0;
	sLink slOLink;
	
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
	{
		object oArm = slOLink.dest;
		SimplePost(0, oArm, "SpellStartEffect");
		
		const char* szLimbModel = NVParamGetString(m_iObjId, m_szName, "ActiveLimbModel", NULL);
		if ( szLimbModel != NULL )
		{
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			pPropSrv->Set(oArm, "ModelName", NULL, szLimbModel);
		}
	}
}

void cScr_NVSpellBase::EndSpellEffect()
{
	StopDrain();
	
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);

	object oSchema = NVParamGetNVObject(m_iObjId, m_szName, "StopEffectSchema", 0);
	if ( oSchema != 0 )
	{
		true_bool bReturn;
		pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, oSchema SOUND_NET);
	}
	
	object oArm =  0;
	sLink slOLink;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
	{
		object oArm = slOLink.dest;
		SimplePost(0, oArm, "SpellStopEffect");
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		
//		if (pPropSrv->Possessed(m_iObjId, "	InvLimbModel"))
//		{
			cMultiParm mpValue;
			pPropSrv->Get(mpValue, m_iObjId, "InvLimbModel", NULL);
			pPropSrv->Set(oArm, "ModelName", NULL, (const char*)mpValue);
//		}
	}
}

/***********************
 * NVSpellInvisibility
 */
 
 void cScr_NVSpellInvisibility::BeginWindup()
{
	cScr_NVSpellBase::BeginWindup();
	
//	SService<IDarkInvSrv> pSrv(g_pScriptManager);
//	pSrv->AddSpeedControl("NVSpellInvisibility", 0.501f, 1.00f);
}

void cScr_NVSpellInvisibility::FinishWindup()
{
	cScr_NVSpellBase::FinishWindup();
//	DisplayPrintf("Invisible, whoohoo!");
	StartSpellEffect();
	StartDrain();
}

void cScr_NVSpellInvisibility::AbortSpell()
{
	cScr_NVSpellBase::AbortSpell();
}

void cScr_NVSpellInvisibility::ReleaseSpell()
{
	cScr_NVSpellBase::ReleaseSpell();
	EndSpellEffect();
}

void cScr_NVSpellInvisibility::StartSpellEffect()
{
	cScr_NVSpellBase::StartSpellEffect();

//	DisplayPrintf("Starting spell effect...");
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IDarkInvSrv> pSrv(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	
	pSrv->AddSpeedControl("NVSpellInvisibility", 0.00f, 1.00f);
	
	object oPlayer = StrToObject("Player");
		
	pPhysSrv->SetVelocity(oPlayer, cScrVec::Zero);
	
	pPropSrv->Set(oPlayer, "AI_Visibility", "Level", 0);
	pPropSrv->Set(oPlayer, "AI_Visibility", "Light Rating", 0);
	pPropSrv->Set(oPlayer, "AI_Visibility", "Last Update Time", 2147483647l);
	
	object oArm = StrToObject("PlyrArm");
	if ( oArm )
	{
		pPropSrv->Set(oArm, "RenderAlpha", NULL, 0.25);
	}
}

void cScr_NVSpellInvisibility::EndSpellEffect()
{
	cScr_NVSpellBase::EndSpellEffect();
	
	SService<IDarkInvSrv> pSrv(g_pScriptManager);
	pSrv->RemoveSpeedControl("NVSpellInvisibility");
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	
	object oPlayer = StrToObject("Player");
	pPropSrv->Set(oPlayer, "AI_Visibility", "Last Update Time", 0l);
	
	object oArm = StrToObject("PlyrArm");
	if ( oArm )
	{
		pPropSrv->Set(oArm, "RenderAlpha", NULL, 1.00);
	}
}

/***********************
 * NVSpellProjectile
 */
 
 void cScr_NVSpellProjectile::BeginWindup()
{
	cScr_NVSpellBase::BeginWindup();
//	DisplayPrintf("Winding up spell...");
}

void cScr_NVSpellProjectile::FinishWindup()
{
	cScr_NVSpellBase::FinishWindup();
	if ( NVParamGetInt(m_iObjId, m_szName, "AutoFire", 0) == 1 )
	{
		StartSpellEffect();
		ReleaseSpell();
		SService<IWeaponSrv> pWSrv(g_pScriptManager);
		pWSrv->FinishAttack(m_iObjId, m_iObjId);
	}
	else if ( NVParamGetInt(m_iObjId, m_szName, "Stream", 0) == 1 )
	{
		StartSpellEffect();
		StartDrain();
	}/* else {
		DisplayPrintf("Ready to cast!");	
	}*/
}

void cScr_NVSpellProjectile::AbortSpell()
{
	cScr_NVSpellBase::AbortSpell();
}

void cScr_NVSpellProjectile::ReleaseSpell()
{
	cScr_NVSpellBase::ReleaseSpell();
	if ( ( NVParamGetInt(m_iObjId, m_szName, "Stream", 0) == 1 ) || ( NVParamGetInt(m_iObjId, m_szName, "AutoFire", 0) == 1 ) )
	{
		EndSpellEffect();
	} else {
		StartSpellEffect();
	}
}

void cScr_NVSpellProjectile::StartSpellEffect()
{
	cScr_NVSpellBase::StartSpellEffect();
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	
//	int iProjectileType = NVParamGetNVObject(m_iObjId, m_szName, "Type", -1339);
	object oEmitter = StrToObject("Player");
	
	if ( NVParamGetInt(m_iObjId, m_szName, "TargetFromArm", 0) == 1 )
	{
		oEmitter = m_iObjId;
	} else {
	
		if (pPropSrv->Possessed(m_iObjId, "CfgTweq5Emit"))
		{
			pPropSrv->CopyFrom(oEmitter, "CfgTweq5Emit", m_iObjId); // Copy Tweq->Emit# property to player
		}
	}

	pPropSrv->Set(oEmitter, "StTweq5Emit", "Frame #", 0);
	pPropSrv->Set(oEmitter, "StTweq5Emit", "Cur Time", 0);
	pPropSrv->Set(oEmitter, "StTweq5Emit", "AnimS", 1); // Turn on emitter
}

void cScr_NVSpellProjectile::EndSpellEffect()
{
	cScr_NVSpellBase::EndSpellEffect();
	
	if ( NVParamGetInt(m_iObjId, m_szName, "AutoFire", 0) != 1 )
	{
		// Don't turn off the emitter for an auto-fire
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		
		object oEmitter = StrToObject("Player");
		if ( NVParamGetInt(m_iObjId, m_szName, "TargetFromArm", 0) == 1 )
		{
			oEmitter = m_iObjId;
		}
		pPropSrv->Set(oEmitter, "StTweq5Emit", "AnimS", 0); // Turn off emitter
	}
}


/***********************
 * NVSpellEtherealForm
 */
 
 void cScr_NVSpellEtherealForm::BeginWindup()
{
	cScr_NVSpellBase::BeginWindup();
	
//	SService<IDarkInvSrv> pSrv(g_pScriptManager);
//	pSrv->AddSpeedControl("NVSpellInvisibility", 0.501f, 1.00f);
}

void cScr_NVSpellEtherealForm::FinishWindup()
{
	cScr_NVSpellBase::FinishWindup();
//	DisplayPrintf("Ethereal, whoohoo!");
	StartSpellEffect();
	StartDrain();
}

void cScr_NVSpellEtherealForm::AbortSpell()
{
	cScr_NVSpellBase::AbortSpell();
}

void cScr_NVSpellEtherealForm::ReleaseSpell()
{
	cScr_NVSpellBase::ReleaseSpell();
	EndSpellEffect();
}

void cScr_NVSpellEtherealForm::StartSpellEffect()
{
	cScr_NVSpellBase::StartSpellEffect();
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	
	object oPlayer = StrToObject("Player");
	
	float fGravity = NVParamGetFloat(m_iObjId, m_szName, "Gravity", 0.50);
		
	pPhysSrv->SetGravity(oPlayer, fGravity);
	pPhysSrv->SetVelocity(oPlayer, cScrVec::Zero);
	
	pPropSrv->Set(oPlayer, "Class Tags", NULL, NVParamGetString(m_iObjId, m_szName, "ClassTags", "CreatureType EtherealPlayer"));
	
	object oArm = StrToObject("PlyrArm");
	if ( oArm )
	{
		pPropSrv->Set(oArm, "RenderAlpha", NULL, 0.75);
	}
}

void cScr_NVSpellEtherealForm::EndSpellEffect()
{
	cScr_NVSpellBase::EndSpellEffect();
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	
	object oPlayer = StrToObject("Player");
		
	pPhysSrv->SetGravity(oPlayer, 1.00f);
	
	pPropSrv->Set(oPlayer, "Class Tags", NULL, "CreatureType Player");
	
	object oArm = StrToObject("PlyrArm");
	if ( oArm )
	{
		pPropSrv->Set(oArm, "RenderAlpha", NULL, 1.00);
	}
}

/***********************
 * NVForgetTrap
 */

MSGHANDLER cScr_NVForgetTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	
	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed("AIAwareness"), m_iObjId, 0);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		cMultiParm mpReturn;
		pLTS->LinkGetData(mpReturn, lsLinks.Link(), "Level");
		
		int iLevel = static_cast<int>(mpReturn);
		if ( iLevel < 3 )
		{
			pLS->Destroy(lsLinks.Link());
		} else {
			if ( ParamGetInt(m_iObjId, "NVForgetTrapDisplayMessage", 0) != 0 )
			{
				DisplayPrintf("Spell failed: target is too aware.");
			}
		}
	}
	
	return 0;
}
	
MSGHANDLER cScr_NVForgetTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Do nothing
	return 0;
}

#endif // _DARKGAME == 2
