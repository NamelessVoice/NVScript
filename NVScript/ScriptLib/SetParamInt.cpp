#include "ScriptLib.h"

#include <cstdio>

using namespace std;

char* SetParamInt(char* pszString, const char* pszParam, int iVal)
{
	char psz[12];
	sprintf(psz, "%d", iVal);
	return SetParamString(pszString, pszParam, psz);
}

