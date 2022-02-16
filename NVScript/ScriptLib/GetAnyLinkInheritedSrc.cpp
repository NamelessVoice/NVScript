#include "ScriptLib.h"
#include <lg/objects.h>

#include <vector>

using namespace std;

long GetAnyLinkInheritedSrc(const char* pszFlavor, int iSource, int iDest, sLink* pResult)
{
	SInterface<ITraitManager> pTM(g_pScriptManager);
	SInterface<ILinkManager> pLM(g_pScriptManager);
	SInterface<IRelation> pRel;
	pRel = pLM->GetRelationNamed(pszFlavor);
	if (!pRel)
		return 0;
	SInterface<IObjectQuery> pTree;
	pTree = pTM->Query(iSource, kTraitQueryMetaProps | kTraitQueryFull);
	if (!pTree)
		return 0;

	vector<long> vLinks;
	for (; ! pTree->Done(); pTree->Next())
	{
		if (pRel->AnyLinks(pTree->Object(), iDest))
		{
			ILinkQuery* pLQ = pRel->Query(pTree->Object(), iDest);
			for (; ! pLQ->Done(); pLQ->Next())
			{
				vLinks.insert(vLinks.begin(), pLQ->ID());
			}
			pLQ->Release();
		}
	}

	long iLinkID = 0;
	if (!vLinks.empty())
	{
		SService<IDataSrv> pDS(g_pScriptManager);
		iLinkID = vLinks.at(pDS->RandInt(0, vLinks.size() - 1));
		if (iLinkID && pResult)
			pLM->Get(iLinkID, pResult);
	}
	return iLinkID;
}

