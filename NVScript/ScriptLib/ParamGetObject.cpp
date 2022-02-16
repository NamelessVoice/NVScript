#include "ParamCache.h"
#include "ScriptLib.h"

using namespace std;

extern cParamCache* g_pGlobalParamCache;

int ParamGetObject(int iObject, const char* pszParam, int iDefault)
{
	if (!g_pGlobalParamCache)
		return iDefault;
	const string* psParam = g_pGlobalParamCache->Retrieve(iObject, pszParam);
	if (!psParam)
		return iDefault;
	return StrToObject(psParam->c_str());
}

