#include "ScriptLib.h"

int GetObjectParamInt(int iObject, const char* pszParam)
{
	char* params = GetObjectParams(iObject);
	if (!params)
		return 0;
	int iRet = GetParamInt(params, pszParam);
	g_pMalloc->Free(params);
	return iRet;
}

