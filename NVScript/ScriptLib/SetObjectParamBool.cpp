#include "ScriptLib.h"

#include <cstdio>

using namespace std;

int SetObjectParamBool(int iObject, const char* pszParam, bool bVal)
{
	return SetObjectParamString(iObject, pszParam, (bVal) ? "true" : "false");
}

