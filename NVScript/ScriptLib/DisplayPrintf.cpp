#include "ScriptLib.h"

#include <cstdio>
#include <cstdarg>

using namespace std;

void DisplayPrintf(const char* pszFormat, ...)
{
	va_list va;
	char psz[1000];
	va_start(va, pszFormat);
	_vsnprintf(psz, 1000, pszFormat, va);
	va_end(va);

	ShowString(psz, CalcTextTime(psz, 500));
}

void DisplayPrintf(COLORREF iColour, const char* pszFormat, ...)
{
	va_list va;
	char psz[1000];
	va_start(va, pszFormat);
	_vsnprintf(psz, 1000, pszFormat, va);
	va_end(va);

	ShowString(psz, CalcTextTime(psz, 500), iColour);
}
