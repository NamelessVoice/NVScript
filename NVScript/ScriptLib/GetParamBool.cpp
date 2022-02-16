#include "ScriptLib.h"

#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

extern const char* g_pszParamDelim;
extern const char* g_pszQuotes;

bool GetParamBool(const char* pszString, const char* pszParam)
{
	if (!pszString || !pszParam)
		return 0;
	bool bRet = false;
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
			char* pszEnd = pszStart + strlen(pszStart);
			if (*pszStart == '\"' || *pszStart == '\'')
			{
				if (*(pszEnd-1) == *pszStart)
					*(--pszEnd) = '\0';
				++pszStart;
			}
			switch (*pszStart)
			{
			case '1': case 't': case 'T': case 'y': case 'Y':
				bRet = true;
				break;
			case '$':
			{
				++pszStart;
				if (pszEnd > pszStart)
				{
					SService<IQuestSrv> pQS(g_pScriptManager);
					if (pQS->Exists(pszStart))
						bRet = pQS->Get(pszStart);
				}
				break;
			}
			default:
				bRet = strtol(pszStart, NULL, 0);
				break;
			}
			break;
		}
	}
	return bRet;
}

