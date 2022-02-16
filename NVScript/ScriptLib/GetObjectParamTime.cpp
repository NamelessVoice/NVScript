#include "ScriptLib.h"

int GetObjectParamTime(int iObject, const char* pszParam)
{
	char* params = GetObjectParams(iObject);
	if (!params)
		return 0;
	int iRet = GetParamTime(params, pszParam);
	g_pMalloc->Free(params);
	return iRet;
}

