#include "ScriptLib.h"
#include <lg/objects.h>

#include <cstring>

#ifdef __BORLANDC__
#define strnicmp	std::strnicmp
#endif

using namespace std;

long GetOneLinkByDataInheritedSrc(const char* pszFlavor, int iSource, int iDest, sLink* pResult, const void* pvData, int iDataLen)
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

	int (__cdecl *cmp)(const void*, const void*, size_t) = memcmp;
	if (pvData && iDataLen == -1)
	{
		iDataLen = strlen(reinterpret_cast<const char*>(pvData)) + 1;
		cmp = reinterpret_cast<int (*)(const void*,const void*,size_t)>(strnicmp);
	}

	long iLinkID = 0;
	for (; ! pTree->Done(); pTree->Next())
	{
		if (pRel->AnyLinks(pTree->Object(), iDest))
		{
			ILinkQuery* pLQ = pRel->Query(pTree->Object(), iDest);
			for (; ! pLQ->Done(); pLQ->Next())
			{
				void* pLinkData = pLQ->Data();
				if (pvData)
				{
					if (pLinkData && !(*cmp)(pvData, pLinkData, iDataLen))
					{
						iLinkID = pLQ->ID();
						if (pResult)
							pLQ->Link(pResult);
						break;
					}
				}
				else
				{
					if (!pLinkData || !*reinterpret_cast<const char*>(pLinkData))
					{
						iLinkID = pLQ->ID();
						if (pResult)
							pLQ->Link(pResult);
						break;
					}
				}
			}
			pLQ->Release();
			if (iLinkID != 0)
				break;
		}
	}
	return iLinkID;
}

