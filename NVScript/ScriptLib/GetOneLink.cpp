#include "ScriptLib.h"

long GetOneLink(const char* pszFlavor, int iSource, int iDest, sLink* pResult)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link l;
	linkkind lkFlavor = (pszFlavor) ? pLTS->LinkKindNamed(pszFlavor) : 0;
	pLS->GetOne(l,lkFlavor, iSource, iDest);
	if (l == 0)
		return 0;
	if (l && pResult)
		pLTS->LinkGet(l, *pResult);
	return l;
}

