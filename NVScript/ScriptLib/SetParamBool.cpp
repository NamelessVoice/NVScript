#include "ScriptLib.h"

#include <cstdio>

using namespace std;

char* SetParamBool(char* pszString, const char* pszParam, bool bVal)
{
	return SetParamString(pszString, pszParam, (bVal) ? "true" : "false");
}

