#include "ScriptLib.h"

int GetAnyLinkDest(const char* pszFlavor, int iSource)
{
	sLink sl;
	if (GetAnyLink(pszFlavor, iSource, 0, &sl))
		return sl.dest;
	return 0;
}

