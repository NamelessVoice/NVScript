#include "ParamCache.h"

#include <cstdio>

extern cParamCache* g_pGlobalParamCache;

void ParamSetInt(int iObject, const char* pszParam, int iValue)
{
	if (!g_pGlobalParamCache)
		return;
	char psz[12];
	std::string sVal(psz, sprintf(psz, "%d", iValue));
	g_pGlobalParamCache->Update(iObject, pszParam, &sVal);
}

