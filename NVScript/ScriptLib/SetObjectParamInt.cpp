#include "ScriptLib.h"

#include <cstdio>

using namespace std;

int SetObjectParamInt(int iObject, const char* pszParam, int iVal)
{
	char psz[12];
	sprintf(psz, "%d", iVal);
	return SetObjectParamString(iObject, pszParam, psz);
}

