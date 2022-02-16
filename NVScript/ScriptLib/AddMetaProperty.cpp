#include "ScriptLib.h"

int AddMetaProperty(const char* pszMetaProp, int iObjId)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oMetaProp;
	pOS->Named(oMetaProp, pszMetaProp);
	if (oMetaProp != 0)
		return pOS->AddMetaProperty(iObjId, oMetaProp);
	return 0;
}

