#if _DARKGAME != 3
#error Citadel scripts are incompatible with Thief.
#else

#include <cmath>

#include "lg/interface.h"
#include "lg/scrservices.h"
#include "lg/scrmanagers.h"
#include "lg/scrmsgs.h"
#include "lg/objects.h"
#include "lg/propdefs.h"
#include "ScriptLib.h"
#include "NVScriptLib.h"
#include "Citadel.h"

#include "lg/iids.h"

#if (_DARKGAME == 3)
	#define CDLINK	"SwitchLink"
#else // Okay, this is pointless since this is a _DARKGAME == 3 only module.
	#define CDLINK	"ControlDevice"
#endif

#define BOOSTERSPEED     25.00f
#define MAXBOOSTERSPEED  30.00f


/***********************
 * CitadelSecurityReporter
 */

// Note: this scrpit is used for CitadelPlayer to inherit from, but it can be used on its own to have security % messages without the whole CitadelPlayer script.
MSGHANDLER cScr_CitadelSecurityReporter::OnSecurityUpdate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	int iTotal = 0;
	int iHideMessages = static_cast<int>(pMsg->data);
	if (pQSrv->Exists("SecurityLevel"))
	{
		iCurrent = pQSrv->Get("SecurityLevel"); // Get QVar value
	}
	if (pQSrv->Exists("TotalSecurityLevel"))
	{
		iTotal = pQSrv->Get("TotalSecurityLevel"); // Get QVar value
	}
	float fSecurityPercent = 0.00f;
	
	if ( iCurrent && iTotal ) // We don't want any Divide By Zero errors...
	{
		 fSecurityPercent = 100.00f * (static_cast<float>(iCurrent) / static_cast<float>(iTotal));
		if ( iHideMessages != 1 )
		{
//			DisplayPrintf("DEBUG: Security level is now %i of %i", iCurrent, iTotal);
			DisplayPrintf("Level security now %i%%", static_cast<int>(fSecurityPercent));
		}
	} else {
		if ( iHideMessages != 1 )
		{
			DisplayPrintf("Level security now 0%%");
		}
	}

	if ( ParamGetInt(m_iObjId, "CitadelPlayerModulesAsSecurityPercent", 0) )
	{
		// This section would set the Cyber Module Stack Count to the Security %. It is currently disabled.
		SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
		object oSecurityCounter;
		pShockGameSrv->Equipped(oSecurityCounter, 11);
		
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		pPropSrv->Set(oSecurityCounter, "StackCount", NULL, static_cast<int>(fSecurityPercent));
	}
	
	return 0;
}


/***********************
 * CitadelPlayer
 */

MSGHANDLER cScr_CitadelPlayer::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "DrawEnergy", 500, kSTM_Periodic, g_mpUndef);
	}
	
	// Check if the 'Booster' timer should be activated.
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 3");
	if ( static_cast<float>(mpReturn) )
	{
		if (m_hBoosterTimerHandle == NULL)
		{ // Start Booster Timer.
			m_hBoosterTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "Booster", 100, kSTM_Periodic, g_mpUndef);
		}
	}
	// Set a one-shot 500ms "SecurityUpdate" timer.
	g_pScriptManager->SetTimedMessage2(m_iObjId, "SecurityUpdate", 100, kSTM_OneShot, g_mpUndef); 
	return 0;
}

MSGHANDLER cScr_CitadelPlayer::OnCreateObjs(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// CreateObjs is sent only once, when the player object is first created.
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	
	// Set PSI stat, base on difficulty:
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("Difficulty"))
	{
		iCurrent = pQSrv->Get("Difficulty"); // Get QVar value
	}
	
	switch ( iCurrent )
	{
		case 1: 
			pPropSrv->Set(m_iObjId, "BaseStatsDesc", "PSI", 16);	
			break;
		case 3:
			pPropSrv->Set(m_iObjId, "BaseStatsDesc", "PSI", 32);
			break;
		case 4:
			pPropSrv->Set(m_iObjId, "BaseStatsDesc", "PSI", 51);
			break;
		default:
			pPropSrv->Set(m_iObjId, "BaseStatsDesc", "PSI", 25);		
	}
	
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	pPropSrv->Set(m_iObjId, "PsiState", "Max Points", 255);
	pShockGameSrv->RecalcStats(m_iObjId);
	float fEnergy = 255.00f;
	
	pPropSrv->SetSimple(m_iObjId, "Energy", fEnergy);
	
	pShockGameSrv->SetPlayerPsiPoints(static_cast<int>(fEnergy));
	
	DisplayPrintf("DEBUG: Initialising CitadelPlayer script...");
	return 0;
}

MSGHANDLER cScr_CitadelPlayer::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "DrawEnergy"))
	{
		
		
		float fEnergyUsage = 0; // Energy usage JPM. Calculated from the energy usage of all active hardware.
		fEnergyUsage = GetEnergyUsage(); // A hack.
		
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		cMultiParm mpReturn;
		pPropSrv->Get(mpReturn, m_iObjId, "Energy", NULL);
		float fEnergy = static_cast<float>(mpReturn);
		if (fEnergy > 0 )
		{
			fEnergy -= (fEnergyUsage / 120 );
			
			if ( fEnergy <= 0 )
			{
				DisplayPrintf("Power supply exhausted.");
				// Turn off all hardware.
				// Strangely enough, this includes Skate in SS1; Skate cannot be activated with no power, but uses no power.
				// The -1 tells the HardwareOff handler not to display the updated energy usage.
				HardwareOff(1);
				HardwareOff(2);
				HardwareOff(3);
				HardwareOff(4);
				HardwareOff(5);
				HardwareOff(6);
				DisplayPrintf("Energy usage is now %.f JPM", GetEnergyUsage());
				pPropSrv->SetSimple(m_iObjId, "Energy", 0);
			} else {
				pPropSrv->SetSimple(m_iObjId, "Energy", fEnergy);
			}
		}
		RecalcEnergy();
		return 0;
	}
	
	if (!stricmp(pszMsgName, "Booster"))
	{
		// Place Skate hack here, if necessary.

		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		SService<IPhysSrv> pPhysSrv(g_pScriptManager);
		cMultiParm mpReturn;
		pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 3");
		
		cScrVec vVelocity;
		pPhysSrv->GetVelocity(m_iObjId, vVelocity);
		
		if ( static_cast<float>(mpReturn) == 40.00f )
		{
			// Luckily, I wrote this formula some time ago for my Sunlight Converter. ;)
			// Still had to be converted JavaScript -> C++ and tweaked a little...
			SService<IObjectSrv> pOS(g_pScriptManager);
			
			cScrVec vRotation;
			// pPropSrv->Get(mpReturn, m_iObjId, "PhysControl", "Velocity");
			// vVelocity.x = static_cast<const mxs_vector*>(mpReturn)->x;
			// vVelocity.y = static_cast<const mxs_vector*>(mpReturn)->y;
			// vVelocity.z = static_cast<const mxs_vector*>(mpReturn)->z;
			pOS->Facing(vRotation, m_iObjId);
			float fLong = static_cast<float>((static_cast<int>(vRotation.z) % 90));
			int iRot = static_cast<int>(vRotation.z / 90);
			float fVal[2];
			float fX, fY;
			
			
			fVal[iRot % 2] = (1 - (fLong / 90.00f));
			fVal[(iRot + 1) % 2] = (fLong / 90.00f);
			
			if ((iRot % 3) == 0) {
				fX = fVal[0];
			} else {
				fX = -fVal[0];
			}
			
			if (iRot >= 2) {
				fY = -fVal[1];
			} else {
				fY = fVal[1];
			}
	
			float fHighestVal = ( fabs(fX) > fabs(fY) ) ? fabs(fX) : fabs(fY);
			
			float fMultiple = BOOSTERSPEED / fHighestVal;
			fX *= fMultiple;
			fY *= fMultiple;
		
			fX += vVelocity.x;
			fY += vVelocity.y;
			
			if ( fX > MAXBOOSTERSPEED )
				fX = MAXBOOSTERSPEED;
			else if ( fX < -MAXBOOSTERSPEED )
				fX = -MAXBOOSTERSPEED;
			if ( fY > MAXBOOSTERSPEED )
				fY = MAXBOOSTERSPEED;
			else if ( fY < -MAXBOOSTERSPEED )
				fY = -MAXBOOSTERSPEED;

			vVelocity.x = fX;
			vVelocity.y = fY;
//			DisplayPrintf("%f, %f", fX, fY);
//			pPropSrv->Set(m_iObjId, "PhysControl", "Controls Active", 2);
//			pPropSrv->Set(m_iObjId, "PhysControl", "Velocity", vVelocity);
			pPhysSrv->SetVelocity(m_iObjId, vVelocity);
		}
		pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 5");
		if ( static_cast<float>(mpReturn) > 0.00f )
		{
			vVelocity.z = 30.00f;
			pPhysSrv->SetVelocity(m_iObjId, vVelocity);
		}
		return 0;
	}
	if (!stricmp(pszMsgName, "SecurityUpdate"))
	{
		SimplePost(m_iObjId, m_iObjId, "SecurityUpdate", 1);
		return 0;
	}
	
	return 0;
}

MSGHANDLER cScr_CitadelPlayer::OnRecalcEnergy(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	RecalcEnergy();
	return 0;
}

void cScr_CitadelPlayer::RecalcEnergy()
{
//	DisplayPrintf("DEBUG: Recalculating energy / PSI points...");
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	
	cMultiParm mpReturn;
	pPropSrv->Get(mpReturn, m_iObjId, "Energy", NULL);
	float fEnergy = static_cast<float>(mpReturn);
	pShockGameSrv->SetPlayerPsiPoints(static_cast<int>(fEnergy));
	
	// Send 'EnergyUpdate' messages to all inventory items (to tell Energy weapons to update their ammo count)
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->BroadcastOnAllLinks(m_iObjId, "EnergyUpdate", pLTS->LinkKindNamed("Contains"));
	
	return;
}

#pragma pack(push,1)

// NV: This is for convenience.
struct sEnergyUsage
{
	float fLantern,
	fShield,
	fBooster,
	fNightSight,
	fJumpJets,
	fEnviroSuit;
//	fSensaround; // Too many: this doesn't fit in the property. And won't work anyway!
};
#pragma pack(pop)

float cScr_CitadelPlayer::GetEnergyUsage() {
	float fEnergyUsage = 0.00f;
	
	SInterface<IPropertyManager> pPropMan(g_pScriptManager);
	SInterface<IProperty> pProp;
	pProp = pPropMan->GetPropertyNamed("JointPos");
	if (pProp)
	{
		IGenericProperty* pEnergyUsageProp;
		sEnergyUsage* pEnergyUsage;
		if (E_NOINTERFACE != pProp->QueryInterface(IID_IJointPosProperty, reinterpret_cast<void**>(&pEnergyUsageProp)))
		{
			if (pEnergyUsageProp->Get(m_iObjId, reinterpret_cast<void**>(&pEnergyUsage)))
			{
				fEnergyUsage += pEnergyUsage->fLantern;
				fEnergyUsage += pEnergyUsage->fShield;
				fEnergyUsage += pEnergyUsage->fBooster;
				fEnergyUsage += pEnergyUsage->fNightSight;
				fEnergyUsage += pEnergyUsage->fJumpJets;
				fEnergyUsage += pEnergyUsage->fEnviroSuit;
			}
			pEnergyUsageProp->Release();
		}
	}
	
/*
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 1");
	fEnergyUsage += static_cast<float>(mpReturn); // Lantern
	pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 2");
	fEnergyUsage += static_cast<float>(mpReturn); // Shield
	pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 3");
	fEnergyUsage += static_cast<float>(mpReturn); // Booster
	pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 4");
	fEnergyUsage += static_cast<float>(mpReturn); // NightSight
	pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 5");
	fEnergyUsage += static_cast<float>(mpReturn); // JumpJets
	pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 6");
	fEnergyUsage += static_cast<float>(mpReturn); // EnviroSuit
//	pPropSrv->Get(mpReturn, m_iObjId, "AI_AlSnMul", "Alert 1: Range mult");
//	fEnergyUsage += static_cast<float>(mpReturn); // Sensaround
*/	
	return fEnergyUsage;
}

MSGHANDLER cScr_CitadelPlayer::OnHardwareOn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	
	int iHardware = static_cast<int>(pMsg->data);
	int iSetting = static_cast<int>(pMsg->data2);
	
	DisplayPrintf("Hardware %i, Setting %i", iHardware, iSetting);
	
	char szPropName[8];
	sprintf(szPropName, "Joint %i", iHardware);
	
	switch (iHardware)
	{
	case 1: // Lantern ON
		switch (iSetting)
		{
		case 1: 
			pPropSrv->SetSimple(m_iObjId, "SelfLit", 100);
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 10);
			break;
		case 2:
			pPropSrv->SetSimple(m_iObjId, "SelfLit", 175);
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 25);
			break;
		case 3:
			pPropSrv->SetSimple(m_iObjId, "SelfLit", 250);
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 30);
		}
		break;
	case 2: // Shield ON
		switch (iSetting)
		{
		case 1: 
			AddSingleMetaProperty("M-Shield1", m_iObjId);
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 24);
			break;
		case 2:
			AddSingleMetaProperty("M-Shield2", m_iObjId);
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 60);
			break;
		case 3:
			AddSingleMetaProperty("M-Shield3", m_iObjId);
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 105);
			break;
		case 4:
			AddSingleMetaProperty("M-Shield4", m_iObjId);
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 30);
		}
		break;
	case 3: // Booster ON
		if ( iSetting )
		{
			// Set energy usage
			pPropSrv->Set(m_iObjId, "JointPos", szPropName, 40);
			// Change Mass, to reduce the chance of the player turning into a red smear on the walls...
			pPropSrv->Set(m_iObjId, "PhysAttr", "Mass", 80.00f);
			if (m_hBoosterTimerHandle == NULL)
			{ // Start Booster Timer.
				m_hBoosterTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "Booster", 100, kSTM_Periodic, g_mpUndef);
			}
			// Booster!
		} else {
			// Skate!
		}
		break;
	case 4: // NightSight ON
		pPropSrv->Set(m_iObjId, "JointPos", szPropName, 50);
		NightSightOn();
		// Night Vision!
		break;
	case 5: // JumpJets ON
		// JumpJets: how are these going to work?
		DisplayPrintf("DEBUG: JumpJets activated. (Unfinished)");
		// Badly-working hack. Note the extremely low power usage.
		pPropSrv->Set(m_iObjId, "JointPos", szPropName, 1500.00f);
		pPropSrv->Set(m_iObjId, "PhysAttr", "Gravity %", 0.00f);
		pPropSrv->Set(m_iObjId, "PhysAttr", "Base Friction", 1.00f);
		if (m_hBoosterTimerHandle == NULL)
			{ // Start Booster Timer.
				m_hBoosterTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "Booster", 100, kSTM_Periodic, g_mpUndef);
			}
		break;
	case 6: //  EnviroSuit ON
		// Unsure...
		break;
	default:
		DisplayPrintf("ERROR: Attempting to activate unknown piece of hardware.");
	}

	DisplayPrintf("DEBUG: Hardware specified was %i setting %i", iHardware, iSetting);
	DisplayPrintf("Energy usage is now %.f JPM", GetEnergyUsage());
	return 0;
}

MSGHANDLER cScr_CitadelPlayer::OnHardwareOff(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	HardwareOff(static_cast<int>(pMsg->data));
	if ( static_cast<int>(pMsg->data2) != 1 )
	{
		DisplayPrintf("DEBUG: Hardware specified was %i", static_cast<int>(pMsg->data));
		DisplayPrintf("Energy usage is now %.f JPM", GetEnergyUsage());
	}
	return 0;
}

void cScr_CitadelPlayer::HardwareOff(int iHardware)
{
//	int iHardware = static_cast<int>(pMsg->data);
//	int iLevel = static_cast<int>(pMsg->data2);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	SService<IObjectSrv> pOS(g_pScriptManager);
	cMultiParm mpReturn;
	object oMeta;
	
	char szPropName[8];
	sprintf(szPropName, "Joint %i", iHardware);
	pPropSrv->Set(m_iObjId, "JointPos", szPropName, 0);
		
	switch (iHardware)
	{
	case 1: // Lantern OFF
		pPropSrv->Remove(m_iObjId, "SelfLit");
		break;
	case 2: // Shield OFF
		pOS->Named(oMeta, "M-Shield1");
		RemoveSingleMetaProperty(oMeta, m_iObjId);
		pOS->Named(oMeta, "M-Shield2");
		RemoveSingleMetaProperty(oMeta, m_iObjId);
		pOS->Named(oMeta, "M-Shield3");
		RemoveSingleMetaProperty(oMeta, m_iObjId);
		pOS->Named(oMeta, "M-Shield4");
		RemoveSingleMetaProperty(oMeta, m_iObjId);
		break;
	case 3: // Booster OFF
		pPropSrv->Set(m_iObjId, "PhysAttr", "Mass", 180.00f);
		
		pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 5");
		if (static_cast<float>(mpReturn) == 0.00f )
		{ // Only stop the timer if JumpJets are also inactive.
			if (m_hBoosterTimerHandle != NULL)
			{	// Stop timer:
				g_pScriptManager->KillTimedMessage(m_hBoosterTimerHandle);
				m_hBoosterTimerHandle = NULL;
			}
		}
		break;
	case 4: // NightSight OFF
		NightSightOff();
		break;
	case 5: // JumpJets OFF
		DisplayPrintf("DEBUG: JumpJets deactivated.)");
		pPropSrv->Set(m_iObjId, "PhysAttr", "Gravity %", 100.00f);
		pPropSrv->Set(m_iObjId, "PhysAttr", "Base Friction", 0.00f);
		
		pPropSrv->Get(mpReturn, m_iObjId, "JointPos", "Joint 3");
		if (static_cast<float>(mpReturn) != 40.00f )
		{ // Only stop the timer if the Turbo Motion Booster is also inactive.
			if (m_hBoosterTimerHandle != NULL)
			{	// Stop timer:
				g_pScriptManager->KillTimedMessage(m_hBoosterTimerHandle);
				m_hBoosterTimerHandle = NULL;
			}
		}
		break;
	case 6: // EnviroSuit OFF
		break;
	default:
		DisplayPrintf("ERROR: Attempting to deactivate unknown piece of hardware.");
	}

	return;
}

void cScr_CitadelPlayer::NightSightOn(void)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	object oArc, oObj;
	pOS->Named(oArc, "Marker");
	pOS->Create(oObj, oArc);
	if ( !oObj)
		return;
	
	pOS->SetName(oObj, "Citadel-NightSight--1"); // Set the name
	pPropSrv->Set(oObj, "ModelName", NULL, "NVNite");
	pPropSrv->Set(oObj, "CameraObj", "Draw?", true);
	mxs_vector vOffset = { 1.00f, 0.00f, 0.00f };
	pPropSrv->Set(oObj, "CameraObj", "Offset", vOffset);
	pPropSrv->Set(oObj, "RenderAlpha", NULL, 0.25f);
	
	char szName[22];
	for ( int i = 0; i < 17; i++)
	{
		pOS->Create(oObj, oArc);
		if ( !oObj)
			return;
		
		sprintf(szName, "Citadel-NightSight-%i", i);
		pOS->SetName(oObj, szName); // Set the name
		pPropSrv->Set(oObj, "CameraObj", "Draw?", false);
		vOffset.x = (4 * i);
		pPropSrv->Set(oObj, "CameraObj", "Offset", vOffset);
		pPropSrv->Set(oObj, "SelfLit", NULL, 200);
		pPropSrv->Set(oObj, "LightColor", "hue", 0.33f);
		pPropSrv->Set(oObj, "LightColor", "saturation", 1.00f);
	}
	return;
}

void cScr_CitadelPlayer::NightSightOff(void)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oObj;
	char szName[22];
	for ( int i = -1; i < 17; i++ )
	{
		sprintf(szName, "Citadel-NightSight-%i", i);
		pOS->Named(oObj, szName);
		if ( oObj)
			pOS->Destroy(oObj);
	}
	return;
}

/***********************
 * CitadelHardware
 */

int cScr_CitadelHardware::ToggleHardware(int iFrobber, int iHardware, int iSetting)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	
	// Get the player's power level
	pPropSrv->Get(mpReturn, iFrobber, "Energy", NULL);
	float fEnergy = static_cast<float>(mpReturn);
	
	if ( fEnergy > 0.00f)
	{
		char szPropName[8];
		sprintf(szPropName, "Joint %i", iHardware);
		
		pPropSrv->Get(mpReturn, iFrobber, "JointPos", szPropName);
		if (static_cast<float>(mpReturn) > 0 )
		{
			SimplePost(m_iObjId, iFrobber, "HardwareOff", iHardware, 0);
			pPropSrv->Set(m_iObjId, "SHead1", NULL, "CitadelHWInactive");
			pPropSrv->Set(m_iObjId, "Sett1", NULL, "CitadelHWInactive");
			return 0; // Hardware was turned OFF
		} else {
			SimplePost(m_iObjId, iFrobber, "HardwareOn", iHardware, iSetting);
			pPropSrv->Set(m_iObjId, "SHead1", NULL, "CitadelHWActive");
			pPropSrv->Set(m_iObjId, "Sett1", NULL, "CitadelHWActive");
			return 1; // Hardware was turned ON.
		}
	} else {
		DisplayPrintf("Insufficient power to operate.");
	}
	return -1;
}

MSGHANDLER cScr_CitadelHardware::OnStateChange(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iEvent = static_cast<int>(pMsg->data);
	int iSetting = static_cast<int>(pMsg->data2);
	int iVersion = static_cast<int>(pMsg->data3);
	if (!iSetting)
		iSetting = 1;
	if (!iVersion)
		iVersion = 1;
	
	int iHardware = ParamGetInt(m_iObjId, "CitadelHardware", 1);
	
	int iFrobber = StrToObject("Player"); // This should probably be changed for multiplayer.
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	if ( iEvent == 0 )
	{
		if ( ToggleHardware(iFrobber, iHardware, iSetting) )
		{ // Hardware was turned ON
			pPropSrv->Set(m_iObjId, "SHead1", NULL, "CitadelHWActive"); // "ACTIVE" / "INACTIVE"
			pPropSrv->Set(m_iObjId, "Sett1", NULL, "CitadelHWActive"); // "Click to..."
		} else { // Hardware was turned OFF
			pPropSrv->Set(m_iObjId, "SHead1", NULL, "CitadelHWInactive"); // "ACTIVE" / "INACTIVE"
			pPropSrv->Set(m_iObjId, "Sett1", NULL, "CitadelHWInactive"); // "Click to..."
		}
		pPropSrv->Set(m_iObjId, "GunState", "Setting", -1);
	} else if ( iEvent == 1 )
	{
		pPropSrv->Set(m_iObjId, "GunState", "Setting", -1);
		if ( iVersion < 2 )
		{
			iSetting = 0;
			pPropSrv->Set(m_iObjId, "GunState", "Modification", 1);
		} else {
			if ( iVersion == 4 )
			{
				iSetting = 4;
				pPropSrv->Set(m_iObjId, "GunState", "Modification", 4);
			} else {
				iSetting++;
				if (iSetting > iVersion)
				{
					iSetting = 1;
				}
				pPropSrv->Set(m_iObjId, "GunState", "Modification", iSetting);
			}
		}
		
		DisplayPrintf("Setting changed to %i.", iSetting);
		char szSettingText[13];
		sprintf(szSettingText, "CitadelHW%i_%i", iHardware, iSetting);
		pPropSrv->Set(m_iObjId, "Sett2", NULL, szSettingText); // Description of setting.
		
		if ( iSetting )
		{
			char szPropName[8];
			sprintf(szPropName, "Joint %i", iHardware);
			
			cMultiParm mpReturn;
			pPropSrv->Get(mpReturn, iFrobber, "JointPos", szPropName);
			if (static_cast<float>(mpReturn) > 0 )
			{
				SimpleSend(m_iObjId, iFrobber, "HardwareOff", iHardware, 1);
				SimpleSend(m_iObjId, iFrobber, "HardwareOn", iHardware, iSetting);
			}
		}
	}
	
	return 0;
}

MSGHANDLER cScr_CitadelHardware::OnFrobInvEnd(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Modification");
	int iSetting = static_cast<int>(mpReturn);
	if (!iSetting)
		iSetting = 1;
	int iHardware = ParamGetInt(m_iObjId, "CitadelHardware", 1);
	ToggleHardware(pFrobMsg->Frobber, iHardware, iSetting);
	return 0;
}


MSGHANDLER cScr_CitadelHardware::OnContained(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( !m_bSim )
		return 0;
		
	sContainedScrMsg* pConMsg = static_cast<sContainedScrMsg*>(pMsg);
//	DisplayPrintf("Event is %i, container is %i", pConMsg->event, pConMsg->container);
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
//	return 0;
	if ( pConMsg->event == 3 )
	{
		SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
		pShockGameSrv->LoadCursor(0);
//		pShockGameSrv->Equip(-1000, m_iObjId);
		pShockGameSrv->AddInvObj(m_iObjId);
		pShockGameSrv->OverlayChangeObj(21, 1, m_iObjId);
		pShockGameSrv->OverlayChange(44, 0);
		
		StartListener();
		if (m_hTimerHandle == NULL)
		{ // Start a timer.
			m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "CheckOverlays", 200, kSTM_Periodic, g_mpUndef);
		}
		
	}
	return 0;
}

MSGHANDLER cScr_CitadelHardware::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "CheckOverlays"))
		return 0; // Abort if it's not the right timer.
	
	// Do something!
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	if ( !pShockGameSrv->OverlayOn(21) )
	{
	
		if ( m_hListenHandle )
		{
			DisplayPrintf("DEBUG: Settings panel closed; no longer listening for settings changes.");
			m_pProp->Unlisten(m_hListenHandle);
			m_hListenHandle = NULL;
		}
		if (m_hTimerHandle != NULL)
		{	// Stop timer:
			g_pScriptManager->KillTimedMessage(m_hTimerHandle);
			m_hTimerHandle = NULL;
		}
	}
	return 0;
}

MSGHANDLER cScr_CitadelHardware::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Init the hardware
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	
	// Check if the hardware has been initted already
	if (pPropSrv->Possessed(m_iObjId, "Initted"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "Initted", NULL);
		if ( static_cast<bool>(mpReturn) )
		{
			DisplayPrintf("DEBUG: Hardware already initialised.");
			return 0; // Already initialised!
		}
	}
	
	// Make sure the GunState property is set
	pPropSrv->SetSimple(m_iObjId, "Initted", true);
	if (!pPropSrv->Possessed(m_iObjId, "GunState"))
	{
		pPropSrv->Set(m_iObjId, "GunState", "Ammo", ParamGetInt(m_iObjId, "CitadelHardwareVersion", 1));
	}
	
	// Get the hardware version (stored in the Ammo field of the GunState property
	pPropSrv->Get(mpReturn, m_iObjId, "GunState", "Ammo");
	int iVersion = static_cast<int>(mpReturn);
	if ( iVersion < 1 )
	{ // Set the version to 1 if no version is specified
		iVersion = ParamGetInt(m_iObjId, "CitadelHardwareVersion", 1);
		pPropSrv->Set(m_iObjId, "GunState", "Ammo", iVersion);	
	}
	// Set the current setting (stored in the 'Modification' field) to the highest availiable
	pPropSrv->Set(m_iObjId, "GunState", "Modification", iVersion);
	// Set the Setting to -1 (this is used to hijack the gun settings interface)
	pPropSrv->Set(m_iObjId, "GunState", "Setting", -1);
	
	char szSettingText[13];
	sprintf(szSettingText, "CitadelHW%i_%i", ParamGetInt(m_iObjId, "CitadelHardware", 1), (iVersion == 1) ? 0: iVersion);
	pPropSrv->Set(m_iObjId, "Sett2", NULL, szSettingText); // Description of setting.
	
	pPropSrv->Set(m_iObjId, "SHead1", NULL, "CitadelHWInactive");
	pPropSrv->Set(m_iObjId, "Sett1", NULL, "CitadelHWInactive");
	pPropSrv->Set(m_iObjId, "SHead2", NULL, "CitadelCurrSett");

	return 0;
}

MSGHANDLER cScr_CitadelHardware::OnEndScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( m_hListenHandle )
	{
		DisplayPrintf("DEBUG: Script exited. Shutting down listener.");
		m_pProp->Unlisten(m_hListenHandle);
		m_hListenHandle = NULL;
	}
	return 0;
}

cScr_CitadelHardware::~cScr_CitadelHardware()
{
	if ( m_hListenHandle )
	{
		DisplayPrintf("DEBUG: Script exited. Shutting down listener.");
		m_pProp->Unlisten(m_hListenHandle);
		m_hListenHandle = NULL;
	}
	if ( m_pProp )
	{
		m_pProp->Release();
	}
}

#pragma pack(push,1)
struct sGunState
{
	int iAmmo;
	float iCondition;
	int iSetting,
	iModification;
};
#pragma pack(pop)

void __stdcall cScr_CitadelHardware::ListenFunc(sPropertyListenMsg* sMsg, PropListenerData pData)
{
	int iObjId = reinterpret_cast<int>(pData);
	if (sMsg->iObjId == iObjId)
	{ // Do nothing UNLESS this is the object with the script...
		sGunState* pGunState;
		pGunState = reinterpret_cast<sGunState*>(sMsg->pData);
		// Post 'SettingChange' message, with the current Setting as data.
		SimplePost(iObjId, iObjId, "StateChange", pGunState->iSetting, pGunState->iModification, pGunState->iAmmo);
	}
}

void cScr_CitadelHardware::StartListener(void)
{
	if ( !m_pProp )
	{
		SInterface<IPropertyManager> pPM(g_pScriptManager);
		m_pProp = static_cast<IGenericProperty*>(pPM->GetPropertyNamed("GunState"));
	}
	if ( !m_hListenHandle )
	{
		DisplayPrintf("DEBUG: Settings panel opened; listening for settings changes.");

		m_hListenHandle = m_pProp->Listen(kPropertyChange, ListenFunc, reinterpret_cast<PropListenerData__*>(m_iObjId));
	} 
	return;
}

/***********************
 * CitadelRecharge
 */
int cScr_CitadelRecharge::Recharge(int iFrobber)
{
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	
	// Get the player's power level
	pPropSrv->Get(mpReturn, iFrobber, "Energy", NULL);
	float fEnergy = static_cast<float>(mpReturn);
	
	if ( fEnergy < 249.00f )
	{ // If the player's battery is not full
		// Increase energy by Battery Size (assume 255 if none is specified)
		fEnergy += ParamGetFloat(m_iObjId, "CitadelBatterySize", 255.00f);
		if ( fEnergy > 255.00f )
		{ // Don't allow energy levels to get higher than 255
			fEnergy = 255.00f;
		}
		pPropSrv->SetSimple(iFrobber, "Energy", fEnergy);
		SimpleSend(m_iObjId, iFrobber, "RecalcEnergy"); // Recalculate energy meter, weapon ammo.
			
		// Play sound
		SService<ISoundScrSrv> pSoundSrv(g_pScriptManager);
		true_bool bReturn;
		pSoundSrv->PlayEnvSchema(bReturn, m_iObjId, "Event Activate", m_iObjId, 0, kEnvSoundAmbient, kSoundNetwork0);
		return 1;
	} else {
		DisplayPrintf("Power levels already at maximum.");
		return 0;
	}
	return 0;
}

/***********************
 * CitadelBattery
 */

MSGHANDLER cScr_CitadelBattery::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobber = pFrobMsg->Frobber;
	
	if ( Recharge(iFrobber) )
	{
		// Reduce Stack Count by 1
		SInterface<IContainSys> pContainSys(g_pScriptManager);
		pContainSys->StackAdd(m_iObjId, -1, 1);
	}
	return 0;
}

/***********************
 * CitadelChargingStation
 */

MSGHANDLER cScr_CitadelChargingStation::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sFrobMsg* pFrobMsg = static_cast<sFrobMsg*>(pMsg);
	int iFrobber = pFrobMsg->Frobber;
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	
	if (pPropSrv->Possessed(m_iObjId, "Locked"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "Locked", NULL);
		if ( static_cast<bool>(mpReturn) )
		{
			DisplayPrintf("Power Station still charging.");
			return 1;
		}
	}
	
	if ( Recharge(iFrobber) )
	{
		DisplayPrintf("Energy drawn from Power Station.");
		int iInterval = ParamGetInt(m_iObjId, "CitadelChargingStationInterval", 0);
		if ( iInterval )
		{
			pPropSrv->Set(m_iObjId, "Locked", NULL, true);
			g_pScriptManager->SetTimedMessage2(m_iObjId, "ChargingStation", iInterval, kSTM_OneShot, g_mpUndef); 
		}
	}
	
	return 0;
}

MSGHANDLER cScr_CitadelChargingStation::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (!stricmp(pszMsgName, "ChargingStation"))
	{
		SService<IPropertySrv> pPropSrv(g_pScriptManager);
		pPropSrv->Set(m_iObjId, "Locked", NULL, false);
	}
	
	return 0;
}

/***********************
 * CitadelEnergyWeapon
 */

MSGHANDLER cScr_CitadelEnergyWeapon::OnEnergyUpdate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	// Get player's power levels
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	int iFrom = static_cast<int>(pMsg->from);
	pPropSrv->Get(mpReturn, iFrom, "Energy", NULL);
	float fEnergy = static_cast<float>(mpReturn);
	
	// Set energy weapon's ammo to match power level
	pPropSrv->Set(m_iObjId, "GunState", "Ammo", static_cast<int>(fEnergy));
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	pShockGameSrv->RefreshInv();
	return 0;
}

/***********************
 * CitadelEnergyProjectile
 */

MSGHANDLER cScr_CitadelEnergyProjectile::OnCreate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	object oGun;
	object oPlayer = StrToObject("Player");
	pShockGameSrv->PlayerGun(oGun);
	if ( !oGun )
		return 1;
	
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	pPropSrv->Get(mpReturn, StrToObject("Player"), "Energy", NULL);
	float fEnergy = static_cast<float>(mpReturn);
	pPropSrv->Get(mpReturn, oGun, "GunState", "Setting");
	char* szAmmoUsage = new char[22];
	snprintf(szAmmoUsage, 22, "Setting %i: Ammo Usage", static_cast<int>(mpReturn));
	pPropSrv->Get(mpReturn, oGun, "BaseGunDesc", szAmmoUsage);
	fEnergy -= static_cast<int>(mpReturn);
	pPropSrv->SetSimple(oPlayer, "Energy", fEnergy);
	delete[] szAmmoUsage;

	g_pScriptManager->SetTimedMessage2(m_iObjId, "RecalcEnergy", 100, kSTM_OneShot, g_mpUndef); 
	return 0;
}

MSGHANDLER cScr_CitadelEnergyProjectile::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{ // Yes, this doesn't care what timer triggers it.
	object oPlayer = StrToObject("Player");
	SimpleSend(m_iObjId, oPlayer, "RecalcEnergy");
	return 0;
}

MSGHANDLER cScr_CitadelEnergyProjectile::OnDestroy(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	object oPlayer = StrToObject("Player");
	SimpleSend(m_iObjId, oPlayer, "RecalcEnergy");
	return 0;
}

/***********************
 * CitadelSecurity
 */

MSGHANDLER cScr_CitadelSecurity::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
	cMultiParm mpReturn;
	if (pPropSrv->Possessed(m_iObjId, "Initted"))
	{
		pPropSrv->Get(mpReturn, m_iObjId, "Initted", NULL);
		if ( static_cast<bool>(mpReturn) )
		{
//			DisplayPrintf("DEBUG: Security device already initialised.");
			return 0; // Already initialised!
		}
	}
	pPropSrv->SetSimple(m_iObjId, "Initted", true);
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	int iTotalSecurityLevel = 0;
	if (pQSrv->Exists("SecurityLevel"))
	{
		iCurrent = pQSrv->Get("SecurityLevel"); // Get QVar value
	}
	if (pQSrv->Exists("TotalSecurityLevel"))
	{
		iTotalSecurityLevel = pQSrv->Get("TotalSecurityLevel"); // Get QVar value
	}
	iTotalSecurityLevel += ParamGetInt(m_iObjId, "CitadelSecurityWorth", 1); // Increase security value
	iCurrent += ParamGetInt(m_iObjId, "CitadelSecurityWorth", 1); // Increase security value
	
//	DisplayPrintf("DEBUG: Security level is now %i", iCurrent);
	pQSrv->Set("TotalSecurityLevel", iTotalSecurityLevel, 0);
	pQSrv->Set("SecurityLevel", iCurrent, 0);
	
	SimplePost(m_iObjId, StrToObject("Player"), "SecurityUpdate", 1); // Change this for multiplayer!
	return 0;
}

MSGHANDLER cScr_CitadelSecurity::OnSlay(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	int iTotalSecurityLevel = 0;
	if (pQSrv->Exists("SecurityLevel"))
	{
		iCurrent = pQSrv->Get("SecurityLevel"); // Get QVar value
	}
	if (pQSrv->Exists("TotalSecurityLevel"))
	{
		iTotalSecurityLevel = pQSrv->Get("TotalSecurityLevel"); // Get QVar value
	}
	iCurrent -= ParamGetInt(m_iObjId, "CitadelSecurityWorth", 1); // Decrease security value
	pQSrv->Set("SecurityLevel", iCurrent, 0);
	SimpleSend(m_iObjId, StrToObject("Player"), "SecurityUpdate", 0); // Change this for multiplayer!
	
	return 0;
}


/***********************
 * NVOverlayTest
 */

MSGHANDLER cScr_NVOverlayTest::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iOverlay = static_cast<int>(m_iOverlay);
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	if ( iOverlay != 0 )
	{
		pShockGameSrv->OverlayChange(iOverlay-1, 0);
	}
	
	pShockGameSrv->OverlayChange(iOverlay, 1);
	DisplayPrintf("Activating overlay %i", iOverlay);
	m_iOverlay++;
	
	return 1;
}

MSGHANDLER cScr_NVOverlayTest::OnTurnOn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("NVOverlayTest"))
	{
		iCurrent = pQSrv->Get("NVOverlayTest"); // Get QVar value
	}
	//pShockGameSrv->OverlayChange(iCurrent, 1);
	pShockGameSrv->OverlayChangeObj(iCurrent, 2, m_iObjId);
	DisplayPrintf("Activating overlay %i", iCurrent);
	
	return 1;
}

MSGHANDLER cScr_NVOverlayTest::OnTurnOff(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("NVOverlayTest"))
	{
		iCurrent = pQSrv->Get("NVOverlayTest"); // Get QVar value
	}
	pShockGameSrv->OverlayChange(iCurrent, 0);
	DisplayPrintf("Deactivating overlay %i", iCurrent);
	
	return 1;
}

/***********************
 * NVOverlayIncrease
 */

MSGHANDLER cScr_NVOverlayIncrease::OnTurnOn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IShockGameSrv> pShockGameSrv(g_pScriptManager);
	
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("NVOverlayTest"))
	{
		iCurrent = pQSrv->Get("NVOverlayTest"); // Get QVar value
	}
	iCurrent = iCurrent + ParamGetInt(m_iObjId, "NVOverlayIncrease", 1);
	pQSrv->Set("NVOverlayTest", iCurrent, 0);
	return 0;
}

/***********************
 * NVSecurityBlock
 */
 

MSGHANDLER cScr_NVSecurityBlock::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{ 
	if (m_hTimerHandle == NULL)
	{ // Start a timer.
		m_hTimerHandle = g_pScriptManager->SetTimedMessage2(m_iObjId, "SecurityCheck", 1000, kSTM_Periodic, g_mpUndef);
	}
		
	return 0;
}

MSGHANDLER cScr_NVSecurityBlock::OnFrob(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DisplayPrintf("Blocked by SHODAN level Security");
	// Insert SHODAN overlay code here!
	
	return 0;
}

MSGHANDLER cScr_NVSecurityBlock::OnTimer(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sScrTimerMsg* pTimerMsg = static_cast<sScrTimerMsg*>(pMsg);
	const char* pszMsgName = static_cast<const char*>(pTimerMsg->name);
	if (stricmp(pszMsgName, "SecurityCheck"))
		return 0; // Abort if it's not the right timer.
		
	SService<IQuestSrv> pQSrv(g_pScriptManager);
	int iCurrent = 0; // Default value (if QVar doesn't exist)
	if (pQSrv->Exists("SecurityLevel"))
	{
		iCurrent = pQSrv->Get("SecurityLevel"); // Get QVar value
	}
	
	if ( iCurrent <= ParamGetInt(m_iObjId, "CitadelSecurityLevelRequired", 0) )
	{
		object oMeta = ParamGetObject(m_iObjId, "CitadelSecurityBlockMeta", StrToObject("M-SecurityBlock"));
		if ( oMeta )
			RemoveSingleMetaProperty(oMeta, m_iObjId);
	}
	
	return 0;
}

#endif // _DARKGAME == 3
