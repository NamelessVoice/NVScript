#include "lg/interface.h"
#include "lg/scrservices.h"
#include "lg/objects.h"
#include "ScriptLib.h"
#include "NVScriptLib.h"
#include "NVGenericTrigger.h"

#if (_DARKGAME == 3)
	#define CDLINK	"SwitchLink"
#else
	#define CDLINK	"ControlDevice"
#endif

long cScrNVTrigger::Trigger(bool bOn, sScrMsg* pMsg, int iDefaultDest, const char* szDefaultOnMessage, const char* szDefaultOffMessage)
{
//	int iMaxTriggers = 0;
//	int iTriggerCountOnly = 0;
	char* pszParam = new char[strlen(m_szTriggerName) + 20];
	
	sprintf(pszParam, "%sTCount", m_szTriggerName);
	int iMaxTriggers = ParamGetInt(m_iTrigObjId, pszParam, 0);
	
	sprintf(pszParam, "%sTCountOnly", m_szTriggerName);
	int iTriggerCountOnly = ParamGetInt(m_iTrigObjId, pszParam, 0);

	const char* pszMessage;
	if ( bOn)
	{
		sprintf(pszParam, "%sTOn", m_szTriggerName);
		pszMessage = ParamGetString(m_iTrigObjId, pszParam, szDefaultOnMessage);
	} else {
		sprintf(pszParam, "%sTOff", m_szTriggerName);
		pszMessage = ParamGetString(m_iTrigObjId, pszParam, szDefaultOffMessage);
	}
	
	if (!stricmp(pszMessage, "NULL"))
	{
		delete[] pszParam;
		return 0;
	}

	sprintf(pszParam, "%sTDest", m_szTriggerName);
	const char* pszDest = ParamGetString(m_iTrigObjId, pszParam, NULL);
	
	sprintf(pszParam, "%sKillLinks", m_szTriggerName);
	int iKillLinks = ParamGetInt(m_iTrigObjId, pszParam, 0);
	
	sprintf(pszParam, "%sFailChance", m_szTriggerName);
	float fFailChance = ParamGetFloat(m_iTrigObjId, pszParam, 0.00f);

	bool bCount = false;
	// If iTriggerCountOnly is set and is not the current bOn, then set bCount to false.
	if ( iMaxTriggers )
	{
		bCount = true;
	}

	if ( iTriggerCountOnly )
	{
		if ( (bOn && iTriggerCountOnly!=1) || (!bOn && iTriggerCountOnly!=2) )
		{
			bCount = false;
		}
	}
	
	if ( bCount )
	{
		sprintf(pszParam, "%sCurrentTriggerCount", m_szTriggerName);
		int iTriggerCount = ParamGetInt(m_iTrigObjId, pszParam, 0);
	
		if ( iTriggerCount < iMaxTriggers )
		{
			iTriggerCount++;
			ParamSetInt(m_iTrigObjId, pszParam, iTriggerCount);
		} else {
			delete[] pszParam;
			return 0;
		}
	}
	
	delete[] pszParam;
	
	if (fFailChance)
	{
		#if (_DARKGAME != 3)
			SService<IDataSrv> pDS(g_pScriptManager);
			if (float(pDS->RandInt(1, 100)) <= fFailChance)
		# else
			SService<IShockGameSrv> pShockGame(g_pScriptManager);
			if (float(pShockGame->RandRange(1, 100)) <= fFailChance)
		#endif
		{
//			DisplayPrintf("This NVTrigger script is unreliable. You should ask for your money back!");
			return 0;
		}
	}
	
	
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<ILinkSrv> pLS(g_pScriptManager);
	const char* pszLinkName = NULL;
	
	// Check if a stim has been called for:
	SService<IActReactSrv> pARSrv(g_pScriptManager);

	float fIntensity = 0.00f;
	float fMaxIntensity = 0.00f;
	char pszStimName[80];
	int iIsStim = sscanf(pszMessage, "[%f|%f]%[^\n]", &fIntensity, &fMaxIntensity, pszStimName);
	if ( 3 == iIsStim )
	{
		SService<IDataSrv> pDS(g_pScriptManager);
		fIntensity += ((fMaxIntensity - fIntensity) * pDS->RandFlt0to1());
	} else {
		iIsStim = sscanf(pszMessage, "[%f]%[^\n]", &fIntensity, pszStimName);
	}
	
	
	object oStim = 0;

	if ( ( 2 == iIsStim ) || ( 3 == iIsStim ) )
	{
		oStim = StrToObject(pszStimName);
		if (!oStim )
		{
			return 0;
		}
		
	}	

	bool NearestUseXYZ = false;
	
	object oDest = 0;
	long iLinkFlavour = 0;
	float fNearDistance = 0.00f;
	if ( pszDest )
	{
		int iGetTraitTree = 0;
		switch ( pszDest[0] )
		{
			case '&':
				pszLinkName = (pszDest +1);
				break;
			case '{':
				NearestUseXYZ = true;
			case '<':
				{
				char *pszDest2 = new char[strlen(pszDest)+1];
				sscanf(pszDest+1, "%f:%[^\n]", &fNearDistance, pszDest2);
				oDest = NVStrToObject(pszDest2, pMsg);
				delete[] pszDest2;
				}
			case '@':
				iGetTraitTree = kTraitQueryFull;
			case '*':
			{
				// Affect all objects of a specific type.
				if ( (pszDest[0] != '<' ) && (pszDest[0] != '{' ) )
					oDest = NVStrToObject(pszDest+1, pMsg);
					
				SInterface<ITraitManager> pTraits(g_pScriptManager);
				
				if ( oDest > 0 )
				{
					oDest = pTraits->GetArchetype(m_iTrigObjId); // Make a note of object's archetype.
				}
				// Use the ITraitManager to affect all children of this archetype.
				IObjectQuery* pInheritance = pTraits->Query(oDest, kTraitQueryChildren | iGetTraitTree);
				if (!pInheritance)
				{
					return 0;
				}
				
				SService<IObjectSrv> pOS(g_pScriptManager);
				while (! pInheritance->Done())
				{
					int iChild = pInheritance->Object();
					if ( iChild > 0)
					{
						if ( fNearDistance > 0.00f )
						{
							true_bool bExists;
							pOS->Exists(bExists, m_iTrigObjId);
							if (bExists)
							{
								cScrVec vPosition, vDestPos;
								pOS->Position(vPosition, m_iTrigObjId);
								pOS->Position(vDestPos, iChild);

								if ( (( NearestUseXYZ == false ) && ( GetDistanceXY(&vPosition, &vDestPos) > fNearDistance ))
								|| (( NearestUseXYZ == true ) && ( GetDistance(&vPosition, &vDestPos) > fNearDistance )) )
								{
									pInheritance->Next();
									continue;
								}
							} else {
								DisplayPrintf(0x0000FF, "NVScript Error: %s: Attempting to get location of object which has already been destroyed, for nearest-object trigger on %s (%i).", m_szTriggerName, static_cast<const char*>(GetObjectDescriptiveName(m_iTrigObjId)), m_iTrigObjId);
								DebugPrintf("NVScript Error: %s: Attempting to get location of object which has already been destroyed, for nearest-object trigger on %s (%i).", m_szTriggerName, static_cast<const char*>(GetObjectDescriptiveName(m_iTrigObjId)), m_iTrigObjId);
							}
						}
						if ((2 == iIsStim) || ( 3 == iIsStim ) )
						{ // Send stim to single object
							StimSingleObject(pARSrv, iChild, oStim, fIntensity);
						} else { // Send message to single object
							TriggerSingleObject(iChild, pszMessage, pMsg->time);
						}
					}
					pInheritance->Next();
				}
				pInheritance->Release();
				
				return 0;
			}
			default: // Affect a single object
				oDest = NVStrToObject(pszDest, pMsg);
		}
	}
	if ( pszLinkName )
	{
		if ( pszLinkName[0] == '?' )
		{ // Choose one random link.
			sLink slDest;
			pszLinkName++;
			long lDest = 0;
			/*
			if (!stricmp(pszLinkName, "Delayed"))
			{

				linkset lsLinks;
				linkkind lkFlavor = pLTS->LinkKindNamed("ScriptParams");
				pLS->GetAll(lsLinks, lkFlavor, M_iTrigObjId, 0);
				cMultiParm mpReturn;
				int iLinkDelay;
				int i;
				sLink currsl;
				for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
				{
					void* pLinkData = lsLinks.Data();
					if (pLinkData)
						iLinkDelay = strtol(reinterpret_cast<const char*>(pLinkData),NULL,10);
					else
						iLinkDelay = 0;
					lsLinks.query->Link(&currsl);
					
					oDest = currsl.dest;
					if (!iLinkDelay)
					{
						if ((2 == iIsStim) || ( 3 == iIsStim ) )
						{ // Send stim to single object
							return StimSingleObject(pARSrv, oDest, oStim, fIntensity);
						} else { // Send message to single object
							return TriggerSingleObject(oDest, pszMessage, pMsg->time);
						}
					} else {
						// WARNING: UNFINISHED!!!!!
						// Needs to pass message AND destination, possibly info on stim too.
						if (bOn)
							g_pScriptManager->SetTimedMessage2(m_iTrigObjId, "NVTrapDelayOn", iDelay, kSTM_OneShot, static_cast<int>(oDest));
						else
							g_pScriptManager->SetTimedMessage2(m_iTrigObjId, "NVTrapDelayOff", iDelay, kSTM_OneShot, static_cast<int>(oDest));
					}
				}
			return 0;
			}
*/
			if (!stricmp(pszLinkName, "Weighted"))
			{
				lDest = GetAnyWeightedLink(m_iTrigObjId, 0, &slDest);
			} else {
				lDest = GetAnyLink(pszLinkName, m_iTrigObjId, 0, &slDest);
			}
			if ( lDest )
			{
				oDest = slDest.dest;
				if ( iKillLinks )
				{
					pLS->Destroy(lDest);
				}
			}
		} else {
			iLinkFlavour = pLTS->LinkKindNamed(pszLinkName);
		}
	} else {
		if ( iDefaultDest != 0 )
			oDest = iDefaultDest;
		else
			iLinkFlavour = pLTS->LinkKindNamed(CDLINK);
	}
	
	if ( oDest ) {
		if ((2 == iIsStim) || ( 3 == iIsStim ) )
		{ // Send stim to single object
			return StimSingleObject(pARSrv, oDest, oStim, fIntensity);
		} else { // Send message to single object
			return TriggerSingleObject(oDest, pszMessage, pMsg->time);
		}
	} else {
		if ( iLinkFlavour )
		{
			if ( (2 == iIsStim) || ( 3 == iIsStim ) )
			{ // Send stim along links		
				cScrStr sLinkName;
				pLTS->LinkKindName(sLinkName, iLinkFlavour);
				// IterateLinks won't work from outside an cScript, so we have to do things differently
				linkset lsLinks;
				pLS->GetAll(lsLinks, iLinkFlavour, m_iTrigObjId, 0);
				
				for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
				{
					sLink sl = lsLinks.Get();
					
					StimSingleObject(pARSrv, sl.dest, oStim, fIntensity);
					
				}
			} else { // Send message along links
				SService<ILinkSrv> pLS(g_pScriptManager);
				return pLS->BroadcastOnAllLinks(m_iTrigObjId, pszMessage, iLinkFlavour);
			}
		}
	}

	return 0;
}

long cScrNVTrigger::TriggerSingleObject(object oDest, const char* pszMessage, int iMsgTime)
{
	sScrMsg msg;
		msg.from = m_iTrigObjId;
		msg.to = oDest;
		msg.time = iMsgTime;
		msg.flags = 8;
		msg.message = pszMessage;
	// Send message
	cMultiParm pReply;
	return g_pScriptManager->SendMessage(&msg, &pReply);
}

long cScrNVTrigger::StimSingleObject(object oDest, object oStim, float fIntensity)
{
	if ( oStim )
	{
		SService<IActReactSrv> pARSrv(g_pScriptManager);
		#if (_DARKGAME != 1)
			return pARSrv->Stimulate(oDest, oStim, fIntensity, m_iTrigObjId);	
		#else // Backwards compatibility for T1. Fairly pointless since the module is set to only compile for _DARKGAME 2.
			return pARSrv->Stimulate(oDest, oStim, fIntensity);	
		#endif
	}
	return 0;
}

long cScrNVTrigger::StimSingleObject(IActReactSrv* pARSrv, object oDest, object oStim, float fIntensity)
{
	#if (_DARKGAME != 1)
		return pARSrv->Stimulate(oDest, oStim, fIntensity, m_iTrigObjId);	
	#else // Backwards compatibility for T1. Fairly pointless since the module is set to only compile for _DARKGAME 2.
		return pARSrv->Stimulate(oDest, oStim, fIntensity);	
	#endif
}


void cScrNVTrigger::ResetTriggerCount()
{
	// Upon receiving the "ResetCount" script message, this resets m_iUseCount
	// so that a limited-use trap can be reused.
	
	char* pszParam = new char[strlen(m_szTriggerName) + 20];
	sprintf(pszParam, "%sCurrentTriggerCount", m_szTriggerName);
	ParamSetInt(m_iTrigObjId, pszParam, 0);
	delete[] pszParam;
}
