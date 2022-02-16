#include "ScriptLib.h"

#include <vector>

using namespace std;

long GetAnyLink(const char* pszFlavor, int iSource, int iDest, sLink* pResult)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	vector<long> vLinks;
	linkset lsLinks;
	linkkind lkFlavor = (pszFlavor) ? pLTS->LinkKindNamed(pszFlavor) : 0;
	pLS->GetAll(lsLinks, lkFlavor, iSource, iDest);
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
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

