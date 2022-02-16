#include "ParamCache.h"

extern cParamCache* g_pGlobalParamCache;

void ParamSetBool(int iObject, const char* pszParam, bool bValue)
{
	if (!g_pGlobalParamCache)
		return;
	std::string sVal(bValue ? "true" : "false");
	g_pGlobalParamCache->Update(iObject, pszParam, &sVal);
}

