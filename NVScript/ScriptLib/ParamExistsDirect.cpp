#include "ParamCache.h"

extern cParamCache* g_pGlobalParamCache;

bool ParamExistsDirect(int iObject, const char* pszParam)
{
	if (!g_pGlobalParamCache)
		return false;
	return (g_pGlobalParamCache->RetrieveSingle(iObject, pszParam) != NULL);
}

