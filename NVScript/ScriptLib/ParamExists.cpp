#include "ParamCache.h"

extern cParamCache* g_pGlobalParamCache;

bool ParamExists(int iObject, const char* pszParam)
{
	if (!g_pGlobalParamCache)
		return false;
	return (g_pGlobalParamCache->Retrieve(iObject, pszParam) != NULL);
}

