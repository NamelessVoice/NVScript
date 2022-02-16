#include "ScrInstance.h"
#include <cstring>

inline char* _alloc(size_t __n)
{
	size_t* __r = reinterpret_cast<size_t*>(::operator new(__n + sizeof(size_t)));
	*__r = 1;
	return reinterpret_cast<char*>(__r + 1);
}

inline const char* _copy(const char* __s)
{
	char* __x = _alloc(::strlen(__s)+1);
	::strcpy(__x, __s);
	return __x;
}

inline size_t* _ref(const char* __s)
{
	return reinterpret_cast<size_t*>(const_cast<char*>(__s)) - 1;
}

ScrInstance::~ScrInstance()
{
	if (first)
	{
		size_t* __r = _ref(first);
		if (--*__r == 0)
			::operator delete(__r);
	}
}

ScrInstance::ScrInstance(const char* __a, int __b)
	: std::pair<const char*,int>(NULL,__b)
{
	if (__a)
	{
		first = _copy(__a);
	}
}

ScrInstance::ScrInstance(const ScrInstance& __p)
	: std::pair<const char*,int>(__p.first,__p.second)
{
	if (__p.first)
	{
		size_t* __r = _ref(__p.first);
		*__r += 1;
	}
}

ScrInstance::ScrInstance(const std::pair<const char*,int>& __p)
	: std::pair<const char*,int>(NULL,__p.second)
{
	if (__p.first)
	{
		first = _copy(__p.first);
	}
}

int ScrInstance::compare(const std::pair<const char*,int>& __x) const
{
	if (first == __x.first)
		return second - __x.second;
	int __c = ::stricmp(first, __x.first);
	return (__c != 0) ? __c : second - __x.second;
}

