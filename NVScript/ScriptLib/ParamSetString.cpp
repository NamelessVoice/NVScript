#include "ParamCache.h"

extern cParamCache* g_pGlobalParamCache;

void ParamSetString(int iObject, const char* pszParam, const char* pszValue)
{
	if (!g_pGlobalParamCache)
		return;
	std::string sVal;
	if (pszValue)
		sVal = pszValue;
	g_pGlobalParamCache->Update(iObject, pszParam, &sVal);
}

