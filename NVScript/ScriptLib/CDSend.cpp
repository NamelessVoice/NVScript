#include "ScriptLib.h"

long CDSend(const char* pszMessage, int iSource, const cMultiParm& mp)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	return pLS->BroadcastOnAllLinksData(iSource, pszMessage, pLTS->LinkKindNamed(g_pszCDLinkFlavor), mp);
}
