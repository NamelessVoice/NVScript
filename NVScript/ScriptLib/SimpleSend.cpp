#include "ScriptLib.h"

void SimpleSend(int iSource, int iDest, const char* pszMessage, const cMultiParm& mpData1, const cMultiParm& mpData2, const cMultiParm& mpData3)
{
	cMultiParm mpReply;
	g_pScriptManager->SendMessage2(mpReply, iSource, iDest, pszMessage, mpData1, mpData2, mpData3);
}

