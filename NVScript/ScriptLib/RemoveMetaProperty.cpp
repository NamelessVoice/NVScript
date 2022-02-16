#include "ScriptLib.h"

int RemoveMetaProperty(const char* pszMetaProp, int iObjId)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oMetaProp;
	pOS->Named(oMetaProp, pszMetaProp);
	if (oMetaProp != 0)
		return pOS->RemoveMetaProperty(iObjId, oMetaProp);
	return 0;
}

