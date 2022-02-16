#include "ScriptLib.h"

static int DoRemoveMetaProperty(IObjectSrv* pOS, object oMetaProp, object oObject)
{
	int iRet = 0;
	true_bool has_mp;
	pOS->HasMetaProperty(has_mp, oObject, oMetaProp);
	if (has_mp)
	{
		iRet = pOS->RemoveMetaProperty(oObject, oMetaProp);
		//pOS->HasMetaProperty(has_mp, oObject, oMetaProp);
	}
	return iRet;
}

int RemoveSingleMetaProperty(const char* pszMetaProp, int iObjId)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oMetaProp;
	pOS->Named(oMetaProp, pszMetaProp);
	if (oMetaProp != 0)
		return DoRemoveMetaProperty(pOS, oMetaProp, iObjId);
	return 0;
}

int RemoveSingleMetaProperty(int iMetaProp, int iObjId)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	return DoRemoveMetaProperty(pOS, iMetaProp, iObjId);
}

