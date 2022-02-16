#include "ScriptLib.h"

float GetObjectParamFloat(int iObject, const char* pszParam)
{
	char* params = GetObjectParams(iObject);
	if (!params)
		return 0;
	float fRet = GetParamFloat(params, pszParam);
	g_pMalloc->Free(params);
	return fRet;
}

