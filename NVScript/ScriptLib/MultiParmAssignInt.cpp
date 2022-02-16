#include "ScriptLib.h"

void MultiParmAssignInt(sMultiParm* pParm, int iValue)
{
	if (pParm->type == kMT_String || pParm->type == kMT_Vector)
	{
		g_pMalloc->Free(pParm->psz);
	}
	pParm->type = kMT_Int;
	pParm->i = iValue;
}

