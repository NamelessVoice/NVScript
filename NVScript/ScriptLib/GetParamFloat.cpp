#include "ScriptLib.h"

#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

extern const char* g_pszParamDelim;
extern const char* g_pszQuotes;

float GetParamFloat(const char* pszString, const char* pszParam)
{
	if (!pszString || !pszParam)
		return 0;
	float fRet = 0;
#ifndef _MSC_VER
	char szTemp[strlen(pszString) + 1];
#else
	char *szTemp = (char*)alloca(strlen(pszString) + 1);
#endif
	strcpy(szTemp, pszString);
	char* pszSep, *pszToken;
	for (pszSep = szTemp, pszToken = strqsep(&pszSep, g_pszParamDelim, g_pszQuotes); 
	     pszToken; pszToken = strqsep(&pszSep, g_pszParamDelim, g_pszQuotes))
	{
		while (isspace(*pszToken)) ++pszToken;
		char* pszStart = strchr(pszToken, '=');
		if (pszStart)
			*pszStart++ = '\0';
		if (!stricmp(pszToken, pszParam))
		{
			if (!pszStart)
				break;
			while (isspace(*pszStart)) ++pszStart;
			if (*pszStart == '\"' || *pszStart == '\'')
				++pszStart;
			fRet = strtod(pszStart, NULL);
			break;
		}
	}
	return fRet;
}

