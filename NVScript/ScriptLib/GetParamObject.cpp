#include "ScriptLib.h"

#include <cstring>
#include <cctype>
#include <cstdlib>

extern const char* g_pszParamDelim;
extern const char* g_pszQuotes;

int GetParamObject(const char* pszString, const char* pszParam)
{
	if (!pszString || !pszParam)
		return 0;
	char* pszRet = NULL;
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
			char* pszEnd;
			if (*pszStart == '\"')
			{
				++pszStart;
				pszEnd = strrchr(pszStart, '\"');
				if (!pszEnd)
					pszEnd = pszStart + strlen(pszStart);
			}
			else if (*pszStart == '\'')
			{
				++pszStart;
				pszEnd = strrchr(pszStart, '\'');
				if (!pszEnd)
					pszEnd = pszStart + strlen(pszStart);
			}
			else
				pszEnd = pszStart + strlen(pszStart);
			int iLen = pszEnd - pszStart;
			pszRet = pszStart;
			pszRet[iLen] = '\0';
			break;
		}
	}
	int iRet = StrToObject(pszRet);
	return iRet;
}

