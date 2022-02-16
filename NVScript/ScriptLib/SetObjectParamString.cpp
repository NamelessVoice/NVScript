#include "ScriptLib.h"

#include <cstring>
#include <cstdio>

using namespace std;

int SetObjectParamString(int iObject, const char* pszParam, const char* pszVal)
{
	if (!pszParam || !pszVal)
		return 1;
	char* params = GetObjectParamsNoInherit(iObject);
	if (!params)
	{
		int iLen = 4 + strlen(pszParam) + strlen(pszVal);
		params = reinterpret_cast<char*>(g_pMalloc->Alloc(iLen + 1));
		if (!params)
			return 1;
		sprintf(params, "%s=\"%s\"", pszParam, pszVal);
	}
	else
	{
		char* newparams = SetParamString(params, pszParam, pszVal);
		g_pMalloc->Free(params);
		params = newparams;
	}
	int iRet = 1;
	if (params)
	{
		iRet = SetObjectParams(iObject, params);
		g_pMalloc->Free(params);
	}
	return iRet;
}

