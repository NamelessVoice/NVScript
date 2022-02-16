#define _SHOCKINTERFACES 1
#include "ScriptLib.h"

extern int (__cdecl *g_pfnMPrintf)(const char*, ...);

#include <cstdio>
#include <cstdarg>

using namespace std;

// vs[n]printf is pre-C99 in msvcrt6, 
// so we can't dynamically allocating a buffer.
// 1000 is a limit imposed by Dromed's mprint 
void DebugPrintf(const char* pszFormat, ...)
{
	va_list va;
	char psz[1000];
	va_start(va, pszFormat);
	int iLen = _vsnprintf(psz, 999, pszFormat, va);
	va_end(va);

	if (g_pfnMPrintf)
	{
		g_pfnMPrintf("%s\n", psz);
	}
	else
	{
		IDebugScrSrv* pDSS = static_cast<IDebugScrSrv*>(g_pScriptManager->GetService(IID_IDebugScriptService));
		if (!pDSS)
		{
			return;
		}
		cScrStr nil;
		pDSS->MPrint(psz, nil, nil, nil, nil, nil, nil, nil);
		pDSS->Release();
	}
#ifdef DEBUG
	//ShowString(psz, 1000 + (iLen * 50));
	try
	{
		// ShockAI should fail in Thief.
		SService<IShockAISrv> pShockAI(g_pScriptManager);
		// TDP retains a non-functional copy of ShockGame, 
		// so we can't use it alone to test the game type.
		SService<IShockGameSrv> pShock(g_pScriptManager);
		pShock->AddText(psz, 0, 1000 + (iLen * 50));
	}
	catch (no_interface&)
	{
		SService<IDarkUISrv> pUI(g_pScriptManager);
		pUI->TextMessage(psz, 0, 1000 + (iLen * 50));
	}
#else
	iLen = iLen;
#endif
}

