#include "ScriptLib.h"

void MultiParmAssignFloat(sMultiParm* pParm, float fValue)
{
	if (pParm->type == kMT_String || pParm->type == kMT_Vector)
	{
		g_pMalloc->Free(pParm->psz);
	}
	pParm->type = kMT_Float;
	pParm->f = fValue;
}

