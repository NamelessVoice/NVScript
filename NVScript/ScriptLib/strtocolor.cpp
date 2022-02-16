#include "ScriptLib.h"

#include <cstring>
#include <cstdio>

using namespace std;

typedef struct colorname_
{
	const char* n;
	unsigned long c;
} colorname;
static const colorname g_sColorNames[] = {
	{ "black",   0x000000 },
	{ "silver",  0xC0C0C0 },
	{ "gray",    0x808080 },
	{ "grey",    0x808080 },
	{ "white",   0xFFFFFF },
	{ "maroon",  0x000080 },
	{ "red",     0x0000FF },
	{ "purple",  0x800080 },
	{ "fuchsia", 0xFF00FF },
	{ "green",   0x008000 },
	{ "lime",    0x00FF00 },
	{ "olive",   0x008080 },
	{ "yellow",  0x00FFFF },
	{ "navy",    0x800000 },
	{ "blue",    0xFF0000 },
	{ "teal",    0x808000 },
	{ "aqua",    0xFFFF00 }
};

int strtocolor(const char* str)
{
	register char* c = const_cast<char*>(str);
	if (*c == '#')
	{
		if (strlen(++c) < 6)
			return 0;
		unsigned char r,g,b;
		r = hexbyte(c);
		c += 2;
		g = hexbyte(c);
		c += 2;
		b = hexbyte(c);
		return (r|(g<<8)|(b<<16));
	}
	if (strchr(str, ','))
	{
		unsigned int r,g,b;
		if (3 == sscanf(str, "%u , %u , %u", &r, &g, &b))
		{
			return ((r&0xFF)|((g&0xFF)<<8)|((b&0xFF)<<16));
		}
	}
	else
	{
		for (unsigned int n=0; n < sizeof(g_sColorNames)/sizeof(g_sColorNames[0]); ++n)
		{
			if (!stricmp(str,g_sColorNames[n].n))
				return g_sColorNames[n].c;
		}
	}
	return 0;
}

