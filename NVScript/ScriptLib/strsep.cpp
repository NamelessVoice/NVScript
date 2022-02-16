#include <cstring>

using namespace std;

char* strsep(char** str, const char* delim)
{
	char dtable[256];
	register char* p = *str;
	if (!p) return NULL;
	memset(dtable,0,sizeof(dtable));
	{
		register const unsigned char* d = reinterpret_cast<const unsigned char*>(delim);
		while (*d)
			dtable[*d++] = 0xFF;
	}
	while (*p)
	{
		if (dtable[*reinterpret_cast<unsigned char*>(p)])
		{
			register char* t = *str;
			*p++ = '\0';
			*str = p;
			return t;
		}
		++p;
	}
	p = *str;
	*str = NULL;
	return p;
}

