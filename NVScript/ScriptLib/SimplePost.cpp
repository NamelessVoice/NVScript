#include "ScriptLib.h"

void SimplePost(int iSource, int iDest, const char* pszMessage, const cMultiParm& mpData1, const cMultiParm& mpData2, const cMultiParm& mpData3)
{
#if (_DARKGAME != 1)
	g_pScriptManager->PostMessage2(iSource, iDest, pszMessage, mpData1, mpData2, mpData3, 0L);
#else
	g_pScriptManager->PostMessage2(iSource, iDest, pszMessage, mpData1, mpData2, mpData3);
#endif
}

