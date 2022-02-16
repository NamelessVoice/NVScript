#include "ParamCache.h"

extern cParamCache* g_pGlobalParamCache;

void ParamRemove(int iObject, const char* pszParam, bool bNoInherit)
{
	if (!g_pGlobalParamCache)
		return;
	if (bNoInherit)
		g_pGlobalParamCache->Update(iObject, pszParam, NULL);
	else
		g_pGlobalParamCache->Remove(iObject, pszParam);
}

