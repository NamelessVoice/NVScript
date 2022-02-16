#include "ScriptLib.h"

#include <cstring>
#include <cctype>
#include <cstdio>

using namespace std;

extern const char* g_pszParamDelim;
extern const char* g_pszQuotes;

char* SetParamString(char* pszString, const char* pszParam, const char* pszVal)
{
	if (!pszString || !pszParam || !pszVal)
		return NULL;
	int iLen = 4 + strlen(pszString) + strlen(pszParam) + strlen(pszVal);
	char* pszNew = reinterpret_cast<char*>(g_pMalloc->Alloc(iLen + 1));
	if (!pszNew)
		return NULL;
	sprintf(pszNew, "%s=\"%s\";", pszParam, pszVal);
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
			strcat(pszNew, pszToken);
			if (pszStart)
			{
				strcat(pszNew, "=");
				strcat(pszNew, pszStart);
			}
			strcat(pszNew, g_pszParamDelim);
		}
	}
	return pszNew;
}

