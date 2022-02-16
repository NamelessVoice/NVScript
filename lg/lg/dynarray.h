#ifndef LG_DYNARRAY_H
#define LG_DYNARRAY_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdexcept>
#include <new>

#include <lg/config.h>

#ifdef __BORLANDC__
#pragma warn -inl
#endif

class cDynArrayBase
{
protected:
	void * m_data;
	unsigned long m_size;
	unsigned long m_allocsize;

	static void _resize(void **p, unsigned int sz, unsigned int c) SPEC_THROW(std::bad_alloc);

	~cDynArrayBase();
	cDynArrayBase()
		: m_data(NULL), m_size(0), m_allocsize(0) { }

public:
	void resynch()
		{ m_allocsize = m_size; }
};

/* LGS code uses an allocation scheme that I can't (don't want to) replicate.
 * This can cause problems if you've allocated less space than the internal 
 * code will be expecting.
 * To be safe, when passing an array to the engine that will be written to,
 * set m_size to 0. Then when getting it back, call ``resynch();''.
 */

template <typename _T>
class cDynArray : protected cDynArrayBase
{
private:
	typedef _T		type;
	typedef _T*		ptr;
	typedef _T&		ref;
	typedef const _T*		c_ptr;
	typedef const _T&		c_ref;

public:
	cDynArray()
		: cDynArrayBase() { }
	cDynArray(unsigned int _i);

	cDynArray(const cDynArray<_T>& _cpy);
	cDynArray<_T>& operator=(const cDynArray<_T>& _cpy);

	unsigned long size() const
		{ return m_size; }
	ref operator[](unsigned int _n) SPEC_THROW(std::out_of_range);
	c_ref operator[](unsigned int _n) const SPEC_THROW(std::out_of_range);

	ptr begin() NO_THROW
		{ return reinterpret_cast<ptr>(m_data); }
	c_ptr begin() const NO_THROW
		{ return reinterpret_cast<ptr>(m_data); }
	ptr end() NO_THROW
		{ return reinterpret_cast<ptr>(m_data) + m_size; }
	c_ptr end() const NO_THROW
		{ return reinterpret_cast<ptr>(m_data) + m_size; }

	void resize(unsigned int _i);
	void clear()
		{ resize(0); }

	unsigned int append(c_ref _e);
	unsigned int insert(c_ref _e, unsigned int _n);
	void erase(unsigned int _n) SPEC_THROW(std::out_of_range);
};

#include <lg/dynarray.hpp>

#ifdef __BORLANDC__
#pragma warn .inl
#endif

#endif
