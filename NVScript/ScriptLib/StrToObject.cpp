#include "ScriptLib.h"

#include <cstdlib>

using namespace std;

int StrToObject(const char* pszName)
{
	if (!pszName || !*pszName)
		return 0;
	SService<IObjectSrv> pOS(g_pScriptManager);
	object obj = 0;
	pOS->Named(obj, pszName);
	if (obj == 0)
	{
		char *p = NULL;
		obj = strtol(pszName, &p, 10);
		//if (*p != '\0')
		//	return 0;
		true_bool _p;
		if (! *pOS->Exists(_p, obj))
			return 0;
	}
	return obj;
}

