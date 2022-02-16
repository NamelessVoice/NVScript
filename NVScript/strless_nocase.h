#include <functional>
#include <string>
#include <cstring>

struct stringless_nocase : public std::binary_function<const std::string&,const std::string&,bool>
{
	bool operator()(const std::string& __x, const std::string& __y) const
		{ return ::stricmp(__x.c_str(),__y.c_str()) < 0; }
};

struct strless_nocase : public std::binary_function<const char*,const char*,bool>
{
	bool operator()(const char* __x, const char* __y) const
		{ return ::stricmp(__x,__y) < 0; }
};
