#include "ScriptLib.h"

int GetObjectParamObject(int iObject, const char* pszParam)
{
	char* params = GetObjectParams(iObject);
	if (!params)
		return 0;
	int iRet = GetParamObject(params, pszParam);
	g_pMalloc->Free(params);
	return iRet;
}

