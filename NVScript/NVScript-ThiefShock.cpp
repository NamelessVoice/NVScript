#if _DARKGAME == 3

// #include <cstdlib>
#include <cmath>

#include "lg/interface.h"
#include "lg/scrservices.h"
#include "lg/scrmanagers.h"
#include "lg/scrmsgs.h"
#include "lg/objects.h"
#include "ScriptLib.h"
#include "NVScriptLib.h"
#include "NVScript-ThiefShock.h"

#include "lg/iids.h"

#if (_DARKGAME != 1) // Quite pointless, considering these scripts are all SS2-only
	#define SOUND_NET  ,kSoundNetwork0
#else
	#define SOUND_NET
#endif


/***********************
 * NVVisibilityGem
 */

MSGHANDLER cScr_NVVisibilityGem::OnCreateObjs(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);

	object oVisibilityGem;
	pOS->Create(oVisibilityGem, -3642);
	pOS->SetName(oVisibilityGem, "Visibility Gem");
	pPropSrv->Set(oVisibilityGem, "RenderAlpha", NULL, 1.00f);
	cScrVec vOffset(2.00f, 0.00f, -1.40f), vScale(0.40f, 0.40f, 0.40f);
	pPropSrv->Set(oVisibilityGem, "CameraObj", "Offset", vOffset);
	pPropSrv->Set(oVisibilityGem, "CameraObj", "Draw?", true);
	pPropSrv->Set(oVisibilityGem, "ModelName", NULL, "UICry16");
	pPropSrv->Set(oVisibilityGem, "RenderType", NULL, 2); // unlit
	pPropSrv->Set(oVisibilityGem, "ExtraLight", "Amount (-1..1)", 1.00f); // unlit

	pPropSrv->Set(oVisibilityGem, "Scale", NULL, vScale);

	pShockGame->Equip(23001, oVisibilityGem);


	return 0;
}

MSGHANDLER cScr_NVVisibilityGem::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);

	if (pPropSrv->Possessed(m_iObjId, "AI_Visibility"))
	{
		cMultiParm mpProp;

		pPropSrv->Get(mpProp, m_iObjId, "AI_Visibility", "Level");
		int iLevel = static_cast<int>(mpProp);

		if ( iLevel > 100 )
			iLevel = 100;
		else if ( iLevel < 0 )
			iLevel = 0;

		int iValue = 1 + static_cast<int>(15.00f * (( 100.00f - static_cast<float>(iLevel) ) / 100.00f));

		char szModelName[10];
		sprintf(szModelName, "UICry%02i", iValue);

		object oVisibilityGem;
		pShockGame->Equipped(oVisibilityGem, 23001);

		if ( oVisibilityGem )
			pPropSrv->Set(oVisibilityGem, "ModelName", NULL, szModelName);

	}

	return 0;
}

/***********************
 * NVFrobHilightHack
 */

 MSGHANDLER cScr_NVFrobHilightHack::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
 {
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	object oSelected;

	pShockGame->GetSelectedObj(oSelected);

	if ( oSelected != m_iPrev )
	{
		if ( m_iPrev )
		{
			if (pPropSrv->Possessed(static_cast<int>(m_iPrev), "ExtraLight"))
				pPropSrv->Remove(static_cast<int>(m_iPrev), "ExtraLight");
		}
	}
	if ( oSelected )
	{
		if (pPropSrv->Possessed(oSelected, "HUDSelect"))
		{
			if (pPropSrv->Possessed(oSelected, "AI_Mode"))
			{
				cMultiParm mpMode;
				pPropSrv->Get(mpMode, oSelected, "AI_Mode", NULL);
				if ( static_cast<int>(mpMode) != 5 )
					return 0;
			}

			m_iPrev = oSelected;
			cMultiParm mpProp;

			float fLevel = 0.00f;

			if (pPropSrv->Possessed(oSelected, "ExtraLight"))
			{
				pPropSrv->Get(mpProp, oSelected, "ExtraLight", "Amount (-1..1)");
				fLevel = static_cast<float>(mpProp);
			}

			fLevel += 0.25f;
			if ( fLevel > 0.65f )
				fLevel = 0.65f;
			pPropSrv->Set(oSelected, "ExtraLight", "Amount (-1..1)", fLevel);
		}
	}

	return 0;
 }


/***********************
 * NVKnockoutable
 */

MSGHANDLER cScr_NVKnockoutable::OnKnockout(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IAIScrSrv> pAISrv(g_pScriptManager);
	SService<IPuppetSrv> pPupS(g_pScriptManager);
	true_bool bReturn;

	pAISrv->Signal(m_iObjId, "Knockout");
	pAISrv->ClearAlertness(m_iObjId);
	pAISrv->ClearGoals(m_iObjId);

	pPupS->PlayMotion(bReturn, m_iObjId, "BH213717");
	g_pScriptManager->SetTimedMessage2(m_iObjId, "KnockoutSound", 2000, kSTM_OneShot, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVKnockoutable::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "KnockoutSound"))
		return 0; // Abort if it's not the right timer.

	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	pSoundSrv->HaltSchema(bReturn, m_iObjId, "", 0);
	object oSchema = StrToObject("collapse_player");
	if (oSchema)
	{
		pSoundSrv->PlaySchemaAtObject(bReturn, m_iObjId, oSchema, m_iObjId SOUND_NET);
	}

	return 0;
}

 MSGHANDLER cScr_NVKnockoutable::OnKnockedout(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
 {
	// SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);


	// int iMeta = StrToObject("M-KnockedOut");
	// if ( iMeta )
	// {
		// AddSingleMetaProperty(iMeta, m_iObjId);
	// }

	pPropSrv->Set(m_iObjId, "AI_Mode", NULL, 5);

	return 0;
 }

 /***********************
 * NVArrowHack
 */

 MSGHANDLER cScr_NVArrowHack::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle == NULL)
		{ // Start a timer.
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "ArowHack", 100, kSTM_Periodic, g_mpUndef);
		}

		return 0;
}

MSGHANDLER cScr_NVArrowHack::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "ArowHack"))
		return 0; // Abort if it's not the right timer.

	if ( m_iTime < 30 )
		m_iTime++;
//	DisplayPrintf("%i/30", static_cast<int>(m_iTime));

	return 0;
}

MSGHANDLER cScr_NVArrowHack::OnSlay(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iTime = static_cast<int>(m_iTime);
	if ( iTime > 30 )
		iTime = 30;

	object oArch = ParamGetObject(m_iObjId, "NVHackedArrowType", 0);
	if (!oArch)
		return 0;

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);

	object oArrow;

	pPhysSrv->LaunchProjectile(oArrow, m_iObjId, oArch, static_cast<float>(static_cast<float>(iTime) / 30.00f), 0, cScrVec::Zero);

	// pOS->Create(oArrow, oArch);

	if ( oArrow )
	{

		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		link lLink;

		pLS->Create(lLink, pLTS->LinkKindNamed("Firer"), StrToObject("Player"), oArrow);

		// pOS->Teleport(oArrow, cScrVec::Zero, cScrVec::Zero, m_iObjId);
		// pPropSrv->Set(m_iObjId, "Initted", NULL, true);

		// cScrVec vVel;
		// if (pPropSrv->Possessed(oArrow, "PhysInitVel"))
		// {
			// cMultiParm mpReturn;
			// pPropSrv->Get(mpReturn, oArrow, "PhysInitVel", NULL);
			// vVel.x = static_cast<const mxs_vector*>(mpReturn)->x;
			// vVel.x = vVel.x * (31 - iTime) / 100;
			// pPropSrv->Set(oArrow, "PhysInitVel", NULL, vVel);
		// }
	}

	return 0;
}

 /***********************
 * NVExtinguishable
 */

 MSGHANDLER cScr_NVExtinguishable::OnLight(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
 {
	SService<ILightScrSrv> pLightSrv(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

//	pLightSrv->Activate(m_iObjId);
	pLightSrv->SetMode(m_iObjId, 8);
	pLS->BroadcastOnAllLinks(m_iObjId, "TurnOn", pLTS->LinkKindNamed("~ParticleAttachement"));
	return 0;
 }

 MSGHANDLER cScr_NVExtinguishable	::OnDouse(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILightScrSrv> pLightSrv(g_pScriptManager);

//	pLightSrv->Deactivate(m_iObjId);
	pLightSrv->SetMode(m_iObjId, 5);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->BroadcastOnAllLinks(m_iObjId, "TurnOff", pLTS->LinkKindNamed("~ParticleAttachement"));
	return 0;
}

/***********************
 * NVMossLump
 */

 MSGHANDLER cScr_NVMossLump::OnPhysFellAsleep(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
 {
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	object oArc;
	pOS->Named(oArc, "MossLauncher");
	if ( oArc)
	{
		object oCreated;
		pOS->Create(oCreated, oArc);
		pPropSrv->CopyFrom(oCreated, "CfgTweqEmit", m_iObjId); // Copy Tweq->Emit property to launcher
		pPropSrv->CopyFrom(oCreated, "StTweqEmit", m_iObjId); // Copy Tweq->EmitterState property to launcher
		pPropSrv->Set(oCreated, "StTweqEmit", "AnimS", 1); // Turn on emitter
		pPropSrv->Set(m_iObjId, "PhysType", "Remove on Sleep", true);
		cScrVec vVelocity(0,0,-2);
		pPhysSrv->SetVelocity(m_iObjId, vVelocity);
		pOS->Teleport(oCreated, cScrVec::Zero, cScrVec::Zero, m_iObjId);

	}


	return 0;
 }

 MSGHANDLER cScr_NVMossLump::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Subscribe to PhysFellAsleep messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysEnterExit);
	return 0;
}

MSGHANDLER cScr_NVMossLump::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Unsubscribe to PhysFellAsleep messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysEnterExit);
	return 0;
}

/***********************
 * NVGong
 */

MSGHANDLER cScr_NVGong::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	RingGong();
	return 0;
}

MSGHANDLER cScr_NVGong::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	SService<IObjectSrv> pOS(g_pScriptManager);

	object oArc;
	true_bool bBool;

	pOS->Named(oArc, "The Player");
	pOS->InheritsFrom(bBool, pPhysMsg->collObj, oArc);
	if ( bBool )
		return 0;

	pOS->Named(oArc, "Monsters");
	pOS->InheritsFrom(bBool, pPhysMsg->collObj, oArc);
	if ( bBool )
		return 0;

	pOS->Named(oArc, "Avatar");
	pOS->InheritsFrom(bBool, pPhysMsg->collObj, oArc);
	if ( bBool )
		return 0;

	pOS->Named(oArc, "Creature");
	pOS->InheritsFrom(bBool, pPhysMsg->collObj, oArc);
	if ( bBool )
		return 0;

	if ( ( pPhysMsg->collSubmod == 1 ) || ( pPhysMsg->collSubmod == 4 ) )
	{
		RingGong();
	}

	return 0;
}

void cScr_NVGong::RingGong(void)
{

	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundOnObj SOUND_NET);

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IAIScrSrv> pAI(g_pScriptManager);

	linkset lsLinks;
	linkkind lkFlavor = pLTS->LinkKindNamed("~ScriptParams");
	pLS->GetAll(lsLinks, lkFlavor, m_iObjId, 0);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		sLink sl = lsLinks.Get();
		void* pLinkData = lsLinks.Data();
		if (pLinkData)
		{
			if ( !stricmp(reinterpret_cast<const char*>(pLinkData), "Owns") )
			{
				pAI->Signal(sl.dest, "gong_ring");
			}
		}
	}
	return;
}

 MSGHANDLER cScr_NVGong::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Subscribe to PhysFellAsleep messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVGong::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Unsubscribe to PhysFellAsleep messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

/***********************
 * NVCollisionStick
 */

MSGHANDLER cScr_NVCollisionStick::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	SService<IObjectSrv> pOS(g_pScriptManager);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	bool bAttach = false;
	if (pPropSrv->Possessed(pPhysMsg->collObj, "CanAttach"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "CanAttach", NULL);
		bAttach = static_cast<bool>(mpProp);
	}

	if ( !bAttach )
	{
		SService<IDamageSrv> pDmgS(g_pScriptManager);
		pDmgS->Slay(m_iObjId, m_iObjId);
	} else {
		if (!pPropSrv->Possessed(m_iObjId, "StackCount"))
			pPropSrv->Set(m_iObjId, "StackCount", NULL, 1);

		pPropSrv->Set(m_iObjId, "PhysControl", "Controls Active", 2+8);
		g_pScriptManager->SetTimedMessage2(m_iObjId, "TakePhysicsAway", 1, kSTM_OneShot, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVCollisionStick::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "TakePhysicsAway"))
		return 0; // Abort if it's not the right timer.

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (pPropSrv->Possessed(m_iObjId, "PhysType"))
	{
		pPropSrv->Remove(m_iObjId, "PhysAttr");
		pPropSrv->Remove(m_iObjId, "PhysControl");
		pPropSrv->Remove(m_iObjId, "PhysDims");
		pPropSrv->Remove(m_iObjId, "PhysType");
	}

	return 0;
}

MSGHANDLER cScr_NVCollisionStick::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Subscribe to Phys messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVCollisionStick::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Unsubscribe to PhysFellAsleep messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

/***********************
 * NVZombieRegen
 */

MSGHANDLER cScr_NVZombieRegen::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Subscribe to Phys messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVZombieRegen::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Unsubscribe to Phys messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVZombieRegen::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	SService<IObjectSrv> pOS(g_pScriptManager);

	object oArc;
	true_bool bBool;

	SService<INetworkingSrv> pNetSrv(g_pScriptManager);
	if ( !pNetSrv->IsPlayer(pPhysMsg->collObj) )
		Resurrect();

	return 0;
}

MSGHANDLER cScr_NVZombieRegen::OnDamage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sDamageScrMsg* pDamageMsg = static_cast<sDamageScrMsg*>(pMsg);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);

	object oHolyStim, oFireStim, oLightBright;
	int iDamage;
	pOS->Named(oHolyStim, "HolyStim");
	pOS->Named(oFireStim, "FireStim");
	pOS->Named(oLightBright, "LightBright");
	iDamage = pDamageMsg->damage;

	if ( pDamageMsg->kind == oLightBright )
		iDamage >>= 1;

	if ( ( pDamageMsg->kind == oLightBright ) || ( pDamageMsg->kind == oHolyStim ) || ( pDamageMsg->kind == oFireStim ) )
	{
		int iMaxHP = 10;

		if (pPropSrv->Possessed(m_iObjId, "MAX_HP"))
		{
			cMultiParm mpProp;

			pPropSrv->Get(mpProp, m_iObjId, "MAX_HP", NULL);
			iMaxHP = static_cast<int>(mpProp);
		}

		iMaxHP -= iDamage;
		pPropSrv->Set(m_iObjId, "MAX_HP", NULL, iMaxHP);
	}

	return 0;
}

MSGHANDLER cScr_NVZombieRegen::OnSlain(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	//SService<IShockGameSrv> pShockGame(g_pScriptManager);

	int iMaxHP = 10;

	if (pPropSrv->Possessed(m_iObjId, "MAX_HP"))
	{
		cMultiParm mpProp;

		pPropSrv->Get(mpProp, m_iObjId, "MAX_HP", NULL);
		iMaxHP = static_cast<int>(mpProp);
	}

	if ( iMaxHP <= 0 )
	{
		pPropSrv->Set(m_iObjId, "SlayResult", NULL, 0);
		int iMeta = ParamGetObject(m_iObjId, "ZombiePartsMeta", StrToObject("M-ZombieParts"));
		if ( iMeta )
		{
			AddSingleMetaProperty(iMeta, m_iObjId);
		}
	} else {
		pPropSrv->Set(m_iObjId, "AI_Mode", NULL, 3);
		pPropSrv->Set(m_iObjId, "HitPoints", NULL, iMaxHP);
		SService<IAIScrSrv> pAISrv(g_pScriptManager);
		pAISrv->Signal(m_iObjId, "Possum");
	}


	return 0;
}

MSGHANDLER cScr_NVZombieRegen::OnResurrect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	Resurrect();

	return 0;
}

void cScr_NVZombieRegen::Resurrect(void)
{
	return;
}

/***********************
 * NVLever
 */

void cScr_NVLever::DoTweq(bool bOn, int iStateObj)
 {
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if ( iStateObj == 0 )
		iStateObj = m_iObjId;

	char *szTweq = "StTweqJoints";
	if (!pPropSrv->Possessed(iStateObj, szTweq) )
	{
		szTweq = "StTweqRotate";
		if (!pPropSrv->Possessed(iStateObj, szTweq) )
			return;
	}

	cMultiParm mpAnimS;
	pPropSrv->Get(mpAnimS, iStateObj, szTweq, "AnimS");
	int iAnimS = static_cast<int>(mpAnimS);

	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;

	if ( bOn )
	{
		pPropSrv->Set(m_iObjId, szTweq, "AnimS", iAnimS | 1); // Activate Joints
		pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event StateChange DirectionState Forward", m_iObjId, 0, kEnvSoundOnObj SOUND_NET);
	} else {
		pPropSrv->Set(m_iObjId, szTweq, "AnimS", iAnimS | 3); // Activate/Reverse Joints
		pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event StateChange DirectionState Reverse", m_iObjId, 0, kEnvSoundOnObj SOUND_NET);
	}

	return;
}

MSGHANDLER cScr_NVLever::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoTweq(true);
	return 0;
}

MSGHANDLER cScr_NVLever::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoTweq(false);
	return 0;
}

MSGHANDLER cScr_NVLever::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	#if (_DARKGAME == 2)
		if (pPropSrv->Possessed(m_iObjId, "DarkStat") )
		{
			cMultiParm mpReturn;
			pPropSrv->Get(mpReturn, m_iObjId, "DarkStat", NULL);
			if ( static_cast<int>(mpReturn) & 4 ) // Hidden
			{
				SService<IDarkGameSrv> pDarkSrv(g_pScriptManager);
				pDarkSrv->FoundObject(m_iObjId);
			}
		}
	}
	#endif

	SService<ILockSrv> pLock(g_pScriptManager);
	if ( !pLock->IsLocked(m_iObjId) )
	{
		int iAnimS = 0;
		if (pPropSrv->Possessed(m_iObjId, "StTweqJoints") )
		{
			cMultiParm mpAnimS;
			pPropSrv->Get(mpAnimS, m_iObjId, "StTweqJoints", "AnimS");
			iAnimS = static_cast<int>(mpAnimS);
		}

		if ( iAnimS & 2 ) // tweq is complete, in reverse mode
		{
			DoTweq(true);
		} else {
			DoTweq(false);
		}
	} else {
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);

		linkset lsLinks;
		pLS->GetAll(lsLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, 0);

		for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
		{

			sLink sl = lsLinks.Get();
			void* pLinkData = lsLinks.Data();
			if (pLinkData)
			{
				if ( !stricmp(static_cast<const char*>(pLinkData), "ErrorOutput") )
				{
					SimpleSend(m_iObjId, sl.dest, "TurnOn");
				}
			}
		}
	}

	return 0;
}

MSGHANDLER cScr_NVLever::OnTweqComplete(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sTweqMsg* pTweqMsg = static_cast<sTweqMsg*>(pMsg);

	if ( pTweqMsg->Type == kTweqTypeJoints )
	{
		if ( pTweqMsg->Dir == kTweqDirForward )
		{
			Trigger(true, pMsg);
		} else {
			Trigger(false, pMsg);
		}
	}

	return 0;
}

#endif // _DARKGAME == 3
