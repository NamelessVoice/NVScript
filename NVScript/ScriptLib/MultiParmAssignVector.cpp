#include "ScriptLib.h"

#include <cstring>

using namespace std;

void MultiParmAssignVector(sMultiParm* pParm, const mxs_vector* pvValue)
{
	if (pParm->type == kMT_String || pParm->type == kMT_Vector)
	{
		g_pMalloc->Free(pParm->psz);
		pParm->type = kMT_Undef;
	}
	pParm->pVector = reinterpret_cast<mxs_vector*>(g_pMalloc->Alloc(sizeof(mxs_vector)));
	if (pParm->pVector)
	{
		memcpy(pParm->pVector, pvValue, sizeof(mxs_vector));
		pParm->type = kMT_Vector;
	}
}


