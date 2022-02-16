#include "ScriptLib.h"

int GetOneLinkDest(const char* pszFlavor, int iSource)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link l;
	linkkind lkFlavor = (pszFlavor) ? pLTS->LinkKindNamed(pszFlavor) : 0;
	pLS->GetOne(l,lkFlavor, iSource, 0);
	if (l == 0)
		return 0;
	sLink sl;
	pLTS->LinkGet(l, sl);
	return sl.dest;
}

