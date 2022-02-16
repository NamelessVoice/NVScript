#include "ScriptLib.h"

int GetOneLinkByDataDest(const char* pszFlavor, int iSource, const void* pvData, int iDataLen)
{
	sLink sl;
	if (GetOneLinkByData(pszFlavor, iSource, 0, &sl, pvData, iDataLen))
		return sl.dest;
	return 0;
}

