#ifndef SCRINSTANCE_H
#define SCRINSTANCE_H 1

#include <new>
#include <utility>

struct ScrInstance : public std::pair<const char *,int>
{
	~ScrInstance();
	ScrInstance() : std::pair<const char*,int>(NULL,0) { }
	ScrInstance(const char* __a, int __b);
	ScrInstance(const ScrInstance& __p);
	ScrInstance(const std::pair<const char*,int>& __p);

	bool operator==(const ScrInstance& __x) const
		{ return compare(__x) == 0; }
	bool operator==(const std::pair<const char*,int>& __x) const
		{ return compare(__x) == 0; }
	bool operator!=(const ScrInstance& __x) const
		{ return compare(__x) != 0; }
	bool operator!=(const std::pair<const char*,int>& __x) const
		{ return compare(__x) != 0; }
	bool operator<(const ScrInstance& __x) const
		{ return compare(__x) < 0; }
	bool operator<(const std::pair<const char*,int>& __x) const
		{ return compare(__x) < 0; }
	bool operator>(const ScrInstance& __x) const
		{ return compare(__x) > 0; }
	bool operator>(const std::pair<const char*,int>& __x) const
		{ return compare(__x) > 0; }
	bool operator<=(const ScrInstance& __x) const
		{ return compare(__x) <= 0; }
	bool operator<=(const std::pair<const char*,int>& __x) const
		{ return compare(__x) <= 0; }
	bool operator>=(const ScrInstance& __x) const
		{ return compare(__x) >= 0; }
	bool operator>=(const std::pair<const char*,int>& __x) const
		{ return compare(__x) >= 0; }

	int compare(const std::pair<const char*,int>& __x) const;
};

#endif //SCRINSTANCE_H
