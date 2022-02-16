#include "ScriptLib.h"

#include <cstdio>

using namespace std;

char* SetParamFloat(char* pszString, const char* pszParam, float fVal)
{
	char psz[44];
	sprintf(psz, "%0.2f", fVal);
	return SetParamString(pszString, pszParam, psz);
}

