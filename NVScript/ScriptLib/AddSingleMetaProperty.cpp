#include "ScriptLib.h"

static int DoAddMetaProperty(IObjectSrv* pOS, object oMetaProp, object oObject)
{
	int iRet = 0;
	true_bool has_mp;
	pOS->HasMetaProperty(has_mp, oObject, oMetaProp);
	if (!has_mp)
	{
		iRet = pOS->AddMetaProperty(oObject, oMetaProp);
	}
	return iRet;
}

int AddSingleMetaProperty(const char* pszMetaProp, int iObjId)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	object oMetaProp;
	pOS->Named(oMetaProp, pszMetaProp);
	if (oMetaProp != 0)
		return DoAddMetaProperty(pOS, oMetaProp, iObjId);
	return 0;
}

int AddSingleMetaProperty(int iMetaProp, int iObjId)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	return DoAddMetaProperty(pOS, iMetaProp, iObjId);
}

