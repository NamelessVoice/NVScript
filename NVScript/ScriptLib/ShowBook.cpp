#include "ScriptLib.h"

#include <cstring>

using namespace std;

int ShowBook(int iObject, bool bArt, int iTime)
{
	SService<IPropertySrv> pPS(g_pScriptManager);
	if (!pPS->Possessed(iObject, "Book"))
		return 1;
	cMultiParm mpBook, mpBookArt;
	pPS->Get(mpBook, iObject, "Book", NULL);
	if (!*mpBook.psz)
	{
		return 1;
	}
	SService<IDarkUISrv> pUI(g_pScriptManager);
	if (bArt && pPS->Possessed(iObject, "BookArt"))
	{
		pPS->Get(mpBookArt, iObject, "BookArt", NULL);
		pUI->ReadBook(mpBook, mpBookArt);
	}
	else
	{
		SService<IDataSrv> pDS(g_pScriptManager);
#ifndef _MSC_VER
		char szBookFile[strlen(mpBook) + 10];
#else
		char *szBookFile = (char*)alloca(strlen(mpBook) + 10);
#endif
		strcpy(szBookFile, "..\\books\\");
		strcat(szBookFile, mpBook);
		cScrStr str;
		pDS->GetString(str, szBookFile, "page_0", "", "strings");
		if (*static_cast<const char*>(str))
		{
			if (iTime == 0)
				iTime = CalcTextTime(str, 500);
			pUI->TextMessage(str, 0, iTime);
		}
		str.Free();
	}
	return 0;
}

