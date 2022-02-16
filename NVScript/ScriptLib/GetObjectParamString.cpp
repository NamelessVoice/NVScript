#include "ScriptLib.h"

char* GetObjectParamString(int iObject, const char* pszParam)
{
	char* pszString = GetObjectParams(iObject);
	char* pszRet = GetParamString(pszString, pszParam);
	g_pMalloc->Free(pszString);
	return pszRet;
}

