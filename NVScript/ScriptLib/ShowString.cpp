#include "ScriptLib.h"

void ShowString(const char* pszStr, int iTime, COLORREF color)
{
#if (_DARKGAME == 3)
	SService<IShockGameSrv> pShock(g_pScriptManager);
	pShock->AddText(pszStr, 0, iTime);
	color = color;
#else
	SService<IDarkUISrv> pUI(g_pScriptManager);
	pUI->TextMessage(pszStr, color, iTime);
#endif
	return;
}

