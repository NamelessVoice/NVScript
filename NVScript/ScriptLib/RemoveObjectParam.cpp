#include "ScriptLib.h"

#include <cstring>
#include <cctype>

using namespace std;

extern const char* g_pszParamDelim;
extern const char* g_pszQuotes;

int RemoveObjectParam(int iObject, const char* pszParam)
{
	char* pszString = GetObjectParams(iObject);
	if (!pszString)
		return 0;
#ifndef _MSC_VER
	char szNew[strlen(pszString) + 1];
#else
	char *szNew = (char*)alloca(strlen(pszString) + 1);
#endif
	*szNew = '\0';
	char* pszSep, *pszToken;
	for (pszSep = pszString, pszToken = strqsep(&pszSep, g_pszParamDelim, g_pszQuotes); 
	     pszToken; pszToken = strqsep(&pszSep, g_pszParamDelim, g_pszQuotes))
	{
		while (isspace(*pszToken)) ++pszToken;
		if (!*pszToken)
			continue;
		char* pszStart = strchr(pszToken, '=');
		if (pszStart)
			*pszStart++ = '\0';
		if (stricmp(pszToken, pszParam))
		{
			strcat(szNew, pszToken);
			if (pszStart)
			{
				strcat(szNew, "=");
				strcat(szNew, pszStart);
			}
			strcat(szNew, g_pszParamDelim);
		}
	}
	SetObjectParams(iObject, szNew);
	g_pMalloc->Free(pszString);
	return 0;
}

