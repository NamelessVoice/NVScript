#include <cstdlib>

using namespace std;

int strtotime(const char* str)
{
	char* end = NULL;
	int t = strtoul(str, &end, 10);
	if (t > 0 && end)
	{
		if ((*end | 0x20) == 'm')
			t *= 60000;
		else if ((*end | 0x20) == 's')
			t *= 1000;
	}
	return t;
}

