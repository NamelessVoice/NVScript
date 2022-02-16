#include "ScriptLib.h"

#include <cstdio>

using namespace std;

int SetObjectParamFloat(int iObject, const char* pszParam, float fVal)
{
	char psz[44];
	sprintf(psz, "%0.2f", fVal);
	return SetObjectParamString(iObject, pszParam, psz);
}

