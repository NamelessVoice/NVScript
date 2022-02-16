#include "ParamCache.h"
#include "ScriptLib.h"
#include <lg/scrservices.h>

#include <cstdlib>

using namespace std;

extern cParamCache* g_pGlobalParamCache;

float ParamGetFloat(int iObject, const char* pszParam, float fDefault)
{
	if (!g_pGlobalParamCache)
		return fDefault;
	const string* psParam = g_pGlobalParamCache->Retrieve(iObject, pszParam);
	if (!psParam)
		return fDefault;
	if (psParam->size() == 0)
		return 0.0f;
	if (psParam->at(0) == '$')
	{
		const char* psz = psParam->c_str()+1;
		SService<IQuestSrv> pQS(g_pScriptManager);
		if (pQS->Exists(psz))
			return (float)(pQS->Get(psz));
	}
	else
	{
		return strtod(psParam->c_str(), NULL);
	}
	return fDefault;
}

