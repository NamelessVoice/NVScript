#include "ParamCache.h"

#include <cstdio>

extern cParamCache* g_pGlobalParamCache;

void ParamSetFloat(int iObject, const char* pszParam, float fValue)
{
	if (!g_pGlobalParamCache)
		return;
	char psz[44];
	std::string sVal(psz, sprintf(psz, "%0.2f", fValue));
	g_pGlobalParamCache->Update(iObject, pszParam, &sVal);
}

