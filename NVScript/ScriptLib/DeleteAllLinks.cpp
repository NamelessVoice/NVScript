#include "ScriptLib.h"

void DeleteAllLinks(const char* pszFlavor, int iSource, int iDest)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	linkset lsLinks;
	linkkind lkFlavor = (pszFlavor) ? pLTS->LinkKindNamed(pszFlavor) : 0;
	pLS->GetAll(lsLinks, lkFlavor, iSource, iDest);
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		pLS->Destroy(lsLinks.Link());
	}
}
