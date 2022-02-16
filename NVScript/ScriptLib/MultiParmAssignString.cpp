#include "ScriptLib.h"

#include <cstring>

using namespace std;

void MultiParmAssignString(sMultiParm* pParm, const char* pszValue)
{
	if (pParm->type == kMT_String || pParm->type == kMT_Vector)
	{
		g_pMalloc->Free(pParm->psz);
		pParm->type = kMT_Undef;
	}
	pParm->psz = reinterpret_cast<char*>(g_pMalloc->Alloc(strlen(pszValue) + 1));
	if (pParm->psz)
	{
		strcpy(pParm->psz, pszValue);
		pParm->type = kMT_String;
	}
}

