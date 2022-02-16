#include "ScriptLib.h"

#include <vector>
#include <cstring>

#ifdef __BORLANDC__
#define strnicmp	std::strnicmp
#endif

using namespace std;

long GetAnyLinkByData(const char* pszFlavor, int iSource, int iDest, sLink* pResult, const void* pvData, int iDataLen)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	int (__cdecl *cmp)(const void*, const void*, size_t) = memcmp;
	if (pvData && iDataLen == -1)
	{
		iDataLen = strlen(reinterpret_cast<const char*>(pvData)) + 1;
		cmp = reinterpret_cast<int (*)(const void*,const void*,size_t)>(strnicmp);
	}

	vector<long> vLinks;
	linkset lsLinks;
	linkkind lkFlavor = (pszFlavor) ? pLTS->LinkKindNamed(pszFlavor) : 0;
	pLS->GetAll(lsLinks, lkFlavor, iSource, iDest);
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		void* pLinkData = lsLinks.Data();
		if (pvData)
		{
			if (!pLinkData)
				continue;
			if ((*cmp)(pvData, pLinkData, iDataLen))
				continue;
		}
		else
		{
			if (pLinkData && *reinterpret_cast<const char*>(pLinkData))
				continue;
		}
		vLinks.insert(vLinks.begin(), lsLinks.Link());
	}

	long iLinkID = 0;
	if (!vLinks.empty())
	{
		SService<IDataSrv> pDS(g_pScriptManager);
		iLinkID = vLinks.at(pDS->RandInt(0, vLinks.size() - 1));
		if (iLinkID && pResult)
			pLTS->LinkGet(iLinkID, *pResult);
	}
	return iLinkID;
}

