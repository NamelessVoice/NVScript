#include "ScriptLib.h"

bool GetObjectParamBool(int iObject, const char* pszParam)
{
	char* params = GetObjectParams(iObject);
	if (!params)
		return 0;
	bool bRet = GetParamBool(params, pszParam);
	g_pMalloc->Free(params);
	return bRet;
}

