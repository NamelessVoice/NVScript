#ifndef _MSC_VER
#include <w32api.h>
#else
#include <stdio.h>
#define snprintf _snprintf
#endif
#include <windows.h>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include <set>
#include <sstream>

#include <lg/interface.h>
#include <lg/scrservices.h>
#include <lg/scrmanagers.h>
#include <lg/scrmsgs.h>
#include <lg/objects.h>
#include <lg/sound.h>
#include <lg/propdefs.h>
#include <lg/input.h>

#include <lg/win.h>

#include "ScriptLib.h"
#include "NVScriptLib.h"
#include "NVMaths.h"

#define INITOSM
#include "NVScript.h"
// instantiate script class registration for the other files as well
#include "NVScript-ThiefShock.h"
#include "NVScript-SS2.h"
#include "NVScript-Flames.h"
#include "NVScript-CoSaS.h"

#include "lg/iids.h"

#if (_DARKGAME == 3)
	#define CDLINK	"SwitchLink"
	#define TARGET_CDLINK "&SwitchLink"
	#define QVARPROP "QBName"
#else
	#define CDLINK	"ControlDevice"
	#define TARGET_CDLINK "&ControlDevice"
	#define QVARPROP "TrapQVar"
#endif

#if (_DARKGAME != 1)
	#define SOUND_NET  ,kSoundNetwork0
#else
	#define SOUND_NET
#endif

#define null NULL

#define MAX_INT LONG_MAX


#if (_DARKGAME != 3)
/***********************
 * NVEmitWhileSelected
 */
MSGHANDLER cScr_NVEmitWhileSelected::OnInventory(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
		char pszCfgTweqEmit[16];
		char pszStTweqEmit[16];
		char pszMsgName[80];
		strncpy(pszMsgName, pMsg->message, 80);

		// Get Tweq Number
		int iTweqNum = 0;
		bool bNVLanternHack = false;

		iTweqNum = ParamGetInt(m_iObjId, "NVTweqNum", 1);
		bNVLanternHack = (ParamGetInt(m_iObjId, "NVLanternHack", 0) != 0);

		if ((iTweqNum > 5) || iTweqNum < 1 )
		{
			iTweqNum = 1; // The Tweq number specified is invalid!
		}

		if ( iTweqNum == 1) {
			sprintf(pszCfgTweqEmit, "CfgTweqEmit");
			sprintf(pszStTweqEmit, "StTweqEmit");
		} else {
			sprintf(pszCfgTweqEmit, "CfgTweq%iEmit", iTweqNum);
			sprintf(pszStTweqEmit, "StTweq%iEmit", iTweqNum);
		}
		// Ignore Focus/DeFocus if NVIgnoreDeFocus=1
		if ( ParamGetInt(m_iObjId, "NVIgnoreDeFocus", 0) )
		{
			if (!stricmp(pszMsgName, "InvFocus") || !stricmp(pszMsgName, "InvDeFocus")) {
				return 0;
			}
		}

		// Get handle to player
		object oPlayer = StrToObject("Player");


		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		if (!stricmp(pszMsgName, "InvDeselect") || !stricmp(pszMsgName, "InvDeFocus")) {
			if ( bNVLanternHack ) // An undocumented feature. No, seriously.
			{
				object oLightToKill = StrToObject("LanternLight");
				if ( oLightToKill )
				{
					SService<IObjectSrv> pOS(g_pScriptManager);
					pOS->Destroy(oLightToKill);
				}
			}
			pPropSrv->Set(oPlayer, pszStTweqEmit, "Frame #", 0); // Reset frame
			pPropSrv->Set(oPlayer, pszStTweqEmit, "AnimS", 0); // Turn off emitter
		} else {
			if ( bNVLanternHack ) // An undocumented feature. No, seriously.
			{
				if (!StrToObject("LanternLight"))
				{
					SService<IObjectSrv> pOS(g_pScriptManager);
					object oArc;
					pOS->Named(oArc, "DynamicLanternLight");
					if ( oArc )
					{
						object oLight;
						pOS->Create(oLight, oArc);
					}
				}
			}
			if (pPropSrv->Possessed(m_iObjId, pszCfgTweqEmit))
			{
				pPropSrv->CopyFrom(oPlayer, pszCfgTweqEmit, m_iObjId); // Copy Tweq->Emit# property to player
				pPropSrv->Set(oPlayer, pszStTweqEmit, "AnimS", 1); // Turn on emitter
			}
		}
	return 0;
}
#endif

/***********************
 * NVLinkBuilder
 */

MSGHANDLER cScr_NVLinkBuilder::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoLink(true, pMsg);
	return 0;
}

MSGHANDLER cScr_NVLinkBuilder::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoLink(false, pMsg);
	return 0;
}

long cScr_NVLinkBuilder::DoLink(bool bOn, sScrMsg* pMsg)
{
	char *pszParamName = new char[24 + strlen(m_szName)];

	sprintf(pszParamName, "%sLinkSource", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVLinkSource");

	const char* pszLinkSource = ParamGetString(m_iObjId, pszParamName, NULL);

	sprintf(pszParamName, "%sLinkDest", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVLinkDest");

	const char* pszLinkDest = ParamGetString(m_iObjId, pszParamName, NULL);

	if (!pszLinkSource || !pszLinkDest)
	{ // Source or Destination not defined. Abort.
		delete[] pszParamName;
		return 1;
	}

	object oSource = NVStrToObject(pszLinkSource, pMsg);
	object oDest = NVStrToObject(pszLinkDest, pMsg);

	if ( !oSource || !oDest )
	{ // Source or destination object doesn't exist. Abort.
		delete[] pszParamName;
		return 0;
	}

	sprintf(pszParamName, "%sLinkType", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVLinkType");

	const char* pszLinkType = ParamGetString(m_iObjId, pszParamName, NULL);
	if ( !pszLinkType )
	{ // No link type specified. Abort.
		delete[] pszParamName;
		return 1;
	}

	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	int iFlavour = pLTS->LinkKindNamed(pszLinkType);

	delete[] pszParamName;

	if (bOn)
	{
		// Create a link, from oSource to oDest.
		return BuildLink(oSource, oDest, iFlavour);
	} else {
		return RemoveLink(oSource, oDest, iFlavour);
	}

	return 0;
}

MSGHANDLER cScr_NVLinkBuilder::BuildLink(int iSource, int iDest, int iFlavour)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	link lLink;

	true_bool bLinkExists;
	pLS->AnyExist(bLinkExists, iFlavour, iSource, iDest);
	if ( bLinkExists )
	{
//		DebugPrintf("Link already exists.");
//		DisplayPrintf("Link already exists.");

		if ( NVParamGetInt(m_iObjId, m_szName, "ModifyExistingLink", 0) == 1 )
		{
			pLS->GetOne(lLink, iFlavour, iSource, iDest);
		} else {
			return 0;
		}
	} else {
		pLS->Create(lLink, iFlavour, iSource, iDest);
	}


	if (lLink)
	{
		/*************************************
		 * Set up to 10 fields of link data.
		 */

		int i;
		char *pszParamName = new char[24 + strlen(m_szName)];
		const char* pszFieldName = NULL;
		const char* pszFieldValue;

		for (i=1;i<99;i++) {

			sprintf(pszParamName, "%sLinkData%iValue", m_szName, i);

			if (!ParamExists(m_iObjId, pszParamName) )
				sprintf(pszParamName, "NVLinkData%iValue", i);

			pszFieldValue = ParamGetString(m_iObjId, pszParamName, NULL);

			if (!pszFieldValue) {
				break; }

			cMultiParm mpValue;
			mxs_vector vVector;
			if (3 == sscanf(pszFieldValue, "%f, %f, %f", &vVector.x, &vVector.y, &vVector.z))
			{
				mpValue = vVector;
			} else {
				mpValue = pszFieldValue;
			}

			sprintf(pszParamName, "%sLinkData%iField", m_szName, i);

			if (!ParamExists(m_iObjId, pszParamName) )
				sprintf(pszParamName, "NVLinkData%iField", i);

			pszFieldName = ParamGetString(m_iObjId, pszParamName, NULL);
			pLTS->LinkSetData(lLink, pszFieldName, mpValue);
		} // End of for loop

		delete[] pszParamName;
	}

	return 0;
}

MSGHANDLER cScr_NVLinkBuilder::RemoveLink(int iSource, int iDest, int iFlavour)
{
	SService<ILinkSrv> pLS(g_pScriptManager);

	true_bool bLinkExists;
	pLS->AnyExist(bLinkExists, iFlavour, iSource, iDest);
	if ( bLinkExists )
	{
		link lLink;
		pLS->GetOne(lLink, iFlavour, iSource, iDest);
		if (lLink)
		{
			pLS->Destroy(lLink);
		} else { return 1; }
	}

	return 0;
}


/***********************
 * NVCreateAndLink
 */

MSGHANDLER cScr_NVCreateAndLink::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	char *pszParamName = new char[24 + strlen(m_szName)];

	sprintf(pszParamName, "%sCreate", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVCreate");

	const char* pszCreate = ParamGetString(m_iObjId, pszParamName, NULL);
	m_iArc = NVStrToObject(pszCreate, pMsg);

	if (m_iArc == 0)
	{
		delete[] pszParamName;
		return 0;
	}

	object oDest;
	object oSource;
	int iFlavour;

	sprintf(pszParamName, "%sLinkSource", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVLinkSource");

	const char* pszLinkSource = ParamGetString(m_iObjId, pszParamName, NULL);
	oSource = NVStrToObject(pszLinkSource, pMsg);

	sprintf(pszParamName, "%sLinkType", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVLinkType");

	const char* pszLinkType = ParamGetString(m_iObjId, pszParamName, NULL);
	if ( !pszLinkType )
	{ // No link type specified. Abort.
		delete[] pszParamName;
		return 1;
	}

	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	iFlavour = pLTS->LinkKindNamed(pszLinkType);

	SService<IObjectSrv> pOS(g_pScriptManager);

	if ( NVParamGetInt(m_iObjId, m_szName, "AllowMultiple", 0) == 0 )
	{
		object oCurrDest = NVParamGetInt(m_iObjId, m_szName, "oDest", 0);
		if ( oCurrDest != 0 )
		{
			true_bool bExists;
			pOS->Exists(bExists, oCurrDest);
			if ( bExists )
			{
				delete[] pszParamName;
				return 0;
			}
		}
	}

	pOS->Create(oDest, m_iArc);

	if ( !oSource )
	{
		oSource = m_iObjId;
	}


	if (oDest)
	{
		sprintf(pszParamName, "%sLoc", m_szName);
		if ( ParamExists(m_iObjId, pszParamName) )
		{
			const char* pszLocVector = ParamGetString(m_iObjId, pszParamName, "0,0,0");
			sprintf(pszParamName, "%sRot", m_szName);
			const char* pszRotVector = ParamGetString(m_iObjId, pszParamName, "0,0,0");
			mxs_vector vVector;
			mxs_vector vRot = {0.0, 0.0, 0.0};
			if (3 == sscanf(pszLocVector, "%f, %f, %f", &vVector.x, &vVector.y, &vVector.z))
			{
				sscanf(pszRotVector, "%f, %f, %f", &vRot.x, &vRot.y, &vRot.z);

				sprintf(pszParamName, "%sLocObj", m_szName);
				int iLocObj = ParamGetNVObject(m_iObjId, pszParamName, m_iObjId);
				pOS->Teleport(oDest, vVector, vRot, iLocObj);
			}
		}
		// Create a link, from m_iObjId to oDest.

		sprintf(pszParamName, "%soDest", m_szName);

		ParamSetInt(m_iObjId, pszParamName, oDest);
		BuildLink(oSource, oDest, iFlavour);

		if (iFlavour != pLTS->LinkKindNamed("Contains")) {
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			pPropSrv->Set(oDest, "HasRefs", NULL, true);
		}

		mxs_vector vZero = {0.0, 0.0, 0.0};
		pOS->Teleport(oSource, vZero, vZero, oSource);
	}

	delete[] pszParamName;
	return 0;
}

MSGHANDLER cScr_NVCreateAndLink::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DestroyLinkedObject(pMsg);
	return 0;
}

void cScr_NVCreateAndLink::DestroyLinkedObject(sScrMsg* pMsg)
{
	char *pszParamName = new char[24 + strlen(m_szName)];

	sprintf(pszParamName, "%sLinkSource", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVLinkSource");

	const char* pszLinkSource = ParamGetString(m_iObjId, pszParamName, NULL);
	object oSource = NVStrToObject(pszLinkSource, pMsg);

	sprintf(pszParamName, "%sLinkType", m_szName);
	if (!ParamExists(m_iObjId, pszParamName) )
		sprintf(pszParamName, "NVLinkType");

	const char* pszLinkType = ParamGetString(m_iObjId, pszParamName, NULL);
	if ( !pszLinkType )
	{ // No link type specified. Abort.
		delete[] pszParamName;
		return;
	}

	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	int iFlavour = pLTS->LinkKindNamed(pszLinkType);


	if ( !oSource )
	{
		oSource = m_iObjId;
	}

	sprintf(pszParamName, "%soDest", m_szName);
	object oDest = ParamGetInt(m_iObjId, pszParamName, 0);



	/* Make certain that the object still exists,
	 * and that it's still the same object (inherits from the correct archetype).
	 * Just in case someone deleted it while we were not looking.
	 * If the object is the same, destroy it.
	 */

	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bExists;

	pOS->Exists(bExists, oDest);
	if ( (bExists ) && ( oDest != 0) )
	{
		SService<ILinkSrv> pLS(g_pScriptManager);
		pLS->AnyExist(bExists, iFlavour, oSource, oDest); // Reuse bExists
		if ( bExists )
		{
			// Ouch - this is not specific to the trap number
			if ( ParamGetInt(m_iObjId, "NVSlayCreated", 0) )
			{
				SService<IDamageSrv> pDmgS(g_pScriptManager);
				pDmgS->Slay(oDest, m_iObjId);
			} else
			{
				pOS->Destroy(oDest);
			}
		}
	}

	sprintf(pszParamName, "%soDest", m_szName);
	ParamSetInt(m_iObjId, pszParamName, 0);

	delete[] pszParamName;
	return;
}

MSGHANDLER cScr_NVCreateAndLink::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( NVParamGetInt(m_iObjId, m_szName, "DestroyLinkedOnEnd", 0) == 1 )
	{
		DestroyLinkedObject(pMsg);
	}
	return 0;
}


/***********************
 * NVNameOnCreation
 */

MSGHANDLER cScr_NVNameOnCreation::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		Startup();
	}
	return 0;
}

MSGHANDLER cScr_NVNameOnCreation::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		Startup();
	}

	return 0;
}

void cScr_NVNameOnCreation::Startup()
{
	// Get symbolic name from DesignNote

	const char* pszSymName = ParamGetString(m_iObjId, "NVSymName", NULL);
	if (!pszSymName) {
		return;
	}

	char* strSymName = new char[strlen(pszSymName) + 6];
	int iIncrement = ParamGetInt(m_iObjId, "NVSymNameIncrement", 0);
	int i;

	sprintf(strSymName, "%s", pszSymName);
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oObject = 0;
	pOS->Named(oObject, pszSymName);

	if ( oObject )
	{
		// An object with that name already exists...
		if ( iIncrement )
		{
			// ...so add a number to the end of the name specified.
			for ( i=1; i < 101; i++ )
			{
				sprintf(strSymName, "%s%i", pszSymName, i);
				oObject = 0;
				pOS->Named(oObject, strSymName);
				if (!oObject)
				{
					break;
				}
			}

			if ( i >= 100)
			{
				// All the name/number combinations are used! Abort.
				delete[] strSymName;
				return;
			}
		} else {
			// ... so abort.
			delete[] strSymName;
			return;
		}
	}
	pOS->SetName(m_iObjId, strSymName); // Set the name

	delete[] strSymName;
}

 /***********************
 * NVFrobToggle
 */
MSGHANDLER cScr_NVFrobToggle::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iState = NVParamGetInt(m_iObjId, m_szName,  "State", NVParamGetInt(m_iObjId, m_szName,  "Start", 0));

	if (iState == 0) {
		Trigger(true, pMsg);
		iState = 1;
	} else {
		Trigger(false, pMsg);
		iState = 0;
	}

	NVParamSetInt(m_iObjId, m_szName, "State", iState);
	return 0;
}

 /***********************
 * NVGhostingMessages
 */
MSGHANDLER cScr_NVGhostingMessages::OnAlertness(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (!pPropSrv->Possessed(m_iObjId, "AI_Alertness"))
	{
		return 1; // This object doesn't have AI->State->Current Alertness - Abort.
	}

	cMultiParm mpAlertness;
	pPropSrv->Get(mpAlertness, m_iObjId, "AI_Alertness", "Level");
	int iAlertness = static_cast<int>(mpAlertness);

	if ( iAlertness > 1 ) // Alertness has raised above 1 == Ghosting Failed!
	{
		if (pMsg->time > 10 )
		{ // Don't allow 'ghosting failed' to trigger until 10 seconds of game time have elapsed.
			FailGhosting();
		}
	} else {	// Hopefully Alertness is either 0 or 1.
				// What happens to an AI's Alertness when s/he is KOed or killed?
		if (pPropSrv->Possessed(m_iObjId, "AI_Mode"))
		{
			cMultiParm mpMode;
			pPropSrv->Get(mpMode, m_iObjId, "AI_Mode", NULL);
			int iMode = static_cast<int>(mpMode);
			if ( iMode < 4 )	// Normal or efficient
			{
				m_iIgnore = 0;
			}
		}
	}

	 return 0;
}

MSGHANDLER cScr_NVGhostingMessages::OnDamage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 10 )
	{ // Don't allow 'ghosting failed' to trigger until 10 seconds of game time have elapsed.
		FailGhosting();
	}
	return 0;
}

MSGHANDLER cScr_NVGhostingMessages::OnAIModeChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (!pPropSrv->Possessed(m_iObjId, "AI_Mode"))
	{
		return 1; // This object doesn't have AI->State->Current Mode - Abort.
	}

	cMultiParm mpMode;
	pPropSrv->Get(mpMode, m_iObjId, "AI_Mode", NULL);
	int iMode = static_cast<int>(mpMode);

	if ( iMode > 3 ) // Mode is greater than 3 (must be either Combat or Dead) == Ghosting Failed!
	{
		if (pMsg->time > 10 )
		{ // Don't allow 'ghosting failed' to trigger until 10 seconds of game time have elapsed.
			FailGhosting();
		}
	}

	 return 0;
}

void cScr_NVGhostingMessages::FailGhosting()
 {
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	int iDiff = -1; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("Difficulty"))
	{
		iDiff = pQSrv->Get("Difficulty"); // Get QVar value
	}

	char *pszParamName = new char[14 + strlen(m_szName)];
	sprintf(pszParamName, "%sDifficulty%i", m_szName, iDiff);
	if ( ParamGetInt(m_iObjId, pszParamName, 1) == 0)
		return;

	const char* szQvar = ParamGetString(m_iObjId, "NVGhostingFailedQVar", "NVGhostingFailed");
	if ( !m_iIgnore ) // Don't increment the QVar more than once per AI
	  {
		m_iIgnore = 1; // This is reset if the AI's alertness drops back to under 2.
		int iCurrent = 0; // Default value (if QVar doesn't exist)
		if (pQSrv->Exists(szQvar))
		{
			iCurrent = pQSrv->Get(szQvar); // Get QVar value
		}

		int iAlwaysShowMessages = ParamGetInt(m_iObjId, "NVGhostingMessages", 0);
		bool bIgnoreMe = (ParamGetInt(m_iObjId, "NVGhostingIgnore", 0) != 0);

		if ( !iAlwaysShowMessages && pQSrv->Exists("NVGhostingMessages"))
		{
			iAlwaysShowMessages = pQSrv->Get("NVGhostingMessages");
		}

		if ( bIgnoreMe )
		{
			return;
		}
		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrStr strName;
		pOS->GetName(strName, m_iObjId);

		if ( !stricmp(static_cast<const char*>(strName), "Cavador") )
		{
			// This is Brother Cavador, who needs to be KOed...
			return;
		}

		// Start of big hack for Miss11 archers' conv.
		object oArc;
		pOS->Named(oArc, "bow man");
		true_bool bBool;
		pOS->InheritsFrom(bBool, m_iObjId, oArc);
		if ( bBool )
		{
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			if (pPropSrv->Possessed(m_iObjId, "AI_FleeConds"))
			{
				cMultiParm mpProp;
				pPropSrv->Get(mpProp, m_iObjId, "AI_FleeConds", "Condition");
				int iProp = static_cast<int>(mpProp);
				if ( iProp == 0 )
				{
					if (pPropSrv->Possessed(m_iObjId, "AI_NtcBody"))
					{
						pPropSrv->Get(mpProp, m_iObjId, "AI_NtcBody", NULL);
						bool bProp = static_cast<bool>(mpProp);
						if ( !bProp )
						{
							return;
						}
					}
				}
			}
		} // End of archer conv hack

		iCurrent++; // Increment ghosting failures counter

		int iColour = strtocolor(ParamGetString(m_iObjId, "NVGhostingMessagesColour", "#FFFFFF"));

		SService<IDataSrv> pDataSrv(g_pScriptManager);
		cScrStr strMessage;
		if ( iCurrent == 1 ) {
			// Display the message "Ghosting Failed!";
			// Only happens the first time.
			pDataSrv->GetString(strMessage, "PlayHint.str", "GhostingFailed", "", "Strings");
			if (strMessage.IsEmpty())
			{
				DisplayPrintf(iColour, "Ghosting Failed!");
			} else {
				DisplayPrintf(iColour, "%s", static_cast<const char*>(strMessage));
			}
		} else if ( iAlwaysShowMessages ) {
			pDataSrv->GetString(strMessage, "PlayHint", "GhostingFailed2a", "", "Strings");
			cScrStr strMessage2;
			pDataSrv->GetString(strMessage2, "PlayHint", "GhostingFailed2b", "", "Strings");
			if (strMessage.IsEmpty())
			{
				DisplayPrintf(iColour, "Ghosting failed!\n(Total: %i times)", iCurrent);
			} else {
				DisplayPrintf(iColour, "%s%i%s", static_cast<const char*>(strMessage), iCurrent, static_cast<const char*>(strMessage2));
			}
			if (!strMessage.IsEmpty())
			{
				strMessage2.Free();
			}
		 // Subsequent "Ghosting Failed!" messages - these never appear unless NVGhostingMessages=1
		}

		if (!strMessage.IsEmpty())
		{
			strMessage.Free();
		}




//			DebugPrintf("Ghosting failed on obj %i", m_iObjId);
		pQSrv->Set(szQvar, iCurrent, 0);
	}

	return;
}

MSGHANDLER cScr_NVGhostingMessages::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DisplayPrintf(" ");
	return 0;
}

/***********************
 * NVScaleDims
 */

MSGHANDLER cScr_NVScaleDims::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
		Startup();

	return 0;
}

MSGHANDLER cScr_NVScaleDims::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		Startup();
	}

	return 0;
}

void cScr_NVScaleDims::Startup()
{
		m_fBaseLine = 0;

		cMultiParm mpReturn;
		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		// Don't do anything unless the object has the Physics->Model->Type property
		if (pPropSrv->Possessed(m_iObjId, "PhysType"))
		{
			// Get startup sizes (depending on PhysType)
			pPropSrv->Get(mpReturn, m_iObjId, "PhysType", "Type");
			m_iPhysType = static_cast<int>(mpReturn);
			mxs_vector vSize={0,0,0}, vOffset={0,0,0};
			if (pPropSrv->Possessed(m_iObjId, "PhysDims"))
			{
				// Get offset (always the same for all phys models)
				// Don't bother with Offset 2 (for Spheres), though.
				pPropSrv->Get(mpReturn, m_iObjId, "PhysDims", "Offset 1");
				vOffset.x = static_cast<const mxs_vector*>(mpReturn)->x;
				vOffset.y = static_cast<const mxs_vector*>(mpReturn)->y;
				vOffset.z = static_cast<const mxs_vector*>(mpReturn)->z;

				if ( m_iPhysType == 0 ) // OBB
				{
					pPropSrv->Get(mpReturn, m_iObjId, "PhysDims", "Size");
					vSize.x = static_cast<const mxs_vector*>(mpReturn)->x;
					vSize.y = static_cast<const mxs_vector*>(mpReturn)->y;
					vSize.z = static_cast<const mxs_vector*>(mpReturn)->z;
					if ( ParamGetInt(m_iObjId, "NVBaseLock", 0) )
					{

							SService<IObjectSrv> pOS(g_pScriptManager);
							cScrVec vPosition;
							pOS->Position(vPosition, m_iObjId);

							m_fBaseLine = vPosition.z - ((vSize.z / 2 ) + vOffset.z);
					}
				} else if ( m_iPhysType == 1 ) // Sphere
				{
					pPropSrv->Get(mpReturn, m_iObjId, "PhysDims", "Radius 1");
					vSize.x = static_cast<float>(mpReturn);
					pPropSrv->Get(mpReturn, m_iObjId, "PhysDims", "Radius 2");
					vSize.y = static_cast<float>(mpReturn);
				} else if ( m_iPhysType == 2 ) // Sphere Hat
				{
					pPropSrv->Get(mpReturn, m_iObjId, "PhysDims", "Radius 1");
					vSize.x = static_cast<float>(mpReturn);
					vSize.y = static_cast<float>(mpReturn);
					vSize.z = static_cast<float>(mpReturn);
				}

				// If the object is scaled, then divide the current sizes by the scale factor.
				if ( pPropSrv->Possessed(m_iObjId, "Scale") )
				{
					pPropSrv->Get(mpReturn, m_iObjId, "Scale", NULL);
					vSize.x /= static_cast<const mxs_vector*>(mpReturn)->x;
					vSize.y /= static_cast<const mxs_vector*>(mpReturn)->y;
					vSize.z /= static_cast<const mxs_vector*>(mpReturn)->z;
					vOffset.x /= static_cast<const mxs_vector*>(mpReturn)->x;
					vOffset.y /= static_cast<const mxs_vector*>(mpReturn)->y;
					vOffset.z /= static_cast<const mxs_vector*>(mpReturn)->z;
				}
			}

			m_vSize = &vSize;
			m_vOffset = &vOffset;
		} // No physics model property
	return;
}

MSGHANDLER cScr_NVScaleDims::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_iPhysType != 3) // if the Physics->Model->Type is not 'None'
	{
		if (m_hTimerHandle == NULL)
		{ // Start a timer.
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "FixDims", 100, kSTM_Periodic, g_mpUndef);
		}
	}
	return 0;
}

MSGHANDLER cScr_NVScaleDims::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	KillTimer();
	return 0;
}

MSGHANDLER cScr_NVScaleDims::OnTweqComplete(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sTweqMsg* pTweqMsg = static_cast<sTweqMsg*>(pMsg);
	if ( pTweqMsg->Type == kTweqTypeScale )
	{ // Make sure it was a TweqScale which just completed...
		KillTimer();
	}
	return 0;
}

void cScr_NVScaleDims::KillTimer()
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return;
}

MSGHANDLER cScr_NVScaleDims::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "FixDims"))
		return 0; // Abort if it's not the right timer.

	cMultiParm mpReturn;
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	// Don't do anything unless the object has the Shape->Scale property
	if ( pPropSrv->Possessed(m_iObjId, "Scale") )
	{
		pPropSrv->Get(mpReturn, m_iObjId, "Scale", NULL);
		cScrVec vScale;
		vScale.x = static_cast<const mxs_vector*>(mpReturn)->x;
		vScale.y = static_cast<const mxs_vector*>(mpReturn)->y;
		vScale.z = static_cast<const mxs_vector*>(mpReturn)->z;
		cScrVec vDims, vOffset;

		// Multiply original sizes/offsets by scale factor.
		vDims = vScale * *m_vSize;
		vOffset = vScale * *m_vOffset;

		// Set new sizes (depending on PhysType)
		if ( m_iPhysType == 0 ) // OBB
		{
			pPropSrv->Set(m_iObjId, "PhysDims", "Size", vDims);

			SService<IObjectSrv> pOS(g_pScriptManager);
			cScrVec vFacing, vPosition;
			pOS->Facing(vFacing, m_iObjId);
			pOS->Position(vPosition, m_iObjId);

			if ( ParamGetInt(m_iObjId, "NVBaseLock", 0) )
			{
				vPosition.z = m_fBaseLine + (vDims.z / 2);
				pOS->Teleport(m_iObjId, vPosition, vFacing, 0);
			}


			float fTopSurface = vPosition.z + (vDims.z / 2);

			SService<ILinkSrv> pLS(g_pScriptManager);
			SService<ILinkToolsSrv> pLTS(g_pScriptManager);

			linkset lsLinks;
			linkkind lkFlavor = pLTS->LinkKindNamed("Owns");
			pLS->GetAll(lsLinks, lkFlavor, m_iObjId, 0);

			for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
			{
				sLink sl = lsLinks.Get();
				cScrVec vObjPos, vObjFacing;
				pOS->Facing(vObjFacing, sl.dest);
				pOS->Position(vObjPos, sl.dest);
				vObjPos.z = fTopSurface;

				float fMaxHeight = ParamGetFloat(sl.dest, "NVTopItemSlayHeight", 99999.99f);
				if (fTopSurface > fMaxHeight)
				{
					pOS->Destroy(sl.dest);
				}


				pOS->Teleport(sl.dest, vObjPos, vObjFacing, 0);
			}
		} else if ( m_iPhysType < 3 )
		{
			if ( m_iPhysType == 1 ) // Sphere
			{
				pPropSrv->Set(m_iObjId, "PhysDims", "Radius 1", vDims.x);
				pPropSrv->Set(m_iObjId, "PhysDims", "Radius 2", vDims.y);
			}
			if ( m_iPhysType == 2 ) // Sphere Hat
			{
				pPropSrv->Set(m_iObjId, "PhysDims", "Radius 1", vDims.x);
			}

			// Touch the velocity to 'wake up' the physics model (and make a shrunk model fall)
			// (And make an enlarged model fall through the floor. Ooops.
			SService<IPhysSrv> pPhysSrv(g_pScriptManager);
			cScrVec vVelocity;
			pPhysSrv->GetVelocity(m_iObjId, vVelocity);
			vVelocity.z = vVelocity.z + 0.01f;
			pPhysSrv->SetVelocity(m_iObjId, vVelocity);

		}
		if ( m_iPhysType != 3 )
		{ // Set new offset (so long as the PhysType is not 'None')
			pPropSrv->Set(m_iObjId, "PhysDims", "Offset 1", vOffset);
		}

	} // End of 'if object has Shape->Scale property'

	if ( pPropSrv->Possessed(m_iObjId, "StTweqScale") )
	{
		pPropSrv->Get(mpReturn, m_iObjId, "StTweqScale", "AnimS");
		int iAnimS = static_cast<int>(mpReturn);
		if ( !(iAnimS & 1) )
		{ // The AnimS of Tweq->ScaleState is off. Stop the timer.
			KillTimer();
		}
	} else
	{ // No Tweq->ScaleState property? Stop the timer.
		KillTimer();
	}
	return 0;

}

/***********************
 * NVFakeWater
 */

MSGHANDLER cScr_NVFakeWater::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
// Make sure the timer is running...
	if (m_hTimerHandle == NULL)
	{
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "Drown", 100, kSTM_Periodic, g_mpUndef);
	}
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(m_iObjId, kPhysEnterExit);
	return 0;
}

MSGHANDLER cScr_NVFakeWater::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysEnterExit);
	return 0;
}

void cScr_NVFakeWater::KillTimer()
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return;
}

MSGHANDLER cScr_NVFakeWater::OnFakeWaterOn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Make sure the timer is running...
	if (m_hTimerHandle == NULL)
	{
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "Drown", 100, kSTM_Periodic, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVFakeWater::OnFakeWaterOff(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	KillTimer();
	return 0;
}


MSGHANDLER cScr_NVFakeWater::OnPhysEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);

	SService<IObjectSrv> pOS(g_pScriptManager);
	object oPlayer;
	pOS->Named(oPlayer, "Player");
	object oTransObj = pPhysMsg->transObj;
	if ( oTransObj == oPlayer )
	{
		// Make sure the timer is running...
		if (m_hTimerHandle == NULL)
		{
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "Drown", 100, kSTM_Periodic, g_mpUndef);
		}
		return 0;
	} else {

		// Adjust physics
		SService<IPhysSrv> pPhySrv(g_pScriptManager);
		pPhySrv->SetGravity(oTransObj, 0.10f); // SetGravity uses Gravity % / 100.
		cScrVec vVelocity;
		pPhySrv->GetVelocity(oTransObj, vVelocity);
		vVelocity.x *= 0.10f;
		vVelocity.y *= 0.10f;
		vVelocity.z *= 0.10f;
		pPhySrv->SetVelocity(oTransObj, vVelocity);

		// Make a splashing sound
		object oSplashSound = 0;
		pOS->Named(oSplashSound, "hwater");

		if ( oSplashSound )
		{
			SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
			true_bool bRet;
			pSndSrv->PlaySchemaAtObject(bRet, m_iObjId, oSplashSound, oTransObj SOUND_NET);
		}

		// Hit the object with WaterStim
		object oWaterStim = 0;
		pOS->Named(oWaterStim, "WaterStim");
		if ( oWaterStim )
		{	// Just in case someone deleted the WaterStim stimulus
			SService<IActReactSrv> pARSrv(g_pScriptManager);
			#if (_DARKGAME != 1)
				pARSrv->Stimulate(oTransObj, oWaterStim, 10.00f, m_iObjId);
			#else // Backwards compatibility for T1. Fairly pointless since the module is set to only compile for _DARKGAME 2.
				pARSrv->Stimulate(oTransObj, oWaterStim, 10.00);
			#endif
		}

	}
	return 0;
}

MSGHANDLER cScr_NVFakeWater::OnPhysExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);

	SService<IObjectSrv> pOS(g_pScriptManager);
	object oPlayer;
	pOS->Named(oPlayer, "Player");
	object oTransObj = pPhysMsg->transObj;
	if ( oTransObj != oPlayer )
	{
		SService<IPhysSrv> pPhySrv(g_pScriptManager);
		pPhySrv->SetGravity(oTransObj, 1.00); // SetGravity uses Gravity % / 100.
	}

	return 0;
}

MSGHANDLER cScr_NVFakeWater::InWater()
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	m_iInWater = 1;

	object oPlayer = 0;
	pOS->Named(oPlayer, "Player");
	// Warning: Hack!

	object oInWater = 0;
	pOS->Named(oInWater, "InWater-Mask");
	if (!oInWater)
	{
		object oInWaterArch = 0;
		pOS->Named(oInWaterArch, "InWater");
		if (oInWaterArch)
		{
			pOS->Create(oInWater, oInWaterArch);
			pOS->SetName(oInWater, "InWater-Mask");
			SService<ILinkSrv> pLS(g_pScriptManager);

			SService<ILinkToolsSrv> pLTS(g_pScriptManager);

			link lLink;
			pLS->Create(lLink, pLTS->LinkKindNamed("~ParticleAttachement"), oPlayer, oInWater);
			if ( lLink )
			{
				pLTS->LinkSetData(lLink, "Type", 3);
				pLTS->LinkSetData(lLink, "submod #", 0);
			}
		}
	}
	if (oInWater)
		pPropSrv->Set(oInWater, "RenderType", NULL, 0); // Rendered


	// Store proper Recover Rate in script var
	cMultiParm mpRecover;
	if (pPropSrv->Possessed(oPlayer, "BreathConfig"))
	{
		pPropSrv->Get(mpRecover, oPlayer, "BreathConfig", "Recover Rate");
		m_fRecoverRate = static_cast<float>(mpRecover);
	} else { m_fRecoverRate = 3.00; }

	// Set Recover Rate to negative (so it becomes 'Drown Rate')
	pPropSrv->Set(oPlayer, "BreathConfig", "Recover Rate", -1.0);

	// Play 'underwater' ambience loop
	SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
	true_bool bRet;
	object oUnderwater = 0;
	pOS->Named(oUnderwater, "FakeUnderwater");
	if ( !oUnderwater )
	{
		pOS->Named(oUnderwater, "Underwater");
	}
	pSndSrv->PlaySchemaAmbient(bRet, m_iObjId, oUnderwater SOUND_NET);

	return 0;
}

MSGHANDLER cScr_NVFakeWater::OutWater()
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);

	object oPlayer = 0;
	pOS->Named(oPlayer, "Player");
	// Warning: Hack!

	object oInWater = 0;

	pOS->Named(oInWater, "InWater-Mask");
	if (oInWater)
	{
		pPropSrv->Set(oInWater, "RenderType", NULL, 1); // Not Rendered
	}


	// Restore Recover Rate from script var
	float fRecoverRate = (m_fRecoverRate) ? m_fRecoverRate : 5.00f;
	pPropSrv->Set(oPlayer, "BreathConfig", "Recover Rate", fRecoverRate);

	// Stop 'underwater' ambience
	SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
	true_bool bRet;
	pSndSrv->HaltSchema(bRet, m_iObjId, "", 0);
	m_iInWater = 0;

	// Make a surfacing sound
	object oSound = StrToObject("surface_head");

	if ( oSound )
	{
		SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
		true_bool bRet;
		pSndSrv->PlaySchemaAtObject(bRet, m_iObjId, oSound, oPlayer SOUND_NET);
	}

	return 0;
}

MSGHANDLER cScr_NVFakeWater::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "Drown"))
		return 1;

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	SService<IObjectSrv> pOS(g_pScriptManager);
	object oPlayer = 0;
	pOS->Named(oPlayer, "Player"); // Warning: Hack!

	// Okay, the PhysEnter/Exit is too unreliable for the player.
	// Let's try it another way:
	cScrVec vPlayerPos, vMyPos;
	pOS->Position(vPlayerPos, oPlayer);
	pOS->Position(vMyPos, m_iObjId);

	if (pPropSrv->Possessed(m_iObjId, "PhysDims"))
	{
		cMultiParm mpDims, mpOffset, mpPlayerOffset;
		cScrVec vDims, vOffset;

		pPropSrv->Get(mpDims, m_iObjId, "PhysDims", "Size");
		vDims.x = static_cast<const mxs_vector*>(mpDims)->x / 2;
		vDims.y = static_cast<const mxs_vector*>(mpDims)->y / 2;
		vDims.z = static_cast<const mxs_vector*>(mpDims)->z / 2;

		pPropSrv->Get(mpOffset, m_iObjId, "PhysDims", "Offset 1");
		vOffset.x = static_cast<const mxs_vector*>(mpOffset)->x;
		vOffset.y = static_cast<const mxs_vector*>(mpOffset)->y;
		vOffset.z = static_cast<const mxs_vector*>(mpOffset)->z;

		float fMinX = vMyPos.x - vDims.x + vOffset.x;
		float fMinY = vMyPos.y - vDims.y + vOffset.y;
		float fMinZ = vMyPos.z - vDims.z + vOffset.z;
		float fMaxX = vMyPos.x + vDims.x + vOffset.x;
		float fMaxY = vMyPos.y + vDims.y + vOffset.y;
		float fMaxZ = vMyPos.z + vDims.z + vOffset.z;
		if ( ( vPlayerPos.x > fMinX ) && ( vPlayerPos.x < fMaxX ) )
		{
			if ( ( vPlayerPos.y > fMinY ) && ( vPlayerPos.y < fMaxY ) )
			{
				if ( ( vPlayerPos.z > ( fMinZ -2 ) ) && ( vPlayerPos.z < ( fMaxZ + 4) ) )
				{
					object oSwimSound = 0;
					if ( vPlayerPos.z < (fMaxZ - 1 ) )
					{
						m_iGrav = 1;
						// Set Player physics:
						pPropSrv->Set(oPlayer, "PhysAttr", "Gravity %", 0.0);
						pPropSrv->Set(oPlayer, "PhysAttr", "Base Friction", 1.0);

						// Check if the player is on the surface of the water:
						// Fist calculate player eye offset:

						/********************************************************
						 * I would have thought that the player's view was at the centre
						 * of his submodel 1 (Z +1.8), but it doesn't appear to be.
						 *
						 * The value is, in fact, submodel 1 + the 'eyeloc' config variable, the default
						 * value of which is 0.80. So, 1.80 + 0.80 = 2.60
						 */

						SService<IDarkGameSrv> pDarkSrv(g_pScriptManager);
						float fEyeLoc = 0.80f;
						#if (_DARKGAME == 2)
						if ( pDarkSrv->ConfigIsDefined("eyeloc") )
						{
							pDarkSrv->ConfigGetFloat("eyeloc", &fEyeLoc);
						}
						#endif

						pPropSrv->Get(mpPlayerOffset, oPlayer, "PhysDims", "Offset 1");
						fEyeLoc += static_cast<const mxs_vector*>(mpPlayerOffset)->z;

						// Check if the player is on the surface of the water.
						if ( vPlayerPos.z > ( fMaxZ - fEyeLoc ) )
						{
							if ( m_iInWater == 1 )
							{
								OutWater();
							}
							pOS->Named(oSwimSound, "fakeswim_surface");
							if ( !oSwimSound) {
								pOS->Named(oSwimSound, "swim_surface");
							}
						} else
						{ // Then the player must be underwater...
							pOS->Named(oSwimSound, "fakeswim_underwater");
							if ( !oSwimSound) {
								pOS->Named(oSwimSound, "swim_underwater");
							}
							if ( m_iInWater == 0 )
							{
								InWater();
							}
						}
						if ( !m_iSlowed )
						{
							m_iSlowed = 1;
							// Add 70% speed control to player
							SService<IDarkInvSrv> pDISrv(g_pScriptManager);
							pDISrv->AddSpeedControl("FakeWater", 0.7f, 0.0f);
						}

						// No using weapons  underwater!
						SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
						pDebugSrv->Command("clear_weapon", "", "", "", "", "", "", "");
						// Also, stand up!
						pDebugSrv->Command("-crouchhold", "", "", "", "", "", "", "");

						// Set the player's breath-related properties.
						cMultiParm mpAir;
						pPropSrv->Get(mpAir, oPlayer, "AirSupply", NULL);
						float fAir = static_cast<float>(mpAir);

						if ( fAir <= 0 )
						{
							pPropSrv->Get(mpAir, oPlayer, "BreathConfig", "Drown Damage");
							int iDrownDamage = static_cast<int>(mpAir);
							pPropSrv->Get(mpAir, oPlayer, "BreathConfig", "Drown Freq (ms)");
							int iDrownRate = static_cast<int>(mpAir);

							object oWaterStim = 0;
							pOS->Named(oWaterStim, "WaterStim");
							SService<IDamageSrv> pDSrv(g_pScriptManager);
							// It doesn't matter if WaterStim was deleted.
							pDSrv->Damage(oPlayer, m_iObjId, iDrownDamage, static_cast<int>(oWaterStim));
							pPropSrv->Set(oPlayer, "AirSupply", NULL, iDrownRate);

						}
					} else {
						if ( m_iSlowed )
						{
							// Make a surfacing sound
							pOS->Named(oSwimSound, "surface_body");
						} else {
							pOS->Named(oSwimSound, "foot_water_p");
						}
					}

					cScrVec vPlayer;
					SService<IPhysSrv> pPhysSrv(g_pScriptManager);
					pPhysSrv->GetVelocity(oPlayer, vPlayer);

					float fPlyrVelocity = fabs(vPlayer.x) + fabs(vPlayer.y) + fabs(vPlayer.z);
					if ( fPlyrVelocity )
					{
							if ( vPlayer.z < -14 )
							{
								m_iTimerCount = 0;
								oSwimSound = 0;
								pOS->Named(oSwimSound, "land_water_p");
							}
						if ( !m_iTimerCount)
						{
							// Play spashing sounds
							if ( oSwimSound )
							{
								SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
								true_bool bRet;
								pSndSrv->PlaySchemaAtObject(bRet, m_iObjId, oSwimSound, oPlayer SOUND_NET);
							}
						}
						m_iTimerCount = (m_iTimerCount + 1) % 3;
					}

					if ( vPlayerPos.z < (fMaxZ - 1 ) )
					{
						return 0;
					}
				}
			}
		}

		if ( m_iGrav)
		{
			m_iGrav = 0;
			// Set Player physics to 100% grav, 0.00 friction ('normal').
			pPropSrv->Set(oPlayer, "PhysAttr", "Gravity %", 100.0);
			pPropSrv->Set(oPlayer, "PhysAttr", "Base Friction", 0.0);
		}

		if ( m_iSlowed )
		{
			m_iSlowed = 0;
			// Remove Player speed control
			SService<IDarkInvSrv> pDISrv(g_pScriptManager);
			pDISrv->RemoveSpeedControl("FakeWater");
		}
	}


	if ( m_iInWater == 1 )
	{
		OutWater();
	}


	return 0;
}

/***********************
 * NVLookAtNoises
 */
MSGHANDLER cScr_NVLookAtNoises::OnAlertness(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sAIAlertnessMsg* pAlertMsg = static_cast<sAIAlertnessMsg*>(pMsg);

	//if ( ( pAlertMsg->level == kLowAlert ) && ( pAlertMsg->oldLevel == kNoAlert) )
	if ( pAlertMsg->level == kLowAlert )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (pPropSrv->Possessed(m_iObjId, "AI_Mode"))
		{
			cMultiParm mpMode;
			pPropSrv->Get(mpMode, m_iObjId, "AI_Mode", NULL);
			int iMode = static_cast<int>(mpMode);
			if ( iMode == 3 )	// Normal
			{
				// Alertness is 1, current mode is normal.
				DoCheck();
				if (m_hTimerHandle == NULL)
				{ // Start a timer.
					m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "LookAtNoises", 2000, kSTM_Periodic, g_mpUndef);
				}
			} else { // If Alertness is 1, mode is not 3.
				KillTimer();
			}
		} else {// AI_Mode property exists.
			KillTimer();
		}
	} else { // Alertness is not kLowAlert...
		KillTimer();
	}

	return 0;
}

void cScr_NVLookAtNoises::DoCheck()
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	true_bool bLinkExists;
	pLS->AnyExist(bLinkExists, pLTS->LinkKindNamed("AIAwareness"), m_iObjId, 0);
	if ( bLinkExists )
	{
		int iLastTime = m_iLastTime;
		m_iLastTime = 0;
		IterateLinks("AIAwareness", m_iObjId, 0, AwarenessIterFunc, this, NULL);
		if ( m_iLastTime > ( iLastTime + 2000 ) )
		{

			// Now, calculate the angle:
			SService<IObjectSrv> pOS(g_pScriptManager);
			cScrVec vFacing, vPosition;
			pOS->Facing(vFacing, m_iObjId);
			pOS->Position(vPosition, m_iObjId);
			vPosition -= *m_vLocation;

			float fAbsX = fabs(vPosition.x);
			float fAbsY = fabs(vPosition.y);
			float fDivisor = (fAbsX >= fAbsY) ? fAbsX : fAbsY;

			// Set the vision joint to 'Head', just to be sure...
			pPropSrv->Set(m_iObjId, "AI_VisJoint", "joint", 1);

			SService<IPuppetSrv> pPupS(g_pScriptManager);
			true_bool bReturn;

			if ( (vPosition.z < fDivisor ) )
			{
				int iLongitude = static_cast<int>((630 - vFacing.z) - atan2(vPosition.x, vPosition.y) * 180/M_PI);
				iLongitude = iLongitude % 360;
				//DisplayPrintf("Last Contact: %f, %f, %f.\nAt %i", static_cast<float>(m_fX), static_cast<float>(m_fY), static_cast<float>(m_fZ), static_cast<int>(m_iLastTime));
				//DisplayPrintf("I hear something at %i degrees.", iLongitude);

				// This script is unfinished, so it mostly just outputs to the screen.

				if ( ( iLongitude <= 45 ) || ( iLongitude >= 315 ) )
				{
					DisplayPrintf("I hear something in front of me.\n(%f, %f)\n%i degrees)", vPosition.x, vPosition.y, iLongitude);
				} else if ( iLongitude <= 135 )
				{
					pPupS->PlayMotion(bReturn, m_iObjId, "LookL");
					DisplayPrintf("I hear something to my left.\n(%f, %f)\n%i degrees)", vPosition.x, vPosition.y, iLongitude);
				} else if ( iLongitude <= 180 )
				{
					pPupS->PlayMotion(bReturn, m_iObjId, "LookBL");
					DisplayPrintf("I hear something behind me and to the left!\n(%f, %f)\n%i degrees)", vPosition.x, vPosition.y, iLongitude);
				} else if ( iLongitude <= 225 )
				{
					pPupS->PlayMotion(bReturn, m_iObjId, "LookBR");
					DisplayPrintf("I hear something behind me and to the right!\n(%f, %f)\n%i degrees)", vPosition.x, vPosition.y, iLongitude);
				} else
				{
					pPupS->PlayMotion(bReturn, m_iObjId, "LookR");
					DisplayPrintf("I hear something to my right.\n(%f, %f)\n%i degrees)", vPosition.x, vPosition.y, iLongitude);
				}
			} else { // Look up
				pPupS->PlayMotion(bReturn, m_iObjId, "LookUp");
				DisplayPrintf("I hear something above me!\n(%f units above me.)", vPosition.z);
			}

		}// If it hasn't been reacted to before.
	} // If link exists
	return;
}

void cScr_NVLookAtNoises::KillTimer()
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return;
}

int cScr_NVLookAtNoises::AwarenessIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	cScr_NVLookAtNoises* pSelf = static_cast<cScr_NVLookAtNoises*>(pScript);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	cMultiParm mpReturn;
	pLTS->LinkGetData(mpReturn, pLQ->ID(), "Flags");
	if ( (int)mpReturn & 2 )
	{
		pLTS->LinkGetData(mpReturn, pLQ->ID(), "Time last contact");
		int iCurrTime = static_cast<int>(mpReturn);
		if ( iCurrTime > static_cast<int>(pSelf->m_iLastTime) )
		{
			pSelf->m_iLastTime = iCurrTime;
			pLTS->LinkGetData(mpReturn, pLQ->ID(), "Pos last contact");
			pSelf->m_vLocation = static_cast<const mxs_vector*>(mpReturn);
		}
	}
	return 1; // Oops: we need to return 1 or the loop will be broken.
}

MSGHANDLER cScr_NVLookAtNoises::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "LookAtNoises"))
	{  // Do nothing if it's not the right timer.
		DoCheck();
	}

	return 0;
}

/***********************
 * NVIncapacitatedMessages
 */

MSGHANDLER cScr_NVIncapacitatedMessages::OnDamage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (pPropSrv->Possessed(m_iObjId, "AI_Mode"))
	{ // Just in case the object doesn't have the AI->State->Current Mode property...
		cMultiParm mpMode;
		pPropSrv->Get(mpMode, m_iObjId, "AI_Mode", NULL);
		int iMode = static_cast<int>(mpMode);

		if ( iMode == 5 ) // Mode is 'dead' (also used for KOed people)
		{
			sScrMsg msg;
			msg.from = pMsg->from;
			msg.to = m_iObjId;
			msg.time = pMsg->time;
			msg.flags = pMsg->flags; // No one knows what this means.
			msg.message = "Incapacitated";
			g_pScriptManager->SendMessage(&msg, pReply);
		}
	}


	return 0;
}


/***********************
 * NVConvEnhancer
 */

MSGHANDLER cScr_NVConvEnhancer::OnDelayedFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oToFrob = NVStrToObject(pMsg->data, pMsg);
	if  ( !oToFrob )
	{
		DebugPrintf("NVConvEnhancer (Delayed Frob): Invalid object to frob!");
		return 1;
	}

	int iTimer = 0;
	char szDest[80];
	object oDest = 0;

	switch (sscanf(pMsg->data2, "%i; %s", &iTimer, szDest))
	{
	case 2: // Both timer and destination are specified
		oDest = NVStrToObject(szDest, pMsg);
		break;
	case 0: // The first part is NOT an integer. Maybe it's the destination as a string?
		oDest = NVStrToObject(pMsg->data2, pMsg);
	}

	iTimer = static_cast<int>(pMsg->data2);
	SService<IAIScrSrv> pAISrv(g_pScriptManager);
	true_bool bResult;

	if ( !iTimer )
	{
		// Frob the object immediately if the time specified is zero (or no time was specified).
		if ( oDest != 0 )
			pAISrv->MakeFrobObjWith(bResult, m_iObjId, oToFrob, oDest, kNormalPriorityAction, "endaction");
		else
			pAISrv->MakeFrobObj(bResult, m_iObjId, oToFrob, kNormalPriorityAction, "endaction");
		return 0;
	} else {
		char szExtraData[25]; // It is unlikely that anyone will use even HALF of this buffer...
		sprintf(szExtraData, "%i %i", static_cast<int>(oToFrob), static_cast<int>(oDest));

		g_pScriptManager->SetTimedMessage2(m_iObjId, "DelayedFrob", iTimer, kSTM_OneShot, szExtraData);
	}

	MultiParmAssignInt(pReply, 1); // Set the reply to 1, so the pseudo-script can continue.
	return 0;
}

MSGHANDLER cScr_NVConvEnhancer::OnGiveItem(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	object oItem = NVStrToObject(pMsg->data, pMsg);
	object oDest;
	int iTimer = 0;
	char szDest[80];

	switch (sscanf(pMsg->data2, "%i; %s", &iTimer, szDest))
	{
	case 2: // Both timer and destination are specified
		oDest = NVStrToObject(szDest, pMsg);
		break;
	case 0: // The first part is NOT an integer. Maybe it's the destination as a string?
		oDest = NVStrToObject(pMsg->data2, pMsg);
	}

	if ( !oDest )
	{
		oDest = StrToObject("Player");
	}


	/**************************************************
	 * That's strange, this causes an error message ingame.
	 * The IsMetaProperty() and IsArchetype() functions don't
	 * actually seem to work?
	 *
	 * Ah, well, there's always the old 'check if the object
	 * number is negative' method...
	 *

	SService<ITraitManager> pTraitSrv(g_pScriptManager);
	pTraitSrv->IsMetaProperty(0);
	if ( pTraitSrv->IsMetaProperty(static_cast<int>(oItem)) )
	{
		return 1;
	}
	if ( pTraitSrv->IsArchetype(static_cast<int>(oItem)) )
	*/

	if ( iTimer )
	{
		char szExtraData[25]; // It is unlikely that anyone will use even HALF of this buffer...
		sprintf(szExtraData, "%i %i", static_cast<int>(oItem), static_cast<int>(oDest));
		g_pScriptManager->SetTimedMessage2(m_iObjId, "GiveItem", iTimer, kSTM_OneShot, szExtraData);
	} else {
		if ( oItem < 0 )
		{
			SService<IObjectSrv> pOS(g_pScriptManager);
			object oArc = oItem;
			pOS->Create(oItem, oArc);
		}
		#if ( _DARKGAME != 3 )
			SInterface<IContainSys> pContainSys(g_pScriptManager);
			pContainSys->Add(oDest, oItem, 0, 1);
		#else	// System Shock 2 does things a little differently...
			SService<IShockGameSrv> pShockGame(g_pScriptManager);
			pShockGame->AddInvObj(oItem);
			pShockGame->RefreshInv();
		#endif
	}

	MultiParmAssignInt(pReply, 1); // Set the reply to 1, so the pseudo-script can continue.
	return 0;
}

MSGHANDLER cScr_NVConvEnhancer::OnFollow(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oDest = NVStrToObject(pMsg->data, pMsg);

	if ( !oDest )
	{
		oDest = StrToObject("Player");
	}
	m_iFollowing = oDest;

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	int iMaxAlert = ParamGetInt(m_iObjId, "NVConvEnhancerFollowMaxAlertness", 2);

	// Set up an AIWatchObj link to make sure that the AI will stop when it gets too close to its destination.
	IterateLinks("AIWatchObj", m_iObjId, m_iObjId, FollowRemoveWatchObjIterFunc, NULL, NULL);
	link lLink;
	pLS->Create(lLink, pLTS->LinkKindNamed("AIWatchObj"), m_iObjId, oDest);
	if (lLink)
	{
		pLTS->LinkSetData(lLink, "Watch kind", 1); // Self Entry
		#if (_DARKGAME != 3)
			pLTS->LinkSetData(lLink, "Priority", 3); // Normal
		#endif
		pLTS->LinkSetData(lLink, "Trigger: Radius", ParamGetInt(m_iObjId, "NVFollowDistanceStop", 7));
		pLTS->LinkSetData(lLink, "         Height", 8);
		pLTS->LinkSetData(lLink, "         Maximum alertness", iMaxAlert);
		pLTS->LinkSetData(lLink, "Response: Step 1", 8); // Wait(Time(ms),Motion Tags)
		pLTS->LinkSetData(lLink, "   Argument 1", "1000");
		pLTS->LinkSetData(lLink, "   Argument 2", "Stand");
		pLTS->LinkSetData(lLink, "   Argument 3", "ConvEnhancer:Follow (reserved)");
	}

	lLink = 0;
	pLS->Create(lLink, pLTS->LinkKindNamed("AIWatchObj"), m_iObjId, m_iObjId);
	if (lLink)
	{
		pLTS->LinkSetData(lLink, "Watch kind", 1); // Self Entry
		#if (_DARKGAME != 3)
			pLTS->LinkSetData(lLink, "Priority", 3); // Normal
		#endif
		pLTS->LinkSetData(lLink, "Trigger: Radius", 2);
		pLTS->LinkSetData(lLink, "         Height", 8);
		pLTS->LinkSetData(lLink, "         Maximum alertness", iMaxAlert);
		pLTS->LinkSetData(lLink, "      Reuse delay", 2000);
		pLTS->LinkSetData(lLink, "Response: Step 1", 1); // Script message(Message,MsgData1,MsgData2)
		pLTS->LinkSetData(lLink, "   Argument 1", "FollowRecalculate");
		pLTS->LinkSetData(lLink, "   Argument 3", "ConvEnhancer:Follow (reserved)");
	}

	MultiParmAssignInt(pReply, 1);
	return 0;
}

MSGHANDLER cScr_NVConvEnhancer::OnFollowRecalculate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	// Recalculate the idling origin based upon the object-to-follow's position
	cScrVec vDest;
	pOS->Position(vDest, static_cast<int>(m_iFollowing));
	pPropSrv->Set(m_iObjId, "AI_Patrol", NULL, false);
	pPropSrv->Set(m_iObjId, "AI_IdleOrgn", "Original Location", vDest);
	pPropSrv->Set(m_iObjId, "AI_IdlRetOrg", NULL, true);

	// Now, make the AI run if s/he's too far behind...
	cScrVec vPosition, vPosDest;
	pOS->Position(vPosition, m_iObjId);
	pOS->Position(vPosDest, static_cast<int>(m_iFollowing));
	float fDistance = GetDistanceXY(&vPosition, &vPosDest);

	cMultiParm mpTags;
	if (pPropSrv->Possessed(m_iObjId, "MotActorTagList"))
	{
		pPropSrv->Get(mpTags, m_iObjId, "MotActorTagList", "tags");
	} else {
		MultiParmAssignString(&mpTags, "");
	}

	if ( fDistance > ParamGetInt(m_iObjId, "NVFollowDistanceRun", 16) )
	{
		if ( strncmp(mpTags.psz, "LocoUrgent 0, ", 14) )
		{
			char* pszTagList = new char[strlen(mpTags.psz) + 15];
			sprintf(pszTagList, "LocoUrgent 0, %s", mpTags.psz);
			pPropSrv->Set(m_iObjId, "MotActorTagList", "tags", pszTagList);
			delete[] pszTagList;
		}
	} else {
		if ( !strncmp(mpTags.psz, "LocoUrgent 0, ", 14) )
		{
			pPropSrv->Set(m_iObjId, "MotActorTagList", "tags", mpTags.psz +14);
		}
	}

	MultiParmAssignInt(pReply, 1);
	return 0;
}

MSGHANDLER cScr_NVConvEnhancer::OnStopFollowing(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// First, destroy the AIWatchObj link
	IterateLinks("AIWatchObj", m_iObjId, 0, FollowRemoveWatchObjIterFunc, NULL, NULL);

	// Remove AI->Ability Settings->Idle: Returns to Origin
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (pPropSrv->Possessed(m_iObjId, "AI_IdlRetOrg"))
	{
		pPropSrv->Remove(m_iObjId, "AI_IdlRetOrg");
	}
	if (pPropSrv->Possessed(m_iObjId, "AI_IdleOrgn"))
	{
		pPropSrv->Remove(m_iObjId, "AI_IdleOrgn");
	}

	// Reset m_iFollowing to zero.
	m_iFollowing = 0;

	// Return, allowing the pseudo-script to continue.
	MultiParmAssignInt(pReply, 1);
	return 0;
}

int cScr_NVConvEnhancer::FollowRemoveWatchObjIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	cMultiParm mpReturn;
	pLTS->LinkGetData(mpReturn, pLQ->ID(), "   Argument 3");
	if ( !strcmp(mpReturn, "ConvEnhancer:Follow (reserved)") ) // NOT case insensitive.
	{
		pLS->Destroy(pLQ->ID());
	}

	return 1;
}

MSGHANDLER cScr_NVConvEnhancer::OnSendMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// This script never aborts the pseudo-script, even if it fails, so this can go here.
	MultiParmAssignInt(pReply, 1); // Set the reply to 1, so the pseudo-script can continue.

	int iTimer = 0;
	char szDest[62]; // In truth, anything longer than 62 characters will crash DromEd if entered into any conversation action.

	switch (sscanf(pMsg->data2, "%i; %s", &iTimer, szDest))
	{
	case 0: // The first part is NOT an integer. Maybe it's the message as a string?
		strncpy(szDest, pMsg->data2, 62);
	}

	object oDest = NVStrToObject(szDest, pMsg);
	if ( !oDest )
		return 1;

	if ( iTimer)
	{
		char szExtraData[74]; // Allows for incredibly huge integers...
		sprintf(szExtraData, "%i;%s", static_cast<int>(oDest), static_cast<const char*>(pMsg->data));
		g_pScriptManager->SetTimedMessage2(m_iObjId, "SendMessage", iTimer, kSTM_OneShot, szExtraData);
	} else {
		SimplePost(m_iObjId, oDest, pMsg->data);
	}

	return 0;
}

MSGHANDLER cScr_NVConvEnhancer::OnSetQVar(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// This script never aborts the pseudo-script, even if it fails, so this can go here.
	MultiParmAssignInt(pReply, 1); // Set the reply to 1, so the pseudo-script can continue.


	//
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	char* pszQVar = NULL;
	int iValue = 0;
	char cOperand = 0;
	int iValueLength = 0;

	bool retval = GetQVarInfo(pMsg->data.psz, &cOperand, &iValue, &pszQVar, &iValueLength);

	if ( !retval || !pszQVar )
		return 1;

	int iCurrent = 0;

	int iDatabase = static_cast<int>(pMsg->data2);

	if ( pQSrv->Exists(pszQVar) )
	{
		iCurrent = pQSrv->Get(pszQVar); // Get QVar value
	} else
	{
		DisplayPrintf("Oi?");
		pQSrv->Set(pszQVar, 0, iDatabase);
	}


	NVMathsOperations* m = NVMathsOperations::getInstance();
	int iResult = m->calculate(cOperand, iCurrent, iValue);

	pQSrv->Set(pszQVar, iResult, iDatabase);

	if (pszQVar)
		delete[] pszQVar;
	return 0;
}

#if ( _DARKGAME != 3 )
MSGHANDLER cScr_NVConvEnhancer::OnCheckLock(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Get door object to check
	object oDoor = NVStrToObject(pMsg->data, pMsg);

	// Check if the door is locked
	SService<ILockSrv> pLock(g_pScriptManager);
	if ( !pLock->IsLocked(oDoor) )
	{
		MultiParmAssignInt(pReply, 0); // Set the reply to 0, so the pseudo-script will abort.
		return 0;
	}

	// Check AI for keys
	SService<IKeySrv> pKeySrv(g_pScriptManager);

	// Begin iteration of AI's possessions
	SInterface<IContainSys> pContainSys(g_pScriptManager);
	sContainIter* pContainIter;
	pContainIter = pContainSys->IterStart(m_iObjId);
	while (pContainIter )
	{
		// Try this object on the door
		if ( pKeySrv->TryToUseKey(pContainIter->Object, oDoor, kKeyUseCheck) )
		{
			MultiParmAssignInt(pReply, 0); // Set the reply to 0, so the pseudo-script will abort.
			pContainSys->IterEnd(pContainIter); // End iteration of possessions
			return 0;
		}

		if ( !pContainSys->IterNext(pContainIter) )
			break; // This was the last item, end loop
	}
	pContainSys->IterEnd(pContainIter); // End iteration of possessions


	// Door cannot be opened, allow the pseudo-script to continue
	MultiParmAssignInt(pReply, 1); // Set the reply to 1, so the pseudo-script can continue.
	return 0;
}
#endif

MSGHANDLER cScr_NVConvEnhancer::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "DelayedFrob"))
	{
		int iFrob, iFrobWith;
		if (2 == sscanf(pMsg->data, "%i %i", &iFrob, &iFrobWith) )
		{
			//object oToFrob = static_cast<int>(pMsg->data);

			SService<IAIScrSrv> pAISrv(g_pScriptManager);
			true_bool bResult;
			if ( iFrobWith )
				pAISrv->MakeFrobObjWith(bResult, m_iObjId, iFrob, iFrobWith, kNormalPriorityAction, "endaction");
			else
				pAISrv->MakeFrobObj(bResult, m_iObjId, iFrob, kNormalPriorityAction, "endaction");
		}
		return 0;
	}
	if (!stricmp(pszMsgName, "GiveItem"))
	{
		int iItem, iDest;
		if (2 == sscanf(pMsg->data, "%i %i", &iItem, &iDest) )
		{
			object oItem = iItem;
			if ( oItem < 0 )
			{
				SService<IObjectSrv> pOS(g_pScriptManager);
				object oArc = oItem;
				pOS->Create(oItem, oArc);
			}
			#if ( _DARKGAME != 3 )
				SInterface<IContainSys> pContainSys(g_pScriptManager);
				pContainSys->Add(iDest, oItem, 0, 1);
			#else	// System Shock 2 does things a little differently...
				SService<IShockGameSrv> pShockGame(g_pScriptManager);
				pShockGame->AddInvObj(oItem);
				pShockGame->RefreshInv();
			#endif
		}
		return 0;
	}
	if (!stricmp(pszMsgName, "SendMessage"))
	{
		int iDest;
		char szMsg[80];
		if (2 == sscanf(pMsg->data, "%i;%s", &iDest, szMsg) )
		{
			SimpleSend(m_iObjId, iDest, szMsg);
		}
		return 0;
	}
	return 0;
}

#if (_DARKGAME == 2)
/***********************
 * NVSuspiciousTrap
 */

MSGHANDLER cScr_NVSuspiciousTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		pPropSrv->Set(*it, "SuspObj", "Is Suspicious", true);
	}

	return 0;
}

MSGHANDLER cScr_NVSuspiciousTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		pPropSrv->Set(*it, "SuspObj", "Is Suspicious", false);
		IterateLinks("~AISuspiciousLink", *it, 0, SuspiciousIterFunc, this, NULL);
	}

	return 0;
}

int cScr_NVSuspiciousTrap::SuspiciousIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	cScr_NVSuspiciousTrap* pSelf = static_cast<cScr_NVSuspiciousTrap*>(pScript);

	sLink sl;
	pLQ->Link(&sl);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	int KeepLinks = 0;
	KeepLinks = ParamGetInt(pSelf->m_iObjId, "NVKeepWatchObjLinks", 0);
	if (!KeepLinks)
	{
		IterateLinks("~AIWatchObj", static_cast<int>(sl.source), static_cast<int>(sl.dest), WatchObjIterFunc, NULL, NULL);
	}

	pLS->Destroy(pLQ->ID());

	return 1; // We need to return 1 or the loop will be broken.
}


int cScr_NVSuspiciousTrap::WatchObjIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	pLS->Destroy(pLQ->ID());

  return 1;
}
#endif // Not availiable outside Thief2

/***********************
 * NVHPTrigger
 */

MSGHANDLER cScr_NVHPTrigger::OnDamage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (pPropSrv->Possessed(m_iObjId, "HitPoints"))
	{
		cMultiParm mpHealth;
		pPropSrv->Get(mpHealth, m_iObjId, "HitPoints", NULL);
		int iHP = ParamGetInt(m_iObjId, "NVHitPoints", 0);
		int iHealth = static_cast<int>(mpHealth);

		if ( iHealth <= iHP )
		{
			if ( !m_iState )
			{
				m_iState = 1;
				Trigger(true, pMsg);
			}
		} else {
			if ( m_iState )
			{
				m_iState = 0;
				Trigger(false, pMsg);
			}
		}
	}
	return 0;
}

/***********************
 * NVRelayTrap
 */

MSGHANDLER cScr_NVRelayTrap::Relay(bool bOn, sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_iRepeat )
	{
		if ( bOn != (m_bRepeatingOn!=0) )
		{
			m_iRepeat = 0;
			KillTimer();
		}
		return 0;
	}
	int iTimer = GetInterval(bOn);
	if ( iTimer > 0 )
	{
		char* pszParam = new char[strlen(cScrNVGeneric::m_szName) + 16];

		sprintf(pszParam, "%sRepeat", cScrNVGeneric::m_szName);
		m_iRepeat = ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayRepeat", 0));
		sprintf(pszParam, "%sRepeatOnly", cScrNVGeneric::m_szName);
		if ( ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRepeatOnly", 3)) == bOn+1 )
		{
			delete[] pszParam;
			return 0;
		}

		sprintf(pszParam, "%sExclusiveDelay", cScrNVGeneric::m_szName);
		if ( m_iRepeat == -1 || ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVExclusiveDelay", 0)) )
		{
			KillTimer();
		}

		char szExtraData[12]; // It is unlikely that anyone will even use HALF of this buffer...
		sprintf(szExtraData, "%i %i", static_cast<int>(bOn), static_cast<int>(pMsg->from));

		if ( m_iRepeat)
		{
			m_bRepeatingOn = bOn;
		}

		sprintf(pszParam, "%sDelay", cScrNVGeneric::m_szName);
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, pszParam, iTimer, kSTM_OneShot, szExtraData);
		delete[] pszParam;
	} else {
		TriggerRelayTrap(bOn, pMsg);
	}
	return 0;
}

void cScr_NVRelayTrap::KillTimer(void)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return;
}

MSGHANDLER cScr_NVRelayTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	char* pszParam = new char[strlen(cScrNVGeneric::m_szName) + 16];
	sprintf(pszParam, "%sHaltTimerOnOn", cScrNVGeneric::m_szName);

	if ( ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayHaltTimerOnOn", 0)) )
		KillTimer();

	delete[] pszParam;
	return Relay(true, pMsg, pReply, eTrace);
}

MSGHANDLER cScr_NVRelayTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	char* pszParam = new char[strlen(cScrNVGeneric::m_szName) + 16];
	sprintf(pszParam, "%sHaltTimerOnOff", cScrNVGeneric::m_szName);

	if ( ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayHaltTimerOnOff", 0)) )
		KillTimer();

	delete[] pszParam;
	return Relay(false, pMsg, pReply, eTrace);
}

MSGHANDLER cScr_NVRelayTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);

	char* pszParam = new char[strlen(cScrNVGeneric::m_szName) + 16];
	sprintf(pszParam, "%sDelay", cScrNVGeneric::m_szName);
	if (!stricmp(pszMsgName, pszParam))
	{
		int bOn, iFrom;
		if (2 == sscanf(pMsg->data, "%i %i", &bOn, &iFrom) )
		{
			sScrMsg msg;
				msg.from = iFrom;
				msg.to = pMsg->to;
				msg.time = pMsg->time;
				msg.flags = 8;
				msg.message = pMsg->message;
			TriggerRelayTrap((bOn != 0), &msg);
		}

		if ( m_iRepeat)
		{
			if ( m_iRepeat > 0 )
			{
				if ( --m_iRepeat == 0 )
				{
					delete[] pszParam;
					return 0;
				}
			}

			int iTimer = GetInterval(bOn != 0);
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, pszParam, iTimer, kSTM_OneShot, pMsg->data);
		}
	}

	delete[] pszParam;
	return 0;
}

void cScr_NVRelayTrap::TriggerRelayTrap(bool bOn, sScrMsg* pMsg)
{
	++overflowWatch;

	if ( overflowWatch > 32 )
	{
		// Potential infinite loop - let's break out of it

		DisplayPrintf(0x0000FF, "NVScript Error: Potential infinite loop detected and aborted in %s on %s (%i)!\nCheck that the script is not set to send its own triggering message to itself.", m_szName, static_cast<const char*>(GetObjectDescriptiveName(m_iObjId)), m_iObjId);
		DebugPrintf("NVScript Error: Potential infinite loop detected and aborted in %s on %s (%i)!\nCheck that the script is not set to send its own triggering message to itself.", m_szName, static_cast<const char*>(GetObjectDescriptiveName(m_iObjId)), m_iObjId);
	} else {
		Trigger(bOn, pMsg);
	}
	overflowWatch = 0;
}

int cScr_NVRelayTrap::GetInterval(bool bOn)
{
	char* pszParam = new char[strlen(cScrNVGeneric::m_szName) + 16];

	int iTimer = 0;
	if (!bOn)
	{
		sprintf(pszParam, "%sOffDelay", cScrNVGeneric::m_szName);
		iTimer = ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayOffDelay", 0));
		if (!iTimer)
		{
			sprintf(pszParam, "%sDelay", cScrNVGeneric::m_szName);
			iTimer = ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayDelay", 0));
		}
	} else {
		sprintf(pszParam, "%sOnDelay", cScrNVGeneric::m_szName);
		iTimer = ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayOnDelay", 0));
		if (!iTimer)
		{
			sprintf(pszParam, "%sDelay", cScrNVGeneric::m_szName);
			iTimer = ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayDelay", 0));
		}
	}
	if ( iTimer )
	{
		int iMaxTime = 0;

		if (!bOn)
		{
			sprintf(pszParam, "%sOffDelayMax", cScrNVGeneric::m_szName);
			iMaxTime = ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayOffDelayMax", 0));
			if (!iMaxTime)
			{

				iMaxTime = NVParamGetInt(m_iObjId, m_szName, "DelayMax", 0);
				if (!iMaxTime)
				{
					iMaxTime = ParamGetInt(m_iObjId, "NVRelayDelayMax", 0);
				}
			}
		} else {
			sprintf(pszParam, "%sOnDelayMax", cScrNVGeneric::m_szName);
			iMaxTime = ParamGetInt(m_iObjId, pszParam, ParamGetInt(m_iObjId, "NVRelayOnDelayMax", 0));
			if (!iMaxTime)
			{
				iMaxTime = NVParamGetInt(m_iObjId, m_szName, "DelayMax", 0);
				if (!iMaxTime)
				{
					iMaxTime = ParamGetInt(m_iObjId, "NVRelayDelayMax", 0);
				}
			}
		}

		if ( iMaxTime > iTimer )
		{
			SService<IDataSrv> pDS(g_pScriptManager);
			iTimer = iTimer + pDS->RandInt(0, iMaxTime - iTimer);
		}
	}

	delete[] pszParam;
	return iTimer ;
}

/***********************
 * NVTweqMessages
 */


MSGHANDLER cScr_NVTweqMessages::OnTweqComplete(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sTweqMsg* pTweqMsg = static_cast<sTweqMsg*>(pMsg);

	char szMessage[32];
	char szTweqType[8];
	char szTweqDirection[8];

	switch ( pTweqMsg->Type )
	{
	case kTweqTypeScale:
		strcpy(szTweqType, "Scale");
		break;
	case kTweqTypeRotate:
		strcpy(szTweqType, "Rotate");
		break;
	case kTweqTypeJoints:
		strcpy(szTweqType, "Joints");
		break;
	case kTweqTypeModels:
		strcpy(szTweqType, "Models");
		break;
	case kTweqTypeDelete:
		strcpy(szTweqType, "Delete");
		break;
	case kTweqTypeEmitter:
		strcpy(szTweqType, "Emitter");
		break;
	case kTweqTypeFlicker:
		strcpy(szTweqType, "Flicker");
		break;
	case kTweqTypeLock:
		strcpy(szTweqType, "Lock");
		break;
	case kTweqTypeAll:
		strcpy(szTweqType, "All");
		break;
	default:
		strcpy(szTweqType, "Null");
	}

	if ( pTweqMsg->Dir == kTweqDirReverse )
	{
		strcpy(szTweqDirection, "Reverse");
	} else {
		strcpy(szTweqDirection, "Forward");
	}

	sprintf(szMessage, "Tweq%sComplete%s", szTweqType, szTweqDirection);


	sScrMsg msg;
		msg.from = pMsg->from;
		msg.to = pMsg->to;
		msg.time = pMsg->time;
		msg.flags = 8;
		msg.message = szMessage;

	return g_pScriptManager->SendMessage(&msg, pReply);
}

/***********************
 * NVVOTrap
 */

MSGHANDLER cScr_NVVOTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iPolite = 0;
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	sLink slVOLink;

	iPolite = ParamGetInt(m_iObjId, "NVPoliteVO", 0);

	if ( iPolite )
	{ // Code stolen from tnhScript v2.00
		true_bool bInvested;
		pLS->AnyExist(bInvested, pLTS->LinkKindNamed("~AIInvest"), StrToObject("Player"), 0);
		if (bInvested)
			return 1;
	} // End of stolen code

	const char* pszQVar = ParamGetString(m_iObjId, "NVVOQVar", NULL);

	if ( pszQVar)
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);
		int iCurrent = 0; // Default value (if QVar doesn't exist)
		if (pQSrv->Exists(pszQVar))
		{
			iCurrent = pQSrv->Get(pszQVar); // Get QVar value

			char szLinkData[32];
			sprintf(szLinkData, "%i", iCurrent);

			if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slVOLink, szLinkData, -1))
			{
				pSoundSrv->PlayVoiceOver(bReturn, m_iObjId, slVOLink.dest);
				return 0;
			}
		}
	}

	link lVOLink;
	pLS->GetOne(lVOLink, pLTS->LinkKindNamed("SoundDescription"), m_iObjId, 0);
	if ( lVOLink)
	{
		pLTS->LinkGet(lVOLink, slVOLink);
		pSoundSrv->PlayVoiceOver(bReturn, m_iObjId, slVOLink.dest);
	}

	return 0;
}

MSGHANDLER cScr_NVVOTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVOnscreenText
 */
void cScr_NVOnscreenText::ShowText()
{
	SService<IDataSrv> pDataSrv(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	cMultiParm mpBook;
	cScrStr strPage0, strPage1, strQVar;
	const char* pszPage0; const char*	pszPage1; const char* pszQVar;
	int iCurrent = 0; // Default value (if QVar doesn't exist)

	if (pPropSrv->Possessed(m_iObjId, "Book"))
	{ // Load value from Book property.
		pPropSrv->Get(mpBook, m_iObjId, "Book", NULL);

		char* pszBookFile = new char[strlen(mpBook.psz) + 10];
		strcpy(pszBookFile, "..\\books\\");
		strcat(pszBookFile, mpBook);

		pDataSrv->GetString(strPage0, pszBookFile, "page_0", "", "Strings");
		pDataSrv->GetString(strPage1, pszBookFile, "page_1", "", "Strings");
		pDataSrv->GetString(strQVar, pszBookFile, "QVar", "", "Strings");
		delete[] pszBookFile;


		pszPage0 = static_cast<const char*>(strPage0);
		pszPage1 = static_cast<const char*>(strPage1);
		pszQVar = static_cast<const char*>(strQVar);
	} else { // Book property unspecified? Then use Design Note Params.
		pszPage0 = ParamGetString(m_iObjId, "NVPage_0", NULL);
		if (!pszPage0) {
			pszPage0 = ParamGetString(m_iObjId, "NVText", "No text specified.");
		}
		pszPage1 = ParamGetString(m_iObjId, "NVPage_1", "");
		pszQVar = ParamGetString(m_iObjId, "NVQVar", NULL);

	}

	bool bValidQVar = true;
	if (pszQVar)
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);

		if (pQSrv->Exists(pszQVar))
		{
			iCurrent = pQSrv->Get(pszQVar); // Get QVar value
		} else {
			bValidQVar = false;
		}
	} else {
		bValidQVar = false;
	}

	if (!strQVar.IsEmpty())
	{
		strQVar.Free();
	}

	char *szText = new char[strlen(pszPage0) + strlen(pszPage1) + 10];
	if ( bValidQVar )
	{ // Display the text onscreen
		sprintf(szText, "%s%i%s", pszPage0, iCurrent, pszPage1);
	} else { // Don't display the QVar if it doesn't exist
		sprintf(szText, "%s%s", pszPage0, pszPage1);
	}

	int iColour = strtocolor(ParamGetString(m_iObjId, "NVOnscreenTextColour", "#FFFFFF"));

	if ( ParamGetInt(m_iObjId, "NVOnscreenTextFocusRead", 1 ) )
		ShowString(szText, CalcTextTime(szText, 500), iColour);
	else
		ShowString(szText, 1000000000, iColour);

	delete[] szText;

	// Free memory.
	if (!strPage1.IsEmpty())
		strPage1.Free();

	if (!strPage0.IsEmpty())
		strPage0.Free();

	return;
}

void cScr_NVOnscreenText::ClearText()
{
	DisplayPrintf(" ");
}

MSGHANDLER cScr_NVOnscreenText::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	ShowText();
	return 0;
}

MSGHANDLER cScr_NVOnscreenText::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	ClearText();
	return 0;
}

MSGHANDLER cScr_NVOnscreenText::OnFocus(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( ParamGetInt(m_iObjId, "NVOnscreenTextFocusRead", 1 ) )
	{
		ShowText();
	}
	return 0;
}

MSGHANDLER cScr_NVOnscreenText::OnDefocus(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	ClearText();
	return 0;
}

/***********************
 * NVExclusiveObject
 */

MSGHANDLER cScr_NVExclusiveObject::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		ParamSetInt(m_iObjId, "CreationTime", pMsg->time);
		Startup();
	}
	return 0;
}

MSGHANDLER cScr_NVExclusiveObject::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		ParamSetInt(m_iObjId, "CreationTime", pMsg->time);
		Startup();
	}

	return 0;
}

void cScr_NVExclusiveObject::Startup()
{
	SInterface<ITraitManager> pTraits(g_pScriptManager);
	int iArchetype = pTraits->GetArchetype(m_iObjId); // Make a note of object's archetype.
	IObjectQuery* pInheritance = pTraits->Query(iArchetype, kTraitQueryChildren);
	if (!pInheritance)
	{
		return;
	}

	SInterface<IObjectSystem> pOS(g_pScriptManager);
	SService<IDamageSrv> pDmgS(g_pScriptManager);

	std::multimap<int, int> s;
	int iChild;
	while (! pInheritance->Done())
	{
		iChild = pInheritance->Object();
		if ( iChild > 0)
		{
			if (iChild != m_iObjId)
			{
				s.insert(std::pair<int, int>(-ParamGetInt(iChild, "CreationTime", 0), iChild));
//				pOS->Destroy(iChild);
			}
		}

		pInheritance->Next();
	}
	pInheritance->Release();

	if ( s.empty() )
		return;

	int iMax = ParamGetInt(m_iObjId, "NVExclusiveObjectCount", 1);

	std::multimap<int, int>::iterator p;
	int i = 2;
	for (p = s.begin(); p != s.end() ; ++p, i++)
	{
		if ( i > (iMax) )
		{
			if ( ParamGetInt(m_iObjId, "NVExclusiveObjectSlay", 0) )
				pDmgS->Slay(p->second, m_iObjId);
			else
				pOS->Destroy(p->second);
		}
	}

//	DisplayPrintf("Current object count: %i", i-1);



//	SInterface<ITraitManager> pTraits(g_pScriptManager);
//	int iArchetype = pTraits->GetArchetype(m_iObjId); // Make a note of object's archetype.


	/************************************************************************************
	 * You would think that there would be a more efficient way to affect every object that inherits
	 * from a certain archetype, but if there is I cannot see it.
	 */

	// SInterface<IObjectSystem> pOS(g_pScriptManager);
	// int iMaxObjects = pOS->MaxObjID();
	// for (int i=0; i < iMaxObjects; i++)
	// { // Loop through every possible object ID and test if it inherits from m_iObjId's archetype.
		// if ( pOS->Exists(i) )
		// { // Don't do anything if this object ID is unoccupied
			// if ( iArchetype == pTraits->GetArchetype(i) )
			// { // Check if this object ID has the same archetype.
				// if (i != m_iObjId )
				// { // Make sure not to destroy m_iObjId.
					// pOS->Destroy(i);
				// }
			// }
		// }
	// }

	return;
}

/***********************
 * NVAttachMyObj
 */

MSGHANDLER cScr_NVAttachMyObj::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		Startup();
	}
	return 0;
}

MSGHANDLER cScr_NVAttachMyObj::OnEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( ParamGetInt(m_iObjId, "NVAttachMyObjAndSlayItToo", 0) )
	{
		sLink sl;
		if (!GetOneLinkByData("ScriptParams", m_iObjId, 0, &sl, "AttachedMyObj", -1))
		{
			return 0;
		}
		SInterface<IObjectSystem> pOS(g_pScriptManager);
		SService<IDamageSrv> pDmgS(g_pScriptManager);
		pDmgS->Slay(sl.dest, m_iObjId);
	}

	return 0;
}

MSGHANDLER cScr_NVAttachMyObj::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		Startup();
	}
	return 0;
}

void cScr_NVAttachMyObj::Startup()
{
	int iObjId;
	sLink slLink;
	if (!GetOneLinkByDataInheritedSrc("ScriptParams", m_iObjId, 0, &slLink, "Attach", -1))
	{
		return;
	}
	iObjId = slLink.dest;
	object oCreated;

	SService<IObjectSrv> pOS(g_pScriptManager);
	pOS->Create(oCreated, iObjId);

	if (oCreated)
	{
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		link lLink;
		int iAttachMyObjAsParticle = ParamGetInt(m_iObjId, "NVAttachMyObjAsParticle", 0);
		if (iAttachMyObjAsParticle)
		{
			pLS->Create(lLink, pLTS->LinkKindNamed("ParticleAttachement"), oCreated, m_iObjId);
			if (lLink)
			{
				pLTS->LinkSetData(lLink, "Type", 2);
				pLTS->LinkSetData(lLink, "Joint", ParamGetInt(m_iObjId, "NVAttachMyObjJoint", 0));
			}
		}
		if (iAttachMyObjAsParticle != 1)
		{
			pLS->Create(lLink, pLTS->LinkKindNamed("CreatureAttachment"), m_iObjId, oCreated);
			if (lLink)
			{
				pLTS->LinkSetData(lLink, "Joint", ParamGetInt(m_iObjId, "NVAttachMyObjJoint", 0));
			}
		}

		link lAMOLink;
		pLS->Create(lAMOLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oCreated);
		if ( lAMOLink )
		{
			pLTS->LinkSetData(lAMOLink, NULL, "AttachedMyObj");
		}
	}

	return;
}

/***********************
 * NVSafeDoor
 */


MSGHANDLER cScr_NVSafeDoor::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		Startup();
	}
	return 0;
}

MSGHANDLER cScr_NVSafeDoor::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		Startup();
	}

	return 0;
}

void cScr_NVSafeDoor::Startup()
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	const char* pszQVar = ParamGetString(m_iObjId, "NVSafeQVar", "NVSafeDoor");
	if (!pQSrv->Exists(pszQVar))
	{
		SService<IDataSrv> pDS(g_pScriptManager);
		char szCode[5];
		szCode[0] = pDS->RandInt(48, 57); // The character will be between 0 and 9.
		szCode[4] = '\0';
		for ( int i = 1; i < 4; i++ )
		{
			do
			{
				szCode[i] = pDS->RandInt(48, 57); // The character will be between 0 and 9.
			} while ( szCode[i] == szCode[i-1] );
		}
		int iNewCode = atoi(szCode);
		const char* pszQVar = ParamGetString(m_iObjId, "NVSafeQVar", "NVSafeDoor");
		pQSrv->Set(pszQVar, iNewCode, 0);
	}

	return;
}

MSGHANDLER cScr_NVSafeDoor::OnFrobWorldBegin(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	if ( pDoorSrv->GetDoorState(m_iObjId) ) // Door state is not 'Closed'
	{
		m_iCodePosition = 0;
		return 0;
	}

	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	int iAnimS = 0;
	if (pPropSrv->Possessed(m_iObjId, "StTweqJoints") )
	{
		cMultiParm mpAnimS;
		pPropSrv->Get(mpAnimS, m_iObjId, "StTweqJoints", "AnimS");
		iAnimS = static_cast<int>(mpAnimS);
	}

	pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", (1 | iAnimS)); // Activate Joints
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	object oSchema = StrToObject("Safe_Loop");
	if (oSchema)
	{
		pSoundSrv->PlaySchemaAtObject(bReturn, m_iObjId, oSchema, m_iObjId SOUND_NET);
	}

	int iFrobber = pFrobMsg->Frobber;
	if ( iFrobber != StrToObject("Player") )
	{
		if (m_hTimerHandle == NULL)
		{
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "SafeDoor", 250, kSTM_Periodic, iFrobber);
		}
		pPropSrv->Set(m_iObjId, "FrobInfo", "World Action", 0); // Make FrobInert
	}
	return 0;
}

MSGHANDLER cScr_NVSafeDoor::OnFrobWorldEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->Set(m_iObjId, "FrobInfo", "World Action", 18); // Make Frobbable

	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	if ( pDoorSrv->GetDoorState(m_iObjId) ) // Door state is not 'Closed'
	{
		m_iCodePosition = 0;
		return 0;
	}

	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	object oFrobber = pFrobMsg->Frobber;
	object oPlayer = StrToObject("Player");
	if ( oFrobber != oPlayer )
	{
		if (1 != static_cast<int>(pFrobMsg->data))
		{
			return 0;
		} else {
			IterateLinks("AIWatchObj", oFrobber, m_iObjId, DestroyLinksIterFunc, NULL, NULL);
		}
	}

	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	object oSchema;
	pSoundSrv->HaltSchema(bReturn, m_iObjId, "", 0);

	if (pPropSrv->Possessed(m_iObjId, "StTweqJoints"))
	{
		cMultiParm mpAnimS;
		pPropSrv->Get(mpAnimS, m_iObjId, "StTweqJoints", "AnimS");
		int iAnimS = static_cast<int>(mpAnimS);
		if ( iAnimS & 1 ) // AnimS is 'On'
		{

			pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", (30 & (2 ^ iAnimS))); // Deactivate Joints

			oSchema = StrToObject("pinset");
			if (oSchema)
			{
				pSoundSrv->PlaySchemaAtObject(bReturn, m_iObjId, oSchema, m_iObjId SOUND_NET);
			}
			int iCorrect = IsCorrect(); // Returns 0 if false, and the length of the code if true.
			if ( iCorrect )
			{
				object oLinkTo = 0;
				if ( m_iCodePosition >= (iCorrect -1) ) // Code starts at zero.
				{
					m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "OpenSafeDoor", 250, kSTM_OneShot, g_mpUndef);
					const char* pszLinkSource = ParamGetString(m_iObjId, "NVSafeOpenAction", NULL);
					oLinkTo = NVStrToObject(pszLinkSource, oFrobber, m_iObjId) ;
					m_iCodePosition = 0;
					return 0;
				} else {
					m_iCodePosition++;
					DebugPrintf("Code position is now %i", (int)m_iCodePosition);
					oLinkTo = m_iObjId;
				}

				if ( oFrobber != oPlayer )
				{
					SService<ILinkSrv> pLS(g_pScriptManager);
					SService<ILinkToolsSrv> pLTS(g_pScriptManager);
					link lLink;
					pLS->Create(lLink, pLTS->LinkKindNamed("AIWatchObj"), oFrobber, oLinkTo);
				}
			} else {
				m_iCodePosition = 0;
			}
		}
	}

	return 0;
}

int cScr_NVSafeDoor::IsCorrect(bool bSnapJoint)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	int iJointPos = 0;
	if (pPropSrv->Possessed(m_iObjId, "JointPos"))
	{
		cMultiParm mpJointPos;
		pPropSrv->Get(mpJointPos, m_iObjId, "JointPos", "Joint 1");
		float fJointPos = (float)((360 + ((int)(mpJointPos) % 360)) % 360);
		iJointPos = (((int)(0.50f + (fJointPos / 36))) % 10);
	}
	if ( bSnapJoint )
	{
		pPropSrv->Set(m_iObjId, "JointPos", "Joint 1", iJointPos*36); // Snap joint to nearest position.
	}


	SService<IQuestSrv> pQSrv(g_pScriptManager);
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	const char* pszQVar = ParamGetString(m_iObjId, "NVSafeQVar", "NVSafeDoor");
	if (pQSrv->Exists(pszQVar))
	{
		iCurrent = pQSrv->Get(pszQVar); // Get QVar value
	}

	char szCode[9]; // There is, in fact, no reason why the combination can't be longer.
	snprintf(szCode, 8, "%.*d", NVParamGetInt(m_iObjId, m_szName, "CodeLength", 4), iCurrent);
	int iNextPos = static_cast<int>(szCode[m_iCodePosition]) - 48;

	DebugPrintf("NVSafeDoor: Desired position: %i, you entered: %i", iNextPos, iJointPos);

	if ( iJointPos == iNextPos )
	{
		return static_cast<int>(strlen(szCode));
	} else {
		return 0;
	}
}

MSGHANDLER cScr_NVSafeDoor::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "SafeDoor"))
	{
		object oFrobber = static_cast<int>(pMsg->data);
		object oPlayer = StrToObject("Player");
		if ( oFrobber != oPlayer )
		{
			if ( IsCorrect(false) )
			{
				sFrobMsg FrobMsg;
					FrobMsg.from = pMsg->from;
					FrobMsg.to = m_iObjId;
					FrobMsg.time = pMsg->time;
					FrobMsg.flags = pMsg->flags; // No one knows what this means.
					FrobMsg.message = "FrobWorldEnd";
					FrobMsg.Frobber = oFrobber; // An unusual hack...
					FrobMsg.data = 1;
				OnFrobWorldEnd(&FrobMsg, pReply, eTrace);
			}
		}
		return 0;
	}
	if (!stricmp(pszMsgName, "OpenSafeDoor"))
	{
		SimpleSend(m_iObjId, m_iObjId, "TurnOn");
		return 0;
	}

	return 0;
}

int cScr_NVSafeDoor::DestroyLinksIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	pLS->Destroy(pLQ->ID());
	return 1;
}

#if (_DARKGAME != 3) // SS2 has a different inventory system, AND it remembers the selected items on save anyway.
/***********************
 * NVInventoryMemory
 */

MSGHANDLER cScr_NVInventoryMemory::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( pMsg->time > 0 )
	{
		g_pScriptManager->SetTimedMessage2(m_iObjId, "InventoryMemory", 10, kSTM_OneShot, g_mpUndef);
	}
	return 0;
}

MSGHANDLER cScr_NVInventoryMemory::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( pMsg->time > 0 )
	{
		SService<IDarkUISrv> pUI(g_pScriptManager);
		object oWeapon;
		object oItem;
		pUI->InvItem(oItem);
		pUI->InvWeapon(oWeapon);
		m_iCurrWeapon = oWeapon;
		m_iCurrItem = oItem;
	}
	return 0;
}
MSGHANDLER cScr_NVInventoryMemory::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "InventoryMemory"))
	{ // Abort if it's not the right timer.
		SService<IDarkUISrv> pUI(g_pScriptManager);
		if ( m_iCurrWeapon )
		{
			pUI->InvSelect(static_cast<int>(m_iCurrWeapon));
			m_iCurrWeapon = 0;
		}
		if ( m_iCurrItem )
		{
			pUI->InvSelect(static_cast<int>(m_iCurrItem));
			m_iCurrItem = 0;
		}
	}

	return 0;
}
#endif

/***********************
 * NVInvTransform
 */
MSGHANDLER cScr_NVInvTransform::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	return Transform(static_cast<int>(pFrobMsg->Frobber));
}

long cScr_NVInvTransform::Transform(int iFrobber)
{
	object oInvItem = ParamGetObject(m_iObjId, "NVInvTransform", 0);
	if ( oInvItem )
	{
		SService<IObjectSrv> pOS(g_pScriptManager);
		object oItem;
		pOS->Create(oItem, oInvItem);
		if ( oItem )
		{
			SService<IPropertySrv> pPropSrv(g_pScriptManager);

			int iStack = ParamGetInt(m_iObjId, "NVInvTransformStack", 0);
			if ( iStack != 1 )
			{
				//Check stack count:
				cMultiParm mpStack = 0;
				if (pPropSrv->Possessed(m_iObjId, "StackCount"))
				{
					pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
					if ( iStack == 0 || static_cast<int>(mpStack) < iStack )
					{
						iStack = static_cast<int>(mpStack);
					}
				}
			}
			pPropSrv->Set(oItem, "StackCount", NULL, iStack);

			SInterface<IContainSys> pContainSys(g_pScriptManager);
			#if (_DARKGAME != 3) // This crashes SS2, as there is no Thief-style inventory management
				SService<IDarkUISrv> pUI(g_pScriptManager);
				object oSelection; // Get currently selected item before giving the transformed object
				pUI->InvItem(oSelection);
				// And this doesn't work properly in SS2...
				pContainSys->Add(iFrobber, oItem, 0, 1);
			#else	// System Shock 2 does things a little differently...
				SService<IShockGameSrv> pShockGame(g_pScriptManager);
				pShockGame->AddInvObj(oItem);
				pShockGame->RefreshInv();
			#endif


			#if (_DARKGAME != 3) // This is not needed for SS2. And it would cause it to crash...
				// Check if the item that was selected before was 'Junk', and, if so, reselect it.
				// This is to stop people from grabbing junk and then having the NVInvTransform stuff the junk
				// into the player's inventory by selecting the transformed object.
				cMultiParm mpInvType;
				pPropSrv->Get(mpInvType, oSelection, "InvType", NULL);
				if ( static_cast<int>(mpInvType) == 0 )
				{
					pPropSrv->Set(oSelection, "InvType", NULL, 1);
					pUI->InvSelect(oSelection);
					pPropSrv->Set(oSelection, "InvType", NULL, 0);
				} else {
					pUI->InvSelect(oItem);
				}
			#endif

			if ( iStack )
			{
				pContainSys->StackAdd(m_iObjId, -iStack, 1);
			} else {
				pOS->Destroy(m_iObjId);
			}
		}
	}
	return 0;
}

/***********************
 * NVFirerHack
 */

MSGHANDLER cScr_NVFirerHack::OnThrow(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		g_pScriptManager->SetTimedMessage2(m_iObjId, "FirerHack", 350, kSTM_OneShot, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVFirerHack::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "FirerHack"))
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (pPropSrv->Possessed(m_iObjId, "Firer"))
		{
			pPropSrv->Remove(m_iObjId, "Firer");
		}
	}
	return 0;
}


//#if (_DARKGAME != 3) // SS2 handles the player arm differently.
/***********************
 * NVNewWeapon
 */
cScr_NVNewWeapon::~cScr_NVNewWeapon()
{
	if ( m_iListenerHandle != -1 )
	{
		SInterface<IObjectSystem> pOS(g_pScriptManager);
		pOS->Unlisten(m_iListenerHandle);
		m_iListenerHandle = -1;
	}
}

void cScr_NVNewWeapon::StartListener(void)
{
	if ( m_iListenerHandle == -1 )
	{
		SInterface<IObjectSystem> pOS(g_pScriptManager);
		sObjListenerDesc desc;
		desc.pfnListenFunc = ObjListener;
		desc.pData = reinterpret_cast<void*>(this);
		m_iListenerHandle = pOS->Listen(&desc);
	}
	return;
}

#if (_DARKGAME == 3)
MSGHANDLER cScr_NVNewWeapon::OnContained(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( !m_bSim )
		return 0;

//	sContainedScrMsg* pConMsg = static_cast<sContainedScrMsg*>(pMsg);

	// Events: 2 = put into inventory or slot, 3 = picked up into cursor
	SService<IShockGameSrv> pShockGame(g_pScriptManager);

	object oEquipped;

	pShockGame->Equipped(oEquipped, 0);
	if ( oEquipped == m_iObjId)
	{
		if ( !m_iEquipped)
		{
			SimplePost(0, m_iObjId, "Equipped");
		}
		m_iEquipped = 1;
	} else {
		if ( m_iEquipped)
		{
			SimplePost(0, m_iObjId, "Unequipped");
		}
		m_iEquipped = 0;
	}

	return 0;
}
#endif

MSGHANDLER cScr_NVNewWeapon::OnInvSelect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	m_iListenFor = 0;
	StartListener();

/**
	// If we want to stop the sword-drawing samples...
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	object oSchema = StrToObject("bjack_begin");
	if (oSchema)
	{
		pPropSrv->Set(oSchema, "SchPlayParams", "Volume", -32000);
	}
	oSchema = StrToObject("bjack_end");
	if ( oSchema)
	{
		pPropSrv->Set(oSchema, "SchPlayParams", "Volume", -32000);
	}

*/
	return 0;
}

MSGHANDLER cScr_NVNewWeapon::OnInvDeselect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	ForcePutAway();
	return 0;
}

void cScr_NVNewWeapon::ForcePutAway(void)
{
	#if (_DARKGAME != 3)
		m_iListenFor = 1;
		StartListener();
	#endif
	return;
}

void __cdecl cScr_NVNewWeapon::ObjListener(int iObj, unsigned long uEvent, void* pData)
{
	// Okay: uEvent is what happened to the object. 0 = Created; 1 = Destroyed.
	// There are others, but they don't matter.
	cScr_NVNewWeapon* pSelf = static_cast<cScr_NVNewWeapon*>(pData);

	SInterface<IObjectSystem> pOS(g_pScriptManager);
	const char* pszName = pOS->GetName(iObj);

	char *pszArmName = "PlyrArm";
	#if (_DARKGAME == 3)
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (!pPropSrv->Possessed(pSelf->m_iObjId, "Melee Type"))
		{
			pszArmName = "Player Gun";
		}

//		DisplayPrintf("%s (%i) was %i", pszName, iObj, static_cast<int>(uEvent));
	#endif

	if (pszName && ! stricmp(pszName, pszArmName) )
	{
		if ( static_cast<int>(uEvent) != pSelf->m_iListenFor )
		{
			return;
		}

		if ( uEvent == 0 )
		{ // Event is 'Create'
			pSelf->InitArm(iObj);
		} else
		if ( uEvent == 1 )
		{		// Event is 'Destroy'
			pSelf->ClearArm(iObj);

			/**
			// If we want to stop the sword-drawing samples...
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			object oSchema = StrToObject("sword_begin");
			if (oSchema)
			{
				pPropSrv->Set(oSchema, "SchPlayParams", "Volume", -1500);
			}
			oSchema = StrToObject("sword_end");
			if (oSchema)
			{
				pPropSrv->Set(oSchema, "SchPlayParams", "Volume", -1500);
			}
			*/

		}
//		pOS->Unlisten(pSelf->m_iListenerHandle);
//		pSelf->m_iListenerHandle = -1;
	}
	return;
}

int cScr_NVNewWeapon::DestroyAttachedIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	sLink sl;
	pLQ->Link(&sl);

	SInterface<IObjectSystem> pOS(g_pScriptManager);
	pOS->Destroy(sl.dest);
	return 1;
}

MSGHANDLER cScr_NVNewWeapon::OnForceInit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	ForceInit();
	return 0;
}

void cScr_NVNewWeapon::ForceInit(void)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oArm;

	char *pszArmName = "PlyrArm";
	#if (_DARKGAME == 3)
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (!pPropSrv->Possessed(m_iObjId, "Melee Type"))
		{
			pszArmName = "Player Gun";
		}
	#endif

	pOS->Named(oArm, pszArmName);
	true_bool bExists;
	pOS->Exists(bExists, oArm);
	if (bExists)
	{
		InitArm(oArm);
	}

	return;
}

void cScr_NVNewWeapon::InitArm(object oArm)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	if (ParamExists(m_iObjId, "NVNewWeaponSpeed") )
	{
		#if (_DARKGAME != 3)
			SService<IDarkInvSrv> pSrv(g_pScriptManager);
		#else
			SService<IShockGameSrv> pSrv(g_pScriptManager);
		#endif
		pSrv->RemoveSpeedControl("SwordEquip");

		float fSwordSpeed = ParamGetFloat(m_iObjId, "NVNewWeaponSpeed", 1.00f);

		if ( fSwordSpeed != 1.00f )
			pSrv->AddSpeedControl("NVNewWeapon", fSwordSpeed, 1.00f);
	}

	link lNWLink;
	pLS->Create(lNWLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oArm);
	if ( lNWLink )
	{
		pLTS->LinkSetData(lNWLink, NULL, "NewWeapon");
	}

	int iMeta = ParamGetObject(m_iObjId, "NVWeaponMeta", 0);
	if ( iMeta )
	{
		AddSingleMetaProperty(iMeta, oArm);
	}

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
/*	const char* pszWeaponModel = ParamGetString(m_iObjId, "NVWeaponModel", NULL);
	if (pszWeaponModel)
	{
		DisplayPrintf("Setting object of %i to %s", static_cast<int>(oArm), pszWeaponModel);
		pPropSrv->Set(oArm, "ModelName", NULL, pszWeaponModel); // Set weapon model
	}
*/

//	And can you guess what this is for?
	char pszObjToAttach[16];
	char pszAttachPoint[16];
	SService<IObjectSrv> pOS(g_pScriptManager);

	for ( int i=1; i < 5; i++ )
		{
		sprintf(pszObjToAttach, "NVWeaponAttach%i", i);
		int iArc = ParamGetObject(m_iObjId, pszObjToAttach, 0);
		if ( iArc)
		{
			object oCreated;
			pOS->Create(oCreated, iArc);

			if (oCreated)
			{
				sprintf(pszAttachPoint, "attached obj %i", i);
				pPropSrv->Set(oArm, "MeshAttach", pszAttachPoint, static_cast<int>(oCreated));
				pPropSrv->Add(oCreated, "Transient"); // These objects should not be stored in a saved game.
				pPropSrv->SetSimple(oCreated, "Transient", 1);
				link lLink;
				pLS->Create(lLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oCreated);
				if (lLink)
				{
					pLTS->LinkSetData(lLink, NULL, "NewWeaponAttach");
				}
			}
		} else {
			break;
		}
	}
	// delete[] pszObjToAttach;
	// delete[] pszAttachPoint;

	SimpleSend(m_iObjId, oArm, "InitArm");
	return;
}

void cScr_NVNewWeapon::ClearArm(object oArm)
{
	#if (_DARKGAME != 3)
		SService<IDarkInvSrv> pSrv(g_pScriptManager);
	#else
		SService<IShockGameSrv> pSrv(g_pScriptManager);
	#endif
	pSrv->RemoveSpeedControl("NVNewWeapon");

	IterateLinksByData("ScriptParams", m_iObjId, 0, "NewWeaponAttach", -1, DestroyAttachedIterFunc, NULL, NULL);
	return;
}
//#endif // Non-SS2

#if (_DARKGAME == 3)
/***********************
 * NVGun
 */

MSGHANDLER cScr_NVGun::OnModify(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	ParamSetInt(m_iObjId, "NVGunCooldownSetting", -1);
//	m_iCooldown = -1;
	FinishCooldown();

	return 0;
}

MSGHANDLER cScr_NVGun::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( ParamExists(m_iObjId, "NVGunCooldownSetting") )
	{
		FinishCooldown();
	}
//	ParamSetInt(m_iObjId, "NVGunCooldownSetting", -1);
//	m_iCooldown = -1;

	StartGunStateListener();

	SService<IShockGameSrv> pShockGame(g_pScriptManager);

	object oEquipped;

	pShockGame->Equipped(oEquipped, 0);
	if ( oEquipped == m_iObjId)
	{
		ForceInit(); // From NVNewWeapon
		StartListener();

		if (m_hTimerHandle == NULL)
		{ // Start a timer.
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGunRecheck", 5000, kSTM_Periodic, g_mpUndef);
		}

		m_iStrength = pShockGame->GetStat(StrToObject("Player"), 0);
	}

	return 0;
}

void cScr_NVGun::InitArm(object oArm)
{
	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	object oWeap;
	pShockGame->PlayerGun(oWeap);
	if (oWeap != m_iObjId)
		return;

	cScr_NVNewWeapon::InitArm(oArm);

	if ( ParamGetInt(m_iObjId, "NVGunInfoOnEquip", 0))
		SimpleSend(0, m_iObjId, "GetGunInfo");
}

MSGHANDLER cScr_NVGun::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	StopGunStateListener();
	return 0;
}

cScr_NVGun::~cScr_NVGun()
{
//	StopListener();
	if ( m_pProp )
	{
		m_pProp->Release();
	}
}

#pragma pack(push,1)
struct sGunState
{
	int iAmmo;
	float fCondition;
	int iSetting,
	iModification;
};
#pragma pack(pop)

void __stdcall cScr_NVGun::ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData)
{

	int iObjId = reinterpret_cast<int>(pData);
	if (sMsg->iObjId == iObjId)
	{ // Do nothing UNLESS this is the object with the script...
		sGunState* pGunState;
		pGunState = static_cast<sGunState*>(sMsg->pData);
		// Post 'SettingChange' message, with the current Setting as data.
		SimplePost(0, iObjId, "GunStateChange", pGunState->iAmmo, pGunState->fCondition, pGunState->iSetting);
	}

	return;
}

MSGHANDLER cScr_NVGun::OnGetGunInfo(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	int iAmmo = 0;
	float fCondition = 0;
	int iSetting = 0;

	cMultiParm mpReturn;
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Ammo");
	iAmmo = static_cast<int>(mpReturn);
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Condition (%)");
	fCondition = static_cast<float>(mpReturn);
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Setting");
	iSetting = static_cast<int>(mpReturn);

	m_iSetting = iSetting;
	char *pszMsg = new char[18];
	sprintf(pszMsg, "Ammo%i", iAmmo);
	SimpleSend(0, m_iObjId, pszMsg);
	sprintf(pszMsg, "Condition%.2f", fCondition);
	SimpleSend(0, m_iObjId, pszMsg);

	SettingChanged(iSetting);

	delete[] pszMsg;

	return 0;
}

MSGHANDLER cScr_NVGun::OnGunStateChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	int iAmmo = 0; //static_cast<int>(pMsg->data);
	float fCondition = 0; //static_cast<float>(pMsg->data2);
	int iSetting = 0; //static_cast<int>(pMsg->data3);

	cMultiParm mpReturn;
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Ammo");
	iAmmo = static_cast<int>(mpReturn);
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Condition (%)");
	fCondition = static_cast<float>(mpReturn);
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Setting");
	iSetting = static_cast<int>(mpReturn);

	object oArm =  0;
	sLink slOLink;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
	{
		oArm = slOLink.dest;
	}

	SInterface<IContainSys> pContainSys(g_pScriptManager);
	if ( pContainSys->IsHeld(StrToObject("Player"), m_iObjId) == MAX_INT)
		return 0; // Don't process if the player isn't holding it.

	if ( static_cast<int>(m_iAmmo) > iAmmo)
	{
		m_iAmmo = iAmmo;
		SimplePost(0, m_iObjId, "AmmoDown", iAmmo);


		if ( ParamGetInt(m_iObjId, "NVGunCooldownHack", 0) )
		{

//			int iState = 0;
//			if (pPropSrv->Possessed(m_iObjId, "ObjState"))
//			{
//				cMultiParm mpReturn;
//				pPropSrv->Get(mpReturn, m_iObjId, "ObjState", NULL);
//				iState = static_cast<int>(mpReturn);
//			}

//			if ( iState == 0 )
//			{
				if (pPropSrv->Possessed(m_iObjId, "BaseGunDesc"))
				{
					int iTimer = 0;
					char szTimer[25];
					cMultiParm mpTime;

					sprintf(szTimer, "Setting %i: Burst", iSetting);
					pPropSrv->Get(mpTime, m_iObjId, "BaseGunDesc", szTimer);
					if ( static_cast<int>(mpTime) == 1 )
					{
						sprintf(szTimer, "Setting %i: Shot Interval", iSetting);
						pPropSrv->Get(mpTime, m_iObjId, "BaseGunDesc", szTimer);
						iTimer = static_cast<int>(mpTime);

						if ( iTimer > 100 )
						{
								ParamSetInt(m_iObjId, "NVGunCooldownSetting", iSetting);
//								m_iCooldown = iSetting;

								pPropSrv->Get(mpTime, m_iObjId, "BaseGunDesc", "Setting 0: Ammo Usage");
								ParamSetInt(m_iObjId, "NVGunCooldownAmmo0", (int)mpTime);
								//m_iGunAmmoUsage0 = (int)mpTime;

								pPropSrv->Get(mpTime, m_iObjId, "BaseGunDesc", "Setting 1: Ammo Usage");
								ParamSetInt(m_iObjId, "NVGunCooldownAmmo1", (int)mpTime);
								//m_iGunAmmoUsage1 = (int)mpTime;

								pPropSrv->Get(mpTime, m_iObjId, "BaseGunDesc", "Setting 2: Ammo Usage");
								ParamSetInt(m_iObjId, "NVGunCooldownAmmo2", (int)mpTime);
								//m_iGunAmmoUsage2 = (int)mpTime;

								pPropSrv->Set(m_iObjId, "BaseGunDesc", "Setting 0: Ammo Usage", 2000000000);
								pPropSrv->Set(m_iObjId, "BaseGunDesc", "Setting 1: Ammo Usage", 2000000000);
								pPropSrv->Set(m_iObjId, "BaseGunDesc", "Setting 2: Ammo Usage", 2000000000);

//							iSetting = 404;
//							pPropSrv->Set(m_iObjId, "GunState", "Setting", iSetting);
//							pPropSrv->Set(m_iObjId, "ObjState", NULL, 4); // Locked
							g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGunCooldown", iTimer, kSTM_OneShot, g_mpUndef);
						}
					}
				}
//			}
		}


	} else if ( static_cast<int>(m_iAmmo) < iAmmo)
	{
		m_iAmmo = iAmmo;
		SimplePost(0, m_iObjId, "AmmoUp", iAmmo);
		if ( oArm )
			SimplePost(0, oArm, "AmmoUp", iAmmo);
	}

	if ( static_cast<float>(m_fCondition) > fCondition)
	{
		m_fCondition = fCondition;
		SimplePost(0, m_iObjId, "ConditionDown", fCondition);
		if ( fCondition == 0.00f )
		{
			SimplePost(0, m_iObjId, "GunBreak", fCondition);
			if ( oArm )
				SimplePost(0, oArm, "GunBreak", fCondition);
		}
	} else if ( static_cast<float>(m_fCondition) < fCondition)
	{
		m_fCondition = fCondition;
		SimplePost(0, m_iObjId, "ConditionUp", fCondition);
		if ( oArm )
			SimplePost(0, oArm, "ConditionUp", fCondition);
	}

	if ( static_cast<int>(m_iSetting) != iSetting)
	{
		m_iSetting = iSetting;
		SimplePost(0, m_iObjId, "SettingChange", iSetting);
		if ( oArm )
			SimplePost(0, oArm, "SettingChange", iSetting);
		SettingChanged(iSetting);
	}

	return 0;
}

void cScr_NVGun::SettingChanged(int iSetting)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

/*
	if ( m_iCooldown != -1 )
	{

		if ( iSetting != 404 )
			m_iCooldown = iSetting;

		pPropSrv->Set(m_iObjId, "GunState", "Setting", 404);

		return;
	}
*/

	SService<IShockGameSrv> pShockGame(g_pScriptManager);
	object oEquipped;
	pShockGame->Equipped(oEquipped, 0);
	if ( oEquipped != m_iObjId)
		return;

	char *pszParam = new char[24];

	// Different reliabilities for different firing modes
	sprintf(pszParam, "NVGunDegradeSetting%i", iSetting);
	if ( ParamExists(m_iObjId, pszParam) )
	{
		float fDegrade = ParamGetFloat(m_iObjId, pszParam, 0.50);

		pPropSrv->Set(m_iObjId, "GunReliability", "Degrade Rate (%)", fDegrade);
	}

	sprintf(pszParam, "Setting%i", iSetting);
	SimpleSend(0, m_iObjId, pszParam);

	// Different models for different modes
	sLink slOLink;
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
	{
		object oArm = slOLink.dest;
		SimpleSend(0, oArm, pszParam);

		sprintf(pszParam, "NVGunModelSetting%i", iSetting);
		if ( ParamExists(m_iObjId, pszParam) )
		{
//			DisplayPrintf("Setting model to %s", ParamGetString(m_iObjId, pszParam, ""));
			const char* pszGunModel = ParamGetString(m_iObjId, pszParam, "");
			pPropSrv->Set(oArm, "ModelName", NULL, pszGunModel);
			pPropSrv->Set(m_iObjId, "PlayerGunDesc", "Hand Model", pszGunModel);
		}
	}

	delete[] pszParam;

	return;
}

void cScr_NVGun::StartGunStateListener(void)
{

	StopGunStateListener();
	if ( !m_pProp )
	{
		SInterface<IPropertyManager> pPM(g_pScriptManager);
		m_pProp = static_cast<IGenericProperty*>(pPM->GetPropertyNamed("GunState"));
	}
	if ( !m_hListenHandle )
	{
//		DebugPrintf("DEBUG: Starting listener; listening for settings changes on %i.", m_iObjId);
//		DisplayPrintf("DEBUG: Starting listener; listening for settings changes on %i.", m_iObjId);

		m_hListenHandle = m_pProp->Listen(kPropertyChange, ListenFunc, reinterpret_cast<PropListenerData__*>(m_iObjId));
	}

	return;
}

void cScr_NVGun::StopGunStateListener(void)
{

	if ( m_hListenHandle )
	{
//		DisplayPrintf("DEBUG: Script exited. Shutting down listener.");
		m_pProp->Unlisten(m_hListenHandle);
		m_hListenHandle = NULL;
	}

	return;
}

MSGHANDLER cScr_NVGun::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);

	SService<IShockGameSrv> pShockGame(g_pScriptManager);

	// temp
/*
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oArm;
	pOS->Named(oArm, "Player Gun");
	true_bool bExists;
	pOS->Exists(bExists, oArm);
	if (bExists)
	{
		DebugPrintf("Arm exists, oArm is %i", (int)oArm);
		ForceInit();
		sLink slOLink;
		if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "NewWeapon", -1))
		{
			DebugPrintf("Link exists...");
		}
	} else {
		DebugPrintf("Arm does not exist.");
	}

	// temp end
*/

	if (!stricmp(pszMsgName, "NVGunInit"))
	{
		ForceInit();
	}
	if (!stricmp(pszMsgName, "NVGunCooldown"))
	{
		FinishCooldown();
	}
	if (!stricmp(pszMsgName, "NVGunSlow"))
	{
		float fSpeed = ParamGetFloat(m_iObjId, "NVGunSpeedMultiplier", 0.00f);
		if ( fSpeed > 0.00f )
		{

			if ( fSpeed < 1.00f )
			{
				int iStrength = (pShockGame->GetStat(StrToObject("Player"), 0) - 1);
				float fSTRMult = ParamGetFloat(m_iObjId, "NVGunSpeedSTRMultiplier", 0.10f);
				fSpeed += (fSTRMult * iStrength);
				if ( fSpeed > 1.00f )
					fSpeed = 1.00f;

//				DisplayPrintf("Speed is now %f due to a STR of %f", fSpeed, fSTRMult);
			}

			pShockGame->AddSpeedControl("NVGun", fSpeed, 1.00f);
		}
	} else if (!stricmp(pszMsgName, "NVGunRecheck"))
	{
		object oEquipped;
		pShockGame->Equipped(oEquipped, 0);
		if ( oEquipped == m_iObjId)
		{
			int iStrength = (pShockGame->GetStat(StrToObject("Player"), 0) - 1);
			if ( m_iStrength != iStrength )
			{
				#if (_DARKGAME != 3)
					SService<IDarkInvSrv> pSrv(g_pScriptManager);
				#else
					SService<IShockGameSrv> pSrv(g_pScriptManager);
				#endif
				pSrv->RemoveSpeedControl ("NVGun");
				g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGunSlow", 75, kSTM_OneShot, g_mpUndef);
//				DisplayPrintf("Re-checking gun slowdown.\nStrength has changed!!");
				m_iStrength = iStrength;
			} else {
//				DisplayPrintf("Re-checking gun slowdown!");
			}
		}
	}

	return 0;
}

MSGHANDLER cScr_NVGun::OnEquipped(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	ForceInit(); // From NVNewWeapon

	StartListener(); // just in case
	StartGunStateListener(); // just in case

	/*
	if ( m_iCooldown != -1 )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		pPropSrv->Set(m_iObjId, "GunState", "Setting", 404);
//		pPropSrv->Set(m_iObjId, "ObjState", NULL, 4);
	}
	*/

//	DebugPrintf("Equipped %i", m_iObjId);

	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGunSlow", 75, kSTM_OneShot, g_mpUndef);
	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGunInit", 1000, kSTM_OneShot, g_mpUndef);

	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGunRecheck", 5000, kSTM_Periodic, g_mpUndef);

	return 0;
}


MSGHANDLER cScr_NVGun::OnUnequipped(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	ForcePutAway(); // From NVNewWeapon
//	FinishCooldown();

	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	#if (_DARKGAME != 3)
		SService<IDarkInvSrv> pSrv(g_pScriptManager);
	#else
		SService<IShockGameSrv> pSrv(g_pScriptManager);
	#endif
	pSrv->RemoveSpeedControl ("NVGun");

	return 0;
}


void cScr_NVGun::FinishCooldown(void)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
//	if ( m_iCooldown != -1 )
	if ( ParamGetInt(m_iObjId, "NVGunCooldownSetting", 0) != -1 )
	{
//		DisplayPrintf("Setting being changed back to %i!", static_cast<int>(m_iCooldown));
//		pPropSrv->Set(m_iObjId, "GunState", "Setting", static_cast<int>(m_iCooldown));

		int iGunAmmoUsage0 = ParamGetInt(m_iObjId, "NVGunCooldownAmmo0", 0);
		int iGunAmmoUsage1 = ParamGetInt(m_iObjId, "NVGunCooldownAmmo1", 0);
		int iGunAmmoUsage2 = ParamGetInt(m_iObjId, "NVGunCooldownAmmo2", 0);

		if ( iGunAmmoUsage0 != 0 )
			pPropSrv->Set(m_iObjId, "BaseGunDesc", "Setting 0: Ammo Usage", iGunAmmoUsage0);

		if ( iGunAmmoUsage1 != 0 )
			pPropSrv->Set(m_iObjId, "BaseGunDesc", "Setting 1: Ammo Usage", iGunAmmoUsage1);

		if ( iGunAmmoUsage2 != 0 )
			pPropSrv->Set(m_iObjId, "BaseGunDesc", "Setting 2: Ammo Usage", iGunAmmoUsage2);

		ParamSetInt(m_iObjId, "NVGunCooldownSetting", -1);
//		m_iCooldown = -1;
	}
	/*
	int iState = 0;
	if (pPropSrv->Possessed(m_iObjId, "ObjState"))
	{
		cMultiParm mpReturn;
		pPropSrv->Get(mpReturn, m_iObjId, "ObjState", NULL);
		iState = static_cast<int>(mpReturn);

		if ( iState == 4 )
			pPropSrv->Set(m_iObjId, "ObjState", NULL, 0);
	}
	*/

}

#endif


/***********************
 * NVMoveOBB
 */

MSGHANDLER cScr_NVMoveOBB::OnDrop(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (pPropSrv->Possessed(m_iObjId, "PhysType"))
		{
			pPropSrv->Remove(m_iObjId, "Type");
		}
		pPropSrv->Set(m_iObjId, "PhysType", "Type", 0);
		pPropSrv->Set(m_iObjId, "PhysControl", "Controls Active", 0x18);
	}

	return 0;
}

MSGHANDLER cScr_NVMoveOBB::OnGrab(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sContainedScrMsg * pContainedMsg = static_cast<sContainedScrMsg *>(pMsg);
	if (pContainedMsg->event == 2 ) // Picked up
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (pPropSrv->Possessed(m_iObjId, "PhysType"))
		{
			pPropSrv->Remove(m_iObjId, "Type");
		}
		pPropSrv->Set(m_iObjId, "PhysType", "Type", 2);
		pPropSrv->Set(m_iObjId, "PhysControl", "Controls Active", 0x00);
	}

	return 0;
}

MSGHANDLER cScr_NVMoveOBB::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Subscribe to PhysFellAsleep messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysEnterExit);
	return 0;
}

MSGHANDLER cScr_NVMoveOBB::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Unsubscribe to PhysFellAsleep messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysEnterExit);
	return 0;
}


/***********************
 * NVThrowingKnife
 */
#if ( _DARKGAME != 3 )
MSGHANDLER cScr_NVThrowingKnife::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	// Start a timer.
	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "ThrowingKnifeFrob", 1000, kSTM_OneShot, g_mpUndef);
	return 0;
}
#endif

MSGHANDLER cScr_NVThrowingKnife::OnThrow(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	#if ( _DARKGAME != 3 )
		if (m_hTimerHandle == NULL)
		{
			return 0;
		}
	#endif

	// Throw knife:
	char pszCfgTweqEmit[16];
	char pszStTweqEmit[16];

	// Get Tweq Number
	int iTweqNum = 0;

	iTweqNum = ParamGetInt(m_iObjId, "NVKnifeTweqNum", 3);

	if ((iTweqNum > 5) || iTweqNum < 1 )
	{
		iTweqNum = 1; // The Tweq number specified is invalid!
	}
	if ( iTweqNum == 1) {
		sprintf(pszCfgTweqEmit, "CfgTweqEmit");
		sprintf(pszStTweqEmit, "StTweqEmit");
	} else {
		sprintf(pszCfgTweqEmit, "CfgTweq%iEmit", iTweqNum);
		sprintf(pszStTweqEmit, "StTweq%iEmit", iTweqNum);
	}

	// Get handle to player
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oPlayer;
	pOS->Named(oPlayer, "Player");

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (pPropSrv->Possessed(m_iObjId, pszCfgTweqEmit))
	{
		pPropSrv->CopyFrom(oPlayer, pszCfgTweqEmit, m_iObjId); // Copy Tweq->Emit# property to player
		pPropSrv->Set(oPlayer, pszStTweqEmit, "Frame #", 0); // Reset frame
		pPropSrv->Set(oPlayer, pszStTweqEmit, "AnimS", 1); // Turn on emitter
	}

	#if ( _DARKGAME != 3 )
		// Change arm model:
		object oArm;
		pOS->Named(oArm, "PlyrArm");
		pPropSrv->Set(oArm, "ModelName", NULL, "Unarmed");
	#endif

	//Check stack count:
	cMultiParm mpStack = 0;
	if (pPropSrv->Possessed(m_iObjId, "StackCount"))
	{
		pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
	}

	if (static_cast<int>(mpStack) > 1)
	{
		pPropSrv->Set(m_iObjId, "StackCount", NULL, static_cast<int>(mpStack) - 1);
		object oWeap;
		#if ( _DARKGAME != 3 )
			SService<IDarkUISrv> pUI(g_pScriptManager);
			pUI->InvWeapon(oWeap);
		#else
			SService<IShockGameSrv> pShockGame(g_pScriptManager);
			pShockGame->PlayerGun(oWeap);
		#endif
		if ( oWeap )
		{
			m_cThrowing = 1;
			SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
			pDebugSrv->Command("clear_weapon", cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);

			SInterface<IContainSys> pContainSys(g_pScriptManager);
			pContainSys->Remove(0, m_iObjId);
			g_pScriptManager->SetTimedMessage2(m_iObjId, "ReSelectKnife", 800, kSTM_OneShot, static_cast<int>(oWeap));
		}
	} else {
		SService<IObjectSrv> pOS(g_pScriptManager);
		pOS->Destroy(m_iObjId);
		#if ( _DARKGAME != 3 )
			true_bool bExists;
			pOS->Exists(bExists, oArm);
			if ( bExists )
			{
				pOS->Destroy(oArm);
			}
		#endif
	}

	return 0;
}


MSGHANDLER cScr_NVThrowingKnife::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	#if ( _DARKGAME != 3 )
		if (!stricmp(pszMsgName, "ThrowingKnifeFrob"))
		{
			if (m_hTimerHandle != NULL)
			{	// Stop timer:
				g_pScriptManager->KillTimedMessage(m_hTimerHandle);
				m_hTimerHandle = NULL;
			}
		}
	#endif
	if (!stricmp(pszMsgName, "ReSelectKnife"))
	{
		m_cThrowing = 0;
		SInterface<IContainSys> pContainSys(g_pScriptManager);
		pContainSys->Add(StrToObject("Player"), m_iObjId, -1, 0);
		SService<IDarkUISrv> pUI(g_pScriptManager);
		pUI->InvSelect(static_cast<int>(pMsg->data));
	}

	return 0;
}

MSGHANDLER cScr_NVThrowingKnife::OnSelect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return 0;
}

MSGHANDLER cScr_NVThrowingKnife::OnDeselect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( !m_cThrowing)
	{
		return Transform(StrToObject("Player"));
	}
	return 0;
}

#if (_DARKGAME != 3) // SS2 has a keyring already, so this is unneeded.
/***********************
 * NVKeyringKey
 */

MSGHANDLER cScr_NVKeyringKey::OnGrab(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sContainedScrMsg* pContainedMsg = static_cast<sContainedScrMsg*>(pMsg);

	SService<IObjectSrv> pOS(g_pScriptManager);
	SInterface<IContainSys> pContainSys(g_pScriptManager);

	object oPlayer;
	pOS->Named(oPlayer, "Player");

	if ( !m_bSim )
		return 0;

	if (pContainedMsg->event != 2 ) // Picked up
		return 0;

	if ( pContainedMsg->container != oPlayer )
		return 0;

	// Get Keyring/lock ID
	int iKeyringId = ParamGetInt(m_iObjId, "NVKeyringID", 192);
	char* szKeyringName = new char[20];
	sprintf(szKeyringName, "NV-%i-Keyring", iKeyringId);

	// We should create a 'keyring' object if none exists
	object oKeyRing;
	pOS->Named(oKeyRing, szKeyringName);
	if ( !oKeyRing )
	{
		pOS->Create(oKeyRing, -1);
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		pPropSrv->Set(oKeyRing, "Class Tags", "1: Tags", "ObjType StdKey");
		pPropSrv->Set(oKeyRing, "InvCycleOrder", NULL, "00B");
		pPropSrv->Set(oKeyRing, "StackCount", NULL, 1);
		pPropSrv->Set(oKeyRing, "FrobInfo", "Tool Action", 258);
		pPropSrv->Set(oKeyRing, "FrobInfo", "World Action", 1);
		pPropSrv->Set(oKeyRing, "Scripts", "Script 0", "StdKey");
		const char* pszKeyModel = ParamGetString(m_iObjId, "NVKeyringModel", "MKeyRing");
		pPropSrv->Set(oKeyRing, "ModelName", NULL, pszKeyModel);
		pPropSrv->Set(oKeyRing, "InvType", NULL, 1);
		pPropSrv->Set(oKeyRing, "NoDrop", NULL, 1);
		pPropSrv->Set(oKeyRing, "SlayResult", "Effect", 1);
		pPropSrv->Set(oKeyRing, "KeySrc", "MasterBit", 0);
		pPropSrv->Set(oKeyRing, "KeySrc", "RegionMask", 1);
		pPropSrv->Set(oKeyRing, "KeySrc", "LockID", iKeyringId);
		const char* pszKeyName = ParamGetString(m_iObjId, "NVKeyringName", "name_keys");
		pPropSrv->Set(oKeyRing, "GameName", NULL, pszKeyName);

		pOS->SetName(oKeyRing, szKeyringName);
		pContainSys->Add(oPlayer, oKeyRing, -1, 0);

	} else {
		pContainSys->StackAdd(oKeyRing, 1, 0);
		SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
		true_bool bReturn;
		pSoundSrv->PlayEnvSchema(bReturn, oKeyRing, "Event Acquire", oKeyRing, 0, kEnvSoundAmbient SOUND_NET);
	}

	sPropertyObjIter sIter;
	int iObj;

	SInterface<IPropertyManager> pPropMan(g_pScriptManager);
	SInterface<IGenericProperty> pProp;
	sKeyProp* pKeySrc;
	sKeyProp pKeyRing;
	pKeyRing.bMasterBit = 0;
	pKeyRing.iRegionMask = 1;
	pKeyRing.iLockID = iKeyringId;

	pProp = static_cast<IGenericProperty*>(pPropMan->GetPropertyNamed("KeySrc"));
	if (pProp)
	{
		pProp->Get(m_iObjId, reinterpret_cast<void**>(&pKeySrc));
	} else {
		return 1;
	}
	pProp = static_cast<IGenericProperty*>(pPropMan->GetPropertyNamed("KeyDst"));
	if (pProp)
	{
		pProp->IterStart(&sIter);
		for (;pProp->IterNext(&sIter,&iObj);)
		{
			if (iObj > 0 )
			{ // Let's leave the archetypes alone!
				sKeyProp* pDump;
				pProp->Get(iObj, reinterpret_cast<void**>(&pDump));
				if (pDump->iRegionMask & pKeySrc->iRegionMask )
				{
					if ( pDump->iLockID == pKeySrc->iLockID )
					{
						pProp->Set(iObj, reinterpret_cast<void**>(&pKeyRing));
					}
				}
			}
		}
		pProp->IterStop(&sIter);

	}

	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVKeyringSelectTimer", 100, kSTM_OneShot, g_mpUndef);

	return 0;
}

MSGHANDLER cScr_NVKeyringKey::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVKeyringSelectTimer"))
	{
		int iKeyringId = ParamGetInt(m_iObjId, "NVKeyringID", 192);
		char* szKeyringName = new char[20];
		sprintf(szKeyringName, "NV-%i-Keyring", iKeyringId);

		SService<IObjectSrv> pOS(g_pScriptManager);
		object oKeyRing;
		pOS->Named(oKeyRing, szKeyringName);
		if (oKeyRing)
		{
			SService<IDarkUISrv> pUI(g_pScriptManager);
			pUI->InvSelect(oKeyRing);
		}
		pOS->Destroy(m_iObjId);
	}
	return 0;
}
#endif

#if (_BUILD != 1 )
/***********************
 * NVMirrorHack
 */

MSGHANDLER cScr_NVMirrorHack::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVMirrorHack"))
	{  // Do nothing if it's not the right timer.
		SService<IObjectSrv> pOS(g_pScriptManager);
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		SService<IPuppetSrv> pPupS(g_pScriptManager);

		// Define variables
		cMultiParm mpWeaponExposure = 0;
		true_bool bReturn;
		object oPlayer;
		object oMirrorImage;
		cScrVec vPlayerPos, vPlayerFacing, vMyPos, vMirrorPos, vMirrorFacing, vPlayerVel;

		pOS->Named(oPlayer, "Player");
		pOS->Named(oMirrorImage, "NVMirrorImage");
		if ( !oMirrorImage )
		{ // Create a 'Mirror Image' if one doesn't exist already, and set up its properties...
			pOS->Create(oMirrorImage, -1);
			pOS->SetName(oMirrorImage, "NVMirrorImage");
			pPropSrv->Set(oMirrorImage, "Creature", "Type", 0);
			pPropSrv->Set(oMirrorImage, "CretPose", "Motion", "Stand");
			pPropSrv->Set(oMirrorImage, "CretPose", "Motion", "Stand");
			pPropSrv->Set(oMirrorImage, "PhysControl", "Controls Active", 31);
			pPropSrv->Set(oMirrorImage, "AI", "Behavior set", "Default");
			pPropSrv->Set(oMirrorImage, "AI_Team", NULL, "1");
			pPropSrv->Set(oMirrorImage, "AI_Hearing", NULL, 0);
			pPropSrv->Set(oMirrorImage, "AI_Verbosity", NULL, 0);
			pPropSrv->Set(oMirrorImage, "AI_Vision", NULL, 0);
			pPropSrv->Set(oMirrorImage, "Scripts", "Script 0", "NVMirrorImage");
			pPropSrv->Set(oMirrorImage, "DesignNote", NULL, m_iObjId);
		}

		SService<IDarkUISrv> pUI(g_pScriptManager);
		object oWeapon;
		pUI->InvWeapon(oWeapon);
		if ( oWeapon )
		{
			pPropSrv->Get(mpWeaponExposure, oWeapon, "WpnExposure", NULL);
			SService<IBowSrv> pBowSrv(g_pScriptManager);
			if ( pBowSrv->IsEquipped() )
			{
				if ( abs(m_iWeapon) != 3 )
				{
					m_iWeapon = 3;
					m_iMotion = -3;
					pPupS->PlayMotion(bReturn, oMirrorImage, "BH112203"); // ReadyItem, ItemBow
					}
			} else {
				true_bool bBool;
				object oArc;
				pOS->Named(oArc, "sword");
				pOS->InheritsFrom(bBool, oWeapon, oArc);
				if (bBool )
				{
					if ( abs(m_iWeapon) != 1 )
					{
						m_iWeapon = 1;
						m_iMotion = -3;
						pPupS->PlayMotion(bReturn, oMirrorImage, "BH114721"); // ReadyItem, ItemBow
					}
				} else {
					pOS->Named(oArc, "blackjack");
					pOS->InheritsFrom(bBool, oWeapon, oArc);
					if ( bBool )
					{
						if ( abs(m_iWeapon) != 2 )
						{
							m_iWeapon = 2;
							m_iMotion = -3;
							pPupS->PlayMotion(bReturn, oMirrorImage, "BH114721"); // ReadyItem, ItemBow
						}
					} else {
						m_iWeapon = 0;
						pPropSrv->Set(oMirrorImage, "ModelName", NULL, "Garrett");
					}
				}

			}
		} else {
			m_iWeapon = 0;
			pPropSrv->Set(oMirrorImage, "ModelName", NULL, "Garrett");
		}

		// Set the Mirror image's transparency.
		cMultiParm mpVisibility;
		pPropSrv->Get(mpVisibility, oPlayer, "AI_Visibility", "Level");
		pPropSrv->Set(oMirrorImage, "RenderAlpha", NULL, ((static_cast<float>(mpVisibility) / 100) + 0.40));
		pPropSrv->Get(mpVisibility, oPlayer, "AI_Visibility", "Exposure Rating");
		bool bCrouching = ((static_cast<int>(mpVisibility) - static_cast<int>(mpWeaponExposure) ) < -2 ) ? true : false;

		// Get the player's velocity, and have the mirror image play appropriate motions if the player is moving...
		SService<IPhysSrv> pPhysSrv(g_pScriptManager);
		pPhysSrv->GetVelocity(oPlayer, vPlayerVel);
		float fPlayerVelX = fabs(vPlayerVel.x);
		float fPlayerVelY = fabs(vPlayerVel.y);
		float fHighestVel = ( fPlayerVelX > fPlayerVelY ) ? fPlayerVelX : fPlayerVelY;
		float fPlyrVelocity = fPlayerVelX + fPlayerVelY; // + fabs(vPlayerVel.z);
		if ( vPlayerVel.z > 1 )
		{
			if ( m_iMotion != -4 )
			{
				m_iMotion = -4;
				pPupS->PlayMotion(bReturn, oMirrorImage, "plyjmpup1");
			}
		} else
		{
			if ( fPlyrVelocity )
			{
				if ( m_iMotion > -1 )
				{
					if ( bCrouching )
					{
						m_iMotion = -1;
						pPupS->PlayMotion(bReturn, oMirrorImage, "plycrwlk");
					} else {
						if ( m_iMotion == 2 )
						{
							m_iMotion = -1;
							if ( fHighestVel > 10 )
							{
								pPupS->PlayMotion(bReturn, oMirrorImage, "BH111033");
							} else {
								pPupS->PlayMotion(bReturn, oMirrorImage, "BH111o11");
							}
						} else {
							m_iMotion = -2;
							if ( fHighestVel > 10 )
							{
								pPupS->PlayMotion(bReturn, oMirrorImage, "BH111032");
							} else {
								pPupS->PlayMotion(bReturn, oMirrorImage, "BH111o1o");
							}
						}
					}
				}
			} else {
				if ( m_iMotion != -3 )
				{
					m_iMotion = -3;
					if ( bCrouching )
					{
						pPupS->PlayMotion(bReturn, oMirrorImage, "plycrch");
					} else {
						pPupS->PlayMotion(bReturn, oMirrorImage, "stand");
					}
				}
			}
		}

		// Get the current position and facing of the player, and the position of the mirror...
		pOS->Position(vPlayerPos, oPlayer);
		pOS->Position(vMyPos, m_iObjId);
		pOS->Facing(vPlayerFacing, oPlayer);

		// Position mirror image...
		vMirrorPos.x = (vMyPos.x - (vPlayerPos.x - vMyPos.x));
//		vMirrorPos.y = (vMyPos.y - (vPlayerPos.y - vMyPos.y));
		vMirrorPos.y = vPlayerPos.y;
		if ( bCrouching )
		{
			vMirrorPos.z = vPlayerPos.z - 0.25f;
		} else {
			vMirrorPos.z = vPlayerPos.z + 0.50f;
		}
		float fMirrorFacing = 360 - (vPlayerFacing.z + 180);
		if (fMirrorFacing >= 360 )
		{
			fMirrorFacing -= 360; // Modulus only works on intergers.
		}
		vMirrorFacing.z = fMirrorFacing;

		pOS->Teleport(oMirrorImage, vMirrorPos, vMirrorFacing, 0);


	}

	return 0;
}

MSGHANDLER cScr_NVMirrorHack::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		DisplayPrintf("Starting Timer...");
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVMirrorHack", 10, kSTM_Periodic, g_mpUndef);
	}
	return 0;
}

MSGHANDLER cScr_NVMirrorHack::OnMirrorImageMotionEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_iWeapon )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		object oMirrorImage = StrToObject("NVMirrorImage");
		switch ( m_iWeapon )
		{
		case 1:
			pPropSrv->Set(oMirrorImage, "ModelName", NULL, "GarSw");
			break;
		case 2:
			pPropSrv->Set(oMirrorImage, "ModelName", NULL, "GarBJ");
			break;
		case 3:
			pPropSrv->Set(oMirrorImage, "ModelName", NULL, "GarBow");
		}
		m_iWeapon = -m_iWeapon;
	}
	m_iMotion = -m_iMotion;
	return 0;
}



/***********************
 * NVMirrorImage
 */

MSGHANDLER cScr_NVMirrorImage::OnMotionEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpFirer;
	pPropSrv->Get(mpFirer, m_iObjId, "DesignNote", NULL);
	#if (_DARKGAME != 1)
		g_pScriptManager->PostMessage2(m_iObjId, static_cast<int>(mpFirer), "MirrorImageMotionEnd", g_mpUndef, g_mpUndef, g_mpUndef, 0L);
	#else
		g_pScriptManager->PostMessage2(m_iObjId, static_cast<int>(mpFirer), "MirrorImageMotionEnd", g_mpUndef, g_mpUndef, g_mpUndef);
	#endif
	return 0;
}
#endif

/***********************
 * NVItemGiver
 */
MSGHANDLER cScr_NVItemGiver::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	object oDest, oArc, oItem;
	oDest = ParamGetNVObject(m_iObjId, "NVGiveTo", 0);
	if ( !oDest )
	{
		oDest = StrToObject("Player");
	}

	oArc = ParamGetNVObject(m_iObjId, "NVGiveWhat", 0);
	if ( oArc < 0 )
	{
		pOS->Create(oItem, oArc);
	} else {
		oItem = oArc;
	}

	if ( ParamGetInt(m_iObjId, "NVGiveStack", 0) )
	{
		int iStack = 1;
		//Check stack count:
		cMultiParm mpStack = 0;
		if (pPropSrv->Possessed(m_iObjId, "StackCount"))
		{
			pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
			{
				iStack = static_cast<int>(mpStack);
			}
		}
		pPropSrv->Set(oItem, "StackCount", NULL, iStack);
	}

	if ( oItem )
	{
		#if ( _DARKGAME != 3 )
			SInterface<IContainSys> pContainSys(g_pScriptManager);
			pContainSys->Add(oDest, oItem, 0, 1);
		#else	// System Shock 2 does things a little differently...
			SService<IShockGameSrv> pShockGame(g_pScriptManager);
			pShockGame->AddInvObj(oItem);
			pShockGame->RefreshInv();
		#endif

	}
	return 0;
}

MSGHANDLER cScr_NVItemGiver::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}



/***********************
 * NVInvAssembly
 */

MSGHANDLER cScr_NVInvAssembly::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobber = pFrobMsg->Frobber;

	int iUseTrap = ParamGetObject(m_iObjId, "NVAssemblyType", 0);
	if (iUseTrap != 1)
	{
		Assemble(iFrobber, pMsg);
	}
	return 0;
}

MSGHANDLER cScr_NVInvAssembly::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iPlayer = StrToObject("Player");
	int iUseTrap = ParamGetObject(m_iObjId, "NVAssemblyType", 0);
	if (iUseTrap != 0)
	{
		Assemble(iPlayer, pMsg);
	}
	return 0;
}

MSGHANDLER cScr_NVInvAssembly::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Do nothing.
	return 0;
}

void cScr_NVInvAssembly::Assemble(int iFrobber, sScrMsg* pMsg)
{

	object oPartOne = ParamGetObject(m_iObjId, "NVAssemblyPart", 0);
	if ( !oPartOne )
		return;

	int iPart = GetSinglePossession(iFrobber, oPartOne);
	if ( iPart )
	{
		SInterface<IContainSys> pContainSys(g_pScriptManager);
		int iSpendAssembler = ParamGetObject(m_iObjId, "NVSpendAssembler", 1);
		if (iSpendAssembler)
		{
			pContainSys->StackAdd(m_iObjId, -1, 1);
		}

		pContainSys->StackAdd(iPart, -1, 1);

		object oArc, oItem;
		oArc = ParamGetObject(m_iObjId, "NVAssembledItem", 0);
		if ( oArc )
		{
			SService<IObjectSrv> pOS(g_pScriptManager);
			pOS->Create(oItem, oArc);

			SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
			true_bool bReturn;

			#if ( _DARKGAME != 3 )
				pSoundSrv->PlayEnvSchema(bReturn, oItem, "Event Acquire", oItem, 0, kEnvSoundAmbient SOUND_NET);
				pContainSys->Add(iFrobber, oItem, 0, 1);
			#else
				SService<IShockGameSrv> pShockGame(g_pScriptManager);
				pSoundSrv->PlayEnvSchema(bReturn, oItem, "Event Create", oItem, 0, kEnvSoundAmbient SOUND_NET);
				pShockGame->AddInvObj(oItem);
				pShockGame->RefreshInv();
			#endif

			int iResult = GetSinglePossession(iFrobber, oArc);
			Trigger(true, pMsg, iResult, "Assembled");
		}

	} else {
		SService<IDataSrv> pDataSrv(g_pScriptManager);
		cScrStr strError;
		pDataSrv->GetString(strError, "NVInvAssembly.str", ParamGetString(m_iObjId, "NVAssemblyError", "Default"), "", "Strings");
		DisplayPrintf("%s", static_cast<const char*>(strError));

		if (!strError.IsEmpty())
		{
			strError.Free();
		}
	}

	return;
}

/***********************
 * NVInvAssembly2
 */

MSGHANDLER cScr_NVInvAssembly2::OnToolFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	object oArc = 0;

	#if ( _DARKGAME != 3 )
	int iFrobber = pFrobMsg->Frobber;
	#endif
	int iDest = pFrobMsg->DstObjId;
	int iStackUse = 1;
	int iStackUse2 = 1;
	int iNewCount = 1;

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	#if (_DARKGAME == 3 )
		int iTrait = 0;

		SService<IShockGameSrv> pShockGame(g_pScriptManager);

		int iState = 0;
		cMultiParm mpState;
		if (pPropSrv->Possessed(m_iObjId, "ObjState"))
		{
			pPropSrv->Get(mpState, m_iObjId, "ObjState", NULL);
			iState = static_cast<int>(mpState);
		}

		if ( ( iState != 0 ) && ( iState != 5 ) )
		{	// Not "normal" or "hacked"
			pShockGame->AddTranslatableText("NVInvAssemblyNeedResearch", "Misc", StrToObject("Player"), 5000);
			return 0;
		}
	#endif

	object oPartOne = ParamGetObject(m_iObjId, "NVAssemblyPart", 0);
	if ( oPartOne )
	{
		oArc = ParamGetObject(m_iObjId, "NVAssembledItem", 0);
		#if (_DARKGAME == 3 )
			iTrait = ParamGetInt(m_iObjId, "NVAssemblyTrait", 0);
		#endif
		iStackUse = ParamGetInt(m_iObjId, "NVAssemblyPartStack", 1);
		iStackUse2 = ParamGetInt(m_iObjId, "NVAssemblyStack", 1);
		iNewCount = ParamGetInt(m_iObjId, "NVAssembledItemCount", 1);
	}
	// if ( !oPartOne )
		// return 1;

	SInterface<IContainSys> pContainSys(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bReturn;
	bool bFirstTime = true;
	char* szParamName = new char[33];
	bool bNoChange = false;

	for (int i=0; ; i++)
	{
		bNoChange = false;

		if ( !bFirstTime )
		{
			sprintf(szParamName, "NVAssemblyPart%i", i);
			oPartOne = ParamGetObject(m_iObjId, szParamName, 0);
			if ( !oPartOne )
				break;

			sprintf(szParamName, "NVAssembledItem%i", i);
			oArc = ParamGetObject(m_iObjId, szParamName, 0);

			sprintf(szParamName, "NVAssemblyPart%iStack", i);
			iStackUse = ParamGetInt(m_iObjId, szParamName, 1);

			sprintf(szParamName, "NVAssembledItem%iCount", i);
			iNewCount = ParamGetInt(m_iObjId, szParamName, 1);

			sprintf(szParamName, "NVAssembly%iStack", i);
			iStackUse2 = ParamGetInt(m_iObjId, szParamName, 1);
			#if (_DARKGAME == 3 )
				sprintf(szParamName, "NVAssembly%iTrait", i);
				iTrait = ParamGetInt(m_iObjId, szParamName, 0);
			#endif
		} else {
			bFirstTime = false;
		}

		pOS->InheritsFrom(bReturn, iDest, oPartOne);
		if ( bReturn)
		{

			if ( !oArc )
				continue;


			#if (_DARKGAME == 3 )

//			DisplayPrintf("Needed trait is %i.", iTrait);
				if ( iTrait != 0 )
				{
					if ( !pShockGame->HasTrait(StrToObject("Player"), iTrait))
					{
						pShockGame->AddTranslatableText("NVInvAssemblyNeedTrait", "Misc", StrToObject("Player"), 5000);
						bNoChange = true;
					}
				}

				iState = 0;
				if (pPropSrv->Possessed(iDest, "ObjState"))
				{
					pPropSrv->Get(mpState, iDest, "ObjState", NULL);
					iState = static_cast<int>(mpState);
				}

				if ( ( iState != 0 ) && ( iState != 5 ) )
				{	// Not "normal" or "hacked"
					pShockGame->AddTranslatableText("NVInvAssemblyNeedResearch", "Misc", StrToObject("Player"), 5000);
					bNoChange = true;
				}


				object oSlot;
				for (int i = 0; i <= 14; i++)
				{
					pShockGame->Equipped(oSlot, i);
					if (oSlot == iDest)
					{
						DisplayPrintf("Take it off first.");
						bNoChange = true;
					}
				}
			#endif

			if ( bNoChange == false )
			{
				//Check stack counts:
				cMultiParm mpStack1 = 1;
				if (pPropSrv->Possessed(iDest, "StackCount"))
				{
					pPropSrv->Get(mpStack1, iDest, "StackCount", NULL);
				}
				int iStack1 = static_cast<int>(mpStack1);

				cMultiParm mpStack2 = 1;
				if (pPropSrv->Possessed(m_iObjId, "StackCount"))
				{
					pPropSrv->Get(mpStack2, m_iObjId, "StackCount", NULL);
				}
				int iStack2 = static_cast<int>(mpStack2);

				if ( ( iStack1 < iStackUse ) && ( iStack1 != 0 ))
				{
					DisplayPrintf("Insufficient quantity. You need %i of this item.", iStackUse);
					continue;
				}

				if ( ( iStack2 < iStackUse2 ) && ( iStack2 != 0 ))
				{
					DisplayPrintf("Insufficient quantity. You need %i of this item.", iStackUse2);
					continue;
				}

				object oItem;

				for (; iNewCount > 0; --iNewCount)
				{
					SService<IObjectSrv> pOS(g_pScriptManager);
					pOS->Create(oItem, oArc);

					#if ( _DARKGAME != 3 )
						pContainSys->Add(iFrobber, oItem, 0, 1);
					#else
						pShockGame->AddInvObj(oItem);
					#endif
				}

				#if ( _DARKGAME == 3 )
					pShockGame->PreventSwap();
					pShockGame->RefreshInv();
				#endif

				SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
				true_bool bReturn;

				object oSchema = ParamGetObject(oItem, "NVAssembledSchema", 0);
				if ( oSchema != 0 )
				{
					pSoundSrv->PlaySchemaAmbient(bReturn, m_iObjId, oSchema SOUND_NET);
				} else {
					#if ( _DARKGAME != 3 )
						pSoundSrv->PlayEnvSchema(bReturn, oItem, "Event Acquire", oItem, 0, kEnvSoundAmbient SOUND_NET);
					#else
						pSoundSrv->PlayEnvSchema(bReturn, oItem, "Event Create", oItem, 0, kEnvSoundAmbient, kSoundNetwork0);
					#endif
				}

				SService<IDamageSrv> pDmgSrv(g_pScriptManager);

				if ( iStack1 > iStackUse )
				{
					pContainSys->StackAdd(iDest, -iStackUse, 1);
				} else {
					#if ( _DARKGAME != 3 )
						pDmgSrv->Slay(iDest, iFrobber);
					#else
						pOS->Destroy(iDest);
					#endif
				}
				if ( iStack2 > iStackUse2 )
				{
					pContainSys->StackAdd(m_iObjId, -iStackUse2, 1);
				} else {
					#if (_DARKGAME != 3)
						pOS->Destroy(m_iObjId);
					#else
						pShockGame->DestroyInvObj(m_iObjId);
					#endif
				}
				break;
			}
		}
	}
	delete[] szParamName;

	return 0;
}

/***********************
 * NVHolyH2OHack
 */
MSGHANDLER cScr_NVHolyH2OHack::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobber = pFrobMsg->Frobber;

	if ( !GetSinglePossession(iFrobber, StrToObject("water")) )
	{
		return Transform(static_cast<int>(pFrobMsg->Frobber));
	} else {
		SInterface<IContainSys> pContainSys(g_pScriptManager);
		pContainSys->StackAdd(m_iObjId, -1, 1);
		return 0;
	}
}

/***********************
 * NVSpy
 */

 MSGHANDLER cScr_NVSpy::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision | kPhysEnterExit | kPhysContact | kPhysFellAsleep);
	return 0;
}

MSGHANDLER cScr_NVSpy::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision | kPhysEnterExit | kPhysContact | kPhysFellAsleep);
	return 0;
}

long __stdcall cScr_NVSpy::ReceiveMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( ! stricmp(pMsg->message, "Timer"))
		if ( ParamGetInt(m_iObjId, "NVSpyIgnoreTimer", 0) == 1 )
			return 0;

	SInterface<ITraitManager> pTraits(g_pScriptManager);
	int iArchetype = 0;

	SService<IObjectSrv> pOS(g_pScriptManager);
	cScrStr strFromName;
	pOS->GetName(strFromName, static_cast<int>(pMsg->from));
	if ( strFromName.IsEmpty() )
	{
		iArchetype = pTraits->GetArchetype(static_cast<int>(pMsg->from)); // Make a note of object's archetype.
		pOS->GetName(strFromName, iArchetype);
	}

	cScrStr strToName;
	pOS->GetName(strToName, m_iObjId);
	if ( strToName.IsEmpty() )
	{
		iArchetype = pTraits->GetArchetype(m_iObjId); // Make a note of object's archetype.
		pOS->GetName(strToName, iArchetype);
	}

	DisplayPrintf("NVSpy: \"%s\" on %s (%i) from %s (%i)", pMsg->message, static_cast<const char*>(strToName), m_iObjId, static_cast<const char*>(strFromName), pMsg->from);
	DebugPrintf("NVSpy: \"%s\" on %s (%i) from %s (%i)", pMsg->message, static_cast<const char*>(strToName), m_iObjId, static_cast<const char*>(strFromName), pMsg->from);
	strToName.Free();
	strFromName.Free();

	// if ( strlen(pMsg->message) > 8 )
		// if ( !stricmp((pMsg->message +( strlen(pMsg->message) - 8)), "Stimulus") )
		// {
			// sStimMsg* pStimMsg = static_cast<sStimMsg*>(pMsg);

			// SService<ILinkToolsSrv> pLTS(g_pScriptManager);

			// cMultiParm mpReturn;
			// pLTS->LinkGetData(mpReturn, pStimMsg->source, "Description");
			// int iDesc = static_cast<int>(mpReturn);

			// pLTS->LinkGetData(mpReturn, iDesc, "Propagator");
			// int iProp = static_cast<int>(mpReturn);

			// pLTS->LinkGetData(mpReturn, iDesc, "Intensity");
			// float fIntensity = static_cast<int>(mpReturn);

			// DisplayPrintf("NVSpy, Stim detected: Intensity %f, Description: %i, Propagator: %i, Intensity: %f", pStimMsg->intensity, iDesc, iProp, fIntensity);
			// DebugPrintf("NVSpy, Stim detected: Intensity %f, Description: %i, Propagator: %i, Intensity: %f", pStimMsg->intensity, iDesc, iProp, fIntensity);

		// }

	cScriptNV::ReceiveMessage(pMsg, pReply, eTrace);

	return 0;
}

/***********************
 * NVTrapConverse
 */

MSGHANDLER cScr_NVTrapConverse::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IAIScrSrv> pAISrv(g_pScriptManager);
	true_bool bReturn;
	pAISrv->StartConversation(bReturn, m_iObjId);
	return 0;
}

MSGHANDLER cScr_NVTrapConverse::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVAirlock
 */

MSGHANDLER cScr_NVAirlock::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILockSrv> pLockSrv(g_pScriptManager);
	if ( pLockSrv->IsLocked(m_iObjId) )
	{ // Do nothing if the trap is locked.
		return 0;
	}

	// Lock the trap
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->SetSimple(m_iObjId, "Locked", true);

	// Get door ObjIDs

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed(CDLINK), m_iObjId, 0);

	if ( !lsLinks.AnyLinksLeft())
		return 1; // Ooops, no ControlDevice/SwitchLink Links!


	sLink sl = lsLinks.Get();
	m_iDoorOne = sl.dest;
	lsLinks.NextLink();

	if ( !lsLinks.AnyLinksLeft())
		return 1; // Ooops, not enough ControlDevice/SwitchLink Links!

	sl = lsLinks.Get();
	m_iDoorTwo = sl.dest;

	// Check which door is currently closed.
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	if ( pDoorSrv->GetDoorState(static_cast<int>(m_iDoorTwo)) == kDoorStateClosed )
	{
		m_iNextDoor = 2;
	} else {
		m_iNextDoor = 1;
	}

	// Close the doors
	pDoorSrv->CloseDoor(static_cast<int>(m_iDoorOne));
	pDoorSrv->CloseDoor(static_cast<int>(m_iDoorTwo));
	if (m_hTimerHandle == NULL)

	KillTimer();
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "CheckDoors", 1000, kSTM_Periodic, g_mpUndef);
	}

	IterateLinksByData("ScriptParams", m_iObjId, 0, "Alarm", -1, LinkIterFunc, NULL, reinterpret_cast<void*>(1));

	return 0;
}

MSGHANDLER cScr_NVAirlock::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

void cScr_NVAirlock::KillTimer()
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return;
}

MSGHANDLER cScr_NVAirlock::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);

	SService<IDoorSrv> pDoorSrv(g_pScriptManager);

	if (!stricmp(pszMsgName, "CheckDoors"))
	{
		if ( pDoorSrv->GetDoorState(static_cast<int>(m_iDoorTwo)) == kDoorStateClosed )
			if ( pDoorSrv->GetDoorState(static_cast<int>(m_iDoorOne)) == kDoorStateClosed )
			{
				KillTimer();
				IterateLinksByData("ScriptParams", m_iObjId, 0, "Vent", -1, LinkIterFunc, NULL, reinterpret_cast<void*>(1));
				int iTimer = ParamGetInt(m_iObjId, "NVAirlockDelay", 5000);
				g_pScriptManager->SetTimedMessage2(m_iObjId, "OpenDoor", iTimer, kSTM_OneShot, g_mpUndef);
				return 0;
			}
	}

	if (!stricmp(pszMsgName, "CheckDoorOpen"))
	{
		int iDoorState;
		if ( m_iNextDoor == 2 )
		{
			iDoorState = pDoorSrv->GetDoorState(static_cast<int>(m_iDoorTwo));
		} else {
			iDoorState = pDoorSrv->GetDoorState(static_cast<int>(m_iDoorOne));
		}

		if ( iDoorState == kDoorStateOpen )
		{
			KillTimer();
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			pPropSrv->SetSimple(m_iObjId, "Locked", false);

			IterateLinksByData("ScriptParams", m_iObjId, 0, "Alarm", -1, LinkIterFunc, NULL, reinterpret_cast<void*>(0));
		}
		return 0;
	}

	if (!stricmp(pszMsgName, "OpenDoor"))
	{
		IterateLinksByData("ScriptParams", m_iObjId, 0, "Vent", -1, LinkIterFunc, NULL, reinterpret_cast<void*>(0));
		if ( m_iNextDoor == 2 )
		{
			SimplePost(m_iObjId, m_iDoorTwo, "TurnOn");
		} else {
			SimplePost(m_iObjId, m_iDoorOne, "TurnOn");
		}
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "CheckDoorOpen", 1000, kSTM_Periodic, g_mpUndef);
		return 0;
	}
	return 0;
}

int cScr_NVAirlock::LinkIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	int iTurnOn = reinterpret_cast<int>(pData);
	sLink sl;
	pLQ->Link(&sl);
	if ( iTurnOn )
	{
		SimplePost(sl.source, sl.dest, "TurnOn");
	} else {
		SimplePost(sl.source, sl.dest, "TurnOff");
	}
	return 1;
}


/***********************
 * NVDoorStartsOpen
 */

MSGHANDLER cScr_NVDoorStartsOpen::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		Startup();
	}
	return 0;
}

MSGHANDLER cScr_NVDoorStartsOpen::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0 )
	{
		Startup();
	}

	return 0;
}

void cScr_NVDoorStartsOpen::Startup()
{
	cMultiParm mpSpeed;
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	// First, find out if it's a translating door.
	if (pPropSrv->Possessed(m_iObjId, "TransDoor"))
	{ // Set base speed of translating door.
		pPropSrv->Get(mpSpeed, m_iObjId, "TransDoor", "Base Speed");
		pPropSrv->Set(m_iObjId, "TransDoor", "Base Speed", 2500.00f);
	} else { // Check if it's a rotating door.
		if (pPropSrv->Possessed(m_iObjId, "RotDoor"))
		{ // Set base speed of rotating door.
			pPropSrv->Get(mpSpeed, m_iObjId, "RotDoor", "Base Speed");
			pPropSrv->Set(m_iObjId, "RotDoor", "Base Speed", 2500.00f);
		} else {
			return; // Eeek, this isn't a door!
		}
	}
	m_fDoorSpeed = mpSpeed;
	m_iDoorOpened = 0;

	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	pDoorSrv->OpenDoor(m_iObjId);
	return;
}

MSGHANDLER cScr_NVDoorStartsOpen::OnDoorOpen(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( !m_iDoorOpened )
	{
		m_iDoorOpened = 1;
		if ( ! m_fDoorSpeed )
			m_fDoorSpeed = 1.00f;

		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if (pPropSrv->Possessed(m_iObjId, "TransDoor"))
		{ // Set base speed of translating door.
			pPropSrv->Set(m_iObjId, "TransDoor", "Base Speed", static_cast<float>(m_fDoorSpeed));
		} else { // Check if it's a rotating door.
			if (pPropSrv->Possessed(m_iObjId, "RotDoor"))
			{ // Set base speed of rotating door.
				pPropSrv->Set(m_iObjId, "RotDoor", "Base Speed", static_cast<float>(m_fDoorSpeed));
			} else {
				return 1; // Eeek, this isn't a door!
			}
		}
	}
	return 0;
}

#if (_DARKGAME != 3)
/***********************
 * NVCursedObj
 */
MSGHANDLER cScr_NVCursedObj::OnInvDeselect(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	Curse();
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "CursedObj", 100, kSTM_Periodic, g_mpUndef);
	}
//	g_pScriptManager->SetTimedMessage2(m_iObjId, "CursedObj", 100, kSTM_OneShot, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVCursedObj::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "CursedObj"))
	{
		Curse();

		object oSelection;
		SService<IDarkUISrv> pUI(g_pScriptManager);
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		cMultiParm mpInvType;
		pPropSrv->Get(mpInvType, m_iObjId, "InvType", NULL);
		if ( static_cast<int>(mpInvType) == 2 )
		{
			pUI->InvWeapon(oSelection);
		} else {
			pUI->InvItem(oSelection);
		}

		if (oSelection == m_iObjId)
		{
			if (m_hTimerHandle != NULL)
			{	// Stop timer:
				g_pScriptManager->KillTimedMessage(m_hTimerHandle);
				m_hTimerHandle = NULL;
				CurseMessage();
			}
		}
	}
	return 0;
}

MSGHANDLER cScr_NVCursedObj::OnInvDeFocus(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpInvType;
	pPropSrv->Get(mpInvType, m_iObjId, "InvType", NULL);
	if ( static_cast<int>(mpInvType) == 1 )
	{
		SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
		pDebugSrv->Command("next_item", cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);
	}

	return 0;
}

MSGHANDLER cScr_NVCursedObj::OnGrab(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sContainedScrMsg* pContainedMsg = static_cast<sContainedScrMsg*>(pMsg);
	if (pContainedMsg->event == 2 ) // Picked up
	{
		if ( pContainedMsg->container == StrToObject("Player") )
		{
			Curse();
			CurseMessage();
		}
	}
	return 0;
}

void cScr_NVCursedObj::Curse(void)
{
	SService<IDarkUISrv> pUI(g_pScriptManager);
	pUI->InvSelect(m_iObjId);

	return;
}

void cScr_NVCursedObj::CurseMessage(void)
{
	SService<IDataSrv> pDataSrv(g_pScriptManager);
	cScrStr strError;
	pDataSrv->GetString(strError, "NVCursedObj.str", ParamGetString(m_iObjId, "NVCurseMessage", "Default"), "", "Strings");
	DisplayPrintf("%s", static_cast<const char*>(strError));

	if (!strError.IsEmpty())
	{
		strError.Free();
	}
	return;
}
#endif



/***********************
 * NVTrapSetQVar
 */

MSGHANDLER cScr_NVTrapSetQVar::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	char* pszQVar = NULL;
	int iValue = 0;
	char cOperand = 0;
	int iValueLength = 0;

	bool retval = GetQVarInfo(m_iObjId, &cOperand, &iValue, &pszQVar, &iValueLength);

	if ( !retval || !pszQVar )
		return 1;

	int iCurrent = 0;

	int iDatabase = ParamGetInt(m_iObjId, "NVTrapSetQVarCampaign", 0);

	if ( pQSrv->Exists(pszQVar) )
	{
		iCurrent = pQSrv->Get(pszQVar); // Get QVar value
	} else
	{
		pQSrv->Set(pszQVar, 0, iDatabase);
	}

//	SService<IDataSrv> pDataSrv(g_pScriptManager);

	NVMathsOperations* m = NVMathsOperations::getInstance();
	int iResult = m->calculate(cOperand, iCurrent, iValue);

	/*
	int iResult = iCurrent;
	switch ( cOperand )
	{
	case '=':
		iResult = iValue;
		break;
	case '+':
		iResult = iCurrent + iValue;
		break;
	case '-':
		iResult = iCurrent - iValue;
		break;
	case '*':
		iResult = iCurrent * iValue;
		break;
	case '/':
		if (iValue == 0 )
		{
			if (pszQVar)
				delete[] pszQVar;

			return 1;	// No divide-by-zero errors allowed here!
		}

		iResult = iCurrent / iValue;
		break;
	case '%':
		if (iValue == 0 )
		{
			if (pszQVar)
				delete[] pszQVar;

			return 1;	// No divide-by-zero errors allowed here!
		}

		iResult = iCurrent % iValue;
		break;
	case '{':
		iResult = iCurrent << iValue;
		break;
	case '}':
		iResult = iCurrent >> iValue;
		break;
	case '!':
	case '|':
		iResult = iCurrent | iValue;	// Not sure if this is the right operation?
		break;
	case '?':
		iResult = iCurrent + pDataSrv->RandInt(0, iValue);
		break;
	case 'r':
		iResult = pDataSrv->RandInt(0, iValue);
		break;
	case 'd':
		iResult = iCurrent + pDataSrv->RandInt(1, iValue);
		break;
	case '"':
		iResult = (iCurrent * 10) + (iValue % 10);
		break;
	default:
		DisplayPrintf("Unknown operand \"%c\"", cOperand);
	}
	*/

//	DisplayPrintf("NVTrapSetQVar.\nOperand is %c\nValue is %i\nQVar is %s\nValue was: %i\nValue is now: %i", cOperand, iValue, pszQVar, iCurrent, iResult);

	pQSrv->Set(pszQVar, iResult, iDatabase);

	if (pszQVar)
		delete[] pszQVar;
	return 0;
}

MSGHANDLER cScr_NVTrapSetQVar::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	char* pszQVar = NULL;
	int iValue = 0;
	char cOperand = 0;
	int iValueLength = 0;

	bool retval = GetQVarInfo(m_iObjId, &cOperand, &iValue, &pszQVar, &iValueLength);

	if ( !retval || !pszQVar )
		return 1;

	int iCurrent = 0;

	int iDatabase = ParamGetInt(m_iObjId, "NVTrapSetQVarCampaign", 0);

	if ( pQSrv->Exists(pszQVar) )
	{
		iCurrent = pQSrv->Get(pszQVar); // Get QVar value
	}

	SService<IDataSrv> pDataSrv(g_pScriptManager);

	NVMathsOperations* m = NVMathsOperations::getInstance();
	int iResult = m->calculateReverse(cOperand, iCurrent, iValue);

	/*
	int iResult = iCurrent;
	switch ( cOperand )
	{
	case '=':
		iResult = 0;
		break;
	case '+':
		iResult = iCurrent - iValue;
		break;
	case '-':
		iResult = iCurrent + iValue;
		break;
	case '*':
		if (iValue == 0 )
		{
			if (pszQVar)
				delete[] pszQVar;

			return 1;	// No divide-by-zero errors allowed here!
		}

		iResult = iCurrent / iValue;
		break;
	case '/':
		iResult = iCurrent * iValue;
		break;
	case '%':
		iResult = iCurrent * iValue;
		break;
	case '{':
		iResult = iCurrent >> iValue;
		break;
	case '}':
		iResult = iCurrent << iValue;
		break;
	case '!':
	case '|':
		iResult = iCurrent & ~iValue;	// Not sure if this is the right operation?
		break;
	case '?':
		iResult = iCurrent - pDataSrv->RandInt(0, iValue);
		break;
	case 'd':
		iResult = iCurrent - pDataSrv->RandInt(1, iValue);
		break;
	case '"':
		iResult = iCurrent / 10;
		break;
	default:
		DisplayPrintf("Unknown operand \"%c\"", cOperand);
	}
	*/

//	DisplayPrintf("NVTrapSetQVar (TurnOff).\nOperand is %c\nValue is %i\nQVar is %s\nValue was: %i\nValue is now: %i", cOperand, iValue, pszQVar, iCurrent, iResult);

	pQSrv->Set(pszQVar, iResult, iDatabase);

	if (pszQVar)
		delete[] pszQVar;

	return 0;
}

/***********************
 * NVTrigQVar
 */

MSGHANDLER cScr_NVTrigQVar::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	GetQVarName();
	if ( ParamGetInt(m_iObjId, "NVTrigQVarInitialised", 0) == 0)
	{
		ParamSetInt(m_iObjId, "NVTrigQVarInitialised", 1);
		SimpleSend(m_iObjId, m_iObjId, "QuestChange");
	}
	return 0;
}

void cScr_NVTrigQVar::GetQVarName()
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	int iValue = 0;
	char cOperand = 0;
	int iValueLength = 0;

	bool retval = GetQVarInfo(m_iObjId, &cOperand, &iValue, &m_szQVar, &iValueLength);
	if (!retval || !m_szQVar)
		return;

	pQSrv->SubscribeMsg(m_iObjId, m_szQVar, 0);
	pQSrv->SubscribeMsg(m_iObjId, m_szQVar, 1);

	return;
}

MSGHANDLER cScr_NVTrigQVar::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	if (m_szQVar)
	{
		pQSrv->UnsubscribeMsg(m_iObjId, m_szQVar);
//		delete[] m_szQVar;
	}
	return 0;
}

MSGHANDLER cScr_NVTrigQVar::OnQuestChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	char* pszQVar = NULL;
	int iValue = 0;
	char cOperand = 0;
	int iValueLength = 0;

	bool retval = GetQVarInfo(m_iObjId, &cOperand, &iValue, &pszQVar, &iValueLength);

	if ( !retval || !pszQVar )
		return 1;

	int iCurrent = 0;

	if ( pQSrv->Exists(pszQVar) )
	{
		iCurrent = pQSrv->Get(pszQVar); // Get QVar value
	}
	if (pszQVar)
		delete[] pszQVar;

	int iResult = 0;
	switch ( cOperand )
	{
	case '=':
		iResult = ( iCurrent == iValue );
		break;
	case '-':
		iValue = -iValue;
	case '+':
		m_iPrevResult = -1;
		if ( iCurrent == (m_iPrev + iValue) )
		{
			iResult = 1;
		} else {
			if ( iCurrent == (m_iPrev - iValue) )
			{
				iResult = 0;
			} else {
				return 0; // Do not send anything if the difference is not the specified value.
			}
		}
		break;
	// case '*':

		// break;
	// case '/':
		// if (iValue == 0 )
			// return 1;	// No divide-by-zero errors allowed here!

		// break;
	 case '%':
		if ( iValue )
			iResult = ( (iCurrent % iValue) == 0 );
		break;
	 case '}':
		m_iPrevResult = -1;
		if ( iCurrent >= (m_iPrev + iValue) )
		{
			iResult = 1;
		} else {
			if ( iCurrent <= (m_iPrev - iValue) )
			{
				iResult = 0;
			} else {
				return 0; // Do not send anything if the difference is less than the specified value.
			}
		}
		break;
	case '{':

		m_iPrevResult = -1;
		if ( iCurrent <= (m_iPrev - iValue) )
		{
			iResult = 1;
		} else {
			if ( iCurrent >= (m_iPrev + iValue) )
			{
				iResult = 0;
			} else {
				return 0; // Do not send anything if the difference is less than the specified value.
			}
		}
		break;
	case '>':
		iResult = ( iCurrent > iValue );
		break;
	case '<':
		iResult = ( iCurrent < iValue );
		break;
	case '&':
		iResult = ( iCurrent & iValue );
		break;
	case 'm':
	{
		int iCount;
		int iLength;
		for ( int m = 0; m < 10; ++m )
		{
			iCount = 0;
			iLength = 0;

			for ( int x = 1; x < iCurrent; x *= 10)
			{
				++iLength;
				if ( ( (iCurrent % (x*10)) / x ) == m )
				{
					++iCount;
				}
			}

			int iDesiredLength = NVParamGetInt(m_iObjId, m_szName, "MatchLength", 0);
			if (m == 0 && iLength < iDesiredLength)
			{
				iCount += (iDesiredLength - iLength);
			}

			if ( iCount == iValue )
			{
				iResult = 1;
				break;
			}
		}
		break;
	}
	case '"':
	{
		// C'mon, there must be a more effecient way of doing this. Can't think of it right now, though.
//		char szCheck[12], szCurrent[12];
//		sprintf(szCheck, "%i", iValue);
//		sprintf(szCurrent, "%i", iCurrent);
//
//		if ( !strcmp(szCheck, szCurrent+(strlen(szCurrent)-strlen(szCheck))) )
//			iResult = 1;

		if ((iCurrent % (int)pow(10.0,(double)iValueLength)) == iValue)
		{
			iResult = 1;
		}

/*		char szDigits[12];
		sprintf (szDigits, "%i", iValue);
		int iDigits = static_cast<int>(pow(10, strlen(szDigits)));
		if (iValue == 0)
			iDigits = 10;
		if ( ( iCurrent % iDigits ) == iValue )
		{
			iResult = 1;
		}
*/
		break;
	}
	default:
		DisplayPrintf("Unknown operand \"%c\"", cOperand);
		break;
	}

	if (( iResult != m_iPrevResult) || ( ParamGetInt(m_iObjId, "NVTrigQVarAllowRepeats", 0) ) )
	{
		Trigger((iResult!=0), pMsg);
	}

//DisplayPrintf("QVar changed to %i: Result=%i!\nPrevious Value was: %i\nPrevious result was: %i", iCurrent, iResult, static_cast<int>(m_iPrev), static_cast<int>(m_iPrevResult));
//DebugPrintf("QVar changed to %i: Result=%i!\nPrevious Value was: %i\nPrevious result was: %i", iCurrent, iResult, static_cast<int>(m_iPrev), static_cast<int>(m_iPrevResult));

	m_iPrev = iCurrent;
	m_iPrevResult = iResult;

	return 0;
}


/***********************
 * NVTweqFirstDoor
 */
MSGHANDLER cScr_NVTweqFirstDoor::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
//	DisplayPrintf("Frob.");
	if (pDoorSrv->GetDoorState(m_iObjId) == kDoorStateOpen)
	{
//		DisplayPrintf("Close the door.");
		// third frob: close door then rotate tweq back
		pDoorSrv->CloseDoor(m_iObjId);
	} else {
		// Now, calculate the angle:
		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrVec vFacing, vPosition, vPlayerPos;
		object oPlayer = StrToObject("Player");
		pOS->Facing(vFacing, m_iObjId);
		pOS->Position(vPosition, m_iObjId);
		pOS->Position(vPlayerPos, oPlayer);
		vPosition -= vPlayerPos;

		int iLongitude = static_cast<int>((630 - vFacing.z) - atan2(vPosition.x, vPosition.y) * 180/M_PI);
		iLongitude = iLongitude % 360;
		int iAngle1 = ParamGetInt(m_iObjId, "NVOpenableAngle1", 0);
		int iAngle2 = ParamGetInt(m_iObjId, "NVOpenableAngle2", 360);
		if (iAngle1 > iAngle2)
		{
			if ( !(( iLongitude > iAngle1 ) || ( iLongitude < iAngle2 )) )
			{
				DisplayPrintf("Cannot be opened from this angle.");
				return 0;
			}
		} else {
			if ( !(( iLongitude > iAngle1 ) && ( iLongitude < iAngle2 )) )
			{
				DisplayPrintf("Cannot be opened from this angle.");
				return 0;
			}
		}

//		DisplayPrintf("Thinking what to do...");
		int iAnimS = 0;
		if (pPropSrv->Possessed(m_iObjId, "StTweqJoints") )
		{
			cMultiParm mpAnimS;
			pPropSrv->Get(mpAnimS, m_iObjId, "StTweqJoints", "AnimS");
			iAnimS = static_cast<int>(mpAnimS);
		}

		if ( iAnimS & 2 ) // tweq is complete, in reverse mode
		{
//			DisplayPrintf("Open the door.");
			pDoorSrv->OpenDoor(m_iObjId);
		} else {
//			DisplayPrintf("Start the Tweq");
			pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", 1); // Activate Joints
		}
	}

	return 0;
}

MSGHANDLER cScr_NVTweqFirstDoor::OnClosed(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// close tweq
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", 3); // Activate/Reverse Joints
	return 0;
}


MSGHANDLER cScr_NVTweqFirstDoor::OnTweqComplete(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
/*	sTweqMsg* pTweqMsg = static_cast<sTweqMsg*>(pMsg);

	if ( pTweqMsg->Type == kTweqTypeJoints )
	{
		if ( pTweqMsg->Dir == kTweqDirForward )
		{
			DisplayPrintf("Tweq is opened.");
		}
	}
*/	return 0;
}

/***********************
 * NVActivatorRoom
 */
MSGHANDLER cScr_NVActivatorRoom::OnPlayerRoomEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	CDSend("NVActivate", m_iObjId);
	return 0;
}

MSGHANDLER cScr_NVActivatorRoom::OnPlayerRoomExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	CDSend("NVDeactivate", m_iObjId);
	return 0;
}

/***********************
 * NVTrigRoomPlayer
 */
MSGHANDLER cScr_NVTrigRoomPlayer::OnPlayerRoomEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	Trigger(true, pMsg);
	return 0;
}

MSGHANDLER cScr_NVTrigRoomPlayer::OnPlayerRoomExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	Trigger(false, pMsg);
	return 0;
}

/***********************
 * NVObjectWatcher
 */

MSGHANDLER cScr_NVObjectWatcher::OnActivated(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Make sure the timer is running...
	if (m_hTimerHandle == NULL)
	{
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVConstantRotate", ParamGetInt(m_iObjId, "NVObjectWatcherRate", 50), kSTM_Periodic, g_mpUndef);
	}

	return 0;;
}

MSGHANDLER cScr_NVObjectWatcher::OnDeactivated(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return 0;
}

MSGHANDLER cScr_NVObjectWatcher::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oDest = ParamGetNVObject(m_iObjId, "NVObjectToWatch", 0);
	if ( !oDest )
	{
		DisplayPrintf("No object!");
		return 0;
	}
	cScrVec vPosition, vPos, vDestPos, vRot;

	SService<IObjectSrv> pOS(g_pScriptManager);
	pOS->Position(vPos, m_iObjId);
	pOS->Position(vDestPos, oDest);
	pOS->Facing(vRot, oDest);
	vPosition = vPos - vDestPos;

	float fLongitude = (float)((270.00f) - atan2(vPosition.x, vPosition.y) * 180.00f/M_PI);
	while ( fLongitude >= 360.00f )
		fLongitude -= 360.00f;
//	iLongitude = iLongitude % 360;
	vRot.z = fLongitude;

	if ( !ParamGetInt(m_iObjId, "NVObjectWatcherZOnly", 0) )
	{

		float fLatitude = (float)((450.00f) - atan2(sqrt((vPosition.x * vPosition.x)+(vPosition.y * vPosition.y)), vPosition.z) * 180.00f/M_PI);
		while ( fLatitude >= 360.00f )
			fLatitude -= 360.00f;
	//	iLatitude = iLatitude % 360;
		vRot.y = fLatitude;
	}

		// DisplayPrintf("Object %i is at a %i° angle and a %i° (vertical) angle ", static_cast<int>(oDest), iLongitude, iLatitude);
	pOS->Teleport(m_iObjId, vPos, vRot, 0);

	return 0;
}


/***********************
 * NVMetaTrap
 */

MSGHANDLER cScr_NVMetaTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	char* pszParam = new char[strlen(m_szName) + 10];

	sprintf(pszParam, "%sOnDelay", m_szName);

	int iDelay = ParamGetInt(m_iObjId, pszParam, 0);
	if (iDelay)
	{
		sprintf(pszParam, "%sOn", m_szName);
		g_pScriptManager->SetTimedMessage2(m_iObjId, pszParam, iDelay, kSTM_OneShot
, g_mpUndef);
	} else {
		DoMeta(true, pMsg);
	}
	delete[] pszParam;
	return 0;
}

MSGHANDLER cScr_NVMetaTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	char* pszParam = new char[strlen(m_szName) + 10];

	sprintf(pszParam, "%sOffDelay", m_szName);

	int iDelay = ParamGetInt(m_iObjId, pszParam, 0);
	if (iDelay)
	{
		sprintf(pszParam, "%sOff", m_szName);
		g_pScriptManager->SetTimedMessage2(m_iObjId, pszParam, iDelay, kSTM_OneShot
, g_mpUndef);
	} else {
		DoMeta(false, pMsg);
	}
	delete[] pszParam;
	return 0;
}

void cScr_NVMetaTrap::DoMeta(bool bOn, sScrMsg* pMsg)
{
	char* pszParam = new char[strlen(m_szName) + 24];

	sprintf(pszParam, "%sQVar", m_szName);
	const char* pszQVar = ParamGetString(m_iObjId, pszParam, NULL);

	sprintf(pszParam, "%sMeta", m_szName);
	const char* pszMetaBase = ParamGetString(m_iObjId, pszParam, 0);

	if ( pszMetaBase == NULL )
		pszMetaBase = ParamGetString(m_iObjId, "NVMetaproperty", 0);

	int iMeta = 0;


	if ( pszQVar )
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);
		int iCurrent = 0; // Default value (if QVar doesn't exist)
		if (pQSrv->Exists(pszQVar))
		{
			iCurrent = pQSrv->Get(pszQVar); // Get QVar value
		}

		char* pszMeta = new char[strlen(pszMetaBase) + 12];
		sprintf(pszMeta, "%s%i", pszMetaBase, iCurrent);

		iMeta = StrToObject(pszMeta);

		delete[] pszMeta;
	} else {
		iMeta = StrToObject(pszMetaBase);
	}

	if ( iMeta )
	{
		const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
		std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

		if (bOn)
		{
			for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
			{
				AddSingleMetaProperty(iMeta, *it);
			}
		} else {
			for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
			{
				RemoveSingleMetaProperty(iMeta, *it);
			}
		}
	}

	delete[] pszParam;

	return;
}

MSGHANDLER cScr_NVMetaTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);

	char* pszParam = new char[strlen(m_szName) + 10];

	sprintf(pszParam, "%sOn", m_szName);
	if (!stricmp(pszMsgName, pszParam))
		DoMeta(true, pMsg);

	sprintf(pszParam, "%sOff", m_szName);
	if (!stricmp(pszMsgName, pszParam))
		DoMeta(false, pMsg);

	delete[] pszParam;
	return 0;
}


/***********************
 * NVParticleGroup
 */

MSGHANDLER cScr_NVParticleGroup::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPGroupSrv> pParticleSrv(g_pScriptManager);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		pParticleSrv->SetActive(*it, 1);
	}

	return 0;
}

MSGHANDLER cScr_NVParticleGroup::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPGroupSrv> pParticleSrv(g_pScriptManager);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		pParticleSrv->SetActive(*it, 0);
	}

	return 0;
}

/***********************
 * NVUnusableUnlessQVar
 */

MSGHANDLER cScr_NVUnusableUnlessQVar::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	GetQVarName();
	SimpleSend(m_iObjId, m_iObjId, "InitNVUnusableUnlessQVar");

	return 0;
}

void cScr_NVUnusableUnlessQVar::GetQVarName()
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	const char* pszQuestVar = ParamGetString(m_iObjId, "NVUnusableUnlessQVar", "HasBow");
	if (!pszQuestVar)
		return; // No qvar specifed! Abort.

	if ( m_szQVar != NULL )
		delete[] m_szQVar;

	m_szQVar = new char[strlen(pszQuestVar) +1];
	strcpy(m_szQVar, pszQuestVar);

	pQSrv->SubscribeMsg(m_iObjId, m_szQVar, 2);

}

MSGHANDLER cScr_NVUnusableUnlessQVar::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	if (m_szQVar)
	{
		pQSrv->UnsubscribeMsg(m_iObjId, m_szQVar);
	}
	return 0;
}

MSGHANDLER cScr_NVUnusableUnlessQVar::OnQuestChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_szQVar)
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);

		int iCurrent = 0;

		if ( pQSrv->Exists(m_szQVar) )
		{
			iCurrent = pQSrv->Get(m_szQVar); // Get QVar value
		}

		int iMeta = ParamGetObject(m_iObjId, "NVUnusableUnlessQVarMeta", 0);

		if ( iMeta )
		{
			if ( iCurrent )
			{
				RemoveSingleMetaProperty(iMeta, m_iObjId);
			} else {
				AddSingleMetaProperty(iMeta, m_iObjId);
			}
		} else {

			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			if ( iCurrent )
			{
				if (pPropSrv->Possessed(m_iObjId, "FrobInfo"))
				{
					pPropSrv->Remove(m_iObjId, "FrobInfo");
				}
			} else {
				pPropSrv->Set(m_iObjId, "FrobInfo", "Inv Action", 0);
			}
		}

		#if (_DARKGAME != 3)
			SService<IDarkUISrv> pUI(g_pScriptManager);
			object oWeapon;
			pUI->InvWeapon(oWeapon);

			if ((int)oWeapon == m_iObjId)
			{

				SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
				pDebugSrv->Command("clear_weapon", cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);

				pUI->InvSelect(oWeapon);
			}
		#endif
	} else {
		DebugPrintf("NVUnusableUnlessQVar: Invalid qvar specified");
	}

	return 0;
}

/***********************
 * NVTweqDevice
 */

MSGHANDLER cScr_NVTweqDevice::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	cMultiParm mpState;
	pPropSrv->Get(mpState, m_iObjId, "StTweqJoints", "AnimS");
	int iState = static_cast<int>(mpState);

	if ((iState != 1) && (iState != 2))
	{
		pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", 1); // Activate Joints

		char szName[16];
		for ( int i = 1; i <= 6; ++i )
		{
			sprintf(szName, "ControlJoint%i", i);
			if ( NVParamGetInt(m_iObjId, m_szName, szName, 0) ) {
				sprintf(szName, "Joint%iAnimS", i);
				pPropSrv->Set(m_iObjId, "StTweqJoints", szName, 1);
			}
		}

		SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
		true_bool bReturn;
		pSoundSrv->HaltSchema(bReturn, m_iObjId, "", 0);
		pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundOnObj SOUND_NET);
	}

	return 0;
}

MSGHANDLER cScr_NVTweqDevice::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	cMultiParm mpState;
	pPropSrv->Get(mpState, m_iObjId, "StTweqJoints", "AnimS");
	int iState = static_cast<int>(mpState);

	if ((iState == 1) || (iState == 2))
	{
		pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", 3); // Activate/Reverse Joints

		char szName[16];
		for ( int i = 1; i <= 6; ++i )
		{
			sprintf(szName, "ControlJoint%i", i);
			if ( NVParamGetInt(m_iObjId, m_szName, szName, 0) ) {
				sprintf(szName, "Joint%iAnimS", i);
				pPropSrv->Set(m_iObjId, "StTweqJoints", szName, 3);
			}
		}

		SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
		true_bool bReturn;
		pSoundSrv->HaltSchema(bReturn, m_iObjId, "", 0);
		pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Deactivate", m_iObjId, 0, kEnvSoundOnObj SOUND_NET);
	}

	return 0;
}

/***********************
 * NVTrigOBB
 */
MSGHANDLER cScr_NVTrigOBB::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(m_iObjId, kPhysEnterExit);
	return 0;
}

MSGHANDLER cScr_NVTrigOBB::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(m_iObjId, kPhysEnterExit);
	return 0;
}

MSGHANDLER cScr_NVTrigOBB::OnPhysEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	object oTransObj = pPhysMsg->transObj;

	if (isMatchingObject(oTransObj))
	{
		const char* pszMessage;
		pszMessage = NVParamGetString(m_iObjId, m_szName, "EnterMessage", "NVTrigOBBEnter");
		SimpleSend(m_iObjId, oTransObj, pszMessage);

		Trigger(true, pMsg);
	}
	return 0;
}

MSGHANDLER cScr_NVTrigOBB::OnPhysExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	object oTransObj = pPhysMsg->transObj;

	if (isMatchingObject(oTransObj))
	{
		const char* pszMessage;
		pszMessage = NVParamGetString(m_iObjId, m_szName, "ExitMessage", "NVTrigOBBExit");
		SimpleSend(m_iObjId, oTransObj, pszMessage);

		Trigger(false, pMsg);
	}

	return 0;
}

bool cScr_NVTrigOBB::isMatchingObject(object oTransObj)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bBool;
	object oArc = ParamGetNVObject(m_iObjId, "NVTrigOBBArchetype", 0);

	if (oArc != 0) {
		pOS->InheritsFrom(bBool, oTransObj, oArc);
	} else {
		#if (_DARKGAME != 3)
			pOS->Named(oArc, "avatar");
		#else
			pOS->Named(oArc, "the player");
		#endif
		pOS->InheritsFrom(bBool, oTransObj, oArc);
		if (!bBool)
		{
			if (!ParamGetInt(m_iObjId, "NVTrigOBBPlayer", 0))
			{
				#if (_DARKGAME != 3)
					pOS->Named(oArc, "creature");
				#else
					pOS->Named(oArc, "monsters");
				#endif
				pOS->InheritsFrom(bBool, oTransObj, oArc);
			}
		}
	}

	if (bBool) {
		return true;
	} else {
		return false;
	}
}

/***********************
 * NVMachineSlot
 */
MSGHANDLER cScr_NVMachineSlot::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysContact);

	return 0;
}

MSGHANDLER cScr_NVMachineSlot::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysContact);
	return 0;
}

MSGHANDLER cScr_NVMachineSlot::OnPhysContactCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iContactObj = pPhysMsg->contactObj;

	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bBool;
	object oArc;
	#if (_DARKGAME != 3)
		pOS->Named(oArc, "avatar");
	#else
		pOS->Named(oArc, "the player");
	#endif
	pOS->InheritsFrom(bBool, iContactObj, oArc);
	if (bBool)
		return 0;

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link lLink;
	sLink slLink;

	if (GetOneLinkByData("~ScriptParams", m_iObjId, 0, &slLink, "Slot", -1))
	{
		pLS->Create(lLink, pLTS->LinkKindNamed("ScriptParams"), slLink.dest, iContactObj);
		if ( lLink )
		{
			pLTS->LinkSetData(lLink, NULL, "InSlot");

			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			SService<IObjectSrv> pOS(g_pScriptManager);

			pOS->Teleport(iContactObj, cScrVec::Zero, cScrVec::Zero, 0);
			pPropSrv->Set(iContactObj, "HasRefs", NULL, false);
		}
	}
//	DisplayPrintf("%i came into contact with me", iContactObj);
	return 0;
}

/***********************
 * NVMachine
 */

MSGHANDLER cScr_NVMachine::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sLink slOLink;
	if (!GetOneLinkByData("ScriptParams", m_iObjId, 0, &slOLink, "Output", -1))
		return 1;

	object oOutput = slOLink.dest;
	bool bManufacture = false;
	bool bSuccess = false;

	char szParamName[22];
	object oItem = 0;
	object oArc;
	true_bool bBool;
	bool bPartAvailable = false;
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cScrVec vOffset;


	for (int i=0;i<99;i++)
	{
		sprintf(szParamName, "NVMachineItem%i", i);
		oItem = ParamGetObject(m_iObjId, szParamName, 0);
		bManufacture = true;

		if (oItem)
		{
			std::vector<long> vParts;

			for (int j=0;j<10;j++)
			{
				sprintf(szParamName, "NVMachineItem%iPart%i", i, j);
				oArc = ParamGetObject(m_iObjId, szParamName, 0);

				if (oArc != 0)
				{
					bPartAvailable = false;
					linkset lsLinks;
					pLS->GetAll(lsLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, 0);

					for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
					{

						sLink sl = lsLinks.Get();
						void* pLinkData = lsLinks.Data();
						if (pLinkData)
						{
							if ( !stricmp(reinterpret_cast<const char*>(pLinkData), "InSlot") )
							{
								pOS->InheritsFrom(bBool, sl.dest, oArc);
								if ( bBool )
								{
									bPartAvailable = true;
									vParts.insert(vParts.begin(), sl.dest);
									break;
								}

							}
						}
					}
					if ( !bPartAvailable)
					{
						bManufacture = false;
						vParts.clear();
						break;
					}
				} else {
					break;
				}

			}

			if (bManufacture == true )
			{
				sprintf(szParamName, "NVMachineBuilt%i", i);
				SimplePost(m_iObjId, m_iObjId, szParamName);
				bSuccess = true;

				object oOutputItem;
				pOS->Create(oOutputItem, oItem);
				pOS->Teleport(oOutputItem, vOffset, cScrVec::Zero, oOutput);
				vOffset.z += 0.01f;
				if (!vParts.empty())
				{
					std::vector<long>::const_iterator p,q;
					for (p = vParts.begin(), q = vParts.end(); p != q; ++p)
					{
						pOS->Destroy(*p);
					}
				}
			}
			vParts.clear();
		} else {
			break;
		}
	}

	// Now output any leftovers
	linkset lsOutLinks;
	pLS->GetAll(lsOutLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, 0);

	for (; lsOutLinks.AnyLinksLeft(); lsOutLinks.NextLink())
	{

		sLink sl = lsOutLinks.Get();
		void* pLinkData = lsOutLinks.Data();
		if (pLinkData)
		{
			if ( !stricmp(reinterpret_cast<const char*>(pLinkData), "InSlot") )
			{
				pOS->Teleport(sl.dest, vOffset, cScrVec::Zero, oOutput);
				pPropSrv->Set(sl.dest, "HasRefs", NULL, true);
				pPropSrv->Add(sl.dest, "PhysType");
				pLS->Destroy(lsOutLinks.Link());
				vOffset.z += 0.01f;

			}
		}
	}

	if ( bSuccess )
	{
		SimplePost(m_iObjId, m_iObjId, "NVMachineSuccess");
	} else {
		SimplePost(m_iObjId, m_iObjId, "NVMachineFailed");
	}

	return 0;
}

MSGHANDLER cScr_NVMachine::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVDifficultyRelay
 */

MSGHANDLER cScr_NVDifficultyRelay::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	InterateDifficultyLinks(true);
	return 0;
}

MSGHANDLER cScr_NVDifficultyRelay::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	InterateDifficultyLinks(false);
	return 0;
}


void cScr_NVDifficultyRelay::InterateDifficultyLinks(bool bOn)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	int iDiff = -1; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("Difficulty"))
	{
		iDiff = pQSrv->Get("Difficulty"); // Get QVar value
	}

	if (iDiff == -1)
		return;

	char szDiff[16];
	sprintf(szDiff, "Difficulty%i", iDiff);

	linkset lsLinks;
	linkkind lkFlavor = pLTS->LinkKindNamed("ScriptParams");
	pLS->GetAll(lsLinks, lkFlavor, m_iObjId, 0);


	char* pszParam = new char[strlen(m_szName) + 4];

	const char* pszMessage;
	if ( bOn)
	{
		sprintf(pszParam, "%sTOn", m_szName);
		pszMessage = ParamGetString(m_iObjId, pszParam, "TurnOn");
	} else {
		sprintf(pszParam, "%sTOff", m_szName);
		pszMessage = ParamGetString(m_iObjId, pszParam, "TurnOff");
	}
	delete[] pszParam;

	SService<IActReactSrv> pARSrv(g_pScriptManager);

	float fIntensity = 0;
	char pszStimName[80];
	int iIsStim = sscanf(pszMessage, "[%f]%[^\n]", &fIntensity, pszStimName);
	object oStim = 0;

	if ( 2 == iIsStim )
	{
		oStim = StrToObject(pszStimName);
		if (!oStim )
		{
			return;
		}
	}


	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{

		cMultiParm mpReturn;
		pLTS->LinkGetData(mpReturn, lsLinks.Link(), NULL);

		sLink sl = lsLinks.Get();


		if ( !stricmp(mpReturn, szDiff) )
		{
			if (oStim)
			{
				#if (_DARKGAME != 1)
					pARSrv->Stimulate(sl.dest, oStim, fIntensity, m_iObjId);
				#else // Backwards compatibility for T1. Fairly pointless since the module is set to only compile for _DARKGAME 2.
					pARSrv->Stimulate(sl.dest, oStim, fIntensity);
				#endif
			} else {
				SimpleSend(m_iObjId, sl.dest, pszMessage);
			}
		}
	}

	return;
}

/***********************
 * NVLaserBeam
 */
MSGHANDLER cScr_NVLaserBeam::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVLaserBeam::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVLaserBeam::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iCollObj = pPhysMsg->collObj;


	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);

	int iSource = ParamGetNVObject(m_iObjId, "NVLaserBeamSource", 0);

	if ( iSource == 0 )
	{
		cMultiParm mpProp;
		if (pPropSrv->Possessed(m_iObjId, "Firer"))
		{
			pPropSrv->Get(mpProp, m_iObjId, "Firer", NULL);
		} else if (pPropSrv->Possessed(m_iObjId, "JointPos"))
		{
			pPropSrv->Get(mpProp, m_iObjId, "JointPos", "Joint 6");
		} else {
			return 1;
		}

		iSource = static_cast<int>(mpProp);
	}

	true_bool bExists;
	pOS->Exists(bExists, iSource);
	if (!bExists)
	{
		DebugPrintf("NVLaserBeam: No firer. Aborting");
		return 1;
	}

	#if ( _DARKGAME == 3 )
		if ( iSource == StrToObject("Player") )
			iSource = StrToObject("Player Gun");
	#endif

	cScrVec vPosition, vPosSource, vScale(1,1,1), vCentrePoint, vRot;
	pOS->Position(vPosition, m_iObjId);
	pOS->Position(vPosSource, iSource);

	if ( ParamGetInt(m_iObjId, "NVLaserBeamCentreOnTarget", 1) )
	{
		if (iCollObj > 0)
		{
			true_bool bBool, bBool2;
			pOS->InheritsFrom(bBool, iCollObj, -1423); // 'Texture' archetype
			if ( !bBool )
			{
				pOS->InheritsFrom(bBool2, iCollObj, -2098); // 'Avatar' archetype
				if ( !bBool2 )
					pOS->Position(vPosition, iCollObj);
			}
		}
	}

	if ( ParamExists(m_iObjId, "NVLaserBeamHitOffset") )
	{
		const char* pszOffsetVector = ParamGetString(m_iObjId, "NVLaseBeamrHitOffset", "0,0,0");
		mxs_vector vHitOffsetVector;
		if (3 == sscanf(pszOffsetVector, "%f, %f, %f", &vHitOffsetVector.x, &vHitOffsetVector.y, &vHitOffsetVector.z))
		{
			vPosition.x += vHitOffsetVector.x;
			vPosition.y += vHitOffsetVector.y;
			vPosition.z += vHitOffsetVector.z;
		}
	}

//	DisplayPrintf("I hit %i", iCollObj);

	float fDistance = GetDistance(&vPosition, &vPosSource);
	vScale.z = fDistance;

	vPosition -= vPosSource;

	float fLongitude = static_cast<float>(630 - (atan2(vPosition.x, vPosition.y) * 180/M_PI));
	while (fLongitude >= 360)
		fLongitude -= 360;

	float fLatitude = static_cast<float>(720 - atan2(sqrt((vPosition.x * vPosition.x)+(vPosition.y * vPosition.y)), vPosition.z) * 180/M_PI);
	while (fLatitude >= 360)
		fLatitude -= 360;

//	DisplayPrintf("Angle is %f degrees longitude, %f degrees latitude\nDistance is %f units", fLongitude, fLatitude, fDistance);

	char* szLaserName = new char[18];
	sprintf(szLaserName, "NV-%i-Laser", iSource);

	// We should create a laser object if none exists
	object oLaser;
	pOS->Named(oLaser, szLaserName);
	if ( ( !oLaser ) || (ParamGetInt(m_iObjId, "NVLaserBeamAllowMultiple", 0)) )
	{
		pOS->Create(oLaser, -1);
		pOS->SetName(oLaser, szLaserName);
		if (pPropSrv->Possessed(m_iObjId, "ModelName"))
			pPropSrv->CopyFrom(oLaser, "ModelName", m_iObjId); // Copy model from projectile to laser

		pPropSrv->Set(oLaser, "RenderType", NULL, 2); // set render type to 'unlit'

		if (pPropSrv->Possessed(m_iObjId, "RenderAlpha"))
			pPropSrv->CopyFrom(oLaser, "RenderAlpha", m_iObjId); // Copy transparency from projectile to laser
		int iMeta = ParamGetObject(m_iObjId, "NVLaserMeta", 0);
		if ( iMeta )
		{
			AddSingleMetaProperty(iMeta, oLaser);
		}

		if ( ParamGetInt(m_iObjId, "NVLaserBeamDestroyWithSource", 0) )
		{
			SService<ILinkSrv> pLS(g_pScriptManager);
			SService<ILinkToolsSrv> pLTS(g_pScriptManager);
			link lLink;
			pLS->Create(lLink, pLTS->LinkKindNamed("~ParticleAttachement"), iSource, oLaser);
		}
	}

	pPropSrv->Set(oLaser, "Scale", NULL, vScale);

	vCentrePoint.x = vPosSource.x + (vPosition.x / 2);
	vCentrePoint.y = vPosSource.y + (vPosition.y / 2);
	vCentrePoint.z = vPosSource.z + (vPosition.z / 2);
	vRot.z = fLongitude;
	vRot.y = fLatitude;

	pOS->Teleport(oLaser, vCentrePoint, vRot, 0);

	if ( ParamExists(m_iObjId, "NVLaserOffset") )
	{
		const char* pszOffsetVector = ParamGetString(m_iObjId, "NVLaserOffset", "0,0,0");
		mxs_vector vVector;
		if (3 == sscanf(pszOffsetVector, "%f, %f, %f", &vVector.x, &vVector.y, &vVector.z))
		{
			pOS->Teleport(oLaser, vVector, cScrVec::Zero, oLaser);
		}
	}

	return 0;
}


/***********************
 * NVReflector
 */
MSGHANDLER cScr_NVReflector::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVReflector::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVReflector::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iCollObj = pPhysMsg->collObj;
	if (pPhysMsg->time < 1)
		return 1; // Don't do anything outside of sim time!

	if (!ParamGetObject(iCollObj, "NVReflectable", 0))
		return 0;

	SInterface<ITraitManager> pTraits(g_pScriptManager);
	int iArchetype = 0;
	iArchetype = pTraits->GetArchetype(iCollObj);
	if (!iArchetype)
		return 0;

	SService<IObjectSrv> pOS(g_pScriptManager);
	cScrVec vFacing, vObjFacing;
	pOS->Facing(vFacing, m_iObjId);
	pOS->Facing(vObjFacing, iCollObj);

	cScrVec vObjPos, vPosition, vDifference;
	pOS->Position(vPosition, m_iObjId);
	pOS->Position(vObjPos, iCollObj);
	vDifference = vPosition - vObjPos;
	pOS->Teleport(iCollObj, vPosition, vObjFacing, 0);

	float fAngle = static_cast<float>((720 - vFacing.z) - atan2(vDifference.x, vDifference.y) * 180/M_PI);
	while (fAngle >= 360)
		fAngle -= 360;

	if ((fAngle < 270) && ( fAngle > 90 ))
		return 0;

	cScrVec vNewRot;
//	cScrVec vNewPos;
//	vNewPos.x = (vPosition.x - (vObjPos.x - vPosition.x));
//	vNewPos.y = (vPosition.y - (vObjPos.y - vPosition.y));
//	vNewPos.z = (vPosition.z - (vObjPos.z - vPosition.z));

//	vNewRot.x = 360 - (vObjFacing.x - vFacing.x);
//	vNewRot.y = 360 - (vObjFacing.y - vFacing.y);
	vNewRot.x = vObjFacing.x;
	vNewRot.y = vObjFacing.y;
	vNewRot.z = (360 - (vObjFacing.z - vFacing.z)) + vFacing.z;
//	vNewRot.z = (360 - vFacing.z) - (vObjFacing.z - vFacing.z);

	while (vNewRot.z >= 360)
		vNewRot.z -= 360;

	while (vNewRot.z < 0)
		vNewRot.z += 360;

	char* szBounceName = new char[19];
	sprintf(szBounceName, "NV-%i-Bounce", m_iObjId);

	// We should create a laser object if none exists
	object oBounce;
	pOS->Named(oBounce, szBounceName);
	if ( !oBounce )
	{
		pOS->Create(oBounce, -36);
		pOS->SetName(oBounce, szBounceName);
	}
	delete[] szBounceName;

	vObjPos.z -= 0.05f;
	pOS->Teleport(oBounce, vPosition, vNewRot, 0);

	SService<IPhysSrv> pPhysSrv(g_pScriptManager);

	cScrVec vecVel;
	object oEmit;
	pPhysSrv->LaunchProjectile(oEmit, oBounce, iArchetype, 1.0, 0, vecVel);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->Set(oEmit, "Firer", NULL, static_cast<int>(oBounce));
	pPropSrv->Set(oEmit, "JointPos", "Joint 6", static_cast<int>(oBounce));

	//DisplayPrintf("Ow! I was hit by a %i. Reflecting.", iArchetype);

	if (ParamGetInt(m_iObjId, "NVSendOnOffOnReflect", 1))
		SimpleSend(m_iObjId, m_iObjId, "TurnOn");

	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	int iTimer = ParamGetInt(m_iObjId, "KillReflectionDelay", 500);
	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "KillReflection", iTimer, kSTM_Periodic, g_mpUndef);


	return 0;
}

MSGHANDLER cScr_NVReflector::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "KillReflection"))
		return 0; // Abort if it's not the right timer.

	if (ParamGetInt(m_iObjId, "NVSendOnOffOnReflect", 1))
		SimpleSend(m_iObjId, m_iObjId, "TurnOff");

	char* szObjName = new char[20];
	object oObj;
	SService<IObjectSrv> pOS(g_pScriptManager);

	sprintf(szObjName, "NV-%i-Bounce", m_iObjId);
	pOS->Named(oObj, szObjName);
//	if ( oObj )
//		pOS->Destroy(oObj);

	sprintf(szObjName, "NV-%i-Laser", static_cast<int>(oObj));
	pOS->Named(oObj, szObjName);
	if ( oObj )
	{
		pOS->Destroy(oObj);
	}

	delete[] szObjName;
	return 0;
}

/***********************
 * NVAmbientSoundTrap
 */

MSGHANDLER cScr_NVAmbientSoundTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<ISoundScrSrv> pSndSrv(g_pScriptManager);

	link lLink;
	sLink slLink;
	true_bool bRet;
	pLS->GetOne(lLink, pLTS->LinkKindNamed("SoundDescription"), m_iObjId, 0);
	if ( lLink)
	{
		pLTS->LinkGet(lLink, slLink);
		pSndSrv->PlaySchemaAmbient(bRet, m_iObjId, slLink.dest SOUND_NET);
	}
	return 0;
}

MSGHANDLER cScr_NVAmbientSoundTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
	true_bool bRet;
	pSndSrv->HaltSchema(bRet, m_iObjId, "", 0);
	return 0;
}


/***********************
 * NVAttachOnCollide
 */

MSGHANDLER cScr_NVAttachOnCollide::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	 return 0;
}

MSGHANDLER cScr_NVAttachOnCollide::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVAttachOnCollide::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iCollObj = pPhysMsg->collObj;

	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bBool;
	pOS->InheritsFrom(bBool, iCollObj, ParamGetObject(m_iObjId, "NVAttachOnCollider", 0));
	if (! bBool)
		return 0;

	cScrVec vFacing, vObjFacing;
	pOS->Facing(vFacing, m_iObjId);
	pOS->Facing(vObjFacing, iCollObj);

	cScrVec vObjPos, vPosition, vDifference;
	pOS->Position(vPosition, m_iObjId);
	pOS->Position(vObjPos, iCollObj);
	vDifference = vPosition - vObjPos;

	float fAngle = static_cast<float>((720 - vFacing.z) - atan2(vDifference.x, vDifference.y) * 180/M_PI);
	while (fAngle >= 360)
		fAngle -= 360;

	int iObjId = 0;
	if ((fAngle < 270) && ( fAngle > 90 ))
	{
		// DisplayPrintf("An object hit me from the front!");
		iObjId = ParamGetObject(m_iObjId, "NVAttachOnCollide1", 0);
	} else {
		// DisplayPrintf("An object hit me from the back!");
		iObjId = ParamGetObject(m_iObjId, "NVAttachOnCollide2", 0);
	}

	if (!iObjId)
		return 1;

	object oCreated;
	pOS->Create(oCreated, iObjId);

	if (oCreated)
	{
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		link lLink;
		pLS->Create(lLink, pLTS->LinkKindNamed("~ParticleAttachement"), m_iObjId, oCreated);
	}

	return 0;
}

/***********************
 * NVSortPuzzle
 */

MSGHANDLER cScr_NVSortPuzzle::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	float fSize = ParamGetFloat(m_iObjId, "NVSortPuzzleSize", 2.00f);
	link lLink;
	sLink slLink;

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->GetOne(lLink, pLTS->LinkKindNamed(CDLINK), m_iObjId, 0);
	if ( !lLink)
		return 1;

	pLTS->LinkGet(lLink, slLink);

	SService<IObjectSrv> pOS(g_pScriptManager);
	object oDest = slLink.dest;

	cScrVec vDestPos, vPosition, vDifference;
	pOS->Position(vPosition, m_iObjId);
	pOS->Position(vDestPos, oDest);
	vDifference = vPosition - vDestPos;

	float fDist = fabs(vDifference.x);
	fDist += fabs(vDifference.y);

	if (fDist == fSize)
	{
		m_vDest = &vDestPos;
		pOS->Teleport(oDest, vPosition, cScrVec::Zero, 0);
		if (m_hTimerHandle == NULL)
		{ // Start a timer.
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "SortPuzzle", 10, kSTM_Periodic, g_mpUndef);
		}
		//pOS->Teleport(m_iObjId, static_cast<mxs_vector>(*m_vDest), cScrVec::Zero, 0);
	}

	return 0;
}

MSGHANDLER cScr_NVSortPuzzle::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "SortPuzzle"))
		return 0; // Abort if it's not the right timer.

	// Do Stuff

	SService<IObjectSrv> pOS(g_pScriptManager);

	cScrVec vDestPos, vPosition, vMovePos;
	vDestPos = *m_vDest;
	pOS->Position(vPosition, m_iObjId);
	vMovePos.z = vPosition.z;

	if ((vPosition.x + 0.10f) < vDestPos.x)
		vMovePos.x = vPosition.x + 0.10f;
	else if ((vPosition.x - 0.10f) > vDestPos.x)
		vMovePos.x = vPosition.x - 0.10f;
	else
		vMovePos.x = vDestPos.x;

	if ((vPosition.y + 0.10f) < vDestPos.y)
		vMovePos.y = vPosition.y + 0.10f;
	else if ((vPosition.y - 0.10f) > vDestPos.y)
		vMovePos.y = vPosition.y - 0.10f;
	else
		vMovePos.y = vDestPos.y;

	pOS->Teleport(m_iObjId, vMovePos, cScrVec::Zero, 0);

	if ((vDestPos.x == vMovePos.x) && (vDestPos.y == vMovePos.y) )
	{
		if (m_hTimerHandle != NULL)
		{	// Stop timer:
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = NULL;
		}
	}

	const char* pszFieldValue = ParamGetString(m_iObjId, "NVSortPuzzleDest", NULL);

	if (!pszFieldValue)
		return 1;

	mxs_vector vVector;
	if (3 != sscanf(pszFieldValue, "%f, %f, %f", &vVector.x, &vVector.y, &vVector.z))
	{
		return 1;
	}

	if ((vVector.x == vMovePos.x) && (vVector.y == vMovePos.y) )
	{
		CDSend("TurnOn", m_iObjId);
	} else {
		CDSend("TurnOff", m_iObjId);
	}


	return 0;
}

MSGHANDLER cScr_NVSortPuzzle::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0)
		Startup();

	return 0;
}

MSGHANDLER cScr_NVSortPuzzle::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		Startup();
	}

	return 0;
}

void cScr_NVSortPuzzle::Startup(void)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	cScrVec vPosition;
	pOS->Position(vPosition, m_iObjId);
	m_vDest = &vPosition;

	g_pScriptManager->SetTimedMessage2(m_iObjId, "SortPuzzle", 10, kSTM_OneShot, g_mpUndef);
	return;
}


/***********************
 * NVRotatable
 */

MSGHANDLER cScr_NVRotatable::OnStart(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	int iAnimS = 0;
	if (pPropSrv->Possessed(m_iObjId, "StTweqRotate") )
	{
		cMultiParm mpAnimS;
		pPropSrv->Get(mpAnimS, m_iObjId, "StTweqRotate", "AnimS");
		iAnimS = (~static_cast<int>(mpAnimS) & 2);
	}
	pPropSrv->Set(m_iObjId, "StTweqRotate", "AnimS", iAnimS | 1);

	float fInitialSpeed = ParamGetFloat(m_iObjId, "NVRotatableInitialSpeed", 0.00);
	if (fInitialSpeed)
	{
		if (pPropSrv->Possessed(m_iObjId, "CfgTweqRotate") )
		{
			cMultiParm mpSpeed;
			pPropSrv->Get(mpSpeed, m_iObjId, "CfgTweqRotate", "z rate-low-high");
			mxs_vector vSpeed;
			vSpeed.x = fInitialSpeed;
			vSpeed.y = 0.00;
			vSpeed.z = static_cast<const mxs_vector*>(mpSpeed)->z;
			pPropSrv->Set(m_iObjId, "CfgTweqRotate", "z rate-low-high", vSpeed);

			if (m_hSpeedTimerHandle != NULL)
			{	// Stop speed-shift timer:
				g_pScriptManager->KillTimedMessage(m_hSpeedTimerHandle);
				m_hSpeedTimerHandle = NULL;
			}
			m_hSpeedTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVRotateSpeedShift", 250, kSTM_Periodic, g_mpUndef);
		}
	}


	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	SimpleSend(m_iObjId, m_iObjId, "StartRotate");
//	pPropSrv->Set(m_iObjId, "StTweqRotate", "Axis 3AnimS", iAnimS | 1);
	return 0;
}

MSGHANDLER cScr_NVRotatable::OnStop(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	int iAnimS = 0;
	if (pPropSrv->Possessed(m_iObjId, "StTweqRotate") )
	{
		cMultiParm mpAnimS;
		pPropSrv->Get(mpAnimS, m_iObjId, "StTweqRotate", "AnimS");
		iAnimS = (static_cast<int>(mpAnimS) & ~1);
	}
	pPropSrv->Set(m_iObjId, "StTweqRotate", "AnimS", iAnimS);

	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	if (m_hTimerHandle != NULL)
	{	// Stop speed-shift timer:
		g_pScriptManager->KillTimedMessage(m_hSpeedTimerHandle);
		m_hSpeedTimerHandle = NULL;
	}

	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "StopRotate", 1000, kSTM_OneShot, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVRotatable::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVRotateSpeedShift"))
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		float fMaxSpeed = ParamGetFloat(m_iObjId, "NVRotatableMaxSpeed", 20.00);

		if (pPropSrv->Possessed(m_iObjId, "CfgTweqRotate") )
		{
			cMultiParm mpSpeed;
			pPropSrv->Get(mpSpeed, m_iObjId, "CfgTweqRotate", "z rate-low-high");
			mxs_vector vSpeed;
			vSpeed.x = static_cast<const mxs_vector*>(mpSpeed)->x;
			vSpeed.y = 0.00;
			vSpeed.z = static_cast<const mxs_vector*>(mpSpeed)->z;

			if (vSpeed.x < fMaxSpeed)
			{
				vSpeed.x += ParamGetFloat(m_iObjId, "NVRotatableAcceleration", 0.10f);
				pPropSrv->Set(m_iObjId, "CfgTweqRotate", "z rate-low-high", vSpeed);
			} else {
				if (m_hTimerHandle != NULL)
				{	// Stop speed-shift timer:
					g_pScriptManager->KillTimedMessage(m_hSpeedTimerHandle);
					m_hSpeedTimerHandle = NULL;
				}
			}
		}
	}

	if (!stricmp(pszMsgName, "StopRotate"))
	{
		SimpleSend(m_iObjId, m_iObjId, "StopRotate");
	}

	return 0;
}

/***********************
 * NVStackTrap
 */

MSGHANDLER cScr_NVStackTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SInterface<IContainSys> pContainSys(g_pScriptManager);

	int iStack = ParamGetInt(m_iObjId, "NVStackTrapQuantity", 1);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		pContainSys->StackAdd(*it, iStack, 1);
		#if ( _DARKGAME == 3 )
			SService<IShockGameSrv> pShockGame(g_pScriptManager);
			pShockGame->RefreshInv();
		#endif
	}
	return 0;
}

MSGHANDLER cScr_NVStackTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SInterface<IContainSys> pContainSys(g_pScriptManager);

	int iStack = ParamGetInt(m_iObjId, "NVStackTrapQuantity", 1);


	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		pContainSys->StackAdd(*it, -iStack, 1);
		#if ( _DARKGAME == 3 )
			SService<IShockGameSrv> pShockGame(g_pScriptManager);
			pShockGame->RefreshInv();
		#endif
	}
	return 0;
}

/***********************
 * NVSpikePitRoom
 */
MSGHANDLER cScr_NVSpikePitRoom::OnPlayerRoomEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle == NULL)
	{
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "SpikePitRoom", 250, kSTM_Periodic, g_mpUndef);
	}
	return 0;
}

MSGHANDLER cScr_NVSpikePitRoom::OnPlayerRoomExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	Trigger(false, pMsg);
	return 0;
}

MSGHANDLER cScr_NVSpikePitRoom::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "SpikePitRoom"))
		return 1;

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);

	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed(CDLINK), m_iObjId, 0);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		sLink sl = lsLinks.Get();

		object oObj = ParamGetNVObject(sl.dest, "NVProximityObj", StrToObject("Player"));
		if (oObj < 1)
			continue;

		cScrVec vPosition, vPlayerPos;
		pOS->Position(vPosition, sl.dest);

		if ( ParamExists(sl.dest, "NVOrigin") )
		{
			const char* pszVector = ParamGetString(sl.dest, "NVOrigin", "0,0,0");
			mxs_vector vVector;
			if (3 == sscanf(pszVector, "%f, %f, %f", &vVector.x, &vVector.y, &vVector.z))
				vPosition = vVector;
		}

		pOS->Position(vPlayerPos, oObj);

		float fProximity = ParamGetFloat(sl.dest, "NVProximity", 6.00f);

		if ( ParamGetInt(sl.dest, "NVProximityType", 0 ) ==1 )
		{
			if ( ( fabs(vPosition.x - vPlayerPos.x) < fProximity ) && ( fabs(vPosition.y - vPlayerPos.y) < fProximity ) )
				Trigger(true, pMsg, sl.dest);
			else
				Trigger(false, pMsg, sl.dest);

		} else  {
			float fDistance = GetDistanceXY(&vPosition, &vPlayerPos);

			if (fDistance < fProximity)
				Trigger(true, pMsg, sl.dest);
			else
				Trigger(false, pMsg, sl.dest);

		}

	}
	return 0;
}

/***********************
 * NVParticleTrailProjectile
 */

MSGHANDLER cScr_NVParticleTrailProjectile::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVParticleTrailProjectile::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVParticleTrailProjectile::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
//	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
//	int iCollObj = pPhysMsg->collObj;

	cScrVec vRot;

	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->Set(m_iObjId, "PhysControl", "Controls Active", 24); // Lock Location/Rotation
	pPropSrv->Set(m_iObjId, "PhysAIColl", NULL, false);
	pPropSrv->Set(m_iObjId, "CollisionType", NULL, 0);
	pPropSrv->Set(m_iObjId, "RenderType", NULL, 1);

	if ( !ParamGetInt(m_iObjId, "NVParticleTrailProjectileKeepLights", 0) )
		if (pPropSrv->Possessed(m_iObjId, "SelfLit"))
		{
			pPropSrv->Set(m_iObjId, "SelfLit", NULL, 0);
			cScrVec vOffset(-ParamGetFloat(m_iObjId, "NVParticleTrailProjectileKeepLightsOffset", 1.00f),0.0f,0.0f);
			pOS->Teleport(m_iObjId, vOffset, cScrVec::Zero, m_iObjId);
		}

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->BroadcastOnAllLinks(m_iObjId, "TurnOff", pLTS->LinkKindNamed("~ParticleAttachement"));
	g_pScriptManager->SetTimedMessage2(m_iObjId, "DestroyMeLater", 6000, kSTM_OneShot, g_mpUndef);


	object oClone;
	pOS->Create(oClone, m_iObjId);
	if (ParamGetInt(m_iObjId, "NVUnrotatedProjectileSplash", 0))
	{
		pOS->Facing(vRot, oClone);
		vRot.x = -vRot.x;
		vRot.y = -vRot.y;
		vRot.z = -vRot.z;
	}
	pOS->Teleport(oClone, cScrVec::Zero, vRot, m_iObjId);
	SService<IDamageSrv> pDmgS(g_pScriptManager);
	pDmgS->Slay(oClone, m_iObjId);

	return 0;
}

MSGHANDLER cScr_NVParticleTrailProjectile::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "DestroyMeLater"))
	{
		SService<IObjectSrv> pOS(g_pScriptManager);
		pOS->Destroy(m_iObjId);
	}

	return 0;
}


/***********************
 * NVCloneContactFrob
 */

MSGHANDLER cScr_NVCloneContactFrob::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobber = pFrobMsg->Frobber;

	SService<IActReactSrv> pARSrv(g_pScriptManager);

	object oClone;
	SService<IObjectSrv> pOS(g_pScriptManager);
	pOS->Create(oClone, m_iObjId);
	if (oClone)
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		SService<IObjectSrv> pOS(g_pScriptManager);

		pOS->Teleport(oClone, cScrVec::Zero, cScrVec::Zero, 0);
		pPropSrv->Set(oClone, "HasRefs", NULL, false);
//		DisplayPrintf("Creating AR contact between %i and %i", static_cast<int>(oClone), iFrobber);
		pARSrv->BeginContact(oClone, iFrobber);
	}

	return 0;
}


/***********************
 * NVPhantomTrap
 */
/*
MSGHANDLER cScr_NVPhantomTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	DoFade(true, pMsg->from);
	return 0;
}

MSGHANDLER cScr_NVPhantomTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	DoFade(false, pMsg->from);
	return 0;
}

void cScr_NVPhantomTrap::DoFade(bool bOn, int messageSource)
{
	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = getObjects(pszTarget, m_iObjId, messageSource);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		float fCurrent = 1.00f;
		int iTimer = 100;
		if (pPropSrv->Possessed(m_iObjId, "RenderAlpha"))
		{
			cMultiParm mpReturn;
			pPropSrv->Get(mpReturn, *it, "RenderAlpha", NULL);
			fCurrent = static_cast<float>(mpReturn);
		}

		float fDest = 0.00f;
		bool bFast = false;
		if (bOn)
		{
			fDest = ParamGetFloat(m_iObjId, "NVPhantomAlphaMax", 1.00f);
			iTimer = ParamGetInt(m_iObjId, "NVPhantomFadeOn", 1000) / 50;
			if (iTimer < 100)
			{
				bFast = true;
				iTimer *= 3;
			}
			if (fCurrent < fDest)
			{
				fCurrent += (bFast ? 0.06f : 0.02f);
				pPropSrv->Set(*it, "RenderAlpha", NULL, fCurrent);
				m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "PhantomUp", iTimer, kSTM_OneShot, g_mpUndef);
			} else {
				m_hTimerHandle = NULL;
				SimplePost(*it, *it, "PhantomOn");
			}
		} else {
			fDest = ParamGetFloat(m_iObjId, "NVPhantomAlphaMin", 0.00f);
			iTimer = ParamGetInt(m_iObjId, "NVPhantomFadeOff", 1000) / 50;
			if (iTimer < 100)
			{
				bFast = true;
				iTimer *= 3;
			}
			if (fCurrent > fDest)
			{
				fCurrent -= (bFast ? 0.06f : 0.02f);
				pPropSrv->Set(*it, "RenderAlpha", NULL, fCurrent);
				m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "PhantomDown", iTimer, kSTM_OneShot, g_mpUndef);
			} else {
				m_hTimerHandle = NULL;
				SimplePost(*it, *it, "PhantomOff");
			}
		}

		if ( !ParamGetInt(m_iObjId, "NVPhantomRenderInvisible", 1 ) )
		{
			if ( fCurrent <= 0.01f )
				pPropSrv->Set(*it, "RenderType", NULL, 1);
			else
				pPropSrv->Set(*it, "RenderType", NULL, ParamGetInt(m_iObjId, "NVPhantomRenderType", 0));
		}
	}

	return;
}

MSGHANDLER cScr_NVPhantomTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "PhantomUp"))
		DoFade(true, pMsg->from);
	else if (!stricmp(pszMsgName, "PhantomDown"))
		DoFade(false, pMsg->from);

	return 0;
}
*/
/***********************
 * NVTwoSpeedDoor
 */

MSGHANDLER cScr_NVTwoSpeedDoor::OnDoorOpening(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (pPropSrv->Possessed(m_iObjId, "TransDoor"))
	{ // Set base speed of translating door.
		pPropSrv->Set(m_iObjId, "TransDoor", "Base Speed", ParamGetFloat(m_iObjId, "NVDoorOpeningSpeed", 1.00f));
	} else { // Check if it's a rotating door.
		if (pPropSrv->Possessed(m_iObjId, "RotDoor"))
		{ // Set base speed of rotating door.
			pPropSrv->Set(m_iObjId, "RotDoor", "Base Speed", ParamGetFloat(m_iObjId, "NVDoorOpeningSpeed", 1.00f));
		} else {
			return 1; // Eeek, this isn't a door!
		}
	}

		return 0;
}

MSGHANDLER cScr_NVTwoSpeedDoor::OnDoorClosing(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (pPropSrv->Possessed(m_iObjId, "TransDoor"))
	{ // Set base speed of translating door.
		pPropSrv->Set(m_iObjId, "TransDoor", "Base Speed", ParamGetFloat(m_iObjId, "NVDoorClosingSpeed", 1.00f));
	} else { // Check if it's a rotating door.
		if (pPropSrv->Possessed(m_iObjId, "RotDoor"))
		{ // Set base speed of rotating door.
			pPropSrv->Set(m_iObjId, "RotDoor", "Base Speed", ParamGetFloat(m_iObjId, "NVDoorClosingSpeed", 1.00f));
		} else {
			return 1; // Eeek, this isn't a door!
		}
	}

		return 0;
}

/***********************
 * NVTextureTrap
 */

MSGHANDLER cScr_NVTextureTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	char* pszParam = new char[strlen(m_szName) + 10];
	sprintf(pszParam, "%sDelay0", m_szName);
	int iTimer = ParamGetInt(m_iObjId, pszParam, 0);
	delete[] pszParam;
	if (iTimer == 0)
	{
		DoTexture();
	} else {
		g_pScriptManager->SetTimedMessage2(m_iObjId, "NVTextureTrap", iTimer, kSTM_OneShot, g_mpUndef);
	}
	return 0;
}

MSGHANDLER cScr_NVTextureTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	return 0;
}

MSGHANDLER cScr_NVTextureTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVTextureTrap"))
		return 0; // Abort if it's not the right timer.

	DoTexture();
	return 0;
}

void cScr_NVTextureTrap::DoTexture(void)
{
	char* pszParam = new char[strlen(m_szName) + 10];
	sprintf(pszParam, "%s%i", m_szName, static_cast<int>(m_iStage));
	const char* pszOldTexture = ParamGetString(m_iObjId, pszParam, NULL);

	if (!pszOldTexture)
	{
		delete[] pszParam;
		return;
	}

	//snprintf(pszParam, strlen(pszParam)-1, "%s%i", m_szName, (static_cast<int>(m_iStage)+1));
	sprintf(pszParam, "%s%i", m_szName, (static_cast<int>(m_iStage)+1));
	const char* pszNewTexture = ParamGetString(m_iObjId, pszParam, NULL);
	if (!pszNewTexture)
	{
		sprintf(pszParam, "%sLoop", m_szName);
		if ( ParamGetInt(m_iObjId, pszParam, 0) )
		{
			sprintf(pszParam, "%s0", m_szName);
			pszNewTexture = ParamGetString(m_iObjId, pszParam, NULL);
			if (!pszNewTexture)
			{
				delete[] pszParam;
				return;
			} else {
				m_iStage = -1;
			}
		} else {
			delete[] pszParam;
			return;
		}
	}

	SService<IAnimTextureSrv> pAnimTexSrv(g_pScriptManager);
	pAnimTexSrv->ChangeTexture(m_iObjId, NULL, pszOldTexture, NULL, pszNewTexture);
	m_iStage++;

	sprintf(pszParam, "%sDelay%i", m_szName, static_cast<int>(m_iStage));
	int iTimer = ParamGetInt(m_iObjId, pszParam, 0);
	if (iTimer == 0)
	{
		sprintf(pszParam, "%sDelay", m_szName);
		iTimer = ParamGetInt(m_iObjId, pszParam, 250);
	}
	SimpleSend(m_iObjId, m_iObjId, "TextureChange");
	delete[] pszParam;

	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVTextureTrap", iTimer, kSTM_OneShot, g_mpUndef);

	return;
}


// MSGHANDLER cScr_NVTextureTrap::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
// {
	// char* pszParam = new char[strlen(m_szName) + 10];
	// sprintf(pszParam, "%s%i", m_szName, static_cast<int>(m_iStage));
	// const char* pszOldTexture = ParamGetString(m_iObjId, pszParam, NULL);

	// if (!pszOldTexture)
		// return 0;

	// sprintf(pszParam, "%s0", m_szName);
	// const char* pszNewTexture = ParamGetString(m_iObjId, pszParam, NULL);
	// if (!pszNewTexture)
		// return 0;

	// SService<IAnimTextureSrv> pAnimTexSrv(g_pScriptManager);
	// pAnimTexSrv->ChangeTexture(m_iObjId, NULL, pszOldTexture, NULL, pszNewTexture);
	// m_iStage = 0;

	// return 0;
// }

/***********************
 * NVSimpleSchemaPlayerTrap
 */

MSGHANDLER cScr_NVSimpleSchemaPlayerTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
	object oSchema = ParamGetObject(m_iObjId, "NVSchema", 0);
	if (!oSchema)
		return 1;

	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bRet;
	int iPlayType = ParamGetInt(m_iObjId, "NVSchemaPlayType", 0);
	switch ( iPlayType )
	{
		case 0:
			pSoundSrv->PlaySchemaAtObject(bRet, m_iObjId, oSchema, m_iObjId SOUND_NET);
			break;
		case 1:
			pSoundSrv->PlaySchemaAmbient(bRet, m_iObjId, oSchema SOUND_NET);
			break;
	}

	return 0;
}

MSGHANDLER cScr_NVSimpleSchemaPlayerTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ISoundScrSrv> pSndSrv(g_pScriptManager);
	true_bool bRet;
	pSndSrv->HaltSchema(bRet, m_iObjId, "", 0);
	return 0;
}

/***********************
 * NVDiminishingParticles
 */

MSGHANDLER cScr_NVDiminishingParticles::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	g_pScriptManager->SetTimedMessage2(m_iObjId, "DiminishParticles", ParamGetInt(m_iObjId, "NVDiminishInterval", 250), kSTM_Periodic, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVDiminishingParticles::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Can't be deactivated!
	return 0;
}

MSGHANDLER cScr_NVDiminishingParticles::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "DiminishParticles"))
		return 0; // Abort if it's not the right timer.


	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if (!pPropSrv->Possessed(m_iObjId, "PGLaunchInfo"))
	{
		return 1; // This object doesn't have SFX->Particles. Abort.
	}

	cMultiParm mpTime1;
	pPropSrv->Get(mpTime1, m_iObjId, "PGLaunchInfo", "Min time");
	float fTime1 = static_cast<float>(mpTime1);

	cMultiParm mpTime2;
	pPropSrv->Get(mpTime2, m_iObjId, "PGLaunchInfo", "Max time");
	float fTime2 = static_cast<float>(mpTime2);

	int iCount = 1;
	if (pPropSrv->Possessed(m_iObjId, "ParticleGroup"))
	{
		cMultiParm mpCount;
		pPropSrv->Get(mpCount, m_iObjId, "ParticleGroup", "number of particles");
		iCount = static_cast<int>(mpCount);

		iCount -= ParamGetInt(m_iObjId, "NVDiminishAmount", 1);
		if (iCount < 0)
			iCount = 0;
		pPropSrv->Set(m_iObjId, "ParticleGroup", "number of particles", iCount);
	}

	fTime1 -= 0.01f;
	fTime2 -= 0.01f;
	if (fTime1 < 0)
		fTime1 = 0.00f;
	if (fTime2 < 0)
		fTime2 = 0.00f;

	pPropSrv->Set(m_iObjId, "PGLaunchInfo", "Min Time", fTime1);
	pPropSrv->Set(m_iObjId, "PGLaunchInfo", "Max Time", fTime2);

	if (((fTime1 + fTime2) == 0.00f) || (iCount == 0))
	{
		SService<IDamageSrv> pDmgS(g_pScriptManager);
		pDmgS->Slay(m_iObjId, 0);
	}

	return 0;
}


/***********************
 * NVDeleteTrap
 */

MSGHANDLER cScr_NVDeleteTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IObjectSrv> pOS(g_pScriptManager);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", TARGET_CDLINK);
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		if ( *it != m_iObjId )
		{
			true_bool bExists;
			pOS->Exists(bExists, *it);
			if (bExists)
				pOS->Destroy(*it);
		}
	}

	if ( ParamGetInt(m_iObjId, "NVDeleteTrapDeleteSelf", 0) )
	{
		pOS->Destroy(m_iObjId);
	}

	return 0;
}

MSGHANDLER cScr_NVDeleteTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Do nothing.
	return 0;
}


/***********************
 * NVSetEmissionRateHack
 */

MSGHANDLER cScr_NVSetEmissionRateHack::OnStart(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oObj = ParamGetNVObject(m_iObjId, "NVEmitterToHack", m_iObjId);
	if (oObj)
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		pPropSrv->Set(oObj, "CfgTweqEmit", "Rate", ParamGetInt(m_iObjId, "NVEmitterHackSpeedOn", 150));
	}

	return 0;
}

MSGHANDLER cScr_NVSetEmissionRateHack::OnStop(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oObj = ParamGetNVObject(m_iObjId, "NVEmitterToHack", m_iObjId);
	if (oObj)
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		pPropSrv->Set(oObj, "CfgTweqEmit", "Rate", ParamGetInt(m_iObjId, "NVEmitterHackSpeedOff", 1000));
	}

	return 0;
}

/***********************
 * NVTurningSquare
 */
MSGHANDLER cScr_NVTurningSquare::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Subscribe to PhysEnter/PhysExit messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysEnterExit);

	return 0;
}

MSGHANDLER cScr_NVTurningSquare::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		Startup();
	}

	return 0;
}

MSGHANDLER cScr_NVTurningSquare::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time > 0)
	{
		Startup();
	}

	return 0;
}

void cScr_NVTurningSquare::Startup()
{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		if ( pPropSrv->Possessed(m_iObjId, "PhysDims") )
		{
			cMultiParm mpReturn;
			mxs_vector vSize = {0.0, 0.0, 0.0};
			pPropSrv->Get(mpReturn, m_iObjId, "PhysDims", "Size");
			vSize.x = static_cast<const mxs_vector*>(mpReturn)->x;
			vSize.y = static_cast<const mxs_vector*>(mpReturn)->y;
			vSize.z = static_cast<const mxs_vector*>(mpReturn)->z;
			m_vSize = &vSize;
		}

		return;
}

MSGHANDLER cScr_NVTurningSquare::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Unsubscribe to PhysEnter/PhysExit messages.
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysEnterExit);
	return 0;
}

MSGHANDLER cScr_NVTurningSquare::OnPhysEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);

	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPhysSrv> pPhysS(g_pScriptManager);
	object oPlayer;
#if (_DARKGAME == 3) // Grab any player that crossed the threshhold for SS2.
	SService<INetworkingSrv> pNetSrv(g_pScriptManager);
	if ( !pNetSrv->IsPlayer(pPhysMsg->transObj) )
		return 0;
	oPlayer = pPhysMsg->transObj;
#else // Grab the player for Thief 2
	pOS->Named(oPlayer, "Player");
	if (oPlayer)
		if ( pPhysMsg->transObj != oPlayer ) // This script only affects the player.
			return 0;
#endif
	// Adjust BBox size
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if ( pPropSrv->Possessed(m_iObjId, "PhysDims") )
	{
		const mxs_vector* vMSize = m_vSize;
		cScrVec vSize(vMSize->x + 5.0, vMSize->y + 5.0, vMSize->z + 5.0);
		pPropSrv->Set(m_iObjId, "PhysDims", "Size", vSize);
	}

	// Get player's position, velocity, and facing. Also get TurningSquare's facing.
	cScrVec vPlayerFacing, vPlayerPos, vPlayerVel, vPosition;
	pOS->Facing(vPlayerFacing, oPlayer);
	pOS->Position(vPlayerPos, oPlayer);
	pOS->Position(vPosition, m_iObjId);
	pPhysS->GetVelocity(oPlayer, vPlayerVel);

	// Invert the player's X and Y velocity:
	vPlayerVel.x = (0 - vPlayerVel.x);
	vPlayerVel.y = (0 - vPlayerVel.y);

	// Invert the player's X and Y position relative to the TurningSquare.
	vPlayerPos.x = (vPosition.x - (vPlayerPos.x - vPosition.x));
	vPlayerPos.y = (vPosition.y - (vPlayerPos.y - vPosition.y));

	// Increase the player's facing by 180°
	float fPlayerFacing = (vPlayerFacing.z + 180);
	if (fPlayerFacing >= 360 )
	{
		fPlayerFacing -= 360; // Modulus only works on intergers.
	}
	vPlayerFacing.z = fPlayerFacing;

	// Apply new position, facing, and velocity values to the player.
	pOS->Teleport(oPlayer, vPlayerPos, vPlayerFacing, 0);
	pPhysS->SetVelocity(oPlayer, vPlayerVel);

	return 0;
}

MSGHANDLER cScr_NVTurningSquare::OnPhysExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if ( pPropSrv->Possessed(m_iObjId, "PhysDims") )
	{
		cScrVec vSize = *m_vSize;
		pPropSrv->Set(m_iObjId, "PhysDims", "Size", vSize);

	}

	return 0;
}

/***********************
 * NVFakeDeathTrap
 */
/*
MSGHANDLER cScr_NVFakeDeathTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDarkGameSrv> pDarkSrv(g_pScriptManager);
	pDarkSrv->FadeToBlack(5);

	SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
	pDebugSrv->Command("crouch", "", "", "", "", "", "", "");
	pDebugSrv->Command("clear_weapon", "", "", "", "", "", "", "");
	pDebugSrv->Command("+leanright", "", "", "", "", "", "", "");

	return 0;
}

MSGHANDLER cScr_NVFakeDeathTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Do nothing.
	return 0;
}
*/


/***********************
 * NVVineableDoor
 */
MSGHANDLER cScr_NVVineableDoor::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
/*
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	sLink slLink;
	// link lLink;

	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;


	// pLS->GetOne(lLink, pLTS->LinkKindNamed("Contains"), m_iObjId, 0);

	// if (lLink)
	if (GetOneLinkByData("ScriptParams", m_iObjId, 0, &slLink, "VineAttach", -1))
	{
		SService<IDataSrv> pDataSrv(g_pScriptManager);
		cScrStr strError;
		pDataSrv->GetString(strError, "NVVineableDoor.str", "VineError", "", "Strings");
		DisplayPrintf("%s", static_cast<const char*>(strError));

		if (!strError.IsEmpty())
		{
			strError.Free();
		}


	} else {
		if (pDoorSrv->GetDoorState(m_iObjId) == kDoorStateOpen)
		{
			pDoorSrv->CloseDoor(m_iObjId);
		} else {
			SService<ILockSrv> pLock(g_pScriptManager);
			if ( !pLock->IsLocked(m_iObjId) )
				pDoorSrv->OpenDoor(m_iObjId);
			else {
				pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Reject, Operation OpenDoor", m_iObjId, 0, kEnvSoundAmbient SOUND_NET);
			}

		}
	}

*/
	return 0;
}

MSGHANDLER cScr_NVVineableDoor::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	 return 0;
}

MSGHANDLER cScr_NVVineableDoor::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVVineableDoor::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iCollObj = pPhysMsg->collObj;

	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link lLink;

	true_bool bBool;
	pOS->InheritsFrom(bBool, iCollObj, StrToObject("VineArrow")	);
	if (! bBool)
		return 0;


	object oArrow;
	pOS->Create(oArrow, iCollObj);
	if (!oArrow)
		return 1;

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	pPropSrv->Set(oArrow, "Scripts", "Script 0", "NVVineableDoorHelper");

	if (pPropSrv->Possessed(oArrow, "PhysType"))
	{
		pPropSrv->Remove(oArrow, "PhysType");
	}

	// pLS->Create(lLink, pLTS->LinkKindNamed("Contains"), m_iObjId, oArrow);
	// if ( lLink )
	// {
		// pLTS->LinkSetData(lLink, NULL, -3);
	// }
	pLS->Create(lLink, pLTS->LinkKindNamed("Lock"), m_iObjId, oArrow);
	pLS->Create(lLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oArrow);
	pPropSrv->Set(oArrow, "Locked", NULL, true);
	if ( lLink )
	{
		pLTS->LinkSetData(lLink, NULL, "VineAttach");
	}
	pPropSrv->Set(oArrow, "HasRefs", NULL, true);

	pOS->Teleport(oArrow, cScrVec::Zero, cScrVec::Zero, iCollObj);

	object oArc;
	object oVine;
	pOS->Named(oArc, "VineArrowVine");
	if ( oArc )
	{

		pOS->Create(oVine, oArc);
		pLS->Create(lLink, pLTS->LinkKindNamed("Owns"), oArrow, oVine);
		pOS->Teleport(oVine, cScrVec::Zero, cScrVec::Zero, iCollObj);

		pOS->Named(oArc, "VineClump");
		if (oArc)
		{
			object oClump;
			pOS->Create(oClump, oArc);
			pLS->Create(lLink, pLTS->LinkKindNamed("Owns"), oArrow, oClump);
			pOS->Teleport(oClump, cScrVec::Zero, cScrVec::Zero, iCollObj);
			pPropSrv->Set(oClump, "RenderAlpha", NULL, 1.00);
		}
	}
	pOS->Destroy(iCollObj);

	return 0;
}


/***********************
 * NVVineableDoorHelper
 */
MSGHANDLER cScr_NVVineableDoorHelper::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->Set(m_iObjId, "Locked", NULL, false);
	IterateLinksByData("~ScriptParams", m_iObjId, 0, "VineAttach", -1, DestroyLinksIterFunc, NULL, NULL);
	IterateLinks("~Lock", m_iObjId, 0, DestroyLinksIterFunc, this, NULL);
	return 0;
}

int cScr_NVVineableDoorHelper::DestroyLinksIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	pLS->Destroy(pLQ->ID());
	return 1;
}


/***********************
 * NVTrigContained
 */

MSGHANDLER cScr_NVTrigContained::OnContained(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (pMsg->time == 0 )
		return 0;

	sContainedScrMsg * pContainedMsg = static_cast<sContainedScrMsg *>(pMsg);
	#if ( _DARKGAME != 3 )
		object oPlayer = StrToObject("Player");
		if (oPlayer)
			if ( pContainedMsg->container != oPlayer ) // This script only affects the player.
				return 0;
	#else
		SService<INetworkingSrv> pNetSrv(g_pScriptManager);
		if ( !pNetSrv->IsPlayer(pContainedMsg->container) )
			return 0;
	#endif

	if (pContainedMsg->event == 2 ) // Picked up
	{
		Trigger(true, pMsg);
	} else if (pContainedMsg->event == 3) {// Dropped / thrown
		Trigger(false, pMsg);
	}

	return 0;
}

#if (_DARKGAME != 3)
/***********************
 * NVSwapSword
 */

MSGHANDLER cScr_NVSwapSword::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobber = pFrobMsg->Frobber;

	SService<IObjectSrv> pOS(g_pScriptManager);
	SInterface<IContainSys> pContainSys(g_pScriptManager);

	object oSwordArc;
	pOS->Named(oSwordArc, ParamGetString(m_iObjId, "NVSwapSwordTarget", "Sword"));
	if (oSwordArc)
	{
		int iSword = GetSinglePossession(iFrobber, oSwordArc);
		if ( iSword )
		{
			object oFakeSword;
			pOS->Create(oFakeSword, -1);
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			pPropSrv->Set(oFakeSword, "Scripts", "Script 0", "NVSwapSwordHelper");
			pPropSrv->Set(oFakeSword, "ModelName", NULL, "Sword");
			if (pPropSrv->Possessed(iSword, "ModelName"))
			{
				pPropSrv->CopyFrom(oFakeSword, "ModelName", iSword);
			}
			pOS->Teleport(oFakeSword, cScrVec::Zero, cScrVec::Zero, m_iObjId);

			pContainSys->Add(oFakeSword, iSword, 0, 0);
		}

		pContainSys->Add(iFrobber, m_iObjId, 0, 0);
	}
	return 0;
}

/***********************
 * NVSwapSwordHelper
 */

MSGHANDLER cScr_NVSwapSwordHelper::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	g_pScriptManager->SetTimedMessage2(m_iObjId, "SwapSwordHelper", 2000, kSTM_OneShot, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVSwapSwordHelper::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "SwapSwordHelper"))
	{
		SService<IObjectSrv> pOS(g_pScriptManager);
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);

		sLink slLink;
		link lLink;
		pLS->GetOne(lLink, pLTS->LinkKindNamed("Contains"), m_iObjId, 0);
		if ( lLink)
		{
			pLTS->LinkGet(lLink, slLink);

			pOS->Teleport(slLink.dest, cScrVec::Zero, cScrVec::Zero, m_iObjId);
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			pLS->Destroy(lLink);
			pPropSrv->Set(slLink.dest, "HasRefs", NULL, true);
			pPropSrv->Set(slLink.dest, "RenderType", NULL, 0);
			pOS->Destroy(m_iObjId);
		}
	}

	return 0;
}

#endif

/***********************
 * NVInvSound
 */

MSGHANDLER cScr_NVInvSound::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
	true_bool bReturn;
	pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundAmbient SOUND_NET);
	return 0;
}

#if (_BUILD != 1 )

/***********************
 * NVDirectionalControl
 */

MSGHANDLER cScr_NVDirectionalControl::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	Activate();
	return 0;
}

MSGHANDLER cScr_NVDirectionalControl::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	Deactivate();
	return 0;
}

MSGHANDLER cScr_NVDirectionalControl::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVDirectionalControl"))
		return 0; // Abort if it's not the right timer.

	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);

	object oPlayer;
	pOS->Named(oPlayer, "Player");

	cScrVec vVelocity, vFacing;
	pOS->Facing(vFacing, oPlayer);
	pPhysSrv->GetVelocity(oPlayer, vVelocity);

	if ( vVelocity.x && vVelocity.y )
	{
		int iLongitude = static_cast<int>((810 - vFacing.z) - atan2(vVelocity.x, vVelocity.y) * 180/M_PI);
		iLongitude = iLongitude % 360;

		if ( ( iLongitude <= 45 ) || ( iLongitude >= 315 ) )
		{
			RegisterMovement(0);
			// DisplayPrintf("Up\n(%f, %f)\n%i degrees)", vVelocity.x, vVelocity.y, iLongitude);
		} else if ( iLongitude <= 135 )
		{
			RegisterMovement(1);
			// DisplayPrintf("Left\n(%f, %f)\n%i degrees)", vVelocity.x, vVelocity.y, iLongitude);
		} else if ( iLongitude <= 225 )
		{
			RegisterMovement(2);
			// DisplayPrintf("Down\n(%f, %f)\n%i degrees)", vVelocity.x, vVelocity.y, iLongitude);
		} else
		{
			RegisterMovement(3);
			// DisplayPrintf("Right.\n(%f, %f)\n%i degrees)", vVelocity.x, vVelocity.y, iLongitude);
		}
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVDirectionalControl", ParamGetInt(m_iObjId, "DirectionalControlInterval", 200), kSTM_Periodic, g_mpUndef);
	} else {
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVDirectionalControl", 25, kSTM_Periodic, g_mpUndef);
	}

	// cScrVec vPos = *m_vPos;
	// pOS->Teleport(oPlayer, vPos, vFacing, 0);

	return 0;
}

void cScr_NVDirectionalControl::RegisterMovement(int iDirection)
{
	switch ( iDirection)
	{
	case 0:
		DisplayPrintf("Up");
		CDSend("Up", m_iObjId);
		break;
	case 1:
		DisplayPrintf("Left");
		CDSend("Left", m_iObjId);
		break;
	case 2:
		DisplayPrintf("Down");
		CDSend("Down", m_iObjId);
		break;
	case 3:
		DisplayPrintf("Right");
		CDSend("Right", m_iObjId);
		break;
	}

	return;
}

void cScr_NVDirectionalControl::Activate(void)
{
	DisplayPrintf("Directional Control Activated.");

	if (m_hTimerHandle == NULL)
	{
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVDirectionalControl", 25, kSTM_Periodic, g_mpUndef);
	}

	#if (_DARKGAME != 3)
		SService<IDarkInvSrv> pSrv(g_pScriptManager);
	#else
		SService<IShockGameSrv> pSrv(g_pScriptManager);
	#endif
	pSrv->AddSpeedControl("NVDirectionalControl", 0.001f, 1.00f);

	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);

	object oPlayer;
	pOS->Named(oPlayer, "Player");

	pPhysSrv->SetVelocity(oPlayer, cScrVec::Zero);

	return;
}

void cScr_NVDirectionalControl::Deactivate(void)
{
	#if (_DARKGAME != 3)
		SService<IDarkInvSrv> pSrv(g_pScriptManager);
	#else
		SService<IShockGameSrv> pSrv(g_pScriptManager);
	#endif

	if (m_hTimerHandle != NULL)
	{
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	pSrv->RemoveSpeedControl ("NVDirectionalControl");

	DisplayPrintf("Directional Control Deactivated.");
	return;
}

/***********************
 * NVConversation
 */

 MSGHANDLER cScr_NVConversation::OnConverse(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	std::multiset<sConversationOption*, mConversationOptionSorter>::iterator p;
	int i = 0;
	for (p = s.begin(); p != s.end(); ++p, i++)
	{
		if (i == static_cast<int>(m_iState))
		{
			if ( (*p)->Object != 0 )
				SimpleSend(m_iObjId, (*p)->Object, "TurnOn");

			Deactivate();
		}
	}

	return 0;
}

void cScr_NVConversation::RegisterMovement(int iDirection)
{
	switch ( iDirection )
	{
	case 0:
		// Go up
		DisplayInterface(-1);
		break;
	case 2:
		// Go down
		DisplayInterface(1);
		break;
	default:
		return;
	}

	DisplayInterface(0);
};

void cScr_NVConversation::Activate(void)
{
	cScr_NVDirectionalControl::Activate();

	// Read in conversation option objects / links

	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	SInterface<IContainSys> pContainSys(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, 0);

	s.clear();
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		sLink sl = lsLinks.Get();

		int iPriority = 2048;
		int iDest = sl.dest;

		void* pLinkData = lsLinks.Data();
		if (pLinkData)
		{
			iPriority = strtol(reinterpret_cast<const char*>(pLinkData),NULL,10);
		}

		s.insert(new sConversationOption(iDest, iPriority));
	}
	s.insert(new sConversationOption(0, 9999));

	// End of conversation option read

	object oConversationHelper;
	pOS->Create(oConversationHelper, -1);


	pPropSrv->Set(oConversationHelper, "Scripts", "Script 0", "NVConversationHelper");
	pPropSrv->Set(oConversationHelper, "ModelName", NULL, "waypt");

	pPropSrv->Set(oConversationHelper, "FrobInfo", "Inv Action", 2);

	object oPlayer;
	pOS->Named(oPlayer, "Player");
	pContainSys->Add(oPlayer, oConversationHelper, 0, 0);

	link lLink;
	pLS->Create(lLink, pLTS->LinkKindNamed("Owns"), m_iObjId, oConversationHelper);

	DisplayInterface(0);
	return;
}

void cScr_NVConversation::Deactivate(void)
{
	cScr_NVDirectionalControl::Deactivate();
	DisplayPrintf(" ");

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);

	linkset lsLinks;
	linkkind lkFlavor = pLTS->LinkKindNamed("Owns");
	pLS->GetAll(lsLinks, lkFlavor, m_iObjId, 0);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		sLink sl = lsLinks.Get();
		pOS->Destroy(sl.dest);
	}
	return;
}

void cScr_NVConversation::DisplayInterface(int iMovement)
{
	// std::vector<std::string> vOptions;

	// vOptions.push_back("Hello                   ");
	// vOptions.push_back("Who are you?       ");
	// vOptions.push_back("What is the time? ");
	// vOptions.push_back("END                    ");

	// std::set<sConversationOption*>::iterator q;

	// for ( q = s.begin(); q != s.end(); ++q)
	// {
		// DebugPrintf("Object %i is at priority %i",  (*q)->Object, (*q)->Priority);
	// }

	std::string sOutput = "";

	if ( iMovement )
		m_iState += iMovement;

	if ( m_iState < 0 )
		m_iState = 0;

	if ( static_cast<unsigned long>(m_iState) >= s.size() )
		m_iState = s.size() -1;

	int i = 0;
	std::multiset<sConversationOption*, mConversationOptionSorter>::iterator p;
	for (p = s.begin(); p != s.end(); ++p)
	{
		if (i == m_iState)
			sOutput += ">>>>> ";
		else
			sOutput += "        ";

		const char* pszOptionLabel;
		if ((*p)->Object )
			pszOptionLabel = ParamGetString((*p)->Object, "NVConversationOption", "{{MISSING CONVERSATION OPTION LABEL}}");
		else
			pszOptionLabel = "END";

		sOutput += pszOptionLabel;

		if (i == m_iState)
			sOutput += " <<<<<";
		else
			sOutput += "        ";

		sOutput += '\n';

		i++;
	}

	DisplayPrintf("%s", sOutput.c_str());

	return ;
}

/***********************
 * NVConversationHelper
 */

MSGHANDLER cScr_NVConversationHelper::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->BroadcastOnAllLinks(m_iObjId, "Converse", pLTS->LinkKindNamed("~Owns"));
	return 0;
}
#endif

/***********************
 * NVSimpleDoor
 */
MSGHANDLER cScr_NVSimpleDoor::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);

	if (pDoorSrv->GetDoorState(m_iObjId) == kDoorStateOpen)
	{
		pDoorSrv->CloseDoor(m_iObjId);
	} else {
		pDoorSrv->OpenDoor(m_iObjId);
	}

	return 0;
}

MSGHANDLER cScr_NVSimpleDoor::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	pDoorSrv->OpenDoor(m_iObjId);

	return 0;
}

MSGHANDLER cScr_NVSimpleDoor::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	pDoorSrv->CloseDoor(m_iObjId);

	return 0;
}

/***********************
 * NVTriggeringRoom
 */
MSGHANDLER cScr_NVTriggeringRoom::OnObjectRoomEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sRoomMsg* pRoomMsg = static_cast<sRoomMsg*>(pMsg);
	Trigger(true, pMsg, pRoomMsg->MoveObjId, "EnteredRoom");
	return 0;
}

MSGHANDLER cScr_NVTriggeringRoom::OnObjectRoomExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sRoomMsg* pRoomMsg = static_cast<sRoomMsg*>(pMsg);
	Trigger(false, pMsg, pRoomMsg->MoveObjId, NULL, "ExitedRoom");
	return 0;
}

/***********************
 * NVStimBasedAlpha
 */
MSGHANDLER cScr_NVStimBasedAlpha::OnAlpha(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sStimMsg* pStimMsg = static_cast<sStimMsg*>(pMsg);

	m_fIntensity += pStimMsg->intensity;

	if ( m_fIntensity > 100.00f )
		m_fIntensity = 100.00f;
	else if ( m_fIntensity < 0.00f )
		m_fIntensity = 0.00;

	DisplayPrintf("Got %.2f\nCurrent illumination level: %.2f%%", pStimMsg->intensity, m_fIntensity);

	if (m_hTimerHandle != NULL)
	{
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVStimBasedAlpha", 100, kSTM_OneShot, g_mpUndef);

	return 0;
}

MSGHANDLER cScr_NVStimBasedAlpha::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVStimBasedAlpha"))
	{
		float fAlphaMin = ParamGetFloat(m_iObjId, "NVStimBasedAlphaMin", 0.00f);
		float fAlphaMax = ParamGetFloat(m_iObjId, "NVStimBasedAlphaMax", 1.00f);

		float fAlpha = fAlphaMin + ((fAlphaMax - fAlphaMin) * m_fIntensity / 100);

		m_fTarget = fAlpha;
		m_fIntensity = 0.00f;

		if (m_hTimerHandle != NULL)
		{
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = NULL;
		}
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVStimBasedAlphaReset", 2000, kSTM_OneShot, g_mpUndef);
	} else if (!stricmp(pszMsgName, "NVStimBasedAlphaReset"))
	{
		DisplayPrintf("Reset!");
		if (m_hTimerHandle != NULL)
		{
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = NULL;
		}
		m_fTarget = 0.00f;
	}

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	float fCurrent = 1.00f;
	if (pPropSrv->Possessed(m_iObjId, "RenderAlpha"))
	{
		cMultiParm mpReturn;
		pPropSrv->Get(mpReturn, m_iObjId, "RenderAlpha", NULL);
		fCurrent = static_cast<float>(mpReturn);
	}
	if ( fCurrent > m_fTarget )
		fCurrent -= 0.02f;
	else if ( fCurrent < m_fTarget )
		fCurrent += 0.02f;

	pPropSrv->Set(m_iObjId, "RenderAlpha", NULL, fCurrent);


	return 0;
}

MSGHANDLER cScr_NVStimBasedAlpha::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		g_pScriptManager->SetTimedMessage2(m_iObjId, "NVStimBasedAlphaTick", 10, kSTM_Periodic, g_mpUndef);
	}
	return 0;
}

/***********************
 * NVPatrolRouter
 */

 MSGHANDLER cScr_NVPatrolRouter::OnPatrolPoint(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sAIPatrolPointMsg* pPatrolMsg = static_cast<sAIPatrolPointMsg*>(pMsg);
	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVPatrolRouterDelay", 1000, kSTM_OneShot, static_cast<int>(pPatrolMsg->patrolObj));

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->Set(m_iObjId, "AI_Patrol", NULL, false);
	SService<IAIScrSrv> pAISrv(g_pScriptManager);
	pAISrv->ClearGoals(m_iObjId);

	return 0;
}

MSGHANDLER cScr_NVPatrolRouter::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVPatrolRouterDelay"))
		return 0; // Abort if it's not the right timer.

	int iPatrolId = ParamGetInt(m_iObjId, "NVPatrolRouterID", -1);
	DisplayPrintf("Hello patrol point!");

	if ( iPatrolId != -1 )
	{
		DisplayPrintf("Hello I have an ID: %i!", iPatrolId);
		char szPatrol[16];
		sprintf(szPatrol, "Patrol-%i", iPatrolId);
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		link lLink;
		sLink slLink;

		if (GetOneLinkByData("ScriptParams", pTimerMsg->data, 0, &slLink, szPatrol, -1))
		{
			DisplayPrintf("Hello I found a link for me! %i", iPatrolId);
			IterateLinks("AICurrentPatrol", m_iObjId, 0, DestroyLinksIterFunc, NULL, NULL);

			link lNewLink;
			pLS->Create(lNewLink, pLTS->LinkKindNamed("AICurrentPatrol"), m_iObjId, slLink.dest);

			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			pPropSrv->Set(m_iObjId, "AI_IdlRetOrg", NULL, true);

			// SService<IAIScrSrv> pAISrv(g_pScriptManager);
			// pAISrv->ClearGoals(m_iObjId);
			// true_bool bBool;
			// pAISrv->MakeGotoObjLoc(bBool, m_iObjId, slLink.dest, kNormalSpeed, kNormalPriorityAction, 0);
		}
	}

	return 0;
}

int cScr_NVPatrolRouter::DestroyLinksIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	pLS->Destroy(pLQ->ID());
	return 1;
}

/***********************
 * NVBookHack
 */

 MSGHANDLER cScr_NVBookHack::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( ParamGetInt(m_iObjId, "NVInvBook", 0) != 0 )
		ShowBook();

	return 0;
}

MSGHANDLER cScr_NVBookHack::OnFrobWorldEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( ParamGetInt(m_iObjId, "NVInvBook", 0) != 1 )
		ShowBook();

	return 1;
}

void cScr_NVBookHack::ShowBook(void)
{
	SInterface<IWinDispDevice> pWinDisplay(g_pScriptManager);
	unsigned int iHeight = 0, iWidth = 0;
	pWinDisplay->GetWidthHeight(&iWidth, &iHeight);

	if ( iWidth >= 1280 )
		iWidth = 1280;
	else if ( iWidth >= 1024 )
		iWidth = 1024;
	else if ( iWidth >= 800 )
		iWidth = 800;
	else if (( iWidth >= 640) && (iHeight >=480 ) )
		iWidth = 640;
	else
		iWidth = 400;

	DisplayPrintf("Current resolution: %ix%i", iWidth, iHeight);

	SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
	char szImage[128];
	sprintf(szImage, "show_image %s_%i", ParamGetString(m_iObjId, "NVBookHackArt", ""), iWidth);
	pDebugSrv->Command(szImage, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);


	return;
}

/***********************
 * NVArmTrap
 */

MSGHANDLER cScr_NVArmTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPlayerLimbsSrv> pLimbSrv(g_pScriptManager);
	pLimbSrv->Equip(m_iObjId);

	return 0;
}

MSGHANDLER cScr_NVArmTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPlayerLimbsSrv> pLimbSrv(g_pScriptManager);
	pLimbSrv->UnEquip(m_iObjId);

	return 0;
}


/***********************
 * NVVisibilityGemHider
 */

MSGHANDLER cScr_NVVisibilityGemHider::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	pPropSrv->Set(m_iObjId, "AI_Visibility", "Level", -1);
	pPropSrv->Set(m_iObjId, "AI_Visibility", "Light Rating", 100);
	pPropSrv->Set(m_iObjId, "AI_Visibility", "Last Update Time", 2147483647l);

	return 0;
}

MSGHANDLER cScr_NVVisibilityGemHider::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	pPropSrv->Set(m_iObjId, "AI_Visibility", "Last Update Time", pMsg->time + ParamGetInt(m_iObjId, "NVVisibilityReshowDelay", 0));

	return 0;
}

/***********************
 * NVRepulsorLift
 */

MSGHANDLER cScr_NVRepulsorLift::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_iState == -1 )
	{
		m_iState = ParamGetInt(m_iObjId, "NVRepulsorLiftStart", 0);
	}

	return 0;
}

MSGHANDLER cScr_NVRepulsorLift::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	m_iState = 1;
	DoUpdate();
	return 0;
}

MSGHANDLER cScr_NVRepulsorLift::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	m_iState = 0;
	DoUpdate();
	return 0;
}

MSGHANDLER cScr_NVRepulsorLift::OnObjectRoomEnter(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sRoomMsg* pRoomMsg = static_cast<sRoomMsg*>(pMsg);

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	sLink slLink;
	link lNewLink;
	if (!GetOneLinkByData("ScriptParams", m_iObjId, pRoomMsg->MoveObjId, &slLink, "InRoom", -1))
	{
		pLS->Create(lNewLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, pRoomMsg->MoveObjId);
		if ( lNewLink )
		{
			pLTS->LinkSetData(lNewLink, NULL, "InRoom");
		}
	}

	DoUpdate();

	return 0;
}

void cScr_NVRepulsorLift::DoUpdate(void)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);

	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, 0);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{

		sLink sl = lsLinks.Get();
		void* pLinkData = lsLinks.Data();
		if (pLinkData)
		{
			if ( !stricmp(reinterpret_cast<const char*>(pLinkData), "InRoom") )
			{
				// Don't do anything unless the object has the Physics->Model->Type property
				if (pPropSrv->Possessed(sl.dest, "PhysType"))
				{
					cMultiParm mpReturn;
					pPropSrv->Get(mpReturn, sl.dest, "PhysType", "Type");
					int iPhysType = static_cast<int>(mpReturn);

					if ( ( iPhysType == 1) || ( iPhysType == 2 ) )
					{
						pPropSrv->Set(sl.dest, "PhysAttr", "Gravity %", 0.00f);
						pPropSrv->Set(sl.dest, "PhysAttr", "Base Friction", 0.75f);

						cScrVec vVelocity;
						pPhysSrv->GetVelocity(sl.dest, vVelocity);

						if ( static_cast<int>(m_iState) == 1 )
						{ // Up
							vVelocity.z = ParamGetFloat(m_iObjId, "NVRepulsorLiftUpSpeed", 30.00f);
						} else { // Down
							vVelocity.z = 0.00f - ParamGetFloat(m_iObjId, "NVRepulsorLiftDownSpeed", 30.00f);
						}
						pPhysSrv->SetVelocity(sl.dest, vVelocity);
					}
				}
			}
		}
	}

	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "RepulsorLift", 100, kSTM_Periodic, g_mpUndef);
	}

	return;
}

MSGHANDLER cScr_NVRepulsorLift::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "RepulsorLift"))
	{  // Do nothing if it's not the right timer.
		DoUpdate();
	}

	return 0;
}

MSGHANDLER cScr_NVRepulsorLift::OnObjectRoomExit(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sRoomMsg* pRoomMsg = static_cast<sRoomMsg*>(pMsg);

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	cMultiParm mpReturn;

	if (pPropSrv->Possessed(pRoomMsg->MoveObjId, "PhysType"))
	{
		pPropSrv->Get(mpReturn, pRoomMsg->MoveObjId, "PhysType", "Type");
		int iPhysType = static_cast<int>(mpReturn);

		if ( ( iPhysType == 1) || ( iPhysType == 2 ) )
		{
			pPropSrv->Set(pRoomMsg->MoveObjId, "PhysAttr", "Gravity %", 100.00f);
			pPropSrv->Set(pRoomMsg->MoveObjId, "PhysAttr", "Base Friction", 0.00f);
		}
	}

	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, pRoomMsg->MoveObjId);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{

		sLink sl = lsLinks.Get();
		void* pLinkData = lsLinks.Data();
		if (pLinkData)
		{
			if ( !stricmp(reinterpret_cast<const char*>(pLinkData), "InRoom") )
			{
				pLS->Destroy(lsLinks.Link());
			}
		}
	}

	return 0;
}

/***********************
 * NVAccelerator
 */

MSGHANDLER cScr_NVAccelerator::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		g_pScriptManager->SetTimedMessage2(m_iObjId, "Accelerator", ParamGetInt(m_iObjId, "NVAcceleratorRate", 100), kSTM_Periodic, g_mpUndef);

		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		float fSpeed = 10.00f;
		if (pPropSrv->Possessed(m_iObjId, "PhysInitVel"))
		{
			cMultiParm mpReturn;
			pPropSrv->Get(mpReturn, m_iObjId, "PhysInitVel", NULL);
			cScrVec vVelocity;
			fSpeed = static_cast<const mxs_vector*>(mpReturn)->x;
		}

		m_fSpeed = fSpeed;
	}

	return 0;
}

 MSGHANDLER cScr_NVAccelerator::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "Accelerator"))
	{  // Do nothing if it's not the right timer.
		SService<IPhysSrv> pPhysSrv(g_pScriptManager);
		cScrVec vVelocity;

		pPhysSrv->GetVelocity(m_iObjId, vVelocity);
		m_fSpeed = m_fSpeed * ParamGetFloat(m_iObjId, "NVAcceleratorCoefficient", 0.01f);
		float fCoeff = m_fSpeed / vVelocity.Magnitude();

		vVelocity.x = vVelocity.x * fCoeff;
		vVelocity.y = vVelocity.y * fCoeff;
		vVelocity.z = vVelocity.z * fCoeff;
		pPhysSrv->SetVelocity(m_iObjId, vVelocity);
	}

	return 0;
}

/***********************
 * NVSRContactTrap
 */

MSGHANDLER cScr_NVSRContactTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IActReactSrv> pARSrv(g_pScriptManager);
	pARSrv->BeginContact(m_iObjId, m_iObjId);

	return 0;
}

MSGHANDLER cScr_NVSRContactTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}


/***********************
 * NVSlowSpell
 */

MSGHANDLER cScr_NVSlowSpell::OnSlow(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sStimMsg* pStimMsg = static_cast<sStimMsg*>(pMsg);

	float fIntensity = pStimMsg->intensity;

	if ( m_fWarp == 0.00f )
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		cMultiParm mpProp;
		float fWarp = 1.00f;
		if (pPropSrv->Possessed(m_iObjId, "TimeWarp"))
		{
			pPropSrv->Get(mpProp, m_iObjId, "TimeWarp", NULL);
			fWarp = static_cast<float>(mpProp);
		}

		m_fWarp = fWarp;

		float fSlowBase = ParamGetFloat(m_iObjId, "NVSlowBase", 0.05f);
		float fSlowMult = ParamGetFloat(m_iObjId, "NVSlowMult", 0.10f);
		float fMult = 1.00f + fSlowBase + (fSlowMult * fIntensity);
		//if ( fMult < 0.01f )
		//	fMult = 0.01f;

		fWarp *= fMult;

		int iSlowTimeBase = ParamGetInt(m_iObjId, "NVSlowTimeBase", 1000);
		int iSlowTimeMult = ParamGetInt(m_iObjId, "NVSlowTimeMult", 2000);
		float fSlowTimeRamp = ParamGetFloat(m_iObjId, "NVSlowTimeRamp", 0.50f);
		if ( (fSlowTimeRamp <= 0.00 ) || (fSlowTimeRamp >= 1.00f ) )
			fSlowTimeRamp = 0.50f;

		m_iSlowTime = (int)(iSlowTimeBase + (fIntensity * iSlowTimeMult));
		int iSlowTime =  (int)((int)(m_iSlowTime) * (1.00f - fSlowTimeRamp));


		pPropSrv->Set(m_iObjId, "TimeWarp", NULL, fWarp);

		if (m_hTimerHandle != NULL)
		{	// Stop timer:
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = NULL;
		}

		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVSlow", iSlowTime, kSTM_OneShot, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVSlowSpell::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	float fWarp = 1.00f;
	if (pPropSrv->Possessed(m_iObjId, "TimeWarp"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "TimeWarp", NULL);
		fWarp = static_cast<float>(mpProp);
	}

	if (!stricmp(pszMsgName, "NVSlow"))
	{
		m_fTarget = m_fWarp;
		m_fSpeed = (( m_fTarget - fWarp) / 20.00f);

//		pPropSrv->Set(m_iObjId, "TimeWarp", NULL, static_cast<float>(m_fWarp));
		if (m_hTimerHandle != NULL)
		{	// Stop timer:
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = NULL;
		}

		float fSlowTimeRamp = ParamGetFloat(m_iObjId, "NVSlowTimeRamp", 0.50f);
		if ( (fSlowTimeRamp <= 0.00 ) || (fSlowTimeRamp >= 1.00f ) )
			fSlowTimeRamp = 0.50f;

		int iSlowTime = static_cast<int>((static_cast<int>(m_iSlowTime) * fSlowTimeRamp));
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVSlowRamp", (iSlowTime / 20), kSTM_Periodic, g_mpUndef);
	} else if (!stricmp(pszMsgName, "NVSlowRamp"))
	{
		float fNewWarp = fWarp + m_fSpeed;

		if ( ((fWarp < m_fTarget) && ( fNewWarp >= m_fTarget ) ) || ((fWarp > m_fTarget) && ( fNewWarp <= m_fTarget ) ) )
		{
			fNewWarp = m_fTarget;
			if (m_hTimerHandle != NULL)
			{	// Stop timer:
				g_pScriptManager->KillTimedMessage(m_hTimerHandle);
				m_hTimerHandle = NULL;
			}
			m_fWarp = 0.00f;
		}

		pPropSrv->Set(m_iObjId, "TimeWarp", NULL, fNewWarp);

	}

	return 0;
}

#if (_DARKGAME != 1)
/***********************
 * NVTFMadeMeDoIt
 */
MSGHANDLER cScr_NVTFMadeMeDoIt::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVTFMadeMeDoIt::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVTFMadeMeDoIt::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iCollObj = pPhysMsg->collObj;

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpProp;
	if (pPropSrv->Possessed(m_iObjId, "Firer"))
	{
		pPropSrv->Get(mpProp, m_iObjId, "Firer", NULL);
	} else {
		return 1;
	}

	int iFirer = static_cast<int>(mpProp);

	object oStim = ParamGetNVObject(m_iObjId, "NVTFMadeMeDoItStimulus", 0);
	float fIntensity = ParamGetFloat(m_iObjId, "NVTFMadeMeDoItIntensity", 1.00f);
	if (oStim)
	{
		SService<IActReactSrv> pARSrv(g_pScriptManager);
		pARSrv->Stimulate(iCollObj, oStim, fIntensity, iFirer);
	}

	return 0;
}
#endif

/***********************
 * NVSlayMeTrap
 */

MSGHANDLER cScr_NVSlayMeTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDamageSrv> pDmgS(g_pScriptManager);
	pDmgS->Slay(m_iObjId, m_iObjId);

	return 0;
}

MSGHANDLER cScr_NVSlayMeTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVDistanceBasedAlpha
 */

MSGHANDLER cScr_NVDistanceBasedAlpha::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVDistanceBasedAlpha", 100, kSTM_Periodic, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVDistanceBasedAlpha::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVDistanceBasedAlpha"))
		return 0; // Abort if it's not the right timer.

	SService<IObjectSrv> pOS(g_pScriptManager);

	object oObj = StrToObject("Player");

	cScrVec vPosition, vPlayerPos;
	pOS->Position(vPosition, m_iObjId);
	pOS->Position(vPlayerPos, oObj);

	float fDistance = GetDistance(&vPosition, &vPlayerPos);

	char* pszParam = new char[strlen(m_szName) + 10];

	sprintf(pszParam, "%sNearDist", m_szName);
	float fNearDist = ParamGetFloat(m_iObjId, pszParam, 0.00f);

	sprintf(pszParam, "%sFarDist", m_szName);
	float fFarDist = ParamGetFloat(m_iObjId, pszParam, 16.00f);

	sprintf(pszParam, "%sNearAlpha", m_szName);
	float fNearAlpha = ParamGetFloat(m_iObjId, pszParam, 0.00f);

	sprintf(pszParam, "%sFarAlpha", m_szName);
	float fFarAlpha = ParamGetFloat(m_iObjId, pszParam, 1.00f);

	delete[] pszParam;


	float fDistMult = (fDistance - fNearDist) / (fFarDist - fNearDist);
	float fNewAlpha = fNearAlpha + ((fFarAlpha - fNearAlpha) * fDistMult);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	if ( !ParamGetInt(m_iObjId, "NVDistanceBasedAlphaRenderInvisible", 1 ) )
	{
		if ( fNewAlpha <= 0.01f )
			pPropSrv->Set(m_iObjId, "RenderType", NULL, 1);
		else
			pPropSrv->Set(m_iObjId, "RenderType", NULL, ParamGetInt(m_iObjId, "NVDistanceBasedAlphaRenderType", 0));
	}

	#if (_DARKGAME == 3)
		pPropSrv->SetLocal(m_iObjId, "RenderAlpha", NULL, fNewAlpha);
	#else
		pPropSrv->Set(m_iObjId, "RenderAlpha", NULL, fNewAlpha);
	#endif


	return 0;
}

/***********************
 * NVAITeamSetter
 */

MSGHANDLER cScr_NVAITeamSetter::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	pPropSrv->Set(m_iObjId, "AI_Team", NULL, ParamGetInt(m_iObjId, "NVAITeam", m_iObjId));
	return 0;
}


/***********************
 * NVCombineTypeSetter
 */

MSGHANDLER cScr_NVCombineTypeSetter::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	char *psz = new char[8];
	sprintf(psz, "%i", m_iObjId);

	pPropSrv->Set(m_iObjId, "CombineType", NULL, ParamGetString(m_iObjId, "NVCombineType", psz));

	delete[] psz;
	return 0;
}

/***********************
 * NVSetPropertyTrap
 */

MSGHANDLER cScr_NVSetPropertyTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	const char* pszPropName = NVParamGetString(m_iObjId, m_szName, "Property", NULL);

	if (pszPropName != NULL)
	{
		const char* pszSubProp = NVParamGetString(m_iObjId, m_szName, "SubProperty", NULL);

		const char* pszPropType = NVParamGetString(m_iObjId, m_szName, "Type", "int");

		if (!stricmp(pszPropType, "string")) {
			for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
			{
				pPropSrv->Set(*it, pszPropName, pszSubProp, NVParamGetString(m_iObjId, m_szName, "Value", NULL));
			}
		} else {
			int iValue = NVParamGetInt(m_iObjId, m_szName, "Value", -1);

			if ( iValue == -1 )
			{
				int iMin = NVParamGetInt(m_iObjId, m_szName, "Min", 0);
				int iMax = NVParamGetInt(m_iObjId, m_szName, "Max", 10);

				SService<IDataSrv> pDS(g_pScriptManager);
				iValue = pDS->RandInt(iMin, iMax);

				if (NVParamGetInt(m_iObjId, m_szName, "IsFlags", 0))
				{
					iValue = static_cast<int>(pow((double)2, (double)(iValue-1)));
				}
			}

			const char* pszPref = NVParamGetString(m_iObjId, m_szName, "Prefix", NULL);

			if ( pszPref != NULL )
			{
				char *pszPropValue = new char[strlen(pszPref) + 8];

				sprintf(pszPropValue, "%s%i", pszPref, iValue);
				for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
				{
					pPropSrv->Set(*it, pszPropName, pszSubProp, pszPropValue);
				}

				delete[] pszPropValue;
			} else {
				for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
				{
					pPropSrv->Set(*it, pszPropName, pszSubProp, iValue);
				}
			}
		}
	}

	return 0;
}

MSGHANDLER cScr_NVSetPropertyTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVWhoKilledMe
 */

MSGHANDLER cScr_NVWhoKilledMe::OnSlain(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sSlayMsg* pSlayMsg = static_cast<sSlayMsg*>(pMsg);

	const char* pszMyName = ParamGetString(m_iObjId, "NVWhoKilledMeName", "I");
	const char* pszMidMessage = ParamGetString(m_iObjId, "NVWhoKilledMeMsg", " was killed by ");
	const char* pszCulpritName = ParamGetString(pSlayMsg->culprit, "NVWhoKilledMeName", "You");

	char* psz = new char[strlen(pszMyName) + strlen(pszCulpritName) + strlen(pszMidMessage) + 32];

	// link lLink;
	// sLink slLink;

	// SService<ILinkSrv> pLS(g_pScriptManager);
	// SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	// pLS->GetOne(lLink, pLTS->LinkKindNamed(CDLINK), m_iObjId, 0);
	// if ( !lLink)
		// return 1;

	// pLTS->LinkGet(lLink, slLink);

	// SService<IObjectSrv> pOS(g_pScriptManager);
	// object oDest = slLink.dest;



	sprintf(psz, "%s%s%s - (%i) (%i)", pszMyName, pszMidMessage, pszCulpritName, pSlayMsg->culprit, pSlayMsg->kind);

	int iColour = strtocolor(ParamGetString(m_iObjId, "NVWhoKilledMeColour", "#FFFFFF"));

	ShowString(psz, CalcTextTime(psz, 500), iColour);

	delete[] psz;

	return 0;
}

/***********************
 * NVStackOrDropTrap
 */

MSGHANDLER cScr_NVStackOrDropTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oArc = ParamGetNVObject(m_iObjId, "NVStackOrDropTrapObject", 0);
	if ( oArc )
	{
		int iPart = GetSinglePossession(StrToObject("Player"), oArc);
		if ( iPart )
		{
			SInterface<IContainSys> pContainSys(g_pScriptManager);
			pContainSys->StackAdd(iPart, 1, 1);
			#if (_DARKGAME == 3)
				SService<IShockGameSrv> pShockGame(g_pScriptManager);
				pShockGame->RefreshInv();
			#endif
		} else {
			SService<IObjectSrv> pOS(g_pScriptManager);
			object oObj;
			pOS->Create(oObj, oArc);

			if (oObj)
			{
				if ( ParamExists(m_iObjId, "NVStackOrDropTrapLoc") )
				{
					const char* pszLocVector = ParamGetString(m_iObjId, "NVStackOrDropTrapLoc", "0,0,0");
					const char* pszRotVector = ParamGetString(m_iObjId, "NVStackOrDropTrapRot", "0,0,0");
					mxs_vector vVector;
					mxs_vector vRot = {0.0, 0.0, 0.0};
					if (3 == sscanf(pszLocVector, "%f, %f, %f", &vVector.x, &vVector.y, &vVector.z))
					{
						sscanf(pszRotVector, "%f, %f, %f", &vRot.x, &vRot.y, &vRot.z);
						int iLocObj = ParamGetNVObject(m_iObjId, "NVStackOrDropTrapLocObj", m_iObjId);
						pOS->Teleport(oObj, vVector, vRot, iLocObj);
					}
				}
			}
		}
	}

	return 0;
}

MSGHANDLER cScr_NVStackOrDropTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVConsumeMessages
 */

MSGHANDLER cScr_NVConsumeMessages::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	//Check stack counts:
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpStack = 0;
	if (pPropSrv->Possessed(m_iObjId, "StackCount"))
	{
		pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
	}
	m_iStack = static_cast<int>(mpStack);

	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVConsumeMessages", 50, kSTM_OneShot, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVConsumeMessages::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVConsumeMessages"))
		return 0; // Abort if it's not the right timer.

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpStack = 0;
	if (pPropSrv->Possessed(m_iObjId, "StackCount"))
	{
		pPropSrv->Get(mpStack, m_iObjId, "StackCount", NULL);
	}

	if ( static_cast<int>(mpStack) < m_iStack )
		SimpleSend(0, m_iObjId, "StackConsume");

	m_iStack = static_cast<int>(mpStack);

	return 0;
}

/***********************
 * NVContainedMessages
 */

MSGHANDLER cScr_NVContainedMessages::OnContained(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sContainedScrMsg * pContainedMsg = static_cast<sContainedScrMsg *>(pMsg);
	char *pszMsg = new char[16];
	sprintf(pszMsg, "Contained%i", pContainedMsg->event);
	SimpleSend(0, m_iObjId, pszMsg);
	sprintf(pszMsg, "Container%i", pContainedMsg->event);
	SimpleSend(0, pContainedMsg->container, pszMsg);
	delete[] pszMsg;

	return 0;
}

/***********************
 * NVPickyProjectile
 */
MSGHANDLER cScr_NVPickyProjectile::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVPickyProjectile::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVPickyProjectile::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iCollObj = pPhysMsg->collObj;

	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bBool;
	pOS->InheritsFrom(bBool, iCollObj, ParamGetNVObject(m_iObjId, "NVPickyProjectileIgnore", 0));
	if ( bBool )
	{
		MultiParmAssignInt(pReply, 1);
	} else {
		SService<IDamageSrv> pDmgS(g_pScriptManager);
		pDmgS->Slay(m_iObjId, 0);
//		MultiParmAssignInt(pReply, 3);
	}

	return 0;
}

/***********************
 * NVGibTrap
 */

MSGHANDLER cScr_NVGibTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iTime = ParamGetInt(m_iObjId, "NVGibTrapDelay", 0);

	if ( iTime > 0 )
		g_pScriptManager->SetTimedMessage2(m_iObjId, "NVGibTrapDelay", iTime, kSTM_OneShot, g_mpUndef);
	else
		DoGib(pMsg);

	return 0;
}

void cScr_NVGibTrap::DoGib(sScrMsg* pMsg)
{
	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		SService<IObjectSrv> pOS(g_pScriptManager);

		linkset lsLinks;
		pLS->GetAll(lsLinks, pLTS->LinkKindNamed("Contains"), *it, 0);

		for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
		{
			sLink sl = lsLinks.Get();

			object oObj = sl.dest;

			pOS->Teleport(oObj, cScrVec::Zero, cScrVec::Zero, *it);
			pPropSrv->Set(oObj, "HasRefs", NULL, true);
			pPropSrv->Set(oObj, "PhysType", "Type", 1);
			pPropSrv->Set(oObj, "CollisionType", NULL, 1);
			pLS->Destroy(lsLinks.Link());

			SService<IPhysSrv> pPhysSrv(g_pScriptManager);

			SService<IDataSrv> pDS(g_pScriptManager);
			cScrVec vVelocity;

			vVelocity.x = pDS->RandFlt0to1() * ParamGetFloat(m_iObjId, "NVGibTrapVelocity", 10.00f);
			vVelocity.y = pDS->RandFlt0to1() * ParamGetFloat(m_iObjId, "NVGibTrapVelocity", 10.00f);
			vVelocity.z = pDS->RandFlt0to1() * ParamGetFloat(m_iObjId, "NVGibTrapVelocity", 10.00f);

			pPhysSrv->SetVelocity(oObj, vVelocity);
		}

		pOS->Destroy(*it);
	}

	return;
}

MSGHANDLER cScr_NVGibTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

MSGHANDLER cScr_NVGibTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVGibTrapDelay"))
		return 0; // Abort if it's not the right timer.

	DoGib(pMsg);

	return 0;
}

/***********************
 * NVDoorSpeedTrap
 */

MSGHANDLER cScr_NVDoorSpeedTrap::OnDoorOpen(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_iInit < 2 )
	{
		m_iInit = 2;
//		DisplayPrintf("Setting open position!");
		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrVec vFacing, vPosition;
		pOS->Facing(vFacing, m_iObjId);
		pOS->Position(vPosition, m_iObjId);
		m_vLocation2 = &vPosition;
		m_vRotation2 = &vFacing;
	}

	return 0;
}
MSGHANDLER cScr_NVDoorSpeedTrap::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_iInit == 0 )
	{
		m_iInit = 1;
		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrVec vFacing, vPosition;
		pOS->Facing(vFacing, m_iObjId);
		pOS->Position(vPosition, m_iObjId);
		m_vLocation = &vPosition;
		m_vRotation = &vFacing;
	}

	return 0;
}

void cScr_NVDoorSpeedTrap::DoDoor(float fSpeed)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IDoorSrv> pDoorSrv(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);


	cScrVec vFacing, vPosition, vOriginalPos, vOriginalRot;


//	cMultiParm mpState;
	int iState = pDoorSrv->GetDoorState(m_iObjId);


	if ( iState != 2 )
	{
		vOriginalPos = *m_vLocation;
		vOriginalRot = *m_vRotation;
	} else {
		vOriginalPos = *m_vLocation2;
		vOriginalRot = *m_vRotation2;
	}
	pOS->Facing(vFacing, m_iObjId);
	pOS->Position(vPosition, m_iObjId);



	pOS->Teleport(m_iObjId, vOriginalPos, vOriginalRot, 0);

	if (pPropSrv->Possessed(m_iObjId, "TransDoor"))
	{ // Set base speed of translating door.
//		pPropSrv->Get(mpState, m_iObjId, "TransDoor", "Status");


		pPropSrv->Set(m_iObjId, "TransDoor", "Status", 4);
		pPropSrv->Set(m_iObjId, "TransDoor", "Base Speed", fSpeed);

	} else { // Check if it's a rotating door.
		if (pPropSrv->Possessed(m_iObjId, "RotDoor"))
		{ // Set base speed of rotating door.
//			pPropSrv->Get(mpState, m_iObjId, "RotDoor", "Status");

			pPropSrv->Set(m_iObjId, "RotDoor", "Status", 4);
			pPropSrv->Set(m_iObjId, "RotDoor", "Base Speed", fSpeed);

		} else {
			return; // Eeek, this isn't a door!
		}
	}


//	iState = static_cast<int>(mpState);

//	DisplayPrintf("Door state is %i!", iState);

	if ( iState == 3 )
	{
		pDoorSrv->OpenDoor(m_iObjId);
		pDoorSrv->CloseDoor(m_iObjId);
	} else {
		//	if ( iState == 2 )
			pPropSrv->Set(m_iObjId, "RotDoor", "Status", 3);
			pDoorSrv->OpenDoor(m_iObjId);
			pDoorSrv->CloseDoor(m_iObjId);
	}

	pOS->Teleport(m_iObjId, vPosition, vFacing, 0);

}

MSGHANDLER cScr_NVDoorSpeedTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoDoor(ParamGetFloat(m_iObjId, "NVDoorSpeedTrapOnSpeed", 1.00f));

	return 0;
}

MSGHANDLER cScr_NVDoorSpeedTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoDoor(ParamGetFloat(m_iObjId, "NVDoorSpeedTrapOffSpeed", 2.00f));

	return 0;
}



/***********************
 * NVTimeDilationPhysics
 */

MSGHANDLER cScr_NVTimeDilationPhysics::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	pQSrv->SubscribeMsg(m_iObjId, "NVTimeDilation", 2);
	//DoTimeDilation();
	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVTimeDilationPhysics", 10, kSTM_OneShot, g_mpUndef);

	return 0;
}

MSGHANDLER cScr_NVTimeDilationPhysics::OnCheck(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVTimeDilationPhysics", 10, kSTM_OneShot, g_mpUndef);
	return 0;
}

MSGHANDLER cScr_NVTimeDilationPhysics::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVTimeDilationPhysics"))
		return 0; // Abort if it's not the right timer.

	m_iDilate = 0;
	DoTimeDilation();

	return 0;
}

MSGHANDLER cScr_NVTimeDilationPhysics::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	pQSrv->UnsubscribeMsg(m_iObjId, "NVTimeDilation");

	return 0;
}

MSGHANDLER cScr_NVTimeDilationPhysics::OnQuestChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DoTimeDilation();

	return 0;
}

void cScr_NVTimeDilationPhysics::DoTimeDilation(void)
{

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	true_bool bLinkExists;
	pLS->AnyExist(bLinkExists, pLTS->LinkKindNamed("Contains"), 0, m_iObjId);
	if ( bLinkExists)
		return; // Don't bother processing if the object is currently contained.

	SService<IQuestSrv> pQSrv(g_pScriptManager);

	int iCurrent = 0;

	if ( pQSrv->Exists("NVTimeDilation") )
	{
		iCurrent = pQSrv->Get("NVTimeDilation"); // Get QVar value
	}

	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	cScrVec vVelocity;
	pPhysSrv->GetVelocity(m_iObjId, vVelocity);
	float fGravity = pPhysSrv->GetGravity(m_iObjId);

	if ( iCurrent )
	{
		if ( m_iDilate == 0 )
		{
			m_iDilate = 1;
//			DisplayPrintf("Slowed time!");

			vVelocity.x *= 0.10f;
			vVelocity.y *= 0.10f;
			vVelocity.z *= 0.10f;
			fGravity *= 0.01f;
			pPhysSrv->SetGravity(m_iObjId, fGravity);
			pPhysSrv->SetVelocity(m_iObjId, vVelocity);
		}
	} else {
		if ( m_iDilate == 1 )
		{
			m_iDilate = 0;
//			DisplayPrintf("Normal time!");

			vVelocity.x *= 10.00;
			vVelocity.y *= 10.00;
			vVelocity.z *= 10.00;
			fGravity *= 100.0;
			pPhysSrv->SetGravity(m_iObjId, fGravity);
			pPhysSrv->SetVelocity(m_iObjId, vVelocity);
		}
	}
}

/***********************
 * NVTimeDilationTweq
 */

MSGHANDLER cScr_NVTimeDilationTweq::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_iDilate == 0 )
	{
		m_iDilate = 1;
		DoDilate(0.10f);
	}
	return 0;
}

MSGHANDLER cScr_NVTimeDilationTweq::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_iDilate == 1 )
	{
		m_iDilate = 0;
		DoDilate(10.00f);
	}
	return 0;
}

void cScr_NVTimeDilationTweq::DoDilate(float fScale)
{
	cMultiParm mpReturn;
	mxs_vector vReturn;
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	if (pPropSrv->Possessed(m_iObjId, "CfgTweqDelete"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqDelete", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweqDelete", "Rate", static_cast<int>(mpReturn) * fScale);
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweqEmit"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqEmit", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweqEmit", "Rate", static_cast<int>(mpReturn) * ( 1 / fScale));
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweq2Emit"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweq2Emit", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweq2Emit", "Rate", static_cast<int>(mpReturn) * ( 1 / fScale));
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweq3Emit"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweq3Emit", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweq3Emit", "Rate", static_cast<int>(mpReturn) * ( 1 / fScale));
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweq4Emit"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweq4Emit", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweq4Emit", "Rate", static_cast<int>(mpReturn) * ( 1 / fScale));
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweq5Emit"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweq5Emit", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweq5Emit", "Rate", static_cast<int>(mpReturn) *( 1 / fScale));
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweqBlink"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqBlink", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweqBlink", "Rate", static_cast<int>(mpReturn) * fScale);
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweqModels"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqModels", "Rate");
		pPropSrv->Set(m_iObjId, "CfgTweqModels", "Rate", static_cast<int>(mpReturn) * fScale);
	}

	// These use rate-low-high instead of rate.

	// if (pPropSrv->Possessed(m_iObjId, "CfgTweqJoints"))
	// {
		// pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqJoints", "Rate");
		// pPropSrv->Set(m_iObjId, "CfgTweqJoints", "Rate", static_cast<int>(mpReturn) * fScale);
	// }

	// if (pPropSrv->Possessed(m_iObjId, "CfgTweqLock"))
	// {
		// pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqLock", "Rate");
		// pPropSrv->Set(m_iObjId, "CfgTweqEmit", "Rate", static_cast<int>(mpReturn) * fScale);
	// }

	if (pPropSrv->Possessed(m_iObjId, "CfgTweqRotate"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqRotate", "x rate-low-high");
		vReturn.x = static_cast<const mxs_vector*>(mpReturn)->x * fScale;
		vReturn.y = static_cast<const mxs_vector*>(mpReturn)->y * fScale;
		vReturn.z = static_cast<const mxs_vector*>(mpReturn)->z * fScale;
		pPropSrv->Set(m_iObjId, "CfgTweqRotate", "x rate-low-high", vReturn);

		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqRotate", "y rate-low-high");
		vReturn.x = static_cast<const mxs_vector*>(mpReturn)->x * fScale;
		vReturn.y = static_cast<const mxs_vector*>(mpReturn)->y * fScale;
		vReturn.z = static_cast<const mxs_vector*>(mpReturn)->z * fScale;
		pPropSrv->Set(m_iObjId, "CfgTweqRotate", "y rate-low-high", vReturn);

		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqRotate", "z rate-low-high");
		vReturn.x = static_cast<const mxs_vector*>(mpReturn)->x * fScale;
		vReturn.y = static_cast<const mxs_vector*>(mpReturn)->y * fScale;
		vReturn.z = static_cast<const mxs_vector*>(mpReturn)->z * fScale;
		pPropSrv->Set(m_iObjId, "CfgTweqRotate", "z rate-low-high", vReturn);
	}

	if (pPropSrv->Possessed(m_iObjId, "CfgTweqScale"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqScale", "x rate-low-high");
		vReturn.x = static_cast<const mxs_vector*>(mpReturn)->x * fScale;
		vReturn.y = static_cast<const mxs_vector*>(mpReturn)->y * fScale;
		vReturn.z = static_cast<const mxs_vector*>(mpReturn)->z * fScale;
		pPropSrv->Set(m_iObjId, "CfgTweqScale", "x rate-low-high", vReturn);

		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqScale", "y rate-low-high");
		vReturn.x = static_cast<const mxs_vector*>(mpReturn)->x * fScale;
		vReturn.y = static_cast<const mxs_vector*>(mpReturn)->y * fScale;
		vReturn.z = static_cast<const mxs_vector*>(mpReturn)->z * fScale;
		pPropSrv->Set(m_iObjId, "CfgTweqScale", "y rate-low-high", vReturn);

		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqScale", "z rate-low-high");
		vReturn.x = static_cast<const mxs_vector*>(mpReturn)->x * fScale;
		vReturn.y = static_cast<const mxs_vector*>(mpReturn)->y * fScale;
		vReturn.z = static_cast<const mxs_vector*>(mpReturn)->z * fScale;
		pPropSrv->Set(m_iObjId, "CfgTweqScale", "z rate-low-high", vReturn);


	}

}


/***********************
 * NVGetOrigin
 */

MSGHANDLER cScr_NVGetOrigin::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( !ParamExists(m_iObjId, "NVOrigin") )
	{
		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrVec vPosition, vPlayerPos;
		pOS->Position(vPosition, m_iObjId);

		char szOrigin[80];

		sprintf(szOrigin, "%f,%f,%f", vPosition.x, vPosition.y, vPosition.z);

		ParamSetString(m_iObjId, "NVOrigin", szOrigin);

	}

	return 0;
}

/***********************
 * NVStartUnrotated
 */

MSGHANDLER cScr_NVStartUnrotated::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( ! ParamGetInt(m_iObjId, "NVStartUnrotatedDone", 0) )
	{
/*		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrVec vPosition;

		pOS->Position(vPosition, m_iObjId);
		pOS->Teleport(m_iObjId, vPosition, cScrVec::Zero, 0);
	*/
		g_pScriptManager->SetTimedMessage2(m_iObjId, "NVStartUnrotatedTimer", 10, kSTM_OneShot, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVStartUnrotated::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVStartUnrotatedTimer"))
		return 0; // Abort if it's not the right timer.

	SService<IObjectSrv> pOS(g_pScriptManager);
	cScrVec vPosition;

	pOS->Position(vPosition, m_iObjId);
	pOS->Teleport(m_iObjId, vPosition, cScrVec::Zero, 0);

	ParamSetInt(m_iObjId, "NVStartUnrotatedDone", 1);

	return 0;
}

/***********************
 * NVLeaveTheDeadAlone
 */

MSGHANDLER cScr_NVLeaveTheDeadAlone::OnSlain(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed("~AIAwareness"), m_iObjId, 0);

	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		sLink sl = lsLinks.Get();

		pPropSrv->Set(sl.dest, "AI_Alertness", "Level", 0); // Reset frame
		pPropSrv->Set(sl.dest, "AI_Alertness", "Peak", 0); // Reset frame
		pLS->Destroy(lsLinks.Link());
	}


	return 0;
}

/***********************
 * NVCameraTrap
 */

MSGHANDLER cScr_NVCameraTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	#if (_DARKGAME != 3)
		SService<ICameraSrv> pCamera(g_pScriptManager);
		SService<IDarkInvSrv> pInv(g_pScriptManager);
		if ( ParamGetInt(m_iObjId, "NVCameraTrapStatic", 0) )
		{
			pCamera->StaticAttach(m_iObjId);
		} else {
			pCamera->DynamicAttach(m_iObjId);
		}
	#else
		SService<IShockGameSrv> pShockGame(g_pScriptManager);

		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrStr strName;
		pOS->GetName(strName, m_iObjId);

		pShockGame->AttachCamera(strName);

		pShockGame->SetQBHacked("HideInterface", 1);
		pShockGame->Mouse(0,1);

		SimpleSend(m_iObjId, StrToObject("Player"), "SaveOverlays");
		for ( int i = 0; i <= 50; ++i )
		{
			pShockGame->OverlayChange(i, 0);
		}
		pShockGame->Equip(0, 0);

	#endif

	int defaultReturnTime = 0;

	if ( ParamGetInt(m_iObjId, "NVCameraTrapDisableControls", 0 ) )
	{
		SInterface<IInputBinder> pInput(g_pScriptManager);
		pInput->SetValidEvents(0);

		#if (_DARKGAME != 3)
			pInv->CapabilityControl(kDrkInvCapCycle, kDrkInvControlOff);
			pInv->CapabilityControl(kDrkInvCapWorldFrob, kDrkInvControlOff);
			pInv->CapabilityControl(kDrkInvCapWorldFocus, kDrkInvControlOff);
			pInv->CapabilityControl(kDrkInvCapInvFrob, kDrkInvControlOff);
		#endif

		if (m_hTimerHandle == NULL)
		{ // Start a timer.
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVCameraTrapDisableControls", 20, kSTM_Periodic, g_mpUndef);
		}

		defaultReturnTime = 10000;
	}

	int returnTime = ParamGetInt(m_iObjId, "NVCameraTrapReturnTime", defaultReturnTime );

	if ( returnTime > 0 )
	{
		g_pScriptManager->SetTimedMessage2(m_iObjId, "NVCameraTrapReturnTime", returnTime, kSTM_OneShot, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVCameraTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	deactivateAttach();

	return 0;
}

void cScr_NVCameraTrap::deactivateAttach(void)
{
	#if (_DARKGAME != 3)
		SService<ICameraSrv> pCamera(g_pScriptManager);
		SService<IDarkInvSrv> pInv(g_pScriptManager);
		pCamera->CameraReturn(m_iObjId);
	#else
		SService<IShockGameSrv> pShockGame(g_pScriptManager);

		pShockGame->AttachCamera("Player");
		pShockGame->SetQBHacked("HideInterface", 0);
		pShockGame->Mouse(1,1);

		SimpleSend(m_iObjId, StrToObject("Player"), "RestoreOverlays");
	#endif

	if ( ParamGetInt(m_iObjId, "NVCameraTrapDisableControls", 0 ) )
	{
		SInterface<IInputBinder> pInput(g_pScriptManager);
		pInput->SetValidEvents(-1);
		#if (_DARKGAME != 3)
			pInv->CapabilityControl(kDrkInvCapCycle, kDrkInvControlOn);
			pInv->CapabilityControl(kDrkInvCapWorldFrob, kDrkInvControlOn);
			pInv->CapabilityControl(kDrkInvCapWorldFocus, kDrkInvControlOn);
			pInv->CapabilityControl(kDrkInvCapInvFrob, kDrkInvControlOn);
		#endif

		if (m_hTimerHandle != NULL)
		{	// Stop timer:
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = NULL;
		}
	}
}

MSGHANDLER cScr_NVCameraTrap::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Clean up
	SInterface<IInputBinder> pInput(g_pScriptManager);
	pInput->SetValidEvents(-1);
	return 0;
}

MSGHANDLER cScr_NVCameraTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "NVCameraTrapDisableControls"))
	{
		if ( (m_bWasEscPressed == false) && (GetAsyncKeyState(VK_ESCAPE) >> 8) )
		{
			SInterface<IInputBinder> pInput(g_pScriptManager);
			SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
			pDebugSrv->Command("sim_menu", cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);
			m_bWasEscPressed = true;
		} else {
			m_bWasEscPressed = false;
		}
	} else if (!stricmp(pszMsgName, "NVCameraTrapReturnTime"))
	{
		deactivateAttach();
	}

	return 0;
}

/***********************
 * NVMapTrap
 */

MSGHANDLER cScr_NVMapTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IDarkGameSrv> pDarkSrv(g_pScriptManager);

	int iPage = NVParamGetInt(m_iObjId, m_szName, "Page", 0);
	int iLocation = NVParamGetInt(m_iObjId, m_szName, "Location", 0);

	pDarkSrv->SetAutomapLocationVisited(iPage, iLocation);

	return 0;
}

MSGHANDLER cScr_NVMapTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Do nothing
	return 0;
}

/***********************
 * NVDeathStageFix
 */

void __stdcall cScr_NVDeathStageFix::ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData)
{
	cScr_NVDeathStageFix* pSelf = reinterpret_cast<cScr_NVDeathStageFix*>(pData);
	if ( pSelf->m_bSim == false )
		return;

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	cMultiParm mpHealth = 1;
	if (pPropSrv->Possessed(sMsg->iObjId, "HitPoints"))
		pPropSrv->Get(mpHealth, sMsg->iObjId, "HitPoints", NULL);

	if ( static_cast<int>(mpHealth) > 0 )
	{
		pPropSrv->Remove(sMsg->iObjId, "DeathStage");
	}

	return;
}

void cScr_NVDeathStageFix::StartListener(void)
{
	StopListener();
	if ( !m_pProp )
	{
		SInterface<IPropertyManager> pPM(g_pScriptManager);
		m_pProp = static_cast<IGenericProperty*>(pPM->GetPropertyNamed("DeathStage"));
	}

	m_hListenHandle = m_pProp->Listen(kPropertyChange | kPropertyAdd, ListenFunc, reinterpret_cast<PropListenerData__*>(this));

	return;
}

void cScr_NVDeathStageFix::StopListener(void)
{
	if ( m_hListenHandle )
	{
		m_pProp->Unlisten(m_hListenHandle);
		m_hListenHandle = NULL;
	}

	return;
}

/***********************
 * NVQVarName
 */

MSGHANDLER cScr_NVQVarName::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	GetQVarName();
	SimpleSend(m_iObjId, m_iObjId, "InitNVQvarName");

	return 0;
}

void cScr_NVQVarName::GetQVarName()
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);

	const char* pszQuestVar = ParamGetString(m_iObjId, "NVQVarNameQVar", "NameQVar");
	if (!pszQuestVar)
		return; // No qvar specifed! Abort.

	if ( m_szQVar != NULL )
		delete[] m_szQVar;

	m_szQVar = new char[strlen(pszQuestVar) +1];
	strcpy(m_szQVar, pszQuestVar);

	pQSrv->SubscribeMsg(m_iObjId, m_szQVar, 2);

}

MSGHANDLER cScr_NVQVarName::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	if (m_szQVar)
	{
		pQSrv->UnsubscribeMsg(m_iObjId, m_szQVar);
	}
	return 0;
}

MSGHANDLER cScr_NVQVarName::OnQuestChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_szQVar)
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);

		int iCurrent = 0;

		if ( pQSrv->Exists(m_szQVar) )
		{
			iCurrent = pQSrv->Get(m_szQVar); // Get QVar value
		}

		SService<IDataSrv> pDataSrv(g_pScriptManager);
		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		cMultiParm mpBook;
		cScrStr strPage0, strPage1;

		pDataSrv->GetString(strPage0, "ObjNames", ParamGetString(m_iObjId, "NVQVarNamePart1", ""), "", "Strings");

		const char* pszPart2;
		if ( iCurrent == 1 )
			pszPart2 = ParamGetString(m_iObjId, "NVQVarNamePart2Singular", "");

		if ( ( iCurrent != 1 ) || ( pszPart2[0] == '\0') )
			pszPart2 = ParamGetString(m_iObjId, "NVQVarNamePart2", "");

		pDataSrv->GetString(strPage1, "ObjNames", pszPart2, "", "Strings");



		char *szText = new char[strlen(strPage0) + strlen(strPage1) + 20];

		// Ready text for display
		sprintf(szText, "@Hack: \"%s%i%s\"", (const char*)strPage0, iCurrent, (const char*)strPage1);

		pPropSrv->Set(m_iObjId, "GameName", NULL, szText); // Set label

		delete[] szText;

		// Free memory.
		if (!strPage1.IsEmpty())
			strPage1.Free();

		if (!strPage0.IsEmpty())
			strPage0.Free();

	} else {
		DisplayPrintf("oh noes");
		DebugPrintf("oh noes");
	}

	return 0;
}


/***********************
 * NVSelectTrap
 */

MSGHANDLER cScr_NVSelectTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IDarkUISrv> pUI(g_pScriptManager);
	object oDest = ParamGetNVObject(m_iObjId, "NVSelectTrapItem", 0);
	if ( oDest )
	{
		pUI->InvSelect(oDest);
	}

	return 0;
}

MSGHANDLER cScr_NVSelectTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}


/***********************
 * NVAttackMessages
 */


MSGHANDLER cScr_NVAttackMessages::OnStartAttack(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sAttackMsg* pAttackMsg = static_cast<sAttackMsg*>(pMsg);

	if ( pAttackMsg->weapon != 0 )
		SimpleSend(m_iObjId, m_iObjId, "StartAttackMelee");
	else
		SimpleSend(m_iObjId, m_iObjId, "StartAttackRanged");

	return 0;
}

MSGHANDLER cScr_NVAttackMessages::OnEndAttack(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sAttackMsg* pAttackMsg = static_cast<sAttackMsg*>(pMsg);

	if ( pAttackMsg->weapon != 0 )
		SimpleSend(m_iObjId, m_iObjId, "EndAttackMelee");
	else
		SimpleSend(m_iObjId, m_iObjId, "EndAttackRanged");

	return 0;
}

MSGHANDLER cScr_NVAttackMessages::OnStartWindup(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sAttackMsg* pAttackMsg = static_cast<sAttackMsg*>(pMsg);

	if ( pAttackMsg->weapon != 0 )
		SimpleSend(m_iObjId, m_iObjId, "StartWindupMelee");
	else
		SimpleSend(m_iObjId, m_iObjId, "StartWindupRanged");

	return 0;
}

/***********************
 * NVDifficulty
 */

MSGHANDLER cScr_NVDifficulty::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

/*
	SService<IDataSrv> pDataSrv(g_pScriptManager);
	int iTick = 10;

	#if (_DARKGAME == 3) // Multiplayer check
		SService<INetworkingSrv> pNetSrv(g_pScriptManager);
		if ( pNetSrv->IsMultiplayer() )
		{
//			if ( pNetSrv->IsProxy(m_iObjId) )
//				return 0;

//			if ( pMsg->time < 10000 )
//				iTick = pDataSrv->RandInt(100, 200);
		}
	#endif

	g_pScriptManager->SetTimedMessage2(m_iObjId, "NVDifficultyTick", iTick, kSTM_OneShot, g_mpUndef);
	return 0;
}


MSGHANDLER cScr_NVDifficulty::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVDifficultyTick"))
		return 0; // Abort if it's not the right timer.
*/
	#if (_DARKGAME == 3) // Multiplayer check
		SService<INetworkingSrv> pNetSrv(g_pScriptManager);
		/*
		if ( pNetSrv->IsMultiplayer() )
		{
			if ( pNetSrv->IsProxy(m_iObjId) )
			{
//				DisplayPrintf("Finger weg!");
				return 0;
			}
//			pNetSrv->Suspend();
//			DisplayPrintf("Suspending network activity.");
		}
		*/
		pNetSrv->Suspend();
	#endif

	if ( ParamGetInt(m_iObjId, "NVDifficultyInitted", 0) == 0 )
	{
		ParamSetInt(m_iObjId, "NVDifficultyInitted", 1);

/*
		SInterface<ITraitManager> pTraits(g_pScriptManager);
		int iArchetype = 0;

		SService<IObjectSrv> pOS(g_pScriptManager);

		cScrStr strToName;
		pOS->GetName(strToName, m_iObjId);
		if ( strToName.IsEmpty() )
		{
			iArchetype = pTraits->GetArchetype(m_iObjId); // Make a note of object's archetype.
			pOS->GetName(strToName, iArchetype);
		}

		DisplayPrintf("NVDifficulty: on %s (%i)", static_cast<const char*>(strToName), m_iObjId);
		DebugPrintf("NVDifficulty: on %s (%i)", static_cast<const char*>(strToName), m_iObjId);
		strToName.Free();
*/
//		m_iInitted = 1;

		SService<IQuestSrv> pQSrv(g_pScriptManager);
		int iDiff = -1; // Default value (if QVar doesn't exist)
		if (pQSrv->Exists("Difficulty"))
		{
			iDiff = pQSrv->Get("Difficulty"); // Get QVar value
		}

		if (iDiff != -1)
		{
			SService<IPropertySrv> pPropSrv(g_pScriptManager);
			cMultiParm mpReturn;

#ifndef _MSC_VER
			char sz[14 + strlen(m_szName)];
			char szProp[14 + strlen(m_szName)];
			char szProp2[14 + strlen(m_szName)];
#else
			char *sz = (char*)alloca(14 + strlen(m_szName));
			char *szProp = (char*)alloca(14 + strlen(m_szName));
			char *szProp2 = (char*)alloca(14 + strlen(m_szName));
#endif

			sprintf(szProp, "%s%iOverride", m_szName, iDiff);
			iDiff = ParamGetInt(m_iObjId, szProp, iDiff);	// Replace iDiff with the override value IF IT EXISTS - otherwise default to iDiff.

			sprintf(sz, "Difficulty-%i", iDiff);
			SimpleSend(m_iObjId, m_iObjId, sz);

			float fNewValue = 0.00f, fOperand = 0.00f;
			const char* pszOperator;

			for ( int i = 0; i < 20; ++i )
			{
				sprintf(szProp, "%s%iProp%i", m_szName, iDiff, i);
				sprintf(szProp2, "%s%iSubProp%i", m_szName, iDiff, i);

				const char* pszProp = ParamGetString(m_iObjId, szProp, NULL);
				const char* pszSubProp = ParamGetString(m_iObjId, szProp2, NULL);
				if (pszProp == NULL )
					continue;

				if (!pPropSrv->Possessed(m_iObjId, pszProp))
					continue;

				pPropSrv->Get(mpReturn, m_iObjId, pszProp, pszSubProp);

				sprintf(sz, "%s%iOperator%i", m_szName, iDiff, i);
				pszOperator = ParamGetString(m_iObjId, sz, "*");

				sprintf(sz, "%s%iOperand%i", m_szName, iDiff, i);
				if ( mpReturn. type == kMT_String)
				{
//					pPropSrv->Set(m_iObjId, pszProp, pszSubProp, fNewValue);
					const char* szOperand = ParamGetString(m_iObjId, sz, "");
					char* szResult = NULL;

					switch (pszOperator[0])
					{
						case '=':
							szResult = new char[strlen(szOperand)+1];
							sprintf(szResult, "%s", szOperand);
							break;
						case '+':
							szResult = new char[strlen(szOperand)+strlen(mpReturn.psz)+1];
							sprintf(szResult, "%s%s", mpReturn.psz, szOperand);
							break;
						default:
							DisplayPrintf("Invalid operand for this type: %s", pszOperator);
							break;
					}
					#if (_DARKGAME == 3) // Multiplayer check
						pPropSrv->SetLocal(m_iObjId, pszProp, pszSubProp, szResult);
					#else
						pPropSrv->Set(m_iObjId, pszProp, pszSubProp, szResult);
					#endif

					if ( szResult != NULL )
						delete[] szResult;
				} else {
					fNewValue = static_cast<float>(mpReturn);
					fOperand = ParamGetFloat(m_iObjId, sz, 1.20f);

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
						default:
							DisplayPrintf("Invalid operand for this type: %s", pszOperator);
							break;
					}
					#if (_DARKGAME == 3) // Multiplayer check
						pPropSrv->SetLocal(m_iObjId, pszProp, pszSubProp, fNewValue);
					#else
						pPropSrv->Set(m_iObjId, pszProp, pszSubProp, fNewValue);
					#endif
				}
			}
		}

	}

	#if (_DARKGAME == 3) // Multiplayer check
		pNetSrv->Resume();
//		DisplayPrintf("Resuming network activity");
	#endif

	return 0;
}

/***********************
 * NVPsiProjectileThief
 */
MSGHANDLER cScr_NVPsiProjectileThief::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVPsiProjectileThief::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision);
	return 0;
}

MSGHANDLER cScr_NVPsiProjectileThief::OnPhysCollision(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sPhysMsg* pPhysMsg = static_cast<sPhysMsg*>(pMsg);
	int iCollObj = pPhysMsg->collObj;

	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bBool;
	pOS->InheritsFrom(bBool, iCollObj, ParamGetNVObject(m_iObjId, "NVPickyProjectileIgnore", 0));
	if ( bBool )
	{
		MultiParmAssignInt(pReply, 1);
	} else {
		SService<IDamageSrv> pDmgS(g_pScriptManager);
		pDmgS->Slay(m_iObjId, 0);
//		MultiParmAssignInt(pReply, 3);
	}

	return 0;
}

/***********************
 * NVCutsceneTrap
 */

MSGHANDLER cScr_NVCutsceneTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	const char* pszVideo = ParamGetString(m_iObjId, "NVCutscene", NULL);

	if ( !pszVideo )
		return 1;

	#if (_DARKGAME == 3)
		SService<IShockGameSrv> pShockGame(g_pScriptManager);
		pShockGame->PlayVideo(pszVideo);
	#else
		SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
		char *szVideo = new char[10 + strlen(pszVideo)];
		sprintf(szVideo, "movie %s", pszVideo);
		pDebugSrv->Command(szVideo, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);
	#endif

	return 0;
}

MSGHANDLER cScr_NVCutsceneTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVShakePlayer
 */

MSGHANDLER cScr_NVShakePlayer::OnShakeStim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sStimMsg* pStimMsg = static_cast<sStimMsg*>(pMsg);
	ShakeMeGood(pStimMsg->intensity);

	return 0;
}

void cScr_NVShakePlayer::PlayerShake(float intensity)
{
	SService<IDataSrv> pDS(g_pScriptManager);
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);

	cScrVec v;

	v.x = pDS->RandFlt0to1() * intensity;
	v.y = pDS->RandFlt0to1() * intensity;
	v.z = pDS->RandFlt0to1() * intensity;

	pPhysSrv->PlayerMotionSetOffset(0, v);
}

 void cScr_NVShakePlayer::ShakeMeGood(float intensity)
{
    PlayerShake(intensity * 0.1f);
	g_pScriptManager->SetTimedMessage2(m_iObjId, "ShakeTimer", 50, kSTM_OneShot, g_mpUndef);
	m_iShakeCount = 5;
	m_fShakeSize =  intensity*0.1f;
}

MSGHANDLER cScr_NVShakePlayer::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "ShakeTimer"))
		return 0; // Abort if it's not the right timer.

	PlayerShake(m_fShakeSize);

	if ( m_iShakeCount > 0 )
		g_pScriptManager->SetTimedMessage2(m_iObjId, "ShakeTimer", 50, kSTM_OneShot, g_mpUndef);

	--m_iShakeCount;

	return 0;
}

/***********************
 * NVPhysMessages
 */

MSGHANDLER cScr_NVPhysMessages::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->SubscribeMsg(cScript::m_iObjId, kPhysCollision | kPhysEnterExit | kPhysContact | kPhysFellAsleep);
	return 0;
}

MSGHANDLER cScr_NVPhysMessages::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	pPhysSrv->UnsubscribeMsg(cScript::m_iObjId, kPhysCollision | kPhysEnterExit | kPhysContact | kPhysFellAsleep);
	return 0;
}

/***********************
 * NVVisibilityToQVar
 */

MSGHANDLER cScr_NVVisibilityToQVar::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVVisibilityToQVar", 100, kSTM_Periodic, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVVisibilityToQVar::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVVisibilityToQVar"))
		return 0; // Abort if it's not the right timer.

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	if (pPropSrv->Possessed(m_iObjId, "AI_Visibility"))
	{
		cMultiParm mpProp;

		pPropSrv->Get(mpProp, m_iObjId, "AI_Visibility", "Level");
		int iLevel = static_cast<int>(mpProp);

		SService<IQuestSrv> pQSrv(g_pScriptManager);
		pQSrv->Set("NVVisibilityQVar", iLevel, 0);
	}

	return 0;
}


/***********************
 * NVFreePatrol
 */

MSGHANDLER cScr_NVFreePatrol::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	char *pszParamName = new char[14 + strlen(m_szName)];
	const char* pszFieldValue;
	mxs_vector vDest;

	sprintf(pszParamName, "%sPoint%i", m_szName, ParamGetInt(m_iObjId, "NVFreePatrolCurrentPoint", 0));
	pszFieldValue = ParamGetString(m_iObjId, pszParamName, NULL);

	if (!pszFieldValue)
	{
		DisplayPrintf("No first patrol point found.");
		DebugPrintf("No first patrol point found.");
		return 0;
	}

	if (3 == sscanf(pszFieldValue, "%f, %f, %f", &vDest.x, &vDest.y, &vDest.z))
	{
		// All right, we have a vector position, let's set it to the AI's current idle origin.
//		DisplayPrintf("Going to %f, %f, %f!", vDest.x, vDest.y, vDest.z);

		GoToPoint(vDest);

	} else {
		DisplayPrintf("No valid first patrol point  data - bailing.");
		DebugPrintf("No valid first patrol point data - bailing.");
		return 0;
	}

	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVFreePatrol", 1000, kSTM_Periodic, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVFreePatrol::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVFreePatrol"))
		return 0; // Abort if it's not the right timer.

	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	pPropSrv->Set(m_iObjId, "AI_IdlRetOrg", NULL, true);

	int i;
	char *pszParamName = new char[14 + strlen(m_szName)];
	const char* pszFieldValue;
	mxs_vector vDest;
	cScrVec vPosition;
	pOS->Position(vPosition, m_iObjId);
	float fDistance = 0;

	float fTripDistance = ParamGetFloat(m_iObjId, "NVFreePatrolDistance", 5.00f);
	float fTripHeight = ParamGetFloat(m_iObjId, "NVFreePatrolHeight", 12.00f);

	for (i=0;i<99;i++)
	{

		sprintf(pszParamName, "%sPoint%i", m_szName, i);

		pszFieldValue = ParamGetString(m_iObjId, pszParamName, NULL);

		if (!pszFieldValue)
			break;

		if (3 == sscanf(pszFieldValue, "%f, %f, %f", &vDest.x, &vDest.y, &vDest.z))
		{
			// Is the AI near here?
			fDistance = GetDistanceXY(&vPosition, &vDest);

			if ( fDistance <= fTripDistance )
			{
				if ( fabs(vPosition.z - vDest.z) <= fTripHeight )
				{
					// The AI is here, let's set the next patrol point:
//					DisplayPrintf("Near point %i", i);
					if ( ParamGetInt(m_iObjId, "NVFreePatrolCurrentPoint", -1) == i+1 )
						return 0;	// Don't process the same point twice.

					ParamSetInt(m_iObjId, "NVFreePatrolCurrentPoint", i+1);

					sprintf(pszParamName, "%sPoint%i", m_szName, i+1);

					pszFieldValue = ParamGetString(m_iObjId, pszParamName, NULL);
					if (!pszFieldValue)
					{
						sprintf(pszParamName, "%sPoint%i", m_szName, 0);
						pszFieldValue = ParamGetString(m_iObjId, pszParamName, NULL);

						if (!pszFieldValue)
						{
							DisplayPrintf("No next patrol point found.");
							DebugPrintf("No next patrol point found.");
							return 0;
						}

						ParamSetInt(m_iObjId, "NVFreePatrolCurrentPoint", 0);
					}

					if (3 == sscanf(pszFieldValue, "%f, %f, %f", &vDest.x, &vDest.y, &vDest.z))
					{
						// All right, we have a vector position, let's set it to the AI's current idle origin.
						GoToPoint(vDest);

						return 0;
					}
				}
			}
		} else {
			DisplayPrintf("Value invalid.");
			DebugPrintf("Value invalid.");
		}
	}

	return 0;
}

void cScr_NVFreePatrol::GoToPoint(mxs_vector vDest)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	pPropSrv->Set(m_iObjId, "AI_IdleOrgn", "Original Location", vDest);

	SService<IAIScrSrv> pAISrv(g_pScriptManager);
	pAISrv->ClearGoals(m_iObjId);
	pPropSrv->Set(m_iObjId, "AI_IdlRetOrg", NULL, true);


	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	// Set up an AIWatchObj link to make sure that the AI will stop when it gets too close to its destination.
	IterateLinks("AIWatchObj", m_iObjId, m_iObjId, RemoveWatchObjIterFunc, NULL, NULL);
	link lLink;
	pLS->Create(lLink, pLTS->LinkKindNamed("AIWatchObj"), m_iObjId, m_iObjId);
	if (lLink)
	{
		pLTS->LinkSetData(lLink, "Watch kind", 1); // Self Entry
		#if (_DARKGAME != 3)
			pLTS->LinkSetData(lLink, "Priority", 3); // Normal
		#endif
		pLTS->LinkSetData(lLink, "Trigger: Radius", 2);
		pLTS->LinkSetData(lLink, "         Height", 8);
		pLTS->LinkSetData(lLink, "         Maximum alertness", 2);
		pLTS->LinkSetData(lLink, "Exit: Link kill option", 1); // After trigger
		pLTS->LinkSetData(lLink, "Response: Step 1", 2); // Play sound / motion
		pLTS->LinkSetData(lLink, "   Argument 1", "Nothing");
//		pLTS->LinkSetData(lLink, "Response: Step 1", 1); // Script message(Message,MsgData1,MsgData2)
//		pLTS->LinkSetData(lLink, "   Argument 1", "FreePatrolPoint");
//							pLTS->LinkSetData(lLink, "Response: Step 1", 8); // Wait(Time(ms),Motion Tags)
//							pLTS->LinkSetData(lLink, "   Argument 1", "1000");
//							pLTS->LinkSetData(lLink, "   Argument 2", "crumple, die");
		pLTS->LinkSetData(lLink, "   Argument 3", "NVFreePatrol (reserved)");
	}
	true_bool bBool;
	pAISrv->MakeGotoObjLoc(bBool, m_iObjId, m_iObjId, kNormalSpeed, kNormalPriorityAction, 0);

	return;
}

int cScr_NVFreePatrol::RemoveWatchObjIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	cMultiParm mpReturn;
	pLTS->LinkGetData(mpReturn, pLQ->ID(), "   Argument 3");
	if ( strcmp(mpReturn, "NVFreePatrol (reserved)") )
		return 1;

	pLS->Destroy(pLQ->ID());

	return 1;
}

/***********************
 * NVKeyboardController
 */

 MSGHANDLER cScr_NVKeyboardController::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SInterface<IInputBinder> pInput(g_pScriptManager);
	pInput->SetValidEvents(0);

	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVKeyboardController", 20, kSTM_Periodic, g_mpUndef);
	}

	return 0;
}

MSGHANDLER cScr_NVKeyboardController::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SInterface<IInputBinder> pInput(g_pScriptManager);
	pInput->SetValidEvents(-1);
	return 0;
}

MSGHANDLER cScr_NVKeyboardController::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

MSGHANDLER cScr_NVKeyboardController::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVKeyboardController"))
		return 0; // Abort if it's not the right timer.

	if ( GetAsyncKeyState(VK_ESCAPE) < 0 )
	{
		DisplayPrintf("You are pressing Escape.");
		SInterface<IInputBinder> pInput(g_pScriptManager);
		pInput->SetValidEvents(-1);
	}

	std::string s = ParamGetString(m_iObjId, "NVKeyboardControllerString", "");

//	char keyScan;
	int c;

	std::ostringstream str;

//	for ( char c = 65; c <= 90; ++c)
	for ( c = 0; c < 256; ++c)
	{
		//VkKeyScan(*c);
		if ( GetAsyncKeyState(c) < 0 )
		{
			if ( m_keys[c] == 0 )
			{
				if ( c == VK_BACK )
				{
					s.resize(s.length()-1);
				} else {
//				DisplayPrintf("You are pressing %c.", c[0]);
				str << static_cast<unsigned char>(c);
				}
			}
			m_keys[c] = 1;
		} else {
			m_keys[c] = 0;
		}
	}

	s += str.str();
/*
	if ( GetAsyncKeyState(VkKeyScan(' ')) < 0 )
	{
		s = s + " ";
	}

	if ( GetAsyncKeyState(VK_BACK) < 0 )
	{
		s.resize(s.length()-1);
	}
	*/
	ParamSetString(m_iObjId, "NVKeyboardControllerString", s.c_str());
	DisplayPrintf("%s", s.c_str());
	return 0;
}

/***********************
 * NVTweqEmit
 */

MSGHANDLER cScr_NVTweqEmit::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SetEmitter(true);

	return 0;
}

MSGHANDLER cScr_NVTweqEmit::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SetEmitter(false);

	return 0;
}

void cScr_NVTweqEmit::SetEmitter(bool bOn)
{
	char pszCfgTweqEmit[16];
	char pszStTweqEmit[16];

	// Get Tweq Number
	int iTweqNum = iTweqNum = ParamGetInt(m_iObjId, "NVTweqEmitTweqNum", 1);
	if ((iTweqNum > 5) || iTweqNum < 1 )
	{
		iTweqNum = 1; // The Tweq number specified is invalid!
	}

	if ( iTweqNum == 1) {
		sprintf(pszStTweqEmit, "StTweqEmit");
		sprintf(pszCfgTweqEmit, "CfgTweqEmit");
	} else {
		sprintf(pszStTweqEmit, "StTweq%iEmit", iTweqNum);
		sprintf(pszCfgTweqEmit, "CfgTweq%iEmit", iTweqNum);
	}



	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if ( bOn )
	{
		if ( ParamGetInt(m_iObjId, "NVTweqEmitReload", 0) == 1 )
		{
			if (pPropSrv->Possessed(m_iObjId, pszCfgTweqEmit))
			{
				cMultiParm mpRate;
				pPropSrv->Get(mpRate, m_iObjId, pszCfgTweqEmit, "Rate");
				int iRate = static_cast<int>(mpRate);

				pPropSrv->Set(m_iObjId, pszStTweqEmit, "Frame #", 0);
				pPropSrv->Set(m_iObjId, pszStTweqEmit, "Cur Time", iRate-1);
			}
		}
		pPropSrv->Set(m_iObjId, pszStTweqEmit, "AnimS", 1); // Turn on emitter
	} else {
		pPropSrv->Set(m_iObjId, pszStTweqEmit, "AnimS", 0); // Turn off emitter
	}
}

/***********************
 * NVHangGliderTrap
 */

MSGHANDLER cScr_NVHangGliderTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "NVHangGliderTrapTimer", 100, kSTM_Periodic, g_mpUndef);
	}

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	pPropSrv->Set(m_iObjId, "PhysAttr", "Gravity %", 0.00f);
	pPropSrv->Set(m_iObjId, "PhysAttr", "Base Friction", ParamGetFloat(m_iObjId, "NVHangGliderTrapFriction", 0.75f));

	return 0;
}

MSGHANDLER cScr_NVHangGliderTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	pPropSrv->Set(m_iObjId, "PhysAttr", "Gravity %", 100.00f);
	pPropSrv->Set(m_iObjId, "PhysAttr", "Base Friction", 0.00f);

	return 0;
}

MSGHANDLER cScr_NVHangGliderTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "NVHangGliderTrapTimer"))
		return 0; // Abort if it's not the right timer.

	SService<IPhysSrv> pPhysSrv(g_pScriptManager);
	cScrVec vVelocity;
	pPhysSrv->GetVelocity(m_iObjId, vVelocity);

	vVelocity.z = -ParamGetFloat(m_iObjId, "NVHangGliderTrapDownSpeed", 6.00f);

	pPhysSrv->SetVelocity(m_iObjId, vVelocity);

	return 0;
}


/***********************
 * NVWatchMe
 */

MSGHANDLER cScr_NVWatchMe::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	const char* pszWatchers = NVParamGetString(m_iObjId, m_szName, "Target", "@Human");
	std::vector<int> watchers = GetObjects(pszWatchers, m_iObjId, pMsg);

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link lLink;
	int iFlavour = pLTS->LinkKindNamed("AIWatchObj");

	for ( std::vector<int>::iterator it = watchers.begin(); it != watchers.end(); ++it)
	{
		pLS->Create(lLink, iFlavour, *it, m_iObjId);
	}

	return 0;

}

/***********************
 * NVWatchMeTrap
 */

MSGHANDLER cScr_NVWatchMeTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	const char* pszWatchers = NVParamGetString(m_iObjId, m_szName, "Target", "@Human");
	std::vector<int> watchers = GetObjects(pszWatchers, m_iObjId, pMsg);

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link lLink;
	int iFlavour = pLTS->LinkKindNamed("AIWatchObj");

	for ( std::vector<int>::iterator it = watchers.begin(); it != watchers.end(); ++it)
	{
		pLS->Create(lLink, iFlavour, *it, m_iObjId);
	}

	return 0;
}

MSGHANDLER cScr_NVWatchMeTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	const char* pszWatchers = NVParamGetString(m_iObjId, m_szName, "Target", "@Human");
	std::vector<int> watchers = GetObjects(pszWatchers, m_iObjId, pMsg);

	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link lLink;
	int iFlavour = pLTS->LinkKindNamed("AIWatchObj");

	for ( std::vector<int>::iterator it = watchers.begin(); it != watchers.end(); ++it)
	{
		true_bool bLinkExists;
		pLS->AnyExist(bLinkExists, iFlavour, *it, m_iObjId);
		if ( bLinkExists )
		{
			link lLink;
			pLS->GetOne(lLink, iFlavour, *it, m_iObjId);
			if (lLink)
			{
				pLS->Destroy(lLink);
			}
		}
	}

	return 0;
}

/***********************
 * NVGenericScalarTrap
 */

MSGHANDLER cScr_NVGenericScalarTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	if (GetTimerHandle() != NULL)
	{	// Stop timer:
//		DisplayPrintf("Not null on %s!", m_szName);
//		DebugPrintf("Not null on %s!", m_szName);
		g_pScriptManager->KillTimedMessage(GetTimerHandle());
		SetTimerHandle(NULL);
	}
	DoFade(true, pMsg);
	return 0;
}

MSGHANDLER cScr_NVGenericScalarTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (GetTimerHandle() != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(GetTimerHandle());
		SetTimerHandle(NULL);
	}
	DoFade(false, pMsg);
	return 0;
}

float cScr_NVGenericScalarTrap::GetMax()
{
	return NVParamGetFloat(m_iObjId, m_szName, "Max", 1.00f);
}

float cScr_NVGenericScalarTrap::GetMin()
{
	return NVParamGetFloat(m_iObjId, m_szName, "Min", 0.00f);
}

float cScr_NVGenericScalarTrap::GetDefaultValue()
{
	return NVParamGetFloat(m_iObjId, m_szName, "Default", 0.00f);
}

float cScr_NVGenericScalarTrap::GetIncrement()
{
	return NVParamGetFloat(m_iObjId, m_szName, "Increment", 0.02f);
}

int cScr_NVGenericScalarTrap::GetTimerOn()
{
	return (int)(NVParamGetInt(m_iObjId, m_szName, "FadeOn", 1000) / fabs(GetMax() - GetMin()) * GetIncrement());
}

int cScr_NVGenericScalarTrap::GetTimerOff()
{
	return (int)(NVParamGetInt(m_iObjId, m_szName, "FadeOff", 1000) / fabs(GetMax() - GetMin()) * GetIncrement());
}

const char* cScr_NVGenericScalarTrap::GetPropName()
{
	return NVParamGetString(m_iObjId, m_szName, "PropName", NULL);
}

const char* cScr_NVGenericScalarTrap::GetSubPropName()
{
	return NVParamGetString(m_iObjId, m_szName, "SubPropName", NULL);
}

const char* cScr_NVGenericScalarTrap::GetOnMessage()
{
	return NVParamGetString(m_iObjId, m_szName, "OnMessage", "FadeOn");
}

const char* cScr_NVGenericScalarTrap::GetOffMessage()
{
	return NVParamGetString(m_iObjId, m_szName, "OffMessage", "FadeOff");
}

bool cScr_NVGenericScalarTrap::GetRemovePropOnZero()
{
	return (NVParamGetInt(m_iObjId, m_szName, "RemovePropOnZero", 0) != 0);
}

void cScr_NVGenericScalarTrap::DoExtraTasks(int obj, float fCurrent)
{
	// Do nothing.
}

void cScr_NVGenericScalarTrap::DoFade(bool bOn, sScrMsg* pMsg)
{
	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	if ( GetPropName() == NULL )
	{
		DisplayPrintf(0x0000FF, "NVScript Error: NVGenericScalerTrap with NULL property on %s (%i).", static_cast<const char*>(GetObjectDescriptiveName(m_iObjId)), m_iObjId);
		DebugPrintf("NVScript Error: NVGenericScalerTrap with NULL property on %s (%i).", static_cast<const char*>(GetObjectDescriptiveName(m_iObjId)), m_iObjId);
		return;
	}

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		float fCurrent = GetDefaultValue();
		int iTimer = 100;
		if (pPropSrv->Possessed(*it, GetPropName()))
		{
			cMultiParm mpReturn;
			pPropSrv->Get(mpReturn, *it, GetPropName(), GetSubPropName());
			fCurrent = static_cast<float>(mpReturn);
		}

		float fMax = GetMax();
		float fMin = GetMin();
		float fDest = 0.00f;
		float fIncrement = GetIncrement();

		if (bOn)
		{
			iTimer = GetTimerOn();
		} else {
			iTimer = GetTimerOff();
		}

		if (NVParamGetInt(m_iObjId, m_szName, "HighPrecision", 0) == 0)
		{
			if (iTimer < 100)
			{
				iTimer *= 3;
				fIncrement *= 3.00f;
			}
		}

		if (bOn)
		{
			fDest = fMax;
			if (fCurrent < fDest)
			{
				fCurrent += fIncrement;
				if ( fCurrent > fMax )
					fCurrent = fMax;

				pPropSrv->Set(*it, GetPropName(), GetSubPropName(), fCurrent);

				char *pszTimerNameUp = new char[3 + strlen(m_szName)];
				sprintf(pszTimerNameUp, "%sUp", m_szName);

				SetTimerHandle(g_pScriptManager->SetTimedMessage2(m_iObjId, pszTimerNameUp, iTimer, kSTM_OneShot, g_mpUndef));
			} else {
				SetTimerHandle(NULL);
				SimplePost(*it, *it, GetOnMessage());
			}
		} else {
			fDest = fMin;

			if (fCurrent > fDest)
			{
				fCurrent -= fIncrement;
				if ( fCurrent < fMin )
					fCurrent = fMin;

				if ( ( GetRemovePropOnZero() == true ) && ( fCurrent <= 0.00f ) )
				{
					pPropSrv->Remove(*it, GetPropName());
				} else {
					pPropSrv->Set(*it, GetPropName(), GetSubPropName(), fCurrent);
				}

				char *pszTimerNameDown = new char[5 + strlen(m_szName)];
				sprintf(pszTimerNameDown, "%sDown", m_szName);

				SetTimerHandle(g_pScriptManager->SetTimedMessage2(m_iObjId, pszTimerNameDown, iTimer, kSTM_OneShot, g_mpUndef));
			} else {
				SetTimerHandle(NULL);
				SimplePost(*it, *it, GetOffMessage());
			}
		}

		DoExtraTasks(*it, fCurrent);
	}

	return;
}

MSGHANDLER cScr_NVGenericScalarTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);

	char *pszTimerNameUp = new char[3 + strlen(m_szName)];
	sprintf(pszTimerNameUp, "%sUp", m_szName);
	char *pszTimerNameDown = new char[5 + strlen(m_szName)];
	sprintf(pszTimerNameDown, "%sDown", m_szName);

	if (!stricmp(pszMsgName, pszTimerNameUp))
		DoFade(true, pMsg);
	else if (!stricmp(pszMsgName, pszTimerNameDown))
		DoFade(false, pMsg);

	return 0;
}

/***********************
 * NVGlowTrap
 */

float cScr_NVGlowTrap::GetMax()
{	return ParamGetFloat(m_iObjId, "NVGlowMax", 100.00f); }

float cScr_NVGlowTrap::GetMin()
{	return ParamGetFloat(m_iObjId, "NVGlowMin", 0.00f); }

float cScr_NVGlowTrap::GetDefaultValue()
{	return ParamGetFloat(m_iObjId, "NVGlowTrapDefault", 0.00f); }

float cScr_NVGlowTrap::GetIncrement()
{	return NVParamGetFloat(m_iObjId, m_szName, "Increment", 1.00); }

int cScr_NVGlowTrap::GetTimerOn()
{	return (int)(ParamGetInt(m_iObjId, "NVGlowFadeOn", 1000) / fabs(GetMax() - GetMin()) * GetIncrement()); }

int cScr_NVGlowTrap::GetTimerOff()
{	return (int)(ParamGetInt(m_iObjId, "NVGlowFadeOff", 1000) / fabs(GetMax() - GetMin()) * GetIncrement()); }

const char* cScr_NVGlowTrap::GetPropName()
{	return NVParamGetString(m_iObjId, m_szName, "PropName", "SelfLit"); }

const char* cScr_NVGlowTrap::GetSubPropName()
{	return NVParamGetString(m_iObjId, m_szName, "SubPropName", NULL); }

const char* cScr_NVGlowTrap::GetOnMessage()
{	return NVParamGetString(m_iObjId, m_szName, "OnMessage", "GlowOn"); }

const char* cScr_NVGlowTrap::GetOffMessage()
{	return NVParamGetString(m_iObjId, m_szName, "OffMessage", "GlowOff"); }

bool cScr_NVGlowTrap::GetRemovePropOnZero()
{	return NVParamGetInt(m_iObjId, m_szName, "RemovePropOnZero", 1) != 0 ; 	/* A dynamic light property of 0 will mess up the visibility gem. */ }

void cScr_NVGlowTrap::DoExtraTasks(int obj, float fCurrent)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	if ( ParamExists(m_iObjId, "NVGlowIllumMax") )
	{
		if ( GetMax() > 0.00f )
		{
			float fGlowMin = ParamGetFloat(m_iObjId, "NVGlowIllumMin", 0.00f);
			float fGlowMax = ParamGetFloat(m_iObjId, "NVGlowIllumMax", 1.00f);
			float fIllum = fGlowMin + ((fGlowMax - fGlowMin) * (fCurrent/GetMax()));
			if ( fCurrent == 0.00f )
				fIllum = fGlowMin;
			pPropSrv->Set(obj, "SelfIllum", NULL, fIllum);
		}
	}
}

/***********************
 * NVPhantomTrap
 */

float cScr_NVPhantomTrap::GetMax()
{	return ParamGetFloat(m_iObjId, "NVPhantomAlphaMax", 1.00f); }

float cScr_NVPhantomTrap::GetMin()
{	return ParamGetFloat(m_iObjId, "NVPhantomAlphaMin", 0.00f); }

float cScr_NVPhantomTrap::GetDefaultValue()
{	return ParamGetFloat(m_iObjId, "NVPhantomTrapDefault", 1.00f); }

float cScr_NVPhantomTrap::GetIncrement()
{	return NVParamGetFloat(m_iObjId, m_szName, "Increment", 0.02f); }

int cScr_NVPhantomTrap::GetTimerOn()
{	return (int)(ParamGetInt(m_iObjId, "NVPhantomFadeOn", 1000) / 50); }

int cScr_NVPhantomTrap::GetTimerOff()
{	return (int)(ParamGetInt(m_iObjId, "NVPhantomFadeOff", 1000) / 50); }

const char* cScr_NVPhantomTrap::GetPropName()
{	return NVParamGetString(m_iObjId, m_szName, "PropName", "RenderAlpha"); }

const char* cScr_NVPhantomTrap::GetSubPropName()
{	return NVParamGetString(m_iObjId, m_szName, "SubPropName", NULL); }

const char* cScr_NVPhantomTrap::GetOnMessage()
{	return NVParamGetString(m_iObjId, m_szName, "OnMessage", "PhantomOn"); }

const char* cScr_NVPhantomTrap::GetOffMessage()
{	return NVParamGetString(m_iObjId, m_szName, "OffMessage", "PhantomOff"); }

bool cScr_NVPhantomTrap::GetRemovePropOnZero()
{	return NVParamGetInt(m_iObjId, m_szName, "RemovePropOnZero", 0) != 0 ; }

void cScr_NVPhantomTrap::DoExtraTasks(int obj, float fCurrent)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if ( !ParamGetInt(m_iObjId, "NVPhantomRenderInvisible", 1 ) )
	{
		if ( fCurrent <= 0.01f )
			pPropSrv->Set(obj, "RenderType", NULL, 1);
		else
			pPropSrv->Set(obj, "RenderType", NULL, ParamGetInt(m_iObjId, "NVPhantomRenderType", 0));
	}
}


/***********************
 * NVFogTrap
 */

MSGHANDLER cScr_NVFogTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{

	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	DoFade(true, pMsg);
	return 0;
}

MSGHANDLER cScr_NVFogTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle != NULL)
	{	// Stop timer:
		g_pScriptManager->KillTimedMessage(m_hTimerHandle);
		m_hTimerHandle = NULL;
	}
	DoFade(false, pMsg);
	return 0;
}

void cScr_NVFogTrap::DoFade(bool bOn, sScrMsg* pMsg)
{
	SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);

	int iMax = NVParamGetInt(m_iObjId, m_szName, "Max", 128);
	int iMin = NVParamGetInt(m_iObjId, m_szName, "Min", 8);
	int iIncrement = NVParamGetInt(m_iObjId, m_szName, "Increment", 1);


	int iTimerOn = (int)(NVParamGetInt(m_iObjId, m_szName, "FadeOn", 1000) / (float)abs(iMax - iMin) * iIncrement);
	int iTimerOff = (int)(NVParamGetInt(m_iObjId, m_szName, "FadeOn", 1000) / (float)abs(iMax - iMin) * iIncrement);

	int iCurrent = m_iCurrent;
	int iDest = 0;

	if ( iCurrent == -1 )
	{
		iCurrent = NVParamGetInt(m_iObjId, m_szName, "DefaultValue", 1);
	}

	int iTimer = 100;

	if (bOn)
	{
		iTimer = iTimerOn;
	} else {
		iTimer = iTimerOff;
	}

	if (NVParamGetInt(m_iObjId, m_szName, "HighPrecision", 0) == 0)
	{
		if (iTimer < 100)
		{
			iTimer *= 3;
			iIncrement *= 3;
		}
	}

	if (bOn)
	{
		iDest = iMax;
		if (iCurrent < iDest)
		{
			iCurrent += iIncrement;
			if ( iCurrent > iMax )
				iCurrent = iMax;

			char *pszTimerNameUp = new char[3 + strlen(m_szName)];
			sprintf(pszTimerNameUp, "%sUp", m_szName);

			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, pszTimerNameUp, iTimer, kSTM_OneShot, g_mpUndef);
		} else {
			m_hTimerHandle = NULL;
		}
	} else {
		iDest = iMin;

		if (iCurrent > iDest)
		{
			iCurrent -= iIncrement;
			if ( iCurrent < iMin )
				iCurrent = iMin;

			char *pszTimerNameDown = new char[5 + strlen(m_szName)];
			sprintf(pszTimerNameDown, "%sDown", m_szName);

			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, pszTimerNameDown, iTimer, kSTM_OneShot, g_mpUndef);
		} else {
			m_hTimerHandle = NULL;
		}
	}

	m_iCurrent = iCurrent;
	char *szCommand = new char[18];
	sprintf(szCommand, "fog_dist %i", iCurrent);
	pDebugSrv->Command(szCommand, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null, cScrStr::Null);

	return;
}

MSGHANDLER cScr_NVFogTrap::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);

	char *pszTimerNameUp = new char[3 + strlen(m_szName)];
	sprintf(pszTimerNameUp, "%sUp", m_szName);
	char *pszTimerNameDown = new char[5 + strlen(m_szName)];
	sprintf(pszTimerNameDown, "%sDown", m_szName);

	if (!stricmp(pszMsgName, pszTimerNameUp))
		DoFade(true, pMsg);
	else if (!stricmp(pszMsgName, pszTimerNameDown))
		DoFade(false, pMsg);

	return 0;
}

/***********************
 * NVJointControl
 */


long __stdcall cScr_NVJointControl::ReceiveMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	char *pszParamName = new char[17 + strlen(m_szName)];
	char *pszPropName = new char[20];
	const char* pszMessageName = NULL;

	for ( int i = 1; i < 100; ++i )
	{
		sprintf(pszParamName, "%sOn%i", m_szName, i);

		// Stop once we run out of messages
		if (!ParamExists(m_iObjId, pszParamName) )
			break;

		pszMessageName = ParamGetString(m_iObjId, pszParamName, "TurnOn");

		if ( !stricmp(pMsg->message, pszMessageName) )
		{
//			DisplayPrintf("Activating joint configuration %i", i);

			for ( int j = 1; j <= 6; ++j )
			{
				// "NVJointControlOn1Joint1"
				sprintf(pszParamName, "%sOn%iJoint%i", m_szName, i, j);

				if (ParamExists(m_iObjId, pszParamName) )
				{
					bool reverse = false;
					float jointDest = ParamGetFloat(m_iObjId, pszParamName, 0);

					cMultiParm mpProp;
					sprintf(pszParamName, "Joint %i", j);
					pPropSrv->Get(mpProp, m_iObjId, "JointPos", pszParamName);
					float  jointSource = static_cast<float>(mpProp);

					while ( jointSource >= 360.0f)	// Float modulus
						jointSource -= 360.0f;

					while ( jointSource < 0.0f)	// Float modulus
						jointSource += 360.0f;

					float distanceClockwise = (jointDest - jointSource + 360.0f);
					while ( distanceClockwise >= 360.0f)	// Float modulus
						distanceClockwise -= 360.0f;

					float distanceAnticlockwise = (jointSource - jointDest+ 360.0f);
					while ( distanceAnticlockwise >= 360.0f)	// Float modulus
						distanceAnticlockwise -= 360.0f;

					if ( distanceAnticlockwise < distanceClockwise )
					{
						reverse = true;
					}

					if ( distanceAnticlockwise > 0.0f || distanceClockwise > 0.0f )
					{


						if ( !reverse )
						{
							if ( jointSource > jointDest )
							{
								jointSource -= 360.0f;
							}
						} else {
						if ( jointSource < jointDest )
							{
								jointSource += 360.0f;
							}
						}
						pPropSrv->Set(m_iObjId, "JointPos", pszParamName, jointSource);


						if ( j == 1 )
						{
							sprintf(pszPropName, "    rate-low-high");
						} else {
							sprintf(pszPropName, "    rate-low-high%i", j);
						}

						cMultiParm mpReturn;
						mxs_vector vJoint;
						pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqJoints", pszPropName);

						sprintf(pszParamName, "%sOn%iJoint%iSpeed", m_szName, i, j);
						if (ParamExists(m_iObjId, pszParamName) )
						{
							vJoint.x = ParamGetFloat(m_iObjId, pszParamName, 10.0f);
						} else {
							vJoint.x = static_cast<const mxs_vector*>(mpReturn)->x;
						}

						if ( reverse )
						{
							vJoint.y = jointDest;
							vJoint.z = jointSource;
						} else {
							vJoint.y = jointSource;
							vJoint.z = jointDest;
						}
						pPropSrv->Set(m_iObjId, "CfgTweqJoints", pszPropName, vJoint);


						// Turn the joint on
						sprintf(pszPropName, "Joint%iAnimS", j);
						if ( !reverse )
						{
							pPropSrv->Set(m_iObjId, "StTweqJoints", pszPropName, 1);
						} else {
							pPropSrv->Set(m_iObjId, "StTweqJoints", pszPropName, 3);
						}

						sprintf(pszPropName, "JointState%i", j);
						ParamSetInt(m_iObjId, pszPropName, 1);

						sprintf(pszPropName, "JointStarted%i", j);
						SimpleSend(m_iObjId, m_iObjId, pszPropName);
					}
				}
			}

			SetupPrimaryJoint();

			// We don't want to trigger more than one joint set from a single script message
			break;
		}

	}

	cScriptNV::ReceiveMessage(pMsg, pReply, eTrace);

	delete[] pszParamName;
	delete[] pszPropName;

	return 0;
}

void cScr_NVJointControl::SetupPrimaryJoint()
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	char *pszPropName = new char[20];
	cMultiParm mpReturn;

	float min = 0;
	int minJoint = 0;
	bool minReverse = false;

	for ( int j = 1; j <= 6; ++j )
	{

		sprintf(pszPropName, "Joint%iAnimS", j);
		pPropSrv->Get(mpReturn, m_iObjId, "StTweqJoints", pszPropName);

		if ( !((int)mpReturn & 1))
		{
			// If this joint has no distance to travel, yet is marked as active
			// then un-mark it as active and send a "JointComplete#" message.
			sprintf(pszPropName, "JointState%i", j);
			if ( ParamExists(m_iObjId, pszPropName) )
			{
				ParamRemove(m_iObjId, pszPropName);

				sprintf(pszPropName, "JointComplete%i", j);
				SimpleSend(m_iObjId, m_iObjId, pszPropName);
			}

			// This joint isn't on, so ignore it
			continue;
		}

		bool reverse = (((int)(mpReturn) & 2) != 0);

		sprintf(pszPropName, "Joint %i", j);
		pPropSrv->Get(mpReturn, m_iObjId, "JointPos", pszPropName);
		float current = static_cast<float>(mpReturn);

		if ( j == 1 )
		{
			sprintf(pszPropName, "    rate-low-high");
		} else {
			sprintf(pszPropName, "    rate-low-high%i", j);
		}


		// Calculate the distance for this joint

		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqJoints", pszPropName);
		float distance;

		if ( !reverse )
		{
			distance = (static_cast<const mxs_vector*>(mpReturn)->z - current + 360.0f);
		} else {
			distance = (current - static_cast<const mxs_vector*>(mpReturn)->y + 360.0f);
		}

		while ( distance >= 360.0f)	// Float modulus
			distance -= 360.0f;

		// Divide distance by rate
		distance /= static_cast<const mxs_vector*>(mpReturn)->x;

		if ( (minJoint == 0) || (distance < min) )
		{
			min = distance;
			minJoint = j;
			minReverse = reverse;
//			DebugPrintf("Joint %i is the new Min Joint", j);
		}
	}

//	DisplayPrintf("Primary joint is %i, reverse is %i, distance is %f", minJoint, minReverse, min);

	if ( minJoint != 0 )
	{
		pPropSrv->Set(m_iObjId, "CfgTweqJoints", "Primary Joint", minJoint);

		if ( !minReverse )
		{
			pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", 1);
		} else {
			pPropSrv->Set(m_iObjId, "StTweqJoints", "AnimS", 3);
		}
	}

	delete[] pszPropName;
}

MSGHANDLER cScr_NVJointControl::OnTweqComplete(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sTweqMsg* pTweqMsg = static_cast<sTweqMsg*>(pMsg);

	if ( pTweqMsg->Type == kTweqTypeJoints )
	{ // Make sure it was a TweqJoints which just completed...
		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		cMultiParm mpReturn;
		pPropSrv->Get(mpReturn, m_iObjId, "CfgTweqJoints", "Primary Joint");

		int primaryJoint = static_cast<int>(mpReturn);

		// Turn off the primary joint
		char *pszMessage = new char[16];

		sprintf(pszMessage, "Joint%iAnimS", primaryJoint);
		pPropSrv->Set(m_iObjId, "StTweqJoints", pszMessage, 0);

		delete[] pszMessage;

//		DisplayPrintf("Joint %i completed.", primaryJoint);

		SetupPrimaryJoint();
	}

	return 0;
}

/***********************
 * NVAlertMessages
 */
MSGHANDLER cScr_NVAlertMessages::OnAlertness(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sAIAlertnessMsg* pAlertMsg = static_cast<sAIAlertnessMsg*>(pMsg);

	char *pszMsg = new char[16];

	sprintf(pszMsg, "Alert%i", pAlertMsg->level);
	SimpleSend(pMsg->from, m_iObjId, pszMsg);
	sprintf(pszMsg, "Alert%iTo%i", pAlertMsg->oldLevel, pAlertMsg->level);
	SimpleSend(pMsg->from, m_iObjId, pszMsg);
	delete[] pszMsg;

	return 0;
}

/***********************
 * NVWakePhysicsTrap
 */

MSGHANDLER cScr_NVWakePhysicsTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		cMultiParm mpReturn;
		SService<IPropertySrv> pPropSrv(g_pScriptManager);

		// Don't do anything unless the object has the Physics->Model->Type property
		if (pPropSrv->Possessed(*it, "PhysType"))
		{
			// Get PhysType
			pPropSrv->Get(mpReturn, *it, "PhysType", "Type");
			int iPhysType = static_cast<int>(mpReturn);

			// Don't do anything if it has the "None" physics type...
			if ( iPhysType < 3) {
				// Touch the velocity to 'wake up' the physics model
				// (And make an enlarged model fall through the floor. Ooops.
				SService<IPhysSrv> pPhysSrv(g_pScriptManager);
				cScrVec vVelocity;
				pPhysSrv->GetVelocity(*it, vVelocity);
				vVelocity.z = vVelocity.z + 0.01f;
				pPhysSrv->SetVelocity(*it, vVelocity);
			}
		}
	}

	return 0;
}

MSGHANDLER cScr_NVWakePhysicsTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}

/***********************
 * NVRemovePropertyTrap
 */

MSGHANDLER cScr_NVRemovePropertyTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	const char* pszTarget = NVParamGetString(m_iObjId, m_szName, "Target", "[Me]");
	std::vector<int> objs = GetObjects(pszTarget, m_iObjId, pMsg);

	const char* pszProp = NVParamGetString(m_iObjId, m_szName, "Prop", NULL);

	bool readd = false;
	if (NVParamGetInt(m_iObjId, m_szName, "ReAdd", 0) == 1)
	{
		readd = true;
	}

	if (pszProp != NULL)
	{
		for ( std::vector<int>::iterator it = objs.begin(); it != objs.end(); ++it)
		{
			pPropSrv->Remove(*it, pszProp);
			if (readd)
			{
				pPropSrv->Add(*it, pszProp);
			}
		}
	}

	return 0;
}

MSGHANDLER cScr_NVRemovePropertyTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Do nothing.
	return 0;
}


// End of File
