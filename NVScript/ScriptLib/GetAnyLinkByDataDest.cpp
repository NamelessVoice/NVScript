#include "ScriptLib.h"

int GetAnyLinkByDataDest(const char* pszFlavor, int iSource, const void* pvData, int iDataLen)
{
	sLink sl;
	if (GetAnyLinkByData(pszFlavor, iSource, 0, &sl, pvData, iDataLen))
		return sl.dest;
	return 0;
}

