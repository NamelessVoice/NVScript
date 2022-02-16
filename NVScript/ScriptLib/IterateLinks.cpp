#include "ScriptLib.h"

long IterateLinks(const char* pszFlavor, int iSource, int iDest,
			    LinkIterCallbackFunc pfnLinkIterCallback, IScript* pScript, void* pData)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	linkset lsLinks;
	linkkind lkFlavor = (pszFlavor) ? pLTS->LinkKindNamed(pszFlavor) : 0;
	pLS->GetAll(lsLinks, lkFlavor, iSource, iDest);
	long iCount = 0;
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		++iCount;
		if (pfnLinkIterCallback && !pfnLinkIterCallback(pLS, lsLinks.query, pScript, pData))
			break;
	}
	return iCount;
}

