#include "ScriptLib.h"

static int FixupPlayerLinksFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData)
{
	// Move the destination of the link to the object specified in pData
	SInterface<ILinkManager> pLM(g_pScriptManager);
	int iPlayer = reinterpret_cast<int>(pData);
	sLink slInfo;
	pLQ->Link(&slInfo);
	pLM->AddFull(slInfo.source, iPlayer, slInfo.flavor, pLQ->Data());
	pLM->Remove(pLQ->ID());
	return 1;
}

void FixupPlayerLinks(int iSource, int iPlayer)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	// Get the PlayerFactory link. There should only ever be one of these.
	link lPFLink;
	pLS->GetOne(lPFLink, pLTS->LinkKindNamed("PlayerFactory"), 0, 0);
	if (lPFLink == 0)
		return;
	// Iterate through all links from here to the source of the PF link.
	sLink lPFInfo;
	pLTS->LinkGet(lPFLink, lPFInfo);
	IterateLinks(NULL, iSource, lPFInfo.source, FixupPlayerLinksFunc, NULL, reinterpret_cast<void*>(iPlayer));
}


