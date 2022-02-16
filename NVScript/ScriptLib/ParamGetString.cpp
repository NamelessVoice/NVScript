#include "ParamCache.h"

using namespace std;

extern cParamCache* g_pGlobalParamCache;

const char* ParamGetString(int iObject, const char* pszParam, const char* pszDefault)
{
	if (!g_pGlobalParamCache)
		return pszDefault;
	const string* psParam = g_pGlobalParamCache->Retrieve(iObject, pszParam);
	if (!psParam)
		return pszDefault;
	return psParam->c_str();
}

