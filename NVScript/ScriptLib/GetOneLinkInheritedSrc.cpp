#include "ScriptLib.h"
#include <lg/objects.h>

long GetOneLinkInheritedSrc(const char* pszFlavor, int iSource, int iDest, sLink* pResult)
{
	SInterface<ITraitManager> pTM(g_pScriptManager);
	SInterface<ILinkManager> pLM(g_pScriptManager);
	SInterface<IRelation> pRel;
	pRel = pLM->GetRelationNamed(pszFlavor);
	if (!pRel)
		return 0;

	long iLinkID = 0;
	SInterface<IObjectQuery> pTree;
	pTree = pTM->Query(iSource, kTraitQueryMetaProps | kTraitQueryFull);
	if (!pTree)
		return 0;

	for (; ! pTree->Done(); pTree->Next())
	{
		if (pRel->AnyLinks(pTree->Object(), iDest))
		{
			ILinkQuery* pLQ = pRel->Query(pTree->Object(), iDest);
			iLinkID = pLQ->ID();
			pLQ->Release();
			break;
		}
	}

	if (iLinkID && pResult)
		pLM->Get(iLinkID, pResult);
	return iLinkID;
}

