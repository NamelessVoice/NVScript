#include <cctype>

using namespace std;

int CalcTextTime(const char* pszText, int iWordTime)
{
	register const char *p = pszText;
	register const char *q = p;
	register int c = 0, e = 0;
	while (*p)
	{
		if (isspace((int)(unsigned int)(unsigned char)*p))
		{
			if ((p - q) + e > 3)
			{
				++c;
				e = 0;
			}
			else
				++e;
			while (isspace((int)(unsigned int)(unsigned char)*++p)) ;
			q = p;
			continue;
		}
		++p;
	}
	if ((p - q) + e > 3)
		++c;
	//if (c < ((p-pszText)/8))
	//	c = (p-pszText)/8;
	return iWordTime * ((c < 10) ? 10 : c);
}

