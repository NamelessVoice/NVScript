//#if _DARKGAME != 2
//#error NVScript is written for Thief2. You can try, but I doubt it.
//#else

// #include <cstdlib>
#include <cmath>

#include "lg/interface.h"
#include "lg/scrservices.h"
#include "lg/scrmanagers.h"
#include "lg/scrmsgs.h"
#include "lg/objects.h"
#include "ScriptLib.h"
#include "NVScriptLib.h"
#include "NVScript-Flames.h"

#include "lg/iids.h"



/***********************
 * NVInflammable
 */
 
MSGHANDLER cScr_NVInflammable::OnBurn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) 
{
	DisplayPrintf("I was hit with some fire...");
	if ( !m_iBurning )
	{
		DisplayPrintf("Starting to smoulder...");
		m_iBurning = 1;
		
		SService<IObjectSrv> pOS(g_pScriptManager);
		object oMeta = ParamGetObject(m_iObjId, "NVFlameMeta", 0);
		if ( oMeta )
		{
			AddSingleMetaProperty(oMeta, m_iObjId);
		}
		
		const char* pszAttach;
		int iArc, iJoint;
		link lLink, lSPLink;
		object oCreated;
		char szParam[16];
			
		SService<ILinkSrv> pLS(g_pScriptManager);
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		
		for ( int i = 1; i < 11; i++ )
		{
			DebugPrintf("Considering whether to attach flame %i...", i);
			DisplayPrintf("Considering whether to attach flame %i...", i);
			oCreated = 0;
			sprintf(szParam, "NVFlame%i", i);
			pszAttach = ParamGetString(m_iObjId, szParam, NULL);
			if ( !pszAttach )
			{
				DebugPrintf("No such thing as flame %i", i);
				return 0;
			}
			sprintf(szParam, "NVFlameJoint%i", i);
			iJoint = ParamGetInt(m_iObjId, szParam, 0);
			iArc = StrToObject(pszAttach);
			if ( iArc )
			{
				DebugPrintf("Attaching flame %i...", i);
				DisplayPrintf("Attaching flame %i...", i);
				pOS->Create(oCreated, iArc);
				if ( oCreated )
				{
					DebugPrintf("Object successfully created for %i...", i);
					lLink = 0;
					pLS->Create(lLink, pLTS->LinkKindNamed("~ParticleAttachement"), m_iObjId, oCreated);
					if (lLink)
					{
						DebugPrintf("Link created for %i.", i);
						if ( iJoint )
						{
							DebugPrintf("Joint setup for %i.", i);
							pLTS->LinkSetData(lLink, "Type", 2);
							pLTS->LinkSetData(lLink, "Joint", iJoint);
						}
						pLS->Create(lSPLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, oCreated);
						if ( lSPLink )
						{
							DebugPrintf("SP Link created for %i.", i);
							pLTS->LinkSetData(lSPLink, NULL, "NVFlameObject");
						}
					}
				}
			}
		}
	}
	return 0;
}

MSGHANDLER cScr_NVInflammable::OnDouse(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) 
{
	IterateLinksByData("ScriptParams", m_iObjId, 0, "NVFlameObject", -1, DestroyAttachedIterFunc, NULL, NULL);
	m_iBurning = 0;
	object oMeta = ParamGetObject(m_iObjId, "NVFlameMeta", 0);
	if ( oMeta )
	{
		RemoveSingleMetaProperty(oMeta, m_iObjId);
	}
	return 0;
}

int cScr_NVInflammable::DestroyAttachedIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	sLink sl;
	pLQ->Link(&sl);
	
	SInterface<IObjectSystem> pOS(g_pScriptManager);
	pOS->Destroy(sl.dest);
	return 1;
}

//#endif // _DARKGAME == 2
