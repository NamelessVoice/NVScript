#include "ScriptLib.h"
#include "ParamCache.h"

#ifdef DEBUG_PARAMCACHE
extern int (__cdecl *g_pfnMPrintf)(const char*,...);
#endif

cParamCache* g_pGlobalParamCache = NULL;

void InitParamCache(void)
{
	delete g_pGlobalParamCache;
	g_pGlobalParamCache = new cParamCache(g_pScriptManager);
#ifdef DEBUG_PARAMCACHE
	g_pGlobalParamCache->SetDebug(g_pfnMPrintf);
#endif
}

void ReleaseParamCache(void)
{
	delete g_pGlobalParamCache;
	g_pGlobalParamCache = NULL;
}

