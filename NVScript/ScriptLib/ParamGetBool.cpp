#include "ParamCache.h"
#include "ScriptLib.h"
#include <lg/scrservices.h>

#include <cstdlib>

using namespace std;

extern cParamCache* g_pGlobalParamCache;

bool ParamGetBool(int iObject, const char* pszParam, bool bDefault)
{
	if (!g_pGlobalParamCache)
		return bDefault;
	const string* psParam = g_pGlobalParamCache->Retrieve(iObject, pszParam);
	if (!psParam)
		return bDefault;
	if (psParam->size() == 0)
		return false;
	switch (psParam->at(0))
	{
	case '1': case 't': case 'T': case 'y': case 'Y':
		return true;
	case '$':
	{
		const char* psz = psParam->c_str()+1;
		SService<IQuestSrv> pQS(g_pScriptManager);
		if (pQS->Exists(psz))
			return pQS->Get(psz);
		break;
	}
	default:
		return strtol(psParam->c_str(), NULL, 0);
	}
	return bDefault;
}

