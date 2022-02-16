#include "ScriptLib.h"

#include <cstring>

#ifdef __BORLANDC__
#define strnicmp	std::strnicmp
#endif

using namespace std;

long IterateLinksByData(const char* pszFlavor, int iSource, int iDest,
			    const void* pvFilter, int iFilterLen,
			    LinkIterCallbackFunc pfnLinkIterCallback, IScript* pScript, void* pData)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	int (__cdecl *cmp)(const void*, const void*, size_t) = memcmp;
	if (pvFilter && iFilterLen == -1)
	{
		iFilterLen = strlen(reinterpret_cast<const char*>(pvFilter)) + 1;
		cmp = reinterpret_cast<int (*)(const void*,const void*,size_t)>(strnicmp);
	}

	linkset lsLinks;
	linkkind lkFlavor = (pszFlavor) ? pLTS->LinkKindNamed(pszFlavor) : 0;
	pLS->GetAll(lsLinks, lkFlavor, iSource, iDest);
	long iCount = 0;
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		void* pLinkData = lsLinks.Data();
		if (pvFilter)
		{
			if (!pLinkData)
				continue;
			if ((*cmp)(pvFilter, pLinkData, iFilterLen))
				continue;
		}
		else
		{
			if (pLinkData && *reinterpret_cast<const char*>(pLinkData))
				continue;
		}
		++iCount;
		if (pfnLinkIterCallback && !pfnLinkIterCallback(pLS, lsLinks.query, pScript, pData))
			break;
	}
	return iCount;
}

