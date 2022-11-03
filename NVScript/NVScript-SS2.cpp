#if _DARKGAME == 3

#include <cmath>

#include "lg/interface.h"
#include "lg/scrservices.h"
#include "lg/scrmanagers.h"
#include "lg/scrmsgs.h"
#include "lg/objects.h"
#include "lg/input.h"
#include "ScriptLib.h"
#include "NVScriptLib.h"
#include "NVScript-SS2.h"

#include "lg/iids.h"

#if (_DARKGAME != 1) // Quite pointless, considering these scripts are all SS2-only
	#define SOUND_NET  ,kSoundNetwork0
#else
	#define SOUND_NET
#endif

#define MAX_INT LONG_MAX

/***********************
 * NVComestible
 */

MSGHANDLER cScr_NVComestible::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	object oFrobber = pFrobMsg->Frobber;
	
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);	
	cMultiParm mpReturn;
	
	int iState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "ObjState", NULL);
		iState = static_cast<int>(mpReturn);
	}
	if ( iState != 0 )
		return 0;
	
	int iHeal = ParamGetInt(m_iObjId, "NVComestibleHeal", 1);
	int iPsi = ParamGetInt(m_iObjId, "NVComestiblePsi", 0);
	float fDeRad = ParamGetFloat(m_iObjId, "NVComestibleDeRad", 0.00f);
	float fDeTox = ParamGetFloat(m_iObjId, "NVComestibleDeTox", 0.00f);
	
	if ( ParamExists(m_iObjId, "NVComestibleTrait"))
	{
		if ( pShockGame->HasTrait(oFrobber, ParamGetInt(m_iObjId, "NVComestibleTrait", 2)))
		{
			if ( iHeal > 0 )
				iHeal = static_cast<int>(iHeal * ParamGetFloat(m_iObjId, "NVComestibleTraitBonus", 1.20f));
				
			if ( iPsi > 0 )
				iPsi = static_cast<int>(iPsi * ParamGetFloat(m_iObjId, "NVComestibleTraitBonus", 1.20f));
				
			if ( fDeRad > 0 )
				fDeRad *= ParamGetFloat(m_iObjId, "NVComestibleTraitBonus", 1.20f);
				
			if ( fDeTox > 0 )
				fDeTox *= ParamGetFloat(m_iObjId, "NVComestibleTraitBonus", 1.20f);
		}
	}
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	
	if (ParamExists(m_iObjId, "NVComestibleDifficultyBonus"))
	{
		int iDifficulty = 0; // Default value (if QVar doesn't exist)
		if (pQSrv->Exists("Difficulty"))
		{
			iDifficulty = pQSrv->Get("Difficulty"); // Get QVar value
		}
		
		if ( iDifficulty <= 2 )	// Apply to Playtest, Easy, Normal...  not Hard or Impossible
		{
			if ( iHeal > 0 )
				iHeal = static_cast<int>(iHeal * ParamGetFloat(m_iObjId, "NVComestibleDifficultyBonus", 1.50f));
				
			if ( iPsi > 0 )
				iPsi = static_cast<int>(iPsi * ParamGetFloat(m_iObjId, "NVComestibleDifficultyBonus", 1.50f));
				
			if ( fDeRad > 0 )
				fDeRad *= ParamGetFloat(m_iObjId, "NVComestibleDifficultyBonus", 1.50f);
				
			if ( fDeTox > 0 )
				fDeTox *= ParamGetFloat(m_iObjId, "NVComestibleDifficultyBonus", 1.50f);
		}
	}
	
	if ( ParamExists(m_iObjId, "NVComestibleQVar") )
	{
		int iQVar = 0;
		const char* pszQVar = ParamGetString(m_iObjId, "NVComestibleQVar", "");
		if (pQSrv->Exists(pszQVar) )
		{
			iQVar = pQSrv->Get(pszQVar); // Get QVar value
		}
		
		
		if ( iQVar != 0 )
		{
			if ( iHeal > 0 )
				iHeal = static_cast<int>(iHeal * ParamGetFloat(m_iObjId, "NVComestibleQVarBonus", 1.20f));
				
			if ( iPsi > 0 )
				iPsi = static_cast<int>(iPsi * ParamGetFloat(m_iObjId, "NVComestibleQVarBonus", 1.20f));
				
			if ( fDeRad > 0 )
				fDeRad *= ParamGetFloat(m_iObjId, "NVComestibleQVarBonus", 1.20f);
				
			if ( fDeTox > 0 )
				fDeTox *= ParamGetFloat(m_iObjId, "NVComestibleQVarBonus", 1.20f);
		}
	}

	
	int iHP = 1;
	int iMaxHP = 10;
	float fRadiation = 0;
	float fToxin = 0;
	if (pPropSrv->Possessed(oFrobber, "HitPoints"))
	{
		pPropSrv->Get(mpReturn, oFrobber, "HitPoints", NULL);
		iHP = static_cast<int>(mpReturn);
	}
	
	if (pPropSrv->Possessed(oFrobber, "MAX_HP"))
	{
		pPropSrv->Get(mpReturn, oFrobber, "MAX_HP", NULL);
		iMaxHP = static_cast<int>(mpReturn);
	}
		
	if (pPropSrv->Possessed(oFrobber, "RadLevel"))
	{
		pPropSrv->Get(mpReturn, oFrobber, "RadLevel", NULL);
		fRadiation = static_cast<float>(mpReturn);
	}
	
	if (pPropSrv->Possessed(oFrobber, "Toxin"))
	{
		pPropSrv->Get(mpReturn, oFrobber, "Toxin", NULL);
		fToxin = static_cast<float>(mpReturn);
	}
	
	int iPlayerPsi = pShockGame->GetPlayerPsiPoints();
	
	if ( ( iHeal < 0 ) && ( -iHeal >= iHP ) )
		return 0;
		
	const char* szNoUseMessage = "";
	bool bOkayToUse = false;
	
	if ( !ParamGetInt(m_iObjId, "NVComestibleNoCheck", 0) )
	{
		if ( fDeRad > 0 )
			if ( fRadiation > 0.00f )
				bOkayToUse = true;
			else
				szNoUseMessage = "CantUseRadPatch";
		
		if ( fDeTox > 0 )
			if ( fToxin > 0.00f )
				bOkayToUse = true;
			else
				szNoUseMessage = "ToxinNone";
		
		if ( iPsi > 0 )
			if ( iPlayerPsi < pShockGame->GetPlayerMaxPsiPoints() )
				bOkayToUse = true;
			else
				szNoUseMessage = "PsiMaxed";
		
		if ( iHeal > 0 )
			if ( iHP < iMaxHP )
				bOkayToUse = true;
			else
				szNoUseMessage = ParamGetString(m_iObjId, "NVComestibleHPMaxedMessage", "HPMaxed");
	} else {
		bOkayToUse = true;
	}
		
	
/*
	if ( !ParamGetInt(m_iObjId, "NVComestibleNoCheck", 0) )
	{
		if ( ( iHeal > 0 ) && ( iPsi <= 0 ) && ( fDeTox <= 0.00f ) && ( fDeRad <= 0.00f ) )
		{
			if ( iHP >= iMaxHP )
			{
				pShockGame->AddTranslatableText(ParamGetString(m_iObjId, "NVComestibleHPMaxedMessage", "HPMaxed"), "Misc", StrToObject("Player"), 5000);
				return 0;
			}
		}
	
		if ( ( iHeal <= 0 ) && ( iPsi > 0 ) && ( fDeTox <= 0.00f ) && ( fDeRad <= 0.00f ) )
		{
			if ( iPlayerPsi >= pShockGame->GetPlayerMaxPsiPoints() )
			{
				pShockGame->AddTranslatableText("PsiMaxed", "Misc", StrToObject("Player"), 5000);
				return 0;
			}
		}
		
		
		if ( ( iHeal <= 0 ) && ( iPsi <= 0 ) && ( fDeTox > 0.00f ) && ( fDeRad <= 0.00f ) )
		{
			if ( fToxin <= 0.00f )
			{
				pShockGame->AddTranslatableText("ToxinNone", "Misc", StrToObject("Player"), 5000);
				return 0;
			}
		}
		
		if ( ( iHeal <= 0 ) && ( iPsi <= 0 ) && ( fDeTox <= 0.00f ) && ( fDeRad > 0.00f ) )
		{
			if ( ( fRadiation <= 0.00f ) )
			{
				pShockGame->AddTranslatableText("CantUseRadPatch", "Misc", StrToObject("Player"), 5000);
				return 0;
			}
		}
	}
*/

	if ( !bOkayToUse)
	{
		pShockGame->AddTranslatableText(szNoUseMessage, "Misc", StrToObject("Player"), 5000);
		return 0;
	}
	
	if (iPsi)
	{
		iPsi += iPlayerPsi;
		pShockGame->SetPlayerPsiPoints(iPsi);
	}
	
	if (iHeal)
		pShockGame->HealObj(oFrobber, iHeal);
	
	if ( fDeTox)
	{
		fToxin -= fDeTox;
		
		if ( fToxin <= 0.00f )
		{
			fToxin = 0.00f;
			pShockGame->OverlayChange(31, 0);
		} else {
			pShockGame->OverlayChange(31, 1);
		}
		pPropSrv->SetSimple(oFrobber, "Toxin", fToxin);
	}
	
	if ( fDeRad )
	{
		fRadiation -= fDeRad;
		if ( fRadiation <= 0.00f )
		{
			fRadiation = 0.00f;
			pShockGame->OverlayChange(30, 0);
		} else {
			pShockGame->OverlayChange(30, 1);
		}
		pPropSrv->SetSimple(oFrobber, "RadLevel", fRadiation);
	}
	
	
	
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundAmbient, kSoundNetwork0);
	SimpleSend(0, m_iObjId, "StackConsume");
	
	cMultiParm mpStack = 0;
	if (pPropSrv->Possessed(m_iObjId, "StackCount"))
	{
		pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
	}
	
	if ( static_cast<int>(mpStack) != 0 )
	{
		SInterface<IContainSys> pContainSys(g_pScriptManager);	
		pContainSys->StackAdd(m_iObjId, -ParamGetInt(m_iObjId, "NVComestibleStackCount", 1), 1);
	} else {
		pShockGame->DestroyInvObj(m_iObjId);
		//SService<IObjectSrv> pOS(g_pScriptManager);
		//pOS->Destroy(m_iObjId);
	}
	return 0;
}

/***********************
 * NVPutIntoContainers
 */

MSGHANDLER cScr_NVPutIntoContainers::OnFrobToolEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobDest = pFrobMsg->DstObjId;
	
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	int iSize = 1;
	if (pPropSrv->Possessed(m_iObjId, "InvDims"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "InvDims", "Width");
		int iWidth = static_cast<int>(mpProp);
		pPropSrv->Get(mpProp, m_iObjId, "InvDims", "Height");
		int iHeight = static_cast<int>(mpProp);
		iSize = iWidth * iHeight;
		if (( (iSize != 1) && (iSize != 3) && (iSize != 4) ) || (iWidth > 2) || (iHeight > 3))
		{
			DisplayPrintf("Item does not fit.");
			return 1;
		}
	}
	object oArc;
	pOS->Named(oArc, "Usable Containers");
	true_bool bBool;
	pOS->InheritsFrom(bBool, iFrobDest, oArc);
	if ( !bBool )
		return 0;
	
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	
	true_bool bLinkExists;
	link lLink;
	
	int iFlavour = pLTS->LinkKindNamed("Contains");
	pLS->AnyExist(bLinkExists, iFlavour, 0, m_iObjId);
	if ( bLinkExists )
	{
		link lLink;
		pLS->GetOne(lLink, iFlavour, 0, m_iObjId);
		if (lLink)
		{
			pLS->Destroy(lLink);
		}
	}
	
	bLinkExists = false;
	pLS->AnyExist(bLinkExists, iFlavour, iFrobDest, 0);
	if (bLinkExists)
	{
		int iContents = 0;
		int iGuns = 0;
		int iArmours = 0;
		
		int iObj;
		int iObjWidth;
		int iObjHeight;
		int iObjSize;
		linkset lsLinks;
		pLS->GetAll(lsLinks, iFlavour, iFrobDest, 0);
		
		while ( lsLinks.AnyLinksLeft() )
		{
			sLink sl = lsLinks.Get();
			iObj = sl.dest;
			iObjSize = 1;
			if (pPropSrv->Possessed(iObj, "InvDims"))
			{
				pPropSrv->Get(mpProp, iObj, "InvDims", "Width");
				iObjWidth = static_cast<int>(mpProp);
				pPropSrv->Get(mpProp, iObj, "InvDims", "Height");
				iObjHeight = static_cast<int>(mpProp);
				iObjSize = iObjWidth * iObjHeight;
				if ( iObjSize == 3 ) // a gun
					iGuns += 1;
				if ( iObjSize == 4 ) // some armour
					iArmours += 1;
			}
			
			iContents += iObjSize;
		
			lsLinks.NextLink();
		}
		if ( iContents > (16 - iSize) )
		{
			DisplayPrintf("Container is full.");
			return 1;
		} else {
			if ( iSize == 4 )
			{
				if ( (( iGuns > 0) && (iArmours > 2)) || (iGuns > 2 ) )
				{
					DisplayPrintf("Container is full.");
					return 1;
				}
			} else
			if ( iSize == 3)
			{
				if ( (( iArmours > 0) && (iGuns > 2)) || (iArmours > 2 ) )
				{
					DisplayPrintf("Container is full.");
					return 1;
				}
			}
			
		}
	}
	
	pLS->Create(lLink, iFlavour, iFrobDest, m_iObjId);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->LoadCursor(0);
	pShockGame->Container(iFrobDest);

	return 0;
}

/***********************
 * NVEndTrap
 */

MSGHANDLER cScr_NVEndTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->EndGame();
	const char* pszEndVideo = ParamGetString(m_iObjId, "NVEndVideo", NULL);
	if ( pszEndVideo )
	{
		pShockGame->PlayVideo(pszEndVideo);
	}
	
	return 0;
}

MSGHANDLER cScr_NVEndTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVDeathCutscene
 */

MSGHANDLER cScr_NVDeathCutscene::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "EndGame"))
		return 0; // Abort if it's not the right timer.

	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	//pShockGame->EndGame();
	const char* pszEndVideo = ParamGetString(m_iObjId, "NVEndVideo", NULL);
	if ( pszEndVideo )
	{
		pShockGame->PlayVideo(pszEndVideo);
	}
	
	return 0;
}

/***********************
 * NVRecalcTrap
 */

MSGHANDLER cScr_NVRecalcTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->RecalcStats(StrToObject("Player"));
	
	return 0;
}

MSGHANDLER cScr_NVRecalcTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->RecalcStats(StrToObject("Player"));
	
	return 0;
}

/***********************
 * NVMultiplayerHack
 */

MSGHANDLER cScr_NVMultiplayerHack::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		cMultiParm mpReturn;
		
		if (pPropSrv->Possessed(m_iObjId, "Initted"))
		{
			pPropSrv->Get(mpReturn, m_iObjId, "Initted", NULL);
			if ( static_cast<bool>(mpReturn) )
			{
				return 0;
			}
		}
		
		pPropSrv->SetSimple(m_iObjId, "Initted", true);
		g_pScriptManager->SetTimedMessage2(m_iObjId, "MultiplayerHack", 100, kSTM_OneShot, g_mpUndef);
	}
	
	return 0;
}

MSGHANDLER cScr_NVMultiplayerHack::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "MultiplayerHack"))
		return 0; // Abort if it's not the right timer.
		
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	
	pPropSrv->Set(m_iObjId, "StartLoc", NULL, 12399);
	
	bool bDoDefaultLevelTransport = true;
	if (pPropSrv->Possessed(m_iObjId, "DestLevel"))
	{
		if (pPropSrv->Possessed(m_iObjId, "DestLoc"))
		{
			cMultiParm mpDestLevel, mpDestLoc;
			pPropSrv->Get(mpDestLevel, m_iObjId, "DestLevel", NULL);
			pPropSrv->Get(mpDestLoc, m_iObjId, "DestLoc", NULL);
			pShockGame->LevelTransport(mpDestLevel.psz, static_cast<int>(mpDestLoc), 2);
			bDoDefaultLevelTransport = false;
		}
	}

	if ( bDoDefaultLevelTransport ) 
	{
		pPropSrv->Set(m_iObjId, "DestLoc", NULL, 12399);
		pShockGame->LevelTransport("earth", 12399, 2);
	}
	
	pShockGame->SetQBHacked("HideInterface", 0);

	pShockGame->Mouse(1, 1);
	pShockGame->OverlayChange(16, 1);
    pShockGame->OverlayChange(15, 1);
    pShockGame->OverlayChange(22, 1);
    pShockGame->OverlayChange(17, 1);
    pShockGame->OverlayChange(28, 1);
    pShockGame->OverlayChange(32, 1);
    pShockGame->OverlayChange(2, 1);
    pShockGame->OverlayChange(41, 0);
	
	object oRespawn;
	pOS->Named(oRespawn, "Multiplayer Respawn");
	if ( !oRespawn )
	{ // Create a 'Multiplayer respawn' if one doesn't exist already, and set up its properties...
		pOS->Create(oRespawn, -327);
		pOS->SetName(oRespawn, "Multiplayer Respawn");
		pOS->Teleport(oRespawn, cScrVec::Zero, cScrVec::Zero, m_iObjId);
	}

	return 0;
}

/***********************
 * NVMultiplayerTrap
 */

MSGHANDLER cScr_NVMultiplayerTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	
	
	if (!pPropSrv->Possessed(m_iObjId, "DestLevel"))
	{
		DisplayPrintf("NVMultiplayerTrap: No destination level specified!");
		DebugPrintf("NVMultiplayerTrap: No destination level specified!");
		return 0;
	}
	if (!pPropSrv->Possessed(m_iObjId, "DestLoc"))
	{
		DisplayPrintf("NVMultiplayerTrap: No destination location specified!");
		DebugPrintf("NVMultiplayerTrap: No destination location specified!");
		return 0;
	}
	
	
	cMultiParm mpDestLevel, mpDestLoc;
	pPropSrv->Get(mpDestLevel, m_iObjId, "DestLevel", NULL);
	pPropSrv->Get(mpDestLoc, m_iObjId, "DestLoc", NULL);
	pShockGame->LevelTransport(mpDestLevel.psz, static_cast<int>(mpDestLoc), 2);

	if ( ParamGetInt(m_iObjId, "NVMultiplayerTrapActivateInterface", 1) )
	{
		pShockGame->SetQBHacked("HideInterface", 0);

		pShockGame->Mouse(1, 1);
		pShockGame->OverlayChange(16, 1);
	    pShockGame->OverlayChange(15, 1);
	    pShockGame->OverlayChange(22, 1);
	    pShockGame->OverlayChange(17, 1);
	    pShockGame->OverlayChange(28, 1);
	    pShockGame->OverlayChange(32, 1);
	    pShockGame->OverlayChange(2, 1);
	    pShockGame->OverlayChange(41, 0);
	}
	
	object oRespawn;
	pOS->Named(oRespawn, "Multiplayer Respawn");
	if ( !oRespawn )
	{ // Create a 'Multiplayer respawn' if one doesn't exist already, and set up its properties...
		pOS->Create(oRespawn, -327);
		pOS->SetName(oRespawn, "Multiplayer Respawn");
		pOS->Teleport(oRespawn, cScrVec::Zero, cScrVec::Zero, m_iObjId);
	}
	
	return 0;
}

MSGHANDLER cScr_NVMultiplayerTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVLightTrap
 */

MSGHANDLER cScr_NVLightTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILightScrSrv> pLightSrv(g_pScriptManager);
//	pLightSrv->SetMode(m_iObjId, 6);
	pLightSrv->Activate(m_iObjId);
	
	return 0;
}

MSGHANDLER cScr_NVLightTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILightScrSrv> pLightSrv(g_pScriptManager);
//	pLightSrv->SetMode(m_iObjId, 7);
	pLightSrv->Deactivate(m_iObjId);
	return 0;
}

/***********************
 * NVStatBasedProjectile
 */

MSGHANDLER cScr_NVStatBasedProjectile::OnThrow(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		SimplePost(m_iObjId, m_iObjId, "Calculate");
	}

	return 0;
}

MSGHANDLER cScr_NVStatBasedProjectile::OnCalculate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) {

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<ISoundScrSrv> pSndSrv(g_pScriptManager);

	if (ParamGetInt(m_iObjId, "NVStatBasedProjectileHasRun", 0)) {
		return 0;
	}

	ParamSetInt(m_iObjId, "NVStatBasedProjectileHasRun", 1);

	link lLink;
	sLink slLink;
	true_bool bRet;
	pLS->GetOne(lLink, pLTS->LinkKindNamed("Firer"), m_iObjId, 0);

	object oFirer = StrToObject("Player");
	if ( lLink)
	{
		pLTS->LinkGet(lLink, slLink);
		oFirer = slLink.dest;
	} else {
		DebugPrintf("NVStatBasedProjectile - no firer link available using Player instead");
	}

	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	float fValue = 1.0f;

	if (ParamGetInt(m_iObjId, "NVStatBasedProjectileAsBonus", 0)) {
		if (pPropSrv->Possessed(m_iObjId, "arSrcScale")) {
			cMultiParm mpProp;
			pPropSrv->Get(mpProp, m_iObjId, "arSrcScale", NULL);
			fValue = static_cast<float>(mpProp);
		}
	}

	int iStatNum = ParamGetInt(m_iObjId, "NVStatBasedProjectileStat", 3);

	if (iStatNum != -1) {
		int iStat = pShockGame->GetStat(oFirer, iStatNum);
		float fScale = ParamGetFloat(m_iObjId, "NVStatBasedProjectileScale", 0.10f);
		fValue *= (1.0f + (fScale*static_cast<float>(iStat-1)));
	}

	int iTraitNum = ParamGetInt(m_iObjId, "NVStatBasedProjectileTrait", -1);

	if (iTraitNum != -1) {
		if (pShockGame->HasTrait(oFirer, iTraitNum)) {
			float fTraitScale = ParamGetFloat(m_iObjId, "NVStatBasedProjectileTraitScale", 0.10f);
			fValue *= (1.0f + fTraitScale);
		}
	}

	pPropSrv->Set(m_iObjId, "arSrcScale", NULL,  fValue);
	
	return 0;
}

/***********************
 * NVRegenPsi
 */
 

MSGHANDLER cScr_NVRegenPsi::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{ 
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "RegenPsi", ParamGetInt(m_iObjId, "NVPsiRegenRate", 10000), kSTM_Periodic, g_mpUndef);
	}
		
	return 0;
}

MSGHANDLER cScr_NVRegenPsi::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "RegenPsi"))
		return 0; // Abort if it's not the right timer.
		
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	int iPsi = ParamGetInt(m_iObjId, "NVPsiRegenAmount", 1) + pShockGame->GetPlayerPsiPoints();
	if ( iPsi < 0 )
		iPsi = 0;
	pShockGame->SetPlayerPsiPoints(iPsi);
	return 0;
}

/***********************
 * NVAmmoScrpit
 */

MSGHANDLER cScr_NVAmmoScript::OnFrobToolEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobDest = pFrobMsg->DstObjId;

	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SInterface<IContainSys> pContainSys(g_pScriptManager);
	
	if ( pContainSys->IsHeld(StrToObject("Player"), m_iObjId) != MAX_INT)
	{
		object oEquipped;
		
		pShockGame->Equipped(oEquipped, 0);
		if ( oEquipped == iFrobDest)
		{
			if (! pShockGame->AmmoLoad(iFrobDest, m_iObjId) )
			{
				pShockGame->PreventSwap();
			}
		}
	}
	
	return 0;
}

MSGHANDLER cScr_NVAmmoScript::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	object oEquipped;
		
	pShockGame->Equipped(oEquipped, 0);
	
	if (oEquipped)
		pShockGame->AmmoLoad(oEquipped, m_iObjId);
		
	return 0;
}

/***********************
 * NVImplant
 */
 
 void cScr_NVImplant::StartDrainTimer(void)
 {
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		cMultiParm mpProp;
		float fDrainRate = 10.00f;
		if (pPropSrv->Possessed(m_iObjId, "DrainRate"))
		{
			pPropSrv->Get(mpProp, m_iObjId, "DrainRate", NULL);
			fDrainRate = static_cast<float>(mpProp);
		}
		
		int iDrainRate = static_cast<int>(fDrainRate * 1000);
	
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVImplantDrain", iDrainRate, kSTM_OneShot, g_mpUndef);
	}
 }
 
 void cScr_NVImplant::StartUse(void)
 {
	ParamSetInt(m_iObjId, "NVImplantUsage", 1);
	StartDrainTimer();
	DoOffEffect(false);
	DoEffect(true);

	SimpleSend(0, m_iObjId, "ImplantOn");
}

void cScr_NVImplant::StopUse(void)
{
	ParamSetInt(m_iObjId, "NVImplantUsage", 0);
	
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	DoEffect(false);
	
	SimpleSend(0, m_iObjId, "ImplantOff");
}

void cScr_NVImplant::DoEffect(bool bOn)
{
	int iMeta = ParamGetObject(m_iObjId, "NVImplantMeta", 0);
	object oPlayer = StrToObject("Player");
	if ( iMeta )
	{
		if (bOn)
			AddSingleMetaProperty(iMeta, oPlayer);
		else
			RemoveSingleMetaProperty(iMeta, oPlayer);
	}
	
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	float fSpeed = ParamGetFloat(m_iObjId, "NVImplantSpeed", 0.00f);
	
	if ( ( bOn ) && (fSpeed > 0.00f ) )
	{
		pShockGame->AddSpeedControl("NVImplant", fSpeed, 1.00f);
	} else {
		pShockGame->RemoveSpeedControl ("NVImplant");
	}
	
	return;
}

void cScr_NVImplant::DoOffEffect(bool bOn)
{
	int iMeta = ParamGetObject(m_iObjId, "NVImplantOffMeta", 0);
	object oPlayer = StrToObject("Player");
	if ( iMeta )
	{
		if (bOn)
			AddSingleMetaProperty(iMeta, oPlayer);
		else
			RemoveSingleMetaProperty(iMeta, oPlayer);
	}
	iMeta = ParamGetObject(m_iObjId, "NVImplantOffSelfMeta", 0);
	if ( iMeta )
	{
		if (bOn)
			AddSingleMetaProperty(iMeta, m_iObjId);
		else
			RemoveSingleMetaProperty(iMeta, m_iObjId);
	}

	return;
}

bool cScr_NVImplant::IsEquipped(void)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	object oEquipped;
	
	pShockGame->Equipped(oEquipped, 3); // Implant shot 1
	if ( oEquipped != m_iObjId)
	{
		pShockGame->Equipped(oEquipped, 4); // Implant slot 2
		if ( oEquipped != m_iObjId)
		{
			pShockGame->Equipped(oEquipped, 2); // Armour slot
			if ( oEquipped != m_iObjId)
			{
				pShockGame->Equipped(oEquipped, 0); // Weapon slot
				if ( oEquipped != m_iObjId)
				{
					return false;
				}
			}
		
		}
	}
	
	return true;
}

MSGHANDLER cScr_NVImplant::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	if ( IsEquipped() )
	{	
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		cMultiParm mpProp;
		float fEnergy = 0.00f;
		if (pPropSrv->Possessed(m_iObjId, "Energy"))
		{
			pPropSrv->Get(mpProp, m_iObjId, "Energy", NULL);
			fEnergy = static_cast<float>(mpProp);
		}
		
		if ( fEnergy > 0.00f)
		{
			StartDrainTimer();
			ParamSetInt(m_iObjId, "NVImplantUsage", 1);
		}
	}

	return 0;
}

MSGHANDLER cScr_NVImplant::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	
	return 0;
}

MSGHANDLER cScr_NVImplant::OnTurnOn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	float fEnergy = 0.00f;
	if (pPropSrv->Possessed(m_iObjId, "Energy"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "Energy", NULL);
		fEnergy = static_cast<float>(mpProp);
	}
	
	if ( ParamGetInt(m_iObjId, "NVImplantUsage", 0)  == 0)
	{
		if (fEnergy > 0.00f)
			StartUse();
		else
			DoOffEffect(true);
	}
	
	return 0;
}

MSGHANDLER cScr_NVImplant::OnTurnOff(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	if ( ParamGetInt(m_iObjId, "NVImplantUsage", 0) )
		StopUse();

	DoOffEffect(false);

	return 0;
}

MSGHANDLER cScr_NVImplant::OnRecharge(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	if (!pPropSrv->Possessed(m_iObjId, "Energy"))
	{
		return 0;
	}

	pPropSrv->Get(mpProp, m_iObjId, "Energy", NULL);
	float fEnergy = static_cast<float>(mpProp);
	
	if ( ( fEnergy == 0.00f ) && (IsEquipped() ) )
	{
		if ( ParamGetInt(m_iObjId, "NVImplantUsage", 0)  == 0)
		{
			StartUse();
		}
	}
	
	int iMaintain = 0;
	float fCharge = 0;
	object oPlayer = StrToObject("Player");
	if (pPropSrv->Possessed(oPlayer, "BaseTechDesc"))
	{
		pPropSrv->Get(mpProp, oPlayer, "BaseTechDesc", "Maintain");
		iMaintain = static_cast<int>(mpProp);
	}
	
	if ( static_cast<int>(pMsg->data) == 0 )
	{
		fCharge = 9999;
	} else {
		fCharge = fEnergy + static_cast<int>(pMsg->data);
	}
	
	if ( fCharge > (iMaintain * 10) + 100.00f)
	{
		fCharge = (iMaintain * 10) + 100.00f;
	}
	
	pPropSrv->SetSimple(m_iObjId, "Energy", fCharge);
	
	if ( ( pMsg->data != "") && (fCharge > fEnergy) )
	{
		SimplePost(m_iObjId, pMsg->from, "Consume");
	}
	
	return 0;
}

void cScr_NVImplant::ProcessDrainTimer()
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	cMultiParm mpProp;
	float fEnergy = 0.00f;
	if (pPropSrv->Possessed(m_iObjId, "Energy"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "Energy", NULL);
		fEnergy = static_cast<float>(mpProp);
	}
	
	if (pPropSrv->Possessed(m_iObjId, "DrainAmt"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "DrainAmt", NULL);
		fEnergy -= static_cast<float>(mpProp);
	}
	
	if ( fEnergy < 0.00f )
		fEnergy = 0.00f;
		
	pPropSrv->SetSimple(m_iObjId, "Energy", fEnergy);
	pShockGame->RefreshInv();
	if ( fEnergy <= 0.00f )
	{
		if ( ParamGetInt(m_iObjId, "NVImplantUsage", 0) )
		{
			SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
			true_bool bReturn;
			pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, StrToObject("bb07") SOUND_NET);
			StopUse();
			DoOffEffect(true);
		}
	} else {
		m_hTimerHandle = NULL;
		StartDrainTimer();
	}
}

MSGHANDLER cScr_NVImplant::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVImplantDrain"))
		return 0; // Abort if it's not the right timer.

	ProcessDrainTimer();
	
	return 0;
}

MSGHANDLER cScr_NVImplant::OnFrobToolEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobDest = pFrobMsg->DstObjId;
	
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	object oArc;
	pOS->Named(oArc, "Recharging Station");
	true_bool bBool;
	pOS->InheritsFrom(bBool, iFrobDest, oArc);
	if ( bBool )
	{
		SimplePost(m_iObjId, m_iObjId, "Recharge");
		
		SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
		true_bool bReturn;
		pSoundSrv->PlayEnvSchema(bReturn, iFrobDest, "Event Activate", iFrobDest, 0, kEnvSoundAtObjLoc, kSoundNetwork0);
		
		pShockGame->PreventSwap();
	}
	
	return 0;
}

/***********************
 * NVGillsImplant
 */

MSGHANDLER cScr_NVGillsImplant::OnImplantOn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	object oPlayer = StrToObject("Player");
	if (pPropSrv->Possessed(oPlayer, "AirSupply"))
	{
		cMultiParm mpProp;
		pPropSrv->Get(mpProp, oPlayer, "AirSupply", NULL);
		int iBreath = static_cast<int>(mpProp);
		
		ParamSetInt(m_iObjId, "NVGillsImplantAirSupply", iBreath);
		
		if (m_hTimerHandle2 == NULL)
		{ // Start a timer.
			m_hTimerHandle2 = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGillsImplant", 100, kSTM_Periodic, g_mpUndef);
		}
	}
		
	return 0;
}

MSGHANDLER cScr_NVGillsImplant::OnImplantOff(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle2 != NULL)
	{ // stop a timer.
		g_pScriptManager->KillTimedMessage(m_hTimerHandle2);
		m_hTimerHandle2 = NULL;
	}
		
	return 0;
}

MSGHANDLER cScr_NVGillsImplant::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVGillsImplant"))
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		object oPlayer = StrToObject("Player");
		if (pPropSrv->Possessed(oPlayer, "AirSupply"))
		{
			cMultiParm mpProp;
			pPropSrv->Get(mpProp, oPlayer, "AirSupply", NULL);
			int iBreath = static_cast<int>(mpProp);
			
			int iOldBreath = ParamGetInt(m_iObjId, "NVGillsImplantAirSupply", 1000);

			if (iBreath < iOldBreath) {
				pPropSrv->Set(oPlayer, "AirSupply", NULL, iOldBreath);
			} else { 
				ParamSetInt(m_iObjId, "NVGillsImplantAirSupply", iBreath);
			}
		}
	}
	else if (!stricmp(pszMsgName, "NVImplantDrain"))
	{
		ProcessDrainTimer();
	}
	
	return 0;
}

class cNVPlayerScriptOverlay : public IShockOverlayHandler
{
private:
	int m_iHandle;
	int m_O2ImageBack;
	int m_O2ImageFront;
	int m_iPlayerId;
public:
	

	cNVPlayerScriptOverlay()
	{
		m_O2ImageBack = -2;
		m_O2ImageFront = -2;
		m_iHandle = -2;
		m_iPlayerId = -2;
	}

	STDMETHOD_(void,DrawHUD)()
	{
		// This is no longer needed; it looks like transparent overlays now work in NewDark 2.44 !
		/*
		SService<IShockOverlaySrv> pShockOverlay(g_pScriptManager);

		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (pPropSrv->Possessed(m_iPlayerId, "AirSupply") && pPropSrv->Possessed(m_iPlayerId, "BreathConfig"))
		{
			cMultiParm mpProp;
			pPropSrv->Get(mpProp, m_iPlayerId, "AirSupply", NULL);
			int iBreath = static_cast<int>(mpProp);

			pPropSrv->Get(mpProp, m_iPlayerId, "BreathConfig", "Max Air (ms)");
			int iMaxBreath = static_cast<int>(mpProp);
			
			if (iBreath != iMaxBreath)
			{
				float mult = 94; // 96; // Should be 96, but using 94 because of the hack to avoid buggy script services
				int width = (int)((float)iBreath / (float)iMaxBreath * mult);

				int x, y, w, h;
				pShockOverlay->GetOverlayRect(kOverlayAlarm, x, y, w, h);
				
				y -= (8 + 32);
				
				DebugPrintf("ing x %i y %i", x ,y );

//				DisplayPrintf("Draw HUD %i breath %i (%i / %i)", m_O2ImageBack, width, iBreath, iMaxBreath);
				pShockOverlay->DrawBitmap(m_O2ImageBack, x, y);
				//pShockOverlay->DrawBitmap(m_O2ImageFront, x + 32 + width, y);
				// TOOD: DrawSubBitmap seems to be bugged
				pShockOverlay->DrawSubBitmap(m_O2ImageFront, x + 32, y, 0, 0, width, 32);
			}
		}
		*/
	}

	void cNVPlayerScriptOverlay::init(int objId)
	{
		m_iPlayerId = objId;
		
		SService<IShockOverlaySrv> pShockOverlay(g_pScriptManager);

		if (m_O2ImageBack == -2)
		{
			m_O2ImageBack = pShockOverlay->GetBitmap("oxygenback", "iface\\");
		}

		if (m_O2ImageFront == -2)
		{
			m_O2ImageFront = pShockOverlay->GetBitmap("oxygenfront", "iface\\");
			//m_O2ImageFront = pShockOverlay->GetBitmap("oxygenline", "iface\\");
		}
		
		// We can't init the overlay here - the game doesn't know the rect sizes yet.  :(
		/*
		int x, y, w, h;
		pShockOverlay->GetOverlayRect(kOverlayAlarm, x, y, w, h);
		
		y -= (8 + 32);
		
		if (m_iHandle == -2)
		{
			m_iHandle = pShockOverlay->CreateTOverlayItem(x, y, 128, 32, 127, TRUE);
		}
		*/	
	}

	STDMETHOD_(void,DrawTOverlay)()
	{
		// This seems to be bugged and crashes the game.
		
		SService<IShockOverlaySrv> pShockOverlay(g_pScriptManager);

		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		if (pPropSrv->Possessed(m_iPlayerId, "AirSupply") && pPropSrv->Possessed(m_iPlayerId, "BreathConfig"))
		{
			cMultiParm mpProp;
			pPropSrv->Get(mpProp, m_iPlayerId, "AirSupply", NULL);
			int iBreath = static_cast<int>(mpProp);

			pPropSrv->Get(mpProp, m_iPlayerId, "BreathConfig", "Max Air (ms)");
			int iMaxBreath = static_cast<int>(mpProp);
			
			if (iBreath != iMaxBreath)
			{
				
				SService<IShockOverlaySrv> pShockOverlay(g_pScriptManager);
				
				int x, y, w, h;
				pShockOverlay->GetOverlayRect(kOverlayAlarm, x, y, w, h);
				
				y -= (8 + 32);
				
				if (m_iHandle == -2)
				{
					m_iHandle = pShockOverlay->CreateTOverlayItem(x, y, 128, 32, 178, TRUE);
				}

				if (pShockOverlay->BeginTOverlayUpdate(m_iHandle))
				{
					int width = (int)((float)iBreath / (float)iMaxBreath * 96);

					pShockOverlay->DrawBitmap(m_O2ImageBack, 0, 0);
					pShockOverlay->DrawSubBitmap(m_O2ImageFront, 32, 0, 0, 0, width, 32);

					pShockOverlay->EndTOverlayUpdate();
				}

				pShockOverlay->DrawTOverlayItem(m_iHandle);
			}	
		}
		
	}

	STDMETHOD_(void,OnUIEnterMode)()
	{ }

	STDMETHOD_(BOOL,CanEnableElement)(int which)
	{
		return TRUE;
	}

	STDMETHOD_(BOOL,IsMouseOver)(int x, int y)
	{
		return FALSE;
	}

	STDMETHOD_(BOOL,MouseClick)(int x, int y)
	{
		return FALSE;
	}

	STDMETHOD_(BOOL,MouseDblClick)(int x, int y)
	{
		return FALSE;
	}

	STDMETHOD_(BOOL,MouseDragDrop)(int x, int y, BOOL start_drag, int cursor_mode)
	{
		return FALSE;
	}

	STDMETHOD_(BOOL,HandleKey)(THIS_ int ch, int keycode)
	{
		return FALSE;
	}
};


cNVPlayerScriptOverlay playerOverlay;

/***********************
 * NVPlayerScript
 */
 
// Combining NVTraitHelper, NVBurnoutHelper, NVWeaponAlpha

MSGHANDLER cScr_NVPlayerScript::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	char* pszParam = new char[30];
	
	// Put current traits into design note
	if (pPropSrv->Possessed(m_iObjId, "TraitsDesc"))
	{
		for ( int i = 0; i < 4; ++i )
		{
			sprintf(pszParam, "Trait %i", (i+1));
			pPropSrv->Get(mpProp, m_iObjId, "TraitsDesc", pszParam);
			int iParam = static_cast<int>(mpProp);
			sprintf(pszParam, "NVPlayerScriptTrait%i", i);
			ParamSetInt(m_iObjId, pszParam, iParam);
		}
	}
	
	delete[] pszParam;

	object oXP;
	pShockGame->Equipped(oXP, 11);
	
	//Check cyber module stack count:
	cMultiParm mpStack = 0;
	if (pPropSrv->Possessed(oXP, "StackCount"))
	{
		pPropSrv->Get(mpStack, oXP, "StackCount", NULL);
	}
	
	ParamSetInt(m_iObjId, "NVPlayerScriptCurrentCyberModules", static_cast<int>(mpStack));
	
	StartListener();
	
	cMultiParm mpReturn;
	// Check for alarm state
	if (pPropSrv->Possessed(m_iObjId, "HackVisi"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "HackVisi", NULL);
		if ( static_cast<float>(mpReturn) == 0.00 )
		{
			pShockGame->DisableAlarmGlobal();
			pShockGame->RemoveAlarm();
			pShockGame->OverlayChange(29, 1);	
		}
	}

	SService<IShockOverlaySrv> pShockOverlay(g_pScriptManager);
	playerOverlay.init(m_iObjId);
	pShockOverlay->SetHandler(&playerOverlay);
	
	return 0;
}

MSGHANDLER cScr_NVPlayerScript::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	StopListener();

	SService<IShockOverlaySrv> pShockOverlay(g_pScriptManager);
	pShockOverlay->SetHandler(NULL);
	return 0;
}

cScr_NVPlayerScript::~cScr_NVPlayerScript()
{
	StopListener();
	if ( m_pProp )
	{
		m_pProp->Release();
	}
}

#pragma pack(push,1)
struct sTraits
{
	int iTraits[4];
};
#pragma pack(pop)

void __stdcall cScr_NVPlayerScript::ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData)
{
	cScr_NVPlayerScript* pSelf = reinterpret_cast<cScr_NVPlayerScript*>(pData);
	int iObjId = pSelf->m_iObjId;
	if (sMsg->iObjId == iObjId)
	{ // Do nothing UNLESS this is the object with the script...
		sTraits* pTraits;
		pTraits = reinterpret_cast<sTraits*>(sMsg->pData);
		
		char szTraitParam[30];
		for ( int i = 0; i < 4; i++)
		{
//			DisplayPrintf("Trait %i is %i", i, pTraits->iTraits[i]);
			if ( pTraits->iTraits[i] > 0 )
			{
				sprintf(szTraitParam, "NVPlayerScriptTrait%i", i);
				
//				DisplayPrintf("Old trait %i is %i", i, ParamGetInt(iObjId, szTraitParam, 0));
				if ( pTraits->iTraits[i] != ParamGetInt(iObjId, szTraitParam, 0) )
				{
//					DisplayPrintf("Gained trait: %i", pTraits->iTraits[i]);
					char szParam[16];
					sprintf(szParam, "NVTrait%iMeta", pTraits->iTraits[i]);
					int iMeta = ParamGetObject(iObjId, szParam, 0);
					if ( iMeta)
					{
						AddSingleMetaProperty(iMeta, iObjId);
					}
					sprintf(szParam, "NVTrait%iSpeed", pTraits->iTraits[i]);
					float fSpeed = ParamGetFloat(iObjId, szParam, 0.00f);
					if ( fSpeed > 0.00f )
					{
						SService<IShockGameSrv> pShockGame(g_pScriptManager);
						pShockGame->AddSpeedControl(szParam, fSpeed, 1.00f);

						// HACK!
					}
					
					sprintf(szParam, "NVTrait%iXP", pTraits->iTraits[i]);
					int iXP = ParamGetInt(iObjId, szParam, 0 );
					if ( iXP )
					{
						g_pScriptManager->SetTimedMessage2(iObjId, "AddTraitXP", 100, kSTM_OneShot, iXP);
					}
					
					ParamSetInt(iObjId, szTraitParam, pTraits->iTraits[i]);
					pSelf->StopListener();
					pSelf->StartListener();
				}
			}
		}
	}
	
	return;
}


MSGHANDLER cScr_NVPlayerScript::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SInterface<IContainSys> pContainSys(g_pScriptManager);
	
	object oXP;
	pShockGame->Equipped(oXP, 11);
	
	int iXP = 0;
	if (!stricmp(pszMsgName, "AddTraitXP"))
	{
		pContainSys->StackAdd(oXP, static_cast<int>(pMsg->data), 0);
		iXP += static_cast<int>(pMsg->data);
	}	
	
	//Check cyber module stack count:
	int iStack = 0;
	cMultiParm mpStack;
	if (pPropSrv->Possessed(oXP, "StackCount"))
	{
		pPropSrv->Get(mpStack, oXP, "StackCount", NULL);
		iStack = static_cast<int>(mpStack);
	}
	
	if (!stricmp(pszMsgName, "RegainPsiFromBurnout"))
	{
		SService<IShockGameSrv> pShockGame(g_pScriptManager);
		int iPsi = static_cast<int>(pMsg->data);
		iPsi += pShockGame->GetPlayerPsiPoints();
		pShockGame->SetPlayerPsiPoints(iPsi);
	}
	
	
	// Weapon alpha
	if (!stricmp(pszMsgName, "RadCheck"))
	{
	
		SService<IShockGameSrv> pShockGame(g_pScriptManager);

		char *pszArmName = "PlyrArm";
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		object oEquipped;	
		pShockGame->Equipped(oEquipped, 0);
		if (oEquipped)
		{
				
			if (!pPropSrv->Possessed(oEquipped, "Melee Type"))
			{
				pszArmName = "Player Gun";
			}
			
			object oArm = StrToObject(pszArmName);
			
			
				//Check stack counts:
			cMultiParm mpAlpha1 = 1.00f;
			if (pPropSrv->Possessed(m_iObjId, "RenderAlpha"))
			{
				pPropSrv->Get(mpAlpha1, m_iObjId, "RenderAlpha", NULL);
			}
			float fAlpha1 = static_cast<float>(mpAlpha1);
			
			cMultiParm mpAlpha2 = 1.00f;
			if (pPropSrv->Possessed(oArm, "RenderAlpha"))
			{
				pPropSrv->Get(mpAlpha2, oArm, "RenderAlpha", NULL);
			}
			float fAlpha2 = static_cast<float>(mpAlpha2);
			
			if ( ( fAlpha1 < fAlpha2 +0.04 ) && ( fAlpha1 > fAlpha2 -0.04 ) ) // Accursed rounding errors!
			{
				fAlpha2 = fAlpha1;
			} else {
			
				if ( fAlpha1 > fAlpha2 )
					fAlpha2 += 0.05f;
				else
					fAlpha2 -= 0.05f;
					
				if ( fAlpha2 < 0.00f)
					fAlpha2 = 0.00f;
				else if ( fAlpha2 > 1.00f)
					fAlpha2 = 1.00f;
			}
				
			pPropSrv->Set(oArm, "RenderAlpha", NULL, fAlpha2);
		}
	}
	
	if (!stricmp(pszMsgName, "RadCheck"))
	{
		// Extra cyber modules for trait 6
		int iCyber = ParamGetInt(m_iObjId, "NVPlayerScriptCurrentCyberModules", 0);
		if ( iStack != iCyber )
		{
			int iChange = iStack - iCyber;
			ParamSetInt(m_iObjId, "NVPlayerScriptCurrentCyberModules", iStack);
			if ( iChange < 0 )
			{
				iChange = 0;
			} else {
				
				// Let's deal with that nasty undo bug
				// Ignore any changes if an upgrade overlay (35, 36, 37, 38) is open.
				for ( int i = 35; i <= 38; ++i )
				{
					if ( pShockGame->OverlayOn(i) )
					{
//						DisplayPrintf("No free cyber modules for you, evil undoer!");
						iChange = 0;
					}
				}
			}
			
			if (pPropSrv->Possessed(oXP, "ExP"))
			{
				pPropSrv->Get(mpStack, oXP, "ExP", NULL);
				iXP += static_cast<int>(mpStack);
			}
			iXP += iChange;
			

			if ( pShockGame->HasTrait (m_iObjId, 6))
			{
				int iThreshold = ParamGetInt(m_iObjId, "NVCybBonusThreshold", 0);
				object oSchema = ParamGetObject(m_iObjId, "NVCybBonusSchema", 0);
				if ( iThreshold != 0 )
				{
					while ( iXP >= iThreshold)
					{
						iXP -= iThreshold;
						pContainSys->StackAdd(oXP, 1, 0);

						if ( oSchema)
						{
							SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
							true_bool bReturn;
							
							pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, oSchema SOUND_NET);
						}
					}
				}
			} else {
				iXP = 0;
			}
			pPropSrv->Set(oXP, "ExP", NULL, iXP);	
		}
	
	
		// Fix research levels
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		
		link lLink;
		sLink slLink;
		pLS->GetOne(lLink, pLTS->LinkKindNamed("Research"), m_iObjId, 0);
		if ( lLink)
		{
			pLTS->LinkGet(lLink, slLink);
			
			cMultiParm mpReturn;
			
			int iResearch = 0;
			int iResearchNeeded = 0;
			
			if (pPropSrv->Possessed(m_iObjId, "BaseTechDesc"))
			{
				pPropSrv->Get(mpReturn, m_iObjId, "BaseTechDesc", "Research");
				iResearch += static_cast<int>(mpReturn);
			}
			
			if (pPropSrv->Possessed(m_iObjId, "ImplTechDesc"))
			{
				pPropSrv->Get(mpReturn, m_iObjId, "ImplTechDesc", "Research");
				iResearch += static_cast<int>(mpReturn);
			}
			
			if (pPropSrv->Possessed(slLink.dest, "ReqTechDesc"))
			{
				pPropSrv->Get(mpReturn, slLink.dest, "ReqTechDesc", "Research");
				iResearchNeeded = static_cast<int>(mpReturn);
			}
			
//			DisplayPrintf("You're researching. (%i skill, %i required)", iResearch, iResearchNeeded);
			if ( iResearchNeeded > iResearch )
			{
//				DisplayPrintf("Cheat!  Snapping research!");
				pLS->Destroy(lLink);
			}
		}
		
		if ( pShockGame->OverlayOn(0) )
		{
			object oSelection;
			pShockGame->GetSelectedObj(oSelection);
			
			if (( !oSelection ) || ( !ParamGetInt(oSelection, "NVOverlayElement", 0) ) )
			{
			SInterface<IInputBinder> pInput(g_pScriptManager);
			
				char szBindMouse[32];
				strcpy(szBindMouse, "bind mouse1");
				const char* pszBinding = pInput->ProcessCmd(szBindMouse);

				if (pszBinding == NULL || stricmp(pszBinding, "+drag_and_drop_mode") )
					pInput->Bind("mouse1", "+drag_and_drop_mode");
			}
		}
	}
	
	return 0;
}
		
void cScr_NVPlayerScript::StartListener(void)
{
	if ( !m_pProp )
	{
		SInterface<IPropertyManager> pPM(g_pScriptManager);
		m_pProp = static_cast<IGenericProperty*>(pPM->GetPropertyNamed("TraitsDesc"));
	}
	if ( !m_hListenHandle )
	{
//		DisplayPrintf("DEBUG: Starting listener; listening for settings changes.");

		m_hListenHandle = m_pProp->Listen(kPropertyChange, ListenFunc, reinterpret_cast<PropListenerData__*>(this));
	} 
	return;
}

void cScr_NVPlayerScript::StopListener(void)
{
	if ( m_hListenHandle && m_pProp )
	{
//		DisplayPrintf("DEBUG: Script exited. Shutting down listener.");
		m_pProp->Unlisten(m_hListenHandle);
		m_hListenHandle = NULL;
	}
	return;
}

MSGHANDLER cScr_NVPlayerScript::OnBurnout(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;

	if ( ParamGetInt(m_iObjId, "NVRegainPsiFromBurnout", 1) )
	{
		if ( pShockGame->HasTrait (m_iObjId, 14))
		{
			if (pPropSrv->Possessed(m_iObjId, "PsiState"))
			{
				int iPower = 0;
				pPropSrv->Get(mpProp, m_iObjId, "PsiState", "");
				iPower = static_cast<int>(mpProp);
				
				if ( iPower != 40 )
				{
					int iPsi = 1 + (iPower / 8);
					g_pScriptManager->SetTimedMessage2(m_iObjId, "RegainPsiFromBurnout", 10, kSTM_OneShot, iPsi);
					pShockGame->StartFadeIn(2000, 0, 128, 255);
				}
			}
		}
	}

	return 0;
}

/***********************
 * NVSecurityComputer
 */

MSGHANDLER cScr_NVSecurityComputer::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	cMultiParm mpProp;
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	
	pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundAmbient, kSoundNetwork0);
	
	pShockGame->OverlayChangeObj(42, 1, m_iObjId);
	
	int iState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "ObjState", NULL);
		iState = static_cast<int>(mpProp);
	}
	
	if ( iState == 0 )
	{
		if ( pShockGame->HasTrait (pFrobMsg->Frobber, 10)) // Security Expert
		{
			pShockGame->DisableAlarmGlobal();
		}
	}
	
	return 0;
}

MSGHANDLER cScr_NVSecurityComputer::OnHackSuccess(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	SService<INetworkingSrv> pNetwork(g_pScriptManager);
	true_bool bReturn;
	
	
	pShockGame->DisableAlarmGlobal();
	pShockGame->RemoveAlarm();

	object oPlayer = StrToObject("Player");
    pPropSrv->SetSimple(oPlayer, "HackVisi", 0);
	pShockGame->RecalcStats(oPlayer);
	pShockGame->OverlayChange(29, 1);
	
	
	pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, StrToObject("xer01"), kSoundNetwork0);
	
	cMultiParm mpProp;
			
	int iHackTime = 0;
	if (pPropSrv->Possessed(m_iObjId, "HackTime"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "HackTime", NULL);
		iHackTime = static_cast<int>(mpProp);
	}
    
	iHackTime *= pShockGame->GetStat(oPlayer, 4);
    pPropSrv->SetSimple(oPlayer, "HackTime", iHackTime + pShockGame->SimTime());
	pNetwork->Broadcast(m_iObjId, "NetHackSuccess", 1, iHackTime);
	
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->BroadcastOnAllLinksData(m_iObjId, "TurnOn", pLTS->LinkKindNamed("HackingLink"), 0);
    
	return 0;
}

MSGHANDLER cScr_NVSecurityComputer::OnNetHackSuccess(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	SService<INetworkingSrv> pNetwork(g_pScriptManager);
	true_bool bReturn;
	
	
	object oPlayer = StrToObject("Player");
    pPropSrv->SetSimple(oPlayer, "HackVisi", 0);
	pShockGame->RecalcStats(oPlayer);
	pShockGame->OverlayChange(29, 1);
	
	pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, StrToObject("xer01"), kSoundNetwork0);
    pPropSrv->Set(oPlayer, "HackTime", NULL, static_cast<int>(pMsg->data) + pShockGame->SimTime());
    
	return 0;
}

MSGHANDLER cScr_NVSecurityComputer::OnHackCritfail(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<INetworkingSrv> pNetwork(g_pScriptManager);
	
	pLS->BroadcastOnAllLinksData(m_iObjId, "Alarm", pLTS->LinkKindNamed("SwitchLink"), pMsg->data);
	pLS->BroadcastOnAllLinksData(m_iObjId, "TurnOff", pLTS->LinkKindNamed("HackingLink"), 0);
	pNetwork->Broadcast(m_iObjId, "NetHackCritfail", 0, 0);
//	object oPlayer = StrToObject("Player");
//	pPropSrv->Set(oPlayer, "HackTime", NULL, 0);
	return 0;
}


// MSGHANDLER cScr_NVSecurityComputer::OnNetHackCritfail(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
// {
	// SService<IPropertySrv> pPropSrv(g_pScriptManager);
	
	// object oPlayer = StrToObject("Player");
	// pPropSrv->Set(oPlayer, "HackTime", NULL, 0);
	// return 0;
// }

/***********************
 * NVUpgradeCheatStopper
 */

MSGHANDLER cScr_NVUpgradeCheatStopper::OnCloseTrainer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);	
	pShockGame->OverlayChange(35, 0);
	pShockGame->OverlayChange(36, 0);
	pShockGame->OverlayChange(37, 0);
	pShockGame->OverlayChange(38, 0);
	
	return 0;
}

/***********************
 * NVPlayerAccuracy
 */
 
MSGHANDLER cScr_NVPlayerAccuracy::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);	
	cScrVec vVelocity;
	pPhysSrv->GetVelocity(m_iObjId, vVelocity);
	
	float fVelocity = vVelocity.Magnitude();
	
	if ( fVelocity > 1.00f )
	{
		m_fSpeed += ( fVelocity / 100 );
	} else {
		m_fSpeed -= 0.02f;
	}
	
	if ( m_fSpeed > 1.00f )
			m_fSpeed = 1.00f;
	
	if ( m_fSpeed < 0.00f )
			m_fSpeed = 0.00f;
			
	int iInaccuracy = static_cast<int>(1500 * m_fSpeed);
	
	object oGun;
	pShockGame->Equipped(oGun, 0);
	
	if ( oGun )
	{

		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		sLink slProjectileLink;

		link lProjectileLink;
		pLS->GetOne(lProjectileLink, pLTS->LinkKindNamed("Projectile"), oGun, 0);
		if ( lProjectileLink)
		{
			pLTS->LinkGet(lProjectileLink, slProjectileLink);
			int iProjectile = slProjectileLink.dest;
			
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			if (!pPropSrv->Possessed(iProjectile, "Projectile"))
				pPropSrv->Set(iProjectile, "Projectile", "spray", 1);
			
//			DisplayPrintf("Inaccuracy: %i", iInaccuracy);
			pPropSrv->Set(iProjectile, "Projectile", "spread", iInaccuracy);
			
		}

	}	
 
	return 0;
}


/***********************
 * NVArmour
 */
 
 MSGHANDLER cScr_NVArmour::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{ 
	SService<IShockGameSrv> pShockGame(g_pScriptManager);		
	
	object oEquipped;
		
	pShockGame->Equipped(oEquipped, 2);
	if ( oEquipped == m_iObjId)
	{	
		if (m_hTimerHandle == NULL)
		{ // Start a timer.
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVArmourRecheck", 5000, kSTM_Periodic, g_mpUndef);
		}
		
		m_iStrength = pShockGame->GetStat(StrToObject("Player"), 0);
	}
		
	return 0;
}
 
MSGHANDLER cScr_NVArmour::OnTurnOn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);		
	m_iStrength = pShockGame->GetStat(StrToObject("Player"), 0);
	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVArmourSlow", 75, kSTM_OneShot, g_mpUndef);

	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVArmourRecheck", 5000, kSTM_Periodic, g_mpUndef);

	return 0;
}

MSGHANDLER cScr_NVArmour::OnTurnOff(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->RemoveSpeedControl ("NVArmour");
	
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	
	return 0;
}

MSGHANDLER cScr_NVArmour::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	
	SService<IShockGameSrv> pShockGame(g_pScriptManager);		
	
	
	if (!stricmp(pszMsgName, "NVArmourSlow"))
	{
//		SService<IDarkInvSrv> pInvSrv(g_pScriptManager);
		float fSpeed = ParamGetFloat(m_iObjId, "NVArmourSpeedMultiplier", 0.00f);
		if ( fSpeed > 0.00f )
		{
			
			if ( fSpeed < 1.00f )
			{
				int iStrength = (pShockGame->GetStat(StrToObject("Player"), 0) - 1);
				float fSTRMult = ParamGetFloat(m_iObjId, "NVArmourSpeedSTRMultiplier", 0.10f);
				fSpeed += (fSTRMult * iStrength);
				if ( fSpeed > 1.00f )
					fSpeed = 1.00f;

			}
			
//			DisplayPrintf("Speed is now %f%%", (fSpeed*100.00f));
			pShockGame->AddSpeedControl("NVArmour", fSpeed, 1.00f);
		}
	} else if (!stricmp(pszMsgName, "NVArmourRecheck"))
	{
		object oEquipped;
		pShockGame->Equipped(oEquipped, 2);
		if ( oEquipped == m_iObjId)
		{
			int iStrength = (pShockGame->GetStat(StrToObject("Player"), 0) - 1);
			if ( m_iStrength != iStrength )
			{
				pShockGame->RemoveSpeedControl ("NVArmour");
				g_pScriptManager->SetTimedMessage2(m_iObjId, "NVArmourSlow", 75, kSTM_OneShot, g_mpUndef);
//				DisplayPrintf("Re-checking armour slowdown.\nStrength has changed!!");
				m_iStrength = iStrength;
			} else {
//				DisplayPrintf("Re-checking armour slowdown!");
			}
		}
	}
		
	return 0;
}


/***********************
 * NVBattery
 */
 
MSGHANDLER cScr_NVBattery::OnFrobToolEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobDest = pFrobMsg->DstObjId;
	
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	SimplePost(m_iObjId, iFrobDest, "Recharge", 1000);
	pShockGame->PreventSwap();
	pShockGame->RefreshInv();

	return 0;
}

MSGHANDLER cScr_NVBattery::OnConsume(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SInterface<IContainSys> pContainSys(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	cMultiParm mpStack = 1;
	if (pPropSrv->Possessed(m_iObjId, "StackCount"))
	{
		pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
	}
	
	if ( static_cast<int>(mpStack) > 1 )
	{
		pContainSys->StackAdd(m_iObjId, -1, 0); // Don't destroy if stack=0
	} else {
		pShockGame->DestroyInvObj(m_iObjId);
	}
	
	return 0;
}

MSGHANDLER cScr_NVBattery::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->AddTranslatableText("HelpBattery", "Misc", StrToObject("Player"), 5000);
	return 0;
}



/***********************
 * NVRecycler
 */
 
MSGHANDLER cScr_NVRecycler::OnFrobToolEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iToolDest = pFrobMsg->DstObjId;
	
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	object oEquipped;
	for ( int i = 0; i <= 4; i++ )
	{
		pShockGame->Equipped(oEquipped, i);
		if ( oEquipped == iToolDest )
		{
			return 0;
		}
	}
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	int iRecycle = 0;
	cMultiParm mpProp;
	
	if (pPropSrv->Possessed(iToolDest, "Recycle"))
	{
		pPropSrv->Get(mpProp, iToolDest, "Recycle", NULL);
		iRecycle = static_cast<int>(mpProp);
	}
	
	if ( iRecycle <= 0 )
		return 0;
		
	int iStack = 1;
	cMultiParm mpStack;
	if (pPropSrv->Possessed(iToolDest, "StackCount"))
	{
		pPropSrv->Get(mpStack, iToolDest, "StackCount", NULL);
		iStack = static_cast<int>(mpStack);
	}
		
	int iValue = 0 - (iRecycle * iStack);
	pShockGame->PayNanites(iValue);
	
	SService<IObjectSrv> pOS(g_pScriptManager);
	pOS->Destroy(iToolDest);	
	
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundAmbient, kSoundNetwork0);
	
	SInterface<IContainSys> pContainSys(g_pScriptManager);
	if ( pContainSys->IsHeld(StrToObject("Player"), iToolDest) == MAX_INT)
	{
		pShockGame->PreventSwap();
	}

	return 0;
}


MSGHANDLER cScr_NVRecycler::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->AddTranslatableText("HelpRecycler", "Misc", StrToObject("Player"), 5000);
	return 0;
}

/***********************
 * NVAvatarWeapons
 */

MSGHANDLER cScr_NVAvatarWeapons::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{ 
/*
	if ( m_iObjId != StrToObject("Player") )
		return 0;
	
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

//	DisplayPrintf("NVAvatarWeapons: Starting up!");
	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "AvatarWeapons", 1000, kSTM_Periodic, g_mpUndef);
	*/	
	return 0;
}

MSGHANDLER cScr_NVAvatarWeapons::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
/*
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "AvatarWeapons"))
		return 0; // Abort if it's not the right timer.
		
	int iMe = StrToObject("Player");
		
	if ( m_iObjId != iMe )
		return 0;
		
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<INetworkingSrv> pNetwork(g_pScriptManager);
//	object oEquipped;
		
//	pShockGame->Equipped(oEquipped, 0);
	
//	if (oEquipped)
//	{
	linkset lsLinks;
	link lLink;
	sLink slLink;
	true_bool bRet, bBool;
	object oPlayer;
	
	pNetwork->FirstPlayer(oPlayer);
	
	while (oPlayer)
	{
		if (oPlayer != iMe )
		{
			pLS->GetAll(lsLinks, pLTS->LinkKindNamed("Contains"), oPlayer, 0);
			for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
			{
				sLink sl = lsLinks.Get();
				void* pLinkData = lsLinks.Data();
				
				int iContainsID = 0;
				if (pLinkData)
				{
					iContainsID =  *reinterpret_cast<int*>(pLinkData);
				}
				
				if ( iContainsID == 1000 )
				{
//					DisplayPrintf("NVAvatarWeapons: Found a wepaon!");
					object oEquipped = sl.dest ;
					
					SService<IPropertySrv> pPropSrv(g_pScriptManager);
					if (pPropSrv->Possessed(oPlayer, "RenderAlpha"))
						pPropSrv->CopyFrom(oEquipped, "RenderAlpha", oPlayer); // Copy render alpha from player to avatar weapon
			
					if (ParamExists(oEquipped, "NVAvatarWeapon"))
					{
						object oArch = ParamGetObject(oEquipped, "NVAvatarWeapon", 0);
							
						pLS->GetOne(lLink, pLTS->LinkKindNamed("CreatureAttachment"), oPlayer, 0);
						if ( lLink)
						{
						
							pLTS->LinkGet(lLink, slLink);
							pOS->InheritsFrom(bBool, slLink.dest, oArch);
							if ( bBool )
								return 0;

							pOS->Destroy(slLink.dest);
						}
						
//						DisplayPrintf("NVAvatarWeapons: Creating obj!");
						object oAvWeapon;
						pOS->Create(oAvWeapon, oArch);
						
						link lNewLink;
						pLS->Create(lNewLink, pLTS->LinkKindNamed("CreatureAttachment"), oPlayer, oAvWeapon);
						if ( lNewLink )
						{
							pLTS->LinkSetData(lNewLink, "Joint", 10); // Right Wrist
						}
					
						return 0;
					}
				}
			}
			// Nothing equipped.
			pLS->GetOne(lLink, pLTS->LinkKindNamed("CreatureAttachment"), oPlayer, 0);
			if ( lLink)
			{
				pOS->Destroy(slLink.dest);
			}
		
//			DisplayPrintf("NVAvatarWeapons: Found no weapons, oh no!!");
		}
		pNetwork->NextPlayer(oPlayer);
	}
	*/
	return 0;
}


/***********************
 * NVGuaranteedLoot
 */

MSGHANDLER cScr_NVGuaranteedLoot::OnSlain(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SInterface<IContainSys> pContainSys(g_pScriptManager);
	
	char* szParamName = new char[28];
	for (int i = 0; i<99 ; i++)
	{
		sprintf(szParamName, "NVGuaranteedLoot%i", i);
		object oArc = ParamGetObject(m_iObjId, szParamName, 0);
		
		if ( oArc )
		{
			object oObj;
			pOS->Create(oObj, oArc);
			pContainSys->Add(m_iObjId, oObj, 0, 1);
		}
	}

	return 0;
}

/***********************
 * NVCrateHack
 */

MSGHANDLER cScr_NVCrateHack::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->SetQBHacked("HideInterface", 0);
	pShockGame->Mouse(1,1);
    pShockGame->OverlayChange(22, 1);
    pShockGame->OverlayChange(28, 1);
	
	pShockGame->Container(m_iObjId);
	return 0;
}

/***********************
 * NVRefreshInvTrap
 */

MSGHANDLER cScr_NVRefreshInvTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->RefreshInv();
	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVRefreshInvTrap", 100, kSTM_OneShot, g_mpUndef);
	return 0;
}


MSGHANDLER cScr_NVRefreshInvTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVRefreshInvTrap"))
		return 0; // Abort if it's not the right timer.

	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->RefreshInv();
	
	return 0;
}

MSGHANDLER cScr_NVRefreshInvTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVObjState
 */

MSGHANDLER cScr_NVObjState::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	
	//Check state
	cMultiParm mpState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpState, m_iObjId, "ObjState", NULL);
	}
	
	ParamSetInt(m_iObjId, "NVObjStateCurrentState", static_cast<int>(mpState));
	
	char pszMsg[18];
			
	sprintf(pszMsg, "ObjStartState%i", static_cast<int>(mpState));
	SimpleSend(0, m_iObjId, pszMsg);
	
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		int iTimer = ParamGetInt(m_iObjId, "NVObjStateTimer", 1000);
		if ( iTimer != 0 ) {
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVObjStateTimer", iTimer, kSTM_Periodic, g_mpUndef);
		}
	}
	
	return 0;
}

MSGHANDLER cScr_NVObjState::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVObjStateTimer"))
		return 0; // Abort if it's not the right timer.
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);		
	
	cMultiParm mpState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpState, m_iObjId, "ObjState", NULL);
	}
	
	CheckState(static_cast<int>(mpState));
	
	return 0;
}

cScr_NVObjState::~cScr_NVObjState()
{
	StopListener();
	if ( m_pProp )
	{
		m_pProp->Release();
	}
}

void __stdcall cScr_NVObjState::ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData)
{
	cScr_NVObjState* pSelf = reinterpret_cast<cScr_NVObjState*>(pData);
	if ( pSelf->m_bSim == false )
		return;
		
	int iObjId = pSelf->m_iObjId;
	if (sMsg->iObjId == iObjId)
	{ // Do nothing UNLESS this is the object with the script...
		int iState = reinterpret_cast<int>(sMsg->pData);
		
		pSelf->CheckState(iState);
	}

	
	return;
}

void cScr_NVObjState::CheckState(int iState)
{
	int iOldState = ParamGetInt(m_iObjId, "NVObjStateCurrentState", 0);
	// DisplayPrintf("(DEBUG: Changed from %i to %i)", iOldState, iState);
	if ( iState != iOldState )
	{
		ParamSetInt(m_iObjId, "NVObjStateCurrentState", iState);
		SimpleSend(0, m_iObjId, "ObjStateChange");
		
		char *pszMsg = new char[18];
		
		sprintf(pszMsg, "ObjState%iTo%i", iOldState, iState);
		SimpleSend(0, m_iObjId, pszMsg);
		
		delete[] pszMsg;
		
		if ( iState == 0 )
		{
			switch ( iOldState )
			{
			case 1:
				SimpleSend(0, m_iObjId, "ObjState-Repaired");
				break;
			case 2:
				SimpleSend(0, m_iObjId, "ObjState-Researched");
				break;
			case 3:
				SimpleSend(0, m_iObjId, "ObjState-Undestroyed");
				break;
			case 4:
				SimpleSend(0, m_iObjId, "ObjState-Unlocked");
				break;
			case 5:
				SimpleSend(0, m_iObjId, "ObjState-Unhacked");
				break;
			}
		}
	}
}
		
void cScr_NVObjState::StartListener(void)
{
	// Store current state
	//Check state
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpState, m_iObjId, "ObjState", NULL);
	}
	
	ParamSetInt(m_iObjId, "NVObjStateCurrentState", static_cast<int>(mpState));

//	DisplayPrintf("DEBUG: Starting listener; listening for settings changes.");
	SInterface<IPropertyManager> pPM(g_pScriptManager);
	m_pProp = static_cast<IGenericProperty*>(pPM->GetPropertyNamed("ObjState"));

	m_hListenHandle = m_pProp->Listen(kPropertyChange | kPropertyAdd, ListenFunc, reinterpret_cast<PropListenerData__*>(this));
	
	return;
}

void cScr_NVObjState::StopListener(void)
{
	if ( m_hListenHandle && m_pProp )
	{
//		DisplayPrintf("DEBUG: Script exited. Shutting down listener.");
		m_pProp->Unlisten(m_hListenHandle);
	}
	m_hListenHandle = NULL;
	m_pProp = NULL;
	
	return;
}

/***********************
 * NVSimpleRepair
 */

MSGHANDLER cScr_NVSimpleRepair::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
//	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	
	//Check state
	int iState = 0;
	cMultiParm mpState;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpState, m_iObjId, "ObjState", NULL);
		iState = static_cast<int>(mpState);
	}
	
	if ( iState == 1 ) // broken
	{
		pShockGame->OverlayChangeObj(6, 1, m_iObjId);
//		DisplayPrintf("%i!", pFrobMsg->Abort);
//		pFrobMsg->Abort = 3; // Block frobs ??
//		DisplayPrintf("%i!", pFrobMsg->Abort);
		MultiParmAssignInt(pReply, -1);
	}
	
	return 0;
}


/***********************
 * NVJumpyGrubEgg
 */

MSGHANDLER cScr_NVJumpyGrubEgg::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<INetworkingSrv> pNetwork(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	
	object oPlayer;
	cScrVec vPlayerPos, vClosest, vMyPos;
	float fDistance = 0.00f, fClosest = 0.00f;
	int iCount = 0;
	pOS->Position(vMyPos, m_iObjId);
	
	pNetwork->FirstPlayer(oPlayer);
	
	while (oPlayer)
	{
		pOS->Position(vPlayerPos, oPlayer);

		fDistance = GetDistanceXY(&vMyPos, &vPlayerPos);

		if ( ( iCount++ == 0 ) || ( fDistance < fClosest) )
		{
			fClosest = fDistance;
			vClosest = vPlayerPos;
		}
		pNetwork->NextPlayer(oPlayer);
	}
	
	object oArc = ParamGetNVObject(m_iObjId, "NVJumpyGrubEggEmitter", StrToObject("Grub"));
	
	if ( oArc )
	{
		object oObject;
		pOS->Create(oObject, oArc);
		
		cScrVec vFacing, vPosition, vNewRot;
		pOS->Facing(vFacing, m_iObjId);
		
		vPosition = vClosest - vMyPos;
		float fLongitude = (float)((450.00f) - atan2(vPosition.x, vPosition.y) * 180.00f/M_PI);
		while ( fLongitude > 360.00f )
			fLongitude -= 360.00f;
			
		if ( ParamGetInt(m_iObjId, "NVJumpyGrubEggVerticalAim", 0) )
		{
			float fLatitude = (float)((630.00f) + atan2(sqrt((vPosition.x * vPosition.x)+(vPosition.y * vPosition.y)), vPosition.z) * 180.00f/M_PI);
			while ( fLatitude > 360.00f )
				fLatitude -= 360.00f;
				
			vNewRot.y = fLatitude;	
		}
		vNewRot.z = fLongitude;
		
		pOS->Teleport(oObject, vMyPos, vNewRot, 0);
		
		SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
		true_bool bReturn;
		
		if ( ParamGetInt(m_iObjId, "NVJumpyGrubEggSound", 1) )
		{
			pSoundSrv->PlaySchemaAtObject(bReturn, m_iObjId, ParamGetNVObject(m_iObjId, "NVJumpyGrubEggSchema", StrToObject("pod_exp")), m_iObjId, kSoundNetwork0);
		}
		
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		
		cMultiParm mpAnimS;
		if (pPropSrv->Possessed(m_iObjId, "StTweqRotate") )
		{
			pPropSrv->Get(mpAnimS, m_iObjId, "StTweqRotate", "AnimS");
			pPropSrv->Set(m_iObjId, "StTweqRotate", "AnimS", static_cast<int>(mpAnimS) | 1);
		}
		if (pPropSrv->Possessed(m_iObjId, "StTweqJoints") )
		{
			pPropSrv->Get(mpAnimS, m_iObjId, "StTweqJoints", "AnimS");
			pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", static_cast<int>(mpAnimS) | 1);
		}
		if (pPropSrv->Possessed(m_iObjId, "StTweqEmit") )
		{
			pPropSrv->Get(mpAnimS, m_iObjId, "StTweqEmit", "AnimS");
			pPropSrv->Set(m_iObjId, "StTweqEmit", "AnimS", static_cast<int>(mpAnimS) | 1);
		}
		if (pPropSrv->Possessed(m_iObjId, "StTweqModels") )
		{
			pPropSrv->Get(mpAnimS, m_iObjId, "StTweqModels", "AnimS");
			pPropSrv->Set(m_iObjId, "StTweqModels", "AnimS", static_cast<int>(mpAnimS) | 1);
		}
	}
	
	return 0;
}

MSGHANDLER cScr_NVJumpyGrubEgg::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVBriefingTrap
 */

MSGHANDLER cScr_NVBriefingTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->DebriefMode(ParamGetInt(m_iObjId, "NVBriefingTrapBriefing", 1));
	
	return 0;
}

MSGHANDLER cScr_NVBriefingTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVIrradiator
 */

MSGHANDLER cScr_NVIrradiator::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);	
	cMultiParm mpReturn;
	
	
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "RadCheck"))
	{

		
		float fRad = 0.00f;
		if (pPropSrv->Possessed(m_iObjId, "RadAmb"))
		{
			pPropSrv->Get(mpReturn, m_iObjId, "RadAmb", NULL);
			fRad = static_cast<float>(mpReturn);
		}
		
		sLink sl;
		
		if ( fRad > 0 )
		{
			if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, "IrradiatedGlow", -1))
				return 0;
				
//			DisplayPrintf("Irradiated!");
			int iObjId = ParamGetObject(m_iObjId, "NVIrradiatedGlow", 0);
			if ( iObjId == 0 )
				return 0;
			
			object oCreated;

			SService<IObjectSrv> pOS(g_pScriptManager);
			pOS->Create(oCreated, iObjId);

			if (oCreated)
			{
				SService<ILinkSrv> pLS(g_pScriptManager);
				SService<ILinkToolsSrv> pLTS(g_pScriptManager);
				
				link lAMOLink;
				pLS->Create(lAMOLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oCreated);
				if ( lAMOLink )
				{
					pLTS->LinkSetData(lAMOLink, NULL, "IrradiatedGlow");
				}
			}
		} else {
			if (!GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, "IrradiatedGlow", -1))
			{
				return 0;
			}
			SInterface<IObjectSystem> pOS(g_pScriptManager);
			SService<IDamageSrv> pDmgS(g_pScriptManager);
			pDmgS->Slay(sl.dest, m_iObjId);
		}
	} else if (!stricmp(pszMsgName, "RadDamage"))
	{
		float fRadiation = 0.00f;
		if (pPropSrv->Possessed(m_iObjId, "RadLevel"))
		{
			pPropSrv->Get(mpReturn, m_iObjId, "RadLevel", NULL);
			fRadiation = static_cast<float>(mpReturn);
		}
		
		if ( fRadiation > 0.00f )
		{
			if (ParamExists(m_iObjId, "NVRadFadeTime"))
		{
				SService<IShockGameSrv> pShockGame(g_pScriptManager);
					pShockGame->StartFadeIn(ParamGetInt(m_iObjId, "NVRadFadeTime", 500), ParamGetInt(m_iObjId, "NVRadFadeRed", 0), ParamGetInt(	m_iObjId, "NVRadFadeGreen", 255), ParamGetInt(m_iObjId, "NVRadFadeBlue", 0));
			}
		}
	}
	
	return 0;
}

/***********************
 * NVCopyModify
 */

MSGHANDLER cScr_NVCopyModify::OnModify1(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoModification(1);
	return 0;
}

MSGHANDLER cScr_NVCopyModify::OnModify2(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoModification(2);
	return 0;
}
void cScr_NVCopyModify::DoModification(int iMod)
{
	char* pszParam = new char[15];
	sprintf(pszParam, "NVCopyModify%i", iMod);
	
	object oToCopy = ParamGetNVObject(m_iObjId, pszParam, 0);
	
	if ( oToCopy )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);	
		
		if (pPropSrv->Possessed(oToCopy, "BaseGunDesc"))
			pPropSrv->CopyFrom(m_iObjId, "BaseGunDesc", oToCopy);
			
		if (pPropSrv->Possessed(oToCopy, "PlayerGunDesc"))
			pPropSrv->CopyFrom(m_iObjId, "PlayerGunDesc", oToCopy);
			
		if (pPropSrv->Possessed(oToCopy, "GunReliability"))
			pPropSrv->CopyFrom(m_iObjId, "GunReliability", oToCopy);
			
		if (pPropSrv->Possessed(oToCopy, "SHead1"))
			pPropSrv->CopyFrom(m_iObjId, "SHead1", oToCopy);
		
		if (pPropSrv->Possessed(oToCopy, "SHead2"))
			pPropSrv->CopyFrom(m_iObjId, "SHead2", oToCopy);

		if (pPropSrv->Possessed(oToCopy, "Sett1"))
			pPropSrv->CopyFrom(m_iObjId, "Sett1", oToCopy);

		if (pPropSrv->Possessed(oToCopy, "Sett2"))
			pPropSrv->CopyFrom(m_iObjId, "Sett2", oToCopy);
			
		if (pPropSrv->Possessed(oToCopy, "GunKick"))
			pPropSrv->CopyFrom(m_iObjId, "GunKick", oToCopy);
		
		if (pPropSrv->Possessed(oToCopy, "AnimPost"))
			pPropSrv->CopyFrom(m_iObjId, "AnimPost", oToCopy);

		if (pPropSrv->Possessed(oToCopy, "AnimPre"))
			pPropSrv->CopyFrom(m_iObjId, "AnimPre", oToCopy);
			
		if (pPropSrv->Possessed(oToCopy, "RepairDiff"))
			pPropSrv->CopyFrom(m_iObjId, "RepairDiff", oToCopy);

		if (pPropSrv->Possessed(oToCopy, "ReqTechDesc"))
			pPropSrv->CopyFrom(m_iObjId, "ReqTechDesc", oToCopy);
		
		if (pPropSrv->Possessed(oToCopy, "ReqStatsDesc"))
			pPropSrv->CopyFrom(m_iObjId, "ReqStatsDesc", oToCopy);
		
		if (pPropSrv->Possessed(oToCopy, "BaseWeaponDesc"))
			pPropSrv->CopyFrom(m_iObjId, "BaseWeaponDesc", oToCopy);

		if (pPropSrv->Possessed(oToCopy, "Class Tags"))
			pPropSrv->CopyFrom(m_iObjId, "Class Tags", oToCopy);

		if (pPropSrv->Possessed(oToCopy, "ModelName"))
			pPropSrv->CopyFrom(m_iObjId, "ModelName", oToCopy);
	} else {
		DebugPrintf("No object to copy from for modification %i!", iMod);
	}
}


/***********************
 * NVResurrectionToggle
 */

MSGHANDLER cScr_NVResurrectionToggle::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	true_bool bReturn;
	
	pQSrv->Set("AllowRespawn", 1, 0);
	pShockGame->AddTranslatableText("ResurrectionOn", "Misc", StrToObject("Player"), 5000);
	pSoundSrv->HaltSchema(bReturn, m_iObjId, "", 0);
	pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, ParamGetNVObject(m_iObjId, "NVResurrectionToggleOnSchema", StrToObject("ResurrectionOn")), kSoundNetwork0);
	
	return 0;
}

MSGHANDLER cScr_NVResurrectionToggle::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	true_bool bReturn;
	
	pQSrv->Set("AllowRespawn", 0, 0);
	pShockGame->AddTranslatableText("ResurrectionOff", "Misc", StrToObject("Player"), 5000);
	pSoundSrv->HaltSchema(bReturn, m_iObjId, "", 0);
	pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, ParamGetNVObject(m_iObjId, "NVResurrectionToggleOffSchema", StrToObject("ResurrectionOff")), kSoundNetwork0);
	
	return 0;
}


/***********************
 * NVInstantCloseDoor
 */

MSGHANDLER cScr_NVInstantCloseDoor::OnTurnOff(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	pDoorSrv->CloseDoor(m_iObjId);
	
	return 0;
}


/***********************
 * NVOverlayWatcher
 */

MSGHANDLER cScr_NVOverlayWatcher::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hBaseTimerHandle == NULL)
	{ // Start a timer.
		m_hBaseTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVOverlayWatcherTimer", 100, kSTM_Periodic, g_mpUndef);
	}
	
	return 0;
}

MSGHANDLER cScr_NVOverlayWatcher::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

MSGHANDLER cScr_NVOverlayWatcher::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVOverlayWatcherTimer"))
		return 0; // Abort if it's not the right timer.
		
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	if ( !pShockGameSrv->OverlayOn(ParamGetInt(m_iObjId, "NVOverlayWatcherOverlay", 0) ) )
	{
		SimpleSend(0, m_iObjId, "OverlayOff");
		if (m_hBaseTimerHandle != NULL)
		{	// Stop timer:
			g_pScriptManager->KillTimedMessage(m_hBaseTimerHandle);
			m_hBaseTimerHandle = NULL;
		}
	}


	return 0;
}


/***********************
 * NVBaseOverlay
 */

MSGHANDLER cScr_NVBaseOverlay::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sLink sl;
	if (!GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, "Overlay", -1))
	{
	
		char szParam[30];
		object oArch;
		float fX;
		float fY;
		float fZ;
		object oOverlay;
		
		for ( int i = 0; i < 99; i++)
		{
			sprintf(szParam, "NVBaseOverlay%iArch", i);
			oArch = ParamGetNVObject(m_iObjId, szParam, -1);
			
			sprintf(szParam, "NVBaseOverlay%iX", i);
			fX = ParamGetFloat(m_iObjId, szParam, 1.00f);
			
			sprintf(szParam, "NVBaseOverlay%iY", i);
			fY = ParamGetFloat(m_iObjId, szParam, 0.00f);
			
			sprintf(szParam, "NVBaseOverlay%iZ", i);
			fZ = ParamGetFloat(m_iObjId, szParam, 0.00f);
			
			oOverlay = AddOverlay(fX, fY, fZ, oArch, NULL);
		}
	
	
		StartOverlay();
	}
	
	return 0;
}

MSGHANDLER cScr_NVBaseOverlay::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	StopOverlay();
	
	return 0;
}

MSGHANDLER cScr_NVBaseOverlay::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
//	DisplayPrintf("Attempting timer.");

	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVBaseOverlayDistanceTimer"))
		ProcessBaseTimer();
		
	return 0;
}
	
void cScr_NVBaseOverlay::ProcessBaseTimer()
{
//	DisplayPrintf("Timerin'.");		

	// Now process the timer
		
	SService<IObjectSrv> pOS(g_pScriptManager);
	
	object oPlayer;
	pOS->Named(oPlayer, "Player");
	
	cScrVec vPlayerPos, vMyPos;
	float fDistance = 0.00f;
	pOS->Position(vMyPos, m_iObjId);
	pOS->Position(vPlayerPos, oPlayer);

	fDistance = GetDistance(&vMyPos, &vPlayerPos);
	
	if ( OverlayIsDistanceControlled() )
	{
		if ( fDistance > ParamGetFloat(m_iObjId, "NVOverlayDistance", 8.00f) )
		{
			StopOverlay();
		}
	}
	
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	if ( !pShockGameSrv->OverlayOn(0) )
	{
		StopOverlay();
	} else {
		// Handle mouse button rebinding.
		SInterface<IInputBinder> pInput(g_pScriptManager);
		
		object oSelection;
		pShockGameSrv->GetSelectedObj(oSelection);
		
		char szBindMouse[32];
		strcpy(szBindMouse, "bind mouse1");
		const char* pszBinding = pInput->ProcessCmd(szBindMouse);

		if (( oSelection ) && ( ParamGetInt(oSelection, "NVOverlayElement", 0) ) )
		{
			if (pszBinding == NULL || stricmp(pszBinding, "frob_object_inv") )
				pInput->Bind("mouse1", "frob_object_inv");
		} else {
			if (pszBinding == NULL || stricmp(pszBinding, "+drag_and_drop_mode") )
				pInput->Bind("mouse1", "+drag_and_drop_mode");
		}
	}
		

	return;
}

void cScr_NVBaseOverlay::StartTimer(void)
{
	if (m_hBaseTimerHandle == NULL)
	{ // Start a timer.
	
		m_hBaseTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVBaseOverlayDistanceTimer", 100, kSTM_Periodic, g_mpUndef);
	}
}

void cScr_NVBaseOverlay::KillTimer(void)
{
	if (m_hBaseTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hBaseTimerHandle);
		m_hBaseTimerHandle = NULL;
	}
}

void cScr_NVBaseOverlay::StartOverlay()
{
	KillTimer();
	StartTimer();
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	pShockGameSrv->OverlayChange(0, 1);
	

	
//	SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
//	pDebugSrv->Command("bind mouse1 frob_object_inv", cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);
}

int cScr_NVBaseOverlay::AddOverlay(float x, float y, float z, object oArc, const char *pszShape)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);	
	
	object oObj = 0;
	
	pOS->Create(oObj, oArc);
	
	if (oObj)
	{
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		
		link lLink;
		pLS->Create(lLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oObj);
		if ( lLink )
		{
			pLTS->LinkSetData(lLink, NULL, "Overlay");
		}
		
		cScrVec vOffset(x, y, z);
		
		pPropSrv->Set(oObj, "CameraObj", "Offset", vOffset);
		pPropSrv->Set(oObj, "CameraObj", "Draw?", true);
		
		if ( pszShape != NULL )
		{
			pPropSrv->Set(oObj, "ModelName", NULL, pszShape);
		}
		
		pPropSrv->Add(oObj, "Transient"); // These objects should not be stored in a saved game.
		pPropSrv->Set(oObj, "Transient", NULL, 1);
		pPropSrv->Set(oObj, "ExtraLight", "Amount (-1..1)", 0.75f);
		
		pPropSrv->Set(oObj, "ObjList", NULL, "NVOverlayElement=1");
		
	}
	
	return oObj;
}

void cScr_NVBaseOverlay::StopOverlay()
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	
	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, 0);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		sLink sl = lsLinks.Get();
		void* pLinkData = lsLinks.Data();
		if (pLinkData)
		{
			if ( !stricmp(reinterpret_cast<const char*>(pLinkData), "Overlay" ) )
			{
				pOS->Destroy(sl.dest);
			}
		}
	}
	
//	SInterface<IInputBinder> pInput(g_pScriptManager);
//	pInput->Bind("mouse1", "+drag_and_drop_mode");
	
	KillTimer();
}

/***********************
 * NVWiringPuzzle
 */

 void cScr_NVWiringPuzzle::StartOverlay()
{
	cScr_NVBaseOverlay::StartOverlay();
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	
//	const float X = 1.10f, Y = 0.80f, Z = -0.40f;
	const float X = 1.0f, Y = 0.65f, Z = -0.20f;
	
	object oObj;
	char sz[50];
	
	for ( int i = 0; i < 6; i++ )
	{
		oObj = AddOverlay(X, Y, Z, -1, "WirePuzS2");
		pPropSrv->Set(oObj, "RenderType", NULL, 1); //Not rendered
		pPropSrv->Set(oObj, "CameraObj", "Draw?", false);
		// pPropSrv->Set(oObj, "ExtraLight", "Amount (-1..1)", 0.25f);
		// pPropSrv->Set(oObj, "ExtraLight", "Additive?", true);
		
		sprintf(sz, "obj/txt16/Wire%02i", i+1);
		
		pPropSrv->Set(oObj, "OTxtRepr0", NULL, sz);	
		pPropSrv->Set(oObj, "PickBias", NULL, -30.0f);
		
		link lLink;
		pLS->Create(lLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oObj);
		if ( lLink )
		{
			sprintf(sz, "Overlay-Wire%02i", i);
			pLTS->LinkSetData(lLink, NULL, sz);
		}
	}
	
	
	for ( int i = 0; i < 12; i++ )
	{
		if (i < 6 )
			oObj = AddOverlay(X, Y + 0.24f, (Z + 0.225f - ( 0.09f * i)), -1, "WirePuzS");
		else
			oObj = AddOverlay(X, Y - 0.24f, (Z + 0.225f - ( 0.09f * (i-6))), -1, "WirePuzS2");
			
		
		pPropSrv->Set(oObj, "FrobInfo", "World Action", 2);
		pPropSrv->Set(oObj, "HUDSelect", NULL, true);
		pPropSrv->Set(oObj, "Scripts", "Script 0", "NVWiringPuzzleHelper");
		pPropSrv->Set(oObj, "PickBias", NULL, 16.0f);
	
		ParamSetInt(oObj, "NVWiringPuzzleHelpderID", i);
		
	}
	// AddOverlay(1.00f, 0.65f + 0.24f, -0.30f + 0.225f, -1, "WirePuzS");
	// AddOverlay(1.00f, 0.65f + 0.24f, -0.30f + 0.135f, -1, "WirePuzS");
	// AddOverlay(1.00f, 0.65f + 0.24f, -0.30f + 0.045f, -1, "WirePuzS");
	// AddOverlay(1.00f, 0.65f + 0.24f, -0.30f - 0.045f, -1, "WirePuzS");
	// AddOverlay(1.00f, 0.65f + 0.24f, -0.30f - 0.135f, -1, "WirePuzS");
	// AddOverlay(1.00f, 0.65f + 0.24f, -0.30f - 0.225f, -1, "WirePuzS");
	// AddOverlay(1.00f, 0.65f - 0.24f, -0.30f - 0.225f, -1, "WirePuzS");
	oObj = AddOverlay(X, Y, Z, -1, "WirePuzB");
	pPropSrv->Set(oObj, "PickBias", NULL, -30.0f);
	
	oObj = AddOverlay(X, Y+0.24f, Z-0.32f, -1, "WirePuzE");
	pPropSrv->Set(oObj, "PickBias", NULL, -30.0f);
	
	link lLink2;
	pLS->Create(lLink2, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oObj);
	if ( lLink2 )
	{
		pLTS->LinkSetData(lLink2, NULL, "Overlay-E");
	}
	
	oObj = AddOverlay(X, Y+0.24f, Z-0.32f, -1, "WirePuzEB");
	pPropSrv->Set(oObj, "PickBias", NULL, -30.0f);
	
	DrawWires();
}

MSGHANDLER cScr_NVWiringPuzzle::OnScrewbox(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iSelection = static_cast<int>(pMsg->data);
	const char* pszState = ParamGetString(m_iObjId, "NVWiringPuzzleState", "100000100000");
	if ( strlen(pszState) != 12)
	{
		DisplayPrintf("Error in wire state.");
		return 1;
	}
	const char* pszSolution = ParamGetString(m_iObjId, "NVWiringPuzzleSolution", "000001000001");
	if ( strlen(pszSolution) != 12)
	{
		DisplayPrintf("Error in wire solution.");
		return 1;
	}
	
	char sState[13], sSolution[13];
	strncpy(sState, pszState, 13);
	strncpy(sSolution, pszSolution, 13);
	
	if (( m_iSelection == -1 ) || ( ( m_iSelection / 6) != (iSelection /6 ) ) )
	{
		if ( sState[iSelection] != '0' )
			m_iSelection = iSelection;
		return 0;
	}
	
//	DisplayPrintf("Swapping slot %i with %i!", iSelection, m_iSelection);
	
//	DisplayPrintf("Code was %s", sState);
	char cSwap = sState[m_iSelection];
	sState[m_iSelection] = sState[iSelection];
	sState[iSelection] = cSwap;
	m_iSelection = -1;
	
	if (!stricmp(sState, sSolution) )
	{
//		DisplayPrintf("Solved.");
		if ( ParamGetInt(m_iObjId, "NVWiringPuzzleSolved", 0)  == 0)
		{
			ParamSetInt(m_iObjId, "NVWiringPuzzleSolved", 1);
			if ( ParamGetInt(m_iObjId, "NVWiringPuzzleTriggerOnSolve", 1) )
			{
				Trigger(true, pMsg);
			}
		}
	} else {
		if ( ParamGetInt(m_iObjId, "NVWiringPuzzleSolved", 0)  == 1)
		{
			ParamSetInt(m_iObjId, "NVWiringPuzzleSolved", 0);
			Trigger(false, pMsg);
		}
	}
	
	ParamSetString(m_iObjId, "NVWiringPuzzleState", sState);
//	DisplayPrintf("Code is %s", sState);
	DrawWires();
	
	return 0;
}

void cScr_NVWiringPuzzle::DrawWires(void)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);	
	
	char cWires[6][2] = { {' ', ' '},    {' ', ' '},    {' ', ' '},    {' ', ' '},    {' ', ' '},    {' ', ' '}  };
	
	const char* pszState = ParamGetString(m_iObjId, "NVWiringPuzzleState", "100000100000");
	if ( strlen(pszState) != 12)
	{
		DisplayPrintf("Error in wire state.");
		return;
	}
	
	for (int i = 0; i < 12; i++)
	{
		if ( pszState[i] != '0' )
		{
			cWires[ static_cast<int>(pszState[i]-49) ][i/6] = i;
		}
	}
	
	for ( int i = 0; i < 6; i++ )
	{
		if (cWires[i][0] != ' ' )
		{
//			DisplayPrintf("Wire %i from %i to %i", i+1, cWires[i][0], cWires[i][1]);
			sLink sl;
			char sz[16];
			
			sprintf(sz, "Overlay-Wire%02i", i);
			
			if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, sz, -1))
			{
				sprintf(sz, "Wire-%i-%i", cWires[i][0]+1, cWires[i][1]-5);
				pPropSrv->Set(sl.dest, "ModelName", NULL, sz);
				pPropSrv->Set(sl.dest, "RenderType", NULL, 0); //Normal
				pPropSrv->Set(sl.dest, "CameraObj", "Draw?", true);
			}
		}
	}
	
	
	/* Now, let's calculate the correctness ... */
	
	const char* pszSolution = ParamGetString(m_iObjId, "NVWiringPuzzleSolution", "000001000001");
	if ( strlen(pszSolution) != 12)
	{
		DisplayPrintf("Error in wire solution.");
		return;
	}

	
	float fCorrect = 0;
	float fGoal = 0;

	for ( int k = 0; k <= 6; k += 6 )
	{
		for ( int i = k; i < k+6; ++i )
		{
			if ( pszState[i] != '0' )
			{
				fGoal += 1.0f;
				for ( int j = k; j < k+6; ++j )
				{
					if ( pszSolution[j] == pszState[i] )
					{
						int distance = abs(j-i);
						if ( distance == 0 )
						{
							fCorrect += 1.0f;
						} else {
							fCorrect += (0.6f - (distance*0.1f));
						}
						break;
					}
				}
			}
		}
	}
	float fTotal = fCorrect/fGoal;
	
	sLink sl;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, "Overlay-E", -1))
	{
		cScrVec vScale(1.0f, fTotal, 1.0f);
		pPropSrv->Set(sl.dest, "Scale", NULL, vScale);
	}
	
}

MSGHANDLER cScr_NVWiringPuzzle::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
//	if (ParamGetInt(m_iObjId, "NVWiringPuzzleSolved", 0) == 1 )
//	{
//		if ( ParamGetInt(m_iObjId, "NVWiringPuzzleTriggerOnSolvedFrob", 1) )
//			DisplayPrintf("Trigger(true, pMsg);");
//	} else {
		sLink sl;
		if (!GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, "Overlay", -1))
		{
			StartOverlay();
		}
//	}
	
	return 0;
}
MSGHANDLER cScr_NVWiringPuzzle::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

//	if (ParamGetInt(m_iObjId, "NVWiringPuzzleSolved", 0) == 1 )
//		DisplayPrintf("Trigger(false, pMsg);");
		
	return 0;
}

MSGHANDLER cScr_NVWiringPuzzle::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
//	if (ParamGetInt(m_iObjId, "NVWiringPuzzleSolved", 0) == 1 )
//		DisplayPrintf("Trigger(true, pMsg);");
		
	return 0;
}

/***********************
 * NVWiringPuzzleHelper
 */
 
 
MSGHANDLER cScr_NVWiringPuzzleHelper::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iScrewbox = ParamGetInt(m_iObjId, "NVWiringPuzzleHelpderID", 0);
//	DisplayPrintf("Hello, you frobbed screwbox %i", iScrewbox);
	
	sLink sl;
		
	if (GetOneLinkByData("~ScriptParams", m_iObjId, 0, &sl, "Overlay", -1))
	{
		SimpleSend(0, sl.dest, "Screwbox", iScrewbox);
	}
			
			
	return 0;
}


/***********************
 * NVModify
 */

 void cScr_NVModify::StartOverlay()
{
	cScr_NVBaseOverlay::StartOverlay();
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	
	const float X = 1.00f, Y = 0.92f, Z = 0.05f;
	
	object oObj;
	char sz[256];
	
	cMultiParm mpReturn;
	int iSkillNeeded = 1;
	
	int iModify = 0;
	object oPlayer = StrToObject("Player");
	if (pPropSrv->Possessed(oPlayer, "BaseTechDesc"))
	{
		pPropSrv->Get(mpReturn, oPlayer, "BaseTechDesc", "Modify");
		iModify = static_cast<int>(mpReturn);
	}
	

	// Set up the icons themselves

	int iCols = ParamGetInt(m_iObjId, "NVModifyCols", 6);

	int iCount = 0;
	for ( int i = 0; i < 99; i++ )
	{
		sprintf(sz, "NVModify%iIcon", i);
		const char* pszModelName = ParamGetString(m_iObjId, sz, NULL);
		if ( pszModelName == NULL )
			continue;
			
		sprintf(sz, "NVModify%iMaxCount", i);
		int iMaxCount = ParamGetInt(m_iObjId, sz, 1);
		
		int iTotalMaxCount = ParamGetInt(m_iObjId, "NVModifyMaxCount", 2);
		
		oObj = AddOverlay(X, Y - (0.18f * (iCount%iCols)), (Z - (0.225f * (iCount/iCols)) ), -1, pszModelName);

		++iCount;
			
		pPropSrv->Set(oObj, "FrobInfo", "World Action", 2);
		pPropSrv->Set(oObj, "HUDSelect", NULL, true);
		
		sprintf(sz, "NVModify%iSkill", i);
		iSkillNeeded = ParamGetInt(m_iObjId, sz, 1);
		
		if ( iMaxCount > 0 )
		{
			if ( iTotalMaxCount <= 0 )
			{
				// Maximum number of modifications have already been applied.
				pPropSrv->Set(oObj, "Scripts", "Script 0", "NVOnscreenText");
				ParamSetString(oObj, "NVText", "This weapon already has the maximum number of modifications.");
			} else if ( iModify >= iSkillNeeded ) {
				pPropSrv->Set(oObj, "Scripts", "Script 0", "NVModifyHelper");
				ParamSetInt(oObj, "NVModifyHelpderID", i);
			} else {
				pPropSrv->Set(oObj, "Scripts", "Script 0", "NVOnscreenText");
				ParamSetString(oObj, "NVText", "Insufficient skill.");
			}
		} else {
			// Modification has already been applied the maximum number of times
			pPropSrv->Set(oObj, "Scripts", "Script 0", "NVOnscreenText");
			ParamSetString(oObj, "NVText", "This modification cannot be applied again.");
			pPropSrv->Set(oObj, "RenderAlpha", NULL, ParamGetFloat(m_iObjId, "NVModifyDisabledAlpha", 0.50f));
		}
		
	
		
		cScrVec vScale(0.025f, 0.025f, 0.025f);
		pPropSrv->Set(oObj, "Scale", NULL, vScale);
		
		sprintf(sz, "NVModify%iDescription", i);
		const char* pszDescription = ParamGetString(m_iObjId, sz, "Undescribed modification");
		sprintf(sz, "N: \"%s\"", pszDescription );

		pPropSrv->Set(oObj, "HUDUse", NULL, sz);
		pPropSrv->Set(oObj, "ObjName", NULL, sz);
		
		
	}
	
	// Set up background
	
	const char* pszBackdropName = ParamGetString(m_iObjId, "NVModifyBackground", NULL);
	float fBackCentreX = ParamGetFloat(m_iObjId, "NVModifyBackgroundCentreX", 3) - 1;
	float fBackCentreY = ParamGetFloat(m_iObjId, "NVModifyBackgroundCentreY", 3) - 1;
	
	object oBack = AddOverlay(X+0.2f, Y - (0.18f * fBackCentreX), (Z - (0.225f * fBackCentreY) ), -1, pszBackdropName);
	
	// Set scale for background
	cScrVec vBackScale;	
	const char* pszVector = ParamGetString(m_iObjId, "NVModifyBackgroundScale", "0.17,0.17,0.17");
	mxs_vector vVector;
	if (3 == sscanf(pszVector, "%f, %f, %f", &vVector.x, &vVector.y, &vVector.z))
		vBackScale = vVector;
		
	pPropSrv->Set(oBack, "Scale", NULL, vBackScale);
	pPropSrv->Set(oBack, "PickBias", NULL, -30.0f);
}

void cScr_NVModify::StopOverlay()
{
	cScr_NVBaseOverlay::StopOverlay();
	m_iSelection = -1;
}

MSGHANDLER cScr_NVModify::OnModifyIcon(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iSelection = static_cast<int>(pMsg->data);
	m_iSelection = iSelection;
	
	if ( iSelection != -1 )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);	
		int iTotalMaxCount = ParamGetInt(m_iObjId, "NVModifyMaxCount", 2);
		if ( iTotalMaxCount > 0 )
		{
			if (ParamGetInt(m_iObjId, "NVModifyModificationsApplied", 0) == 0)
			{
				pPropSrv->Set(m_iObjId, "GunState", "Modification", 0);
			} else {
				pPropSrv->Set(m_iObjId, "GunState", "Modification", 1);
			}
		} else {
			pPropSrv->Set(m_iObjId, "GunState", "Modification", 2);
		}
	
		SService<IShockGameSrv> pShockGame(g_pScriptManager);
		pShockGame->OverlayChangeObj(44, 1, m_iObjId);
	}
	
//	DisplayPrintf("You chose modification %i!", iSelection);

	return 0;
}

MSGHANDLER cScr_NVModify::OnModify(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	
	int iSelection = static_cast<int>(m_iSelection);
	bool bRestartTimer = true;
//	DisplayPrintf("Modification %i should be being made here?????", iSelection);

	if ( iSelection == -1 )
	{
		bRestartTimer = false;
		iSelection = ParamGetInt(m_iObjId, "NVModifyDefault", -1);
		if ( iSelection == - 1 )
		{
			DisplayPrintf("No modification chosen.");
			return 0;
		}
	}
	
	char sz[24];
	char szProp[22];
	char szProp2[22];
	char szSelection[8];
	
	float fNewValue, fOperand;
	const char* pszOperator;
	
	sprintf(sz, "NVModify%iMaxCount", iSelection);
	int iMaxCount = ParamGetInt(m_iObjId, sz, 1);
	
	if ( iMaxCount <= 0 )
	{
		DisplayPrintf("This modification cannot be applied again.");
		return 0;
	}
	
	int iTotalMaxCount = ParamGetInt(m_iObjId, "NVModifyMaxCount", 2);
	if ( iTotalMaxCount <= 0 )
	{
		DisplayPrintf("This weapon already has the maximum number of modifications.");
		return 0;
	}
		
	--iMaxCount;
	ParamSetInt(m_iObjId,  sz, iMaxCount);

	--iTotalMaxCount;
	ParamSetInt(m_iObjId,  "NVModifyMaxCount", iTotalMaxCount);
	ParamSetInt(m_iObjId, "NVModifyModificationsApplied", ParamGetInt(m_iObjId, "NVModifyModificationsApplied", 0) +1);
	
	for ( int d = 0; d < 2; ++d )
	{
	
		for ( int i = 0; i < 20; ++i )
		{
			if ( d == 0 )
				sprintf(szSelection, "%i", iSelection);
			else
				sprintf(szSelection, "D");
		
			sprintf(szProp, "NVModify%sProp%i", szSelection, i);
			sprintf(szProp2, "NVModify%sSubProp%i", szSelection, i);
			
			const char* pszProp = ParamGetString(m_iObjId, szProp, NULL);
			const char* pszSubPropName = ParamGetString(m_iObjId, szProp2, NULL);
			if (pszProp == NULL )
				continue;
			
			char *pszSubProp = new char[strlen(pszSubPropName) + 12];
			
			int jMax = 1;
			
			if ( ( !stricmp(pszProp, "BaseGunDesc") ) || (!stricmp(pszProp, "GunKick") ) )
			{
				if ( !( (pszSubPropName != NULL) && (strlen(pszSubPropName) > 7) && (!strnicmp(pszSubPropName, "Setting ", 8)) ) ) {
					jMax = 2;
				} else {
					strcpy(pszSubProp, pszSubPropName);
				}
			} else {
				strcpy(pszSubProp, pszSubPropName);
			}
			
			
			for ( int j = 0; j < jMax; ++j )
			{
			
				if ( jMax == 2 )
					sprintf(pszSubProp, "Setting %i: %s", j, pszSubPropName);
				
				pPropSrv->Get(mpReturn, m_iObjId, pszProp, pszSubProp);
				
				fNewValue = static_cast<float>(mpReturn);
				
				sprintf(sz, "NVModify%sOperand%i", szSelection, i);
				fOperand = ParamGetFloat(m_iObjId, sz, 1.20f);
				
				sprintf(sz, "NVModify%sOperator%i", szSelection, i);
				pszOperator = ParamGetString(m_iObjId, sz, "*");

				switch (pszOperator[0])
				{
					case '*':
						fNewValue *= fOperand;
						break;
					case '/':
						fNewValue /= fOperand;
						break;
					case '+':
						fNewValue += fOperand;
						break;
					case '-':
						fNewValue -= fOperand;
						break;
					case '=':
						fNewValue = fOperand;
						break;
				}
				
				pPropSrv->Set(m_iObjId, pszProp, pszSubProp, fNewValue);
			}
			
			delete[] pszSubProp;
		}
	}
	
	StopOverlay();
	
	if ( bRestartTimer )
		g_pScriptManager->SetTimedMessage2(m_iObjId, "StartOverlay", 20, kSTM_OneShot, g_mpUndef);
	
	return 0;
}


MSGHANDLER cScr_NVModify::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	
	int iState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "ObjState", NULL);
		iState = static_cast<int>(mpReturn);
	}
	if ( iState != 0 )
		return 0;
		
	sLink sl;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, "Overlay", -1))
	{
		StopOverlay();
	}
	
	m_iSelection = -1;
	StartOverlay();

	// Disable 'Modify' button
	g_pScriptManager->SetTimedMessage2(m_iObjId, "DisableModifyButton", 20, kSTM_OneShot, g_mpUndef);
	
	return 0;
}

MSGHANDLER cScr_NVModify::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

MSGHANDLER cScr_NVModify::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{	
	return 0;
}

MSGHANDLER cScr_NVModify::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "DisableModifyButton"))
	{
		SService<IShockGameSrv> pShockGame(g_pScriptManager);
		pShockGame->OverlayChangeObj(44, 0, m_iObjId);
	}
	
	if (!stricmp(pszMsgName, "StartOverlay"))
	{
		StartOverlay();
	}
	
	// Process timer handling for overlay base
	if (!stricmp(pszMsgName, "NVBaseOverlayDistanceTimer"))
		ProcessBaseTimer();
	
	return 0;
}

/***********************
 * NVModifyHelper
 */
 
 
MSGHANDLER cScr_NVModifyHelper::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iScrewbox = ParamGetInt(m_iObjId, "NVModifyHelpderID", 0);
	
	sLink sl;
		
	if (GetOneLinkByData("~ScriptParams", m_iObjId, 0, &sl, "Overlay", -1))
	{
		SimpleSend(0, sl.dest, "ModifyIcon", iScrewbox);
	}
			
			
	return 0;
}

/***********************
 * NVStackConsumable
 */
 
 
void cScr_NVStackConsumable::ConsumeStack(int iDestroy)
{
	SInterface<IContainSys> pContainSys(g_pScriptManager);	
	pContainSys->StackAdd(m_iObjId, -1, 0);
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);	
	cMultiParm mpStack = 0;
	if (pPropSrv->Possessed(m_iObjId, "StackCount"))
	{
		pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
	}
	
	if ( static_cast<int>(mpStack) == 0 )
	{
		SService<IShockGameSrv> pShockGame(g_pScriptManager);
		if ( iDestroy == 0 )
			pShockGame->HideInvObj(m_iObjId);
		else
			pShockGame->DestroyInvObj(m_iObjId);
	}
	
}

/***********************
 * NVMedPatchScript
 */
 
float cScr_NVMedPatchScript::BaseHealTotal()
{
	return 10.00f;
}

float cScr_NVMedPatchScript::BasePsiTotal()
{
	return 0.00f;
}

float cScr_NVMedPatchScript::BaseHealInc()
{
	return 2.00f;
}

float cScr_NVMedPatchScript::BasePsiInc()
{
	return 2.00f;
}

float cScr_NVMedPatchScript::HealTotal(int iFrobber, bool bPsi)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	char *pszParamName = new char[12 + strlen(m_szName)];		
	
	float fReply = 0.00f;
	
	if ( !bPsi )
	{
		sprintf(pszParamName, "%sHealTotal", m_szName);
		fReply = ParamGetFloat(m_iObjId, pszParamName, BaseHealTotal());
	} else {
		sprintf(pszParamName, "%sPsiTotal", m_szName);
		fReply = ParamGetFloat(m_iObjId, pszParamName, BasePsiTotal());
	}
	
	sprintf(pszParamName, "%sTrait", m_szName);
	if ( pShockGame->HasTrait(iFrobber, ParamGetInt(m_iObjId, pszParamName, 2)))
	{
		sprintf(pszParamName, "%sTraitBonus", m_szName);
		fReply *= ParamGetFloat(m_iObjId, pszParamName, 1.20f);
	}
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	
	int iDifficulty = 0; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("Difficulty"))
	{
		iDifficulty = pQSrv->Get("Difficulty"); // Get QVar value
		
		if ( iDifficulty == 1 )
		{
			sprintf(pszParamName, "%sEasyBonus", m_szName);
			fReply *= ParamGetFloat(m_iObjId, pszParamName, 1.50f);
		}
	}
     
	delete[] pszParamName;
	
	return fReply;
}

float cScr_NVMedPatchScript::HealInc(bool bPsi)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	char *pszParamName = new char[12 + strlen(m_szName)];		

	float fReply = 0.00f;

	if ( !bPsi )
	{
		sprintf(pszParamName, "%sHealInc", m_szName);
		fReply = ParamGetFloat(m_iObjId, pszParamName, BaseHealInc());
	} else { 
		sprintf(pszParamName, "%sPsiInc", m_szName);
		fReply = ParamGetFloat(m_iObjId, pszParamName, BasePsiInc());
	}

	sprintf(pszParamName, "%sTrait", m_szName);
	if ( pShockGame->HasTrait(static_cast<int>(m_iPatient), ParamGetInt(m_iObjId, pszParamName, 2)))
	{
		sprintf(pszParamName, "%sTraitBonus", m_szName);
		fReply *= ParamGetFloat(m_iObjId, pszParamName, 1.20f);
	}

	delete[] pszParamName;

	return fReply;
}



void cScr_NVMedPatchScript::DoHeal(float fHealAmount, float fPsiAmount)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	
	if ( ( fHealAmount > 0.00f ) && ( fHealAmount < 1.00f ) )
		fHealAmount = 1.00f;
		
	if ( ( fPsiAmount > 0.00f ) && ( fPsiAmount < 1.00f ) )
		fPsiAmount = 1.00f;
		
	int iHealAmount = static_cast<int>(fHealAmount);
	int iPsiAmount = static_cast<int>(fPsiAmount);

	if ( fHealAmount > 0.00f )
	{
		if ( m_fHealAmt > 0.00f )
		{
			pShockGame->HealObj(static_cast<int>(m_iPatient), iHealAmount);
			m_fHealAmt -= (float)iHealAmount;
		}
	} 

	if ( fPsiAmount > 0.00f )
	{
		if ( m_fPsiAmt > 0.00f )
		{
			int iPsi = pShockGame->GetPlayerPsiPoints();
			iPsi += iPsiAmount;
			pShockGame->SetPlayerPsiPoints(iPsi); // How to specify that we are talking about the patient?
			m_fPsiAmt -= (float)iPsiAmount;
		}
	}

	if ( ( m_fHealAmt <= 0.00f ) && ( m_fPsiAmt <= 0.00f ) )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		cMultiParm mpStack = 0;
		if (pPropSrv->Possessed(m_iObjId, "StackCount"))
		{
			pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
		}
		
		if ( static_cast<int>(mpStack) == 0 )
		{
			pShockGame->DestroyInvObj(m_iObjId);
		}
	} else {
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "DoHeal", 1500, kSTM_OneShot, g_mpUndef);
	}
	
}

MSGHANDLER cScr_NVMedPatchScript::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "DoHeal"))
		return 0; // Abort if it's not the right timer.

	m_hTimerHandle = NULL;
	DoHeal(HealInc(false), HealInc(true));
	
	return 0;
}

MSGHANDLER cScr_NVMedPatchScript::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Level transition?
	// Heal all remaining health points.
	if (m_fHealAmt > 0.00f )
		DoHeal(m_fHealAmt, m_fPsiAmt);
		
	return 0;
}

MSGHANDLER cScr_NVMedPatchScript::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	object oFrobber = pFrobMsg->Frobber;
	
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	
	int iState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "ObjState", NULL);
		iState = static_cast<int>(mpReturn);
	}
	if ( iState != 0 )
		return 0;
		

//    GetProperty "ObjState" == 0:
//      if healtimer != -1:
//        KillTimer healtimer
		
	if ( m_hTimerHandle != NULL )
	{
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
		
//      SetTimer "DoHeal",0.1
	
	m_fHealAmt += HealTotal(oFrobber, false);
	m_fPsiAmt += HealTotal(oFrobber, true);
	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "DoHeal", 100, kSTM_OneShot, g_mpUndef);
	
	// Current maximum HP cap not implemented
	
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundAmbient, kSoundNetwork0);
	
	m_iPatient = oFrobber;
	
	ConsumeStack(0);

//      healamt = GetScriptVar "HealAmt" + HealTotal frobber
//      healmax = GetProperty "MAX_HP" - GetProperty "HitPoints"
//      if healamt > healmax:
//        healamt = healmax
//      SetScriptVar "HealAmt",healamt
//      SetScriptVar "Patient",frobber
//      PlayEnvSchema "Event Activate",0,0,Ambient,net0
//      ConsumeStack 0

	return 0;
}

/***********************
 * NVPsiKitScript
 */
 
float cScr_NVPsiKitScript::BaseHealTotal()
{
	return 0.00f;
}

float cScr_NVPsiKitScript::BasePsiTotal()
{
	return 20.00f;
}

float cScr_NVPsiKitScript::BasePsiInc()
{
	return 2.00f;
}


/***********************
 * NVMedKitScript
 */
 
float cScr_NVMedKitScript::BaseHealTotal()
{
	return 200.00f;
}

float cScr_NVMedKitScript::BaseHealInc()
{
	return 5.00f;
}

/***********************
 * NVHealingGland
 */
 
float cScr_NVHealingGland::BaseHealTotal()
{
	return 15.00f;
}

float cScr_NVHealingGland::BaseHealInc()
{
	return 1.00f;
}

/***********************
 * NVVisibilityModifierHack
 */

void __stdcall cScr_NVVisibilityModifierHack::ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData)
{
	cScr_NVVisibilityModifierHack* pSelf = reinterpret_cast<cScr_NVVisibilityModifierHack*>(pData);
	if ( pSelf->m_bSim == false )
		return;
		
	int iObjId = pSelf->m_iObjId;
	if (sMsg->iObjId != iObjId)
		return;	// Only process for current object
		
	pSelf->DoUpdate();
	
	return;
}

void cScr_NVVisibilityModifierHack::DoUpdate()
{

	StopListener();

	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVVisibilityModifierHackDo", 100, kSTM_OneShot, g_mpUndef);
//	StartListener();
			
	return;
}

MSGHANDLER cScr_NVVisibilityModifierHack::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoUpdate();

	return 0;
}

MSGHANDLER cScr_NVVisibilityModifierHack::OnDeactivatePsi(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->RecalcStats(StrToObject("Player"));
	
	return 0;
}

MSGHANDLER cScr_NVVisibilityModifierHack::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);	
	cMultiParm mpReturn;
	
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	
	
	if (!stricmp(pszMsgName, "NVVisibilityModifierHackDo"))
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		cMultiParm mpReturn;
		char szProp[28];
		float fCurrent, fExpected;
				
		for ( int i = 0; i < 6; ++i )
		{
			sprintf(szProp, "NVVisibilityModifierHack%i", i);
			if ( ParamExists(m_iObjId, szProp))
			{
				fExpected = ParamGetFloat(m_iObjId, szProp, 1.00);
				
				sprintf(szProp, "Vis Type %i Mod", i);
				fCurrent = 1.00;
				
				if (pPropSrv->Possessed(m_iObjId, "AI_VisModifier"))
				{
					pPropSrv->Get(mpReturn, m_iObjId, "AI_VisModifier", szProp);
					fCurrent = static_cast<float>(mpReturn);
				}
				
				if ( fExpected != fCurrent )
				{
					pPropSrv->Set(m_iObjId, "AI_VisModifier", szProp, fExpected);
//			DisplayPrintf("Hey, modifier is %f but should be %f!\nChanging now.", fCurrent, fExpected);
				}
				
			}
		}
		
		g_pScriptManager->SetTimedMessage2(m_iObjId, "NVVisibilityModifierHackBackOn", 100, kSTM_OneShot, g_mpUndef);
	}
	
	if (!stricmp(pszMsgName, "NVVisibilityModifierHackBackOn"))
	{
		StartListener();
	}

	return 0;
}

		
void cScr_NVVisibilityModifierHack::StartListener(void)
{
	//StopListener();
	if ( !m_pProp )
	{
		SInterface<IPropertyManager> pPM(g_pScriptManager);
		m_pProp = static_cast<IGenericProperty*>(pPM->GetPropertyNamed("AI_VisModifier"));
	}
	if ( !m_hListenHandle )
	{
		m_hListenHandle = m_pProp->Listen(kPropertyChange | kPropertyAdd, ListenFunc, reinterpret_cast<PropListenerData__*>(this));
	}
	
	return;
}

void cScr_NVVisibilityModifierHack::StopListener(void)
{
	if ( m_hListenHandle )
	{
		m_pProp->Unlisten(m_hListenHandle);
		m_hListenHandle = NULL;
	}
	
	return;
}

/***********************
 * NVFreeModify
 */

MSGHANDLER cScr_NVFreeModify::OnFrobToolEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobDest = pFrobMsg->DstObjId;

	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	
	if ( !pShockGame->ValidGun(iFrobDest ) )
	{
		pShockGame->AddTranslatableText("FreeModifyCant", "Misc", StrToObject("Player"), 5000);
	} else {
		pShockGame->PreventSwap();
		cMultiParm mpReturn;
		
		int iState = 0;
		if (pPropSrv->Possessed(iFrobDest, "GunState"))
		{
			pPropSrv->Get(mpReturn, iFrobDest, "GunState", "Modification");
			iState = static_cast<int>(mpReturn);
		}
		
		if ( iState < 1 )
		{
			SimpleSend(0, iFrobDest, "ModifyIcon", -1);
			pShockGame->SetModify(iFrobDest, iState + 1);
			
			pShockGame->AddTranslatableText("FreeModify", "Misc", StrToObject("Player"), 5000);
			
			SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
			true_bool bReturn;
			pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundAmbient, kSoundNetwork0);
			
			cMultiParm mpStack = 0;
			if (pPropSrv->Possessed(m_iObjId, "StackCount"))
				pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
			
			if ( static_cast<int>(mpStack) <= 1 )
			{
				pShockGame->DestroyInvObj(m_iObjId);
			} else {
				SInterface<IContainSys> pContainSys(g_pScriptManager);
				pContainSys->StackAdd(m_iObjId, -1, 1);
			}
	
		} else {
			pShockGame->AddTranslatableText("FreeModifyMax", "Misc", StrToObject("Player"), 5000);
		}
	}
	
	return 0;
}

MSGHANDLER cScr_NVFreeModify::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->AddTranslatableText("HelpFreeModify", "Misc", StrToObject("Player"), 5000);
	return 0;
}

/***********************
 * NVDetoxTrap
 */

MSGHANDLER cScr_NVDetoxTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);	
	cMultiParm mpReturn;

	float fDeTox = ParamGetFloat(m_iObjId, "NVDetoxTrapAmount", 1.00f);
	float fToxin = 0;
	if (pPropSrv->Possessed(m_iObjId, "Toxin"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "Toxin", NULL);
		fToxin = static_cast<float>(mpReturn);
	}
	
	if ( fDeTox)
	{
		fToxin -= fDeTox;
		
		if ( fToxin <= 0.00f )
		{
			fToxin = 0.00f;
			pPropSrv->SetSimple(m_iObjId, "Toxin", fToxin);
			pShockGame->OverlayChange(31, 0);
		} else {
			pPropSrv->SetSimple(m_iObjId, "Toxin", fToxin);
			pShockGame->OverlayChange(31, 1);
		}
	}
	
	return 0;
}

MSGHANDLER cScr_NVDetoxTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVTranslucentTextTrap
 */

MSGHANDLER cScr_NVTranslucentTextTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);

	const char* pText = ParamGetString(m_iObjId, "NVTranslucentTextTrapText", NULL);
	const char* pFile = ParamGetString(m_iObjId, "NVTranslucentTextTrapFile", "misc");
	int iNumber = ParamGetInt(m_iObjId, "NVTranslucentTextTrapNumber", 0);
	int iTime = ParamGetInt(m_iObjId, "NVTranslucentTextTrapTime", 5000);
	
	if ( pText == NULL )
		return 1;
	
	pShockGame->TlucTextAdd(pText, pFile, iNumber);
	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVTranslucentTextTrapHide", iTime, kSTM_OneShot, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVTranslucentTextTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

MSGHANDLER cScr_NVTranslucentTextTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVTranslucentTextTrapHide"))
		return 0; // Abort if it's not the right timer.

	// Deactivate translucent text overlay
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->OverlayChange(41, 0);
	
	return 0;
}

MSGHANDLER cScr_NVTranslucentTextTrap::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// In case of level transitions / reload, hide the overlay.
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	pShockGame->OverlayChange(41, 0);
	
	return 0;
}


/***********************
 * NVVaporizeInventory
 */

MSGHANDLER cScr_NVVaporizeInventory::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
//  on 'TurnOn':
//    if Equipped 0:
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	SInterface<IContainSys> pContainSys(g_pScriptManager);	
	SService<IObjectSrv> pOS(g_pScriptManager);
	
//	DisplayPrintf("Vaporize!");
	
	object oEquipped;
	pShockGame->Equipped(oEquipped, 0);
	if ( oEquipped )
	{
		pShockGame->Equip(0, 0);
		pOS->Destroy(oEquipped);
//		DisplayPrintf("Destroy equipped!");
	}
	
	pShockGame->VaporizeInv();
	pShockGame->DestroyCursorObj();
//	DisplayPrintf("Destroy Cursor Obj!!");
	
	while (!pShockGame->PayNanites( 10000 ));
	
	pShockGame->Equipped(oEquipped, 10);
	
	pContainSys->StackAdd(oEquipped, -10000, 1);
	
	pShockGame->Equipped(oEquipped, 14);
	SimplePost(oEquipped, 0, "Hide");

	return 0;
}

MSGHANDLER cScr_NVVaporizeInventory::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

#endif // _DARKGAME == 3
