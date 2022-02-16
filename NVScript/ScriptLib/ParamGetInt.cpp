#include "ParamCache.h"
#include "ScriptLib.h"
#include <lg/scrservices.h>

#include <cstdlib>

using namespace std;

extern cParamCache* g_pGlobalParamCache;

int ParamGetInt(int iObject, const char* pszParam, int iDefault)
{
	if (!g_pGlobalParamCache)
		return iDefault;
	const string* psParam = g_pGlobalParamCache->Retrieve(iObject, pszParam);
	if (!psParam)
		return iDefault;
	if (psParam->size() == 0)
		return 0;
	if (psParam->at(0) == '$')
	{
		const char* psz = psParam->c_str()+1;
		SService<IQuestSrv> pQS(g_pScriptManager);
		if (pQS->Exists(psz))
			return pQS->Get(psz);
	}
	else
	{
		return strtol(psParam->c_str(), NULL, 0);
	}
	return iDefault;
}

