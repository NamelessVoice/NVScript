#include <cstring>

using namespace std;

/* Seperate a string on any of delim characters while preserving 
 * substrings surrounded by an equal pair of any of quotes characters.
 * str is the address of a pointer to the string to be seperated.
 * The pointer to the next token in the string, or NULL, is copied to 
 * the address and the the value that was passed in *str is returned.
 * delim and quotes cannot be NULL.
 * Empty tokens will be returned.
 * Like strtok and strsep, the string data will be modified.
 * str, of course, must be an lvalue.
 */
char* strqsep(char** str, const char* delim, const char* quotes)
{
	char dtable[256];
	char qtable[256];
	register char* p = *str;
	if (!p) return NULL;
	memset(dtable,0,sizeof(dtable));
	memset(qtable,0,sizeof(dtable));
	{
		register const unsigned char* d = reinterpret_cast<const unsigned char*>(delim);
		while (*d)
			dtable[*d++] = 0xFF;
	}
	{
		register const unsigned char* q = reinterpret_cast<const unsigned char*>(quotes);
		while (*q)
			qtable[*q++] = 0xFF;
	}
	while (*p)
	{
		if (qtable[*reinterpret_cast<unsigned char*>(p)])
		{
			register char* t = p;
			do
			{
				t = strchr(t+1, *p);
				if (!t)
				{
					p = *str;
					*str = NULL;
					return p;
				}
			}
			while (*(t-1) == '\\');
			p = t;
		}
		else if (dtable[*reinterpret_cast<unsigned char*>(p)])
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

