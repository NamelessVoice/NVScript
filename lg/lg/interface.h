/************************************************
 * LGS Auto-Release wrapper for Script interface
 */

#ifndef _LG_INTERFACE_H
#define _LG_INTERFACE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <exception>
#include <cstdio>

#include <lg/config.h>
#include <lg/iiddef.h>
#include <lg/scrmanagers.h>
//#include <lg/scrservices.h>

#ifdef __BORLANDC__
#pragma warn -inl
#endif

extern  const GUID  IID_INullScriptService;

class null_pointer : public std::exception
{
public:
	null_pointer() NO_THROW { }
	virtual ~null_pointer() NO_THROW { }

	virtual const char* what() const NO_THROW
	{ return "null pointer"; }
};

class no_interface : public std::exception
{
	const char* ifname;
	char* buf;
public:
	no_interface() NO_THROW : ifname(NULL), buf(NULL) { }
	no_interface(const char* nm) : ifname(nm)
	{
		buf = new char[::strlen(nm) + 27];
		::sprintf(buf, "interface not available (%s)", nm);
	}
	virtual ~no_interface() NO_THROW
	{
		if (buf)
			delete[] buf;
	}

	virtual const char* what() const NO_THROW
	{
		return buf;
	}
};

/**************************************
 * Helper class for interface pointers.
 */
template<class _IFace, typename _IID = IID_Def<_IFace> >
class SInterface
{
	_IFace* m_pIFace;

public:
	~SInterface() NO_THROW
	{
		if (m_pIFace)
			m_pIFace->Release();
	}
	explicit SInterface(_IFace* __p = NULL) NO_THROW
		: m_pIFace(__p)
	{
		if (m_pIFace)
			m_pIFace->AddRef();
	}
	SInterface(SInterface& __x) NO_THROW
		: m_pIFace(__x.m_pIFace)
	{
		if (m_pIFace)
			m_pIFace->AddRef();
	}
	template<class _ExtIFace>
	SInterface(SInterface<_ExtIFace>& __x) NO_THROW
	{
		m_pIFace = static_cast<_IFace*>(__x.m_pIFace);
		if (m_pIFace)
			m_pIFace->AddRef();
	}
	SInterface(IUnknown* pSM) SPEC_THROW(no_interface)
		: m_pIFace(NULL)
	{
		if (E_NOINTERFACE == pSM->QueryInterface(_IID::iid(), reinterpret_cast<void**>(&m_pIFace)))
			throw no_interface(_IID::name());
	}

	SInterface& operator=(SInterface& __x) NO_THROW
	{
		if (m_pIFace)
			m_pIFace->Release();
		m_pIFace = __x.m_pIFace;
		if (m_pIFace)
			m_pIFace->AddRef();
		return *this;
	}
	SInterface& operator=(_IFace* __p) NO_THROW
	{
		if (m_pIFace)
			m_pIFace->Release();
		m_pIFace = __p;
		return *this;
	}
	SInterface& operator=(IUnknown* __p) SPEC_THROW(no_interface)
	{
		_IFace* pTemp;
		if (E_NOINTERFACE == __p->QueryInterface(_IID::iid(), reinterpret_cast<void**>(&pTemp)))
		{
			__p->Release();
			throw no_interface(_IID::name());
		}
		if (m_pIFace)
			m_pIFace->Release();
		m_pIFace = pTemp;
	}

	operator bool () const NO_THROW
	{
		return m_pIFace != NULL;
	}
	bool operator ! () const NO_THROW
	{
		return m_pIFace == NULL;
	}
	bool operator == (const SInterface& __x) const NO_THROW
	{
		return m_pIFace == __x.m_pIFace;
	}
	bool operator != (const SInterface& __x) const NO_THROW
	{
		return m_pIFace != __x.m_pIFace;
	}
	bool operator == (const IID* __iid) const NO_THROW
	{
		_IFace* pTemp;
		if (E_NOINTERFACE != m_pIFace->QueryInterface(__iid, reinterpret_cast<void**>(&pTemp)))
		{
			pTemp->Release();
			return true;
		}
		return false;
	}
	bool operator != (const IID* __iid) const NO_THROW
	{
		_IFace* pTemp;
		if (E_NOINTERFACE != m_pIFace->QueryInterface(__iid, reinterpret_cast<void**>(&pTemp)))
		{
			pTemp->Release();
			return false;
		}
		return true;
	}

	operator _IFace* () const NO_THROW
	{
		return m_pIFace;
	}

	_IFace* get() const NO_THROW
	{
		return m_pIFace;
	}
	_IFace& operator*() const NO_THROW
	{
		//if (!m_pIFace)
		//	throw null_pointer();
		return *m_pIFace;
	}
	_IFace* operator->() const NO_THROW
	{
		//if (!m_pIFace)
		//	throw null_pointer();
		return m_pIFace;
	}
};

/******************************************************
 * Helper class for script service interface pointers.
 */
template<class _IFace, typename _IID = IID_Def<_IFace> >
class SService
{
	_IFace* m_pIFace;

public:
	~SService() NO_THROW
	{
		if (m_pIFace)
			m_pIFace->Release();
	}
	explicit SService(_IFace* __p = NULL) NO_THROW
		: m_pIFace(__p)
	{
		if (m_pIFace)
			m_pIFace->AddRef();
	}
	SService(SService& __x) NO_THROW
		: m_pIFace(__x.m_pIFace)
	{
		if (m_pIFace)
			m_pIFace->AddRef();
	}
	template<class _ExtIFace>
	SService(SService<_ExtIFace>& __x) NO_THROW
	{
		m_pIFace = static_cast<_IFace*>(__x.m_pIFace);
		if (m_pIFace)
			m_pIFace->AddRef();
	}
	SService(IScriptMan* pSM) SPEC_THROW(no_interface)
	{
		m_pIFace = static_cast<_IFace*>(pSM->GetService(_IID::iid()));
		IUnknown* pTemp;
		if (E_NOINTERFACE != m_pIFace->QueryInterface(IID_INullScriptService, reinterpret_cast<void**>(&pTemp)))
		{
			pTemp->Release();
			throw no_interface(_IID::name());
		}
	}

	SService& operator=(SService& __x) NO_THROW
	{
		m_pIFace = __x.m_pIFace;
		if (m_pIFace)
			m_pIFace->AddRef();
		return *this;
	}
	SService& operator=(_IFace* __p) NO_THROW
	{
		m_pIFace = __p;
		return *this;
	}

	operator bool () const NO_THROW
	{
		return m_pIFace != NULL;
	}
	bool operator ! () const NO_THROW
	{
		return m_pIFace == NULL;
	}
	bool operator == (const SService& __x) const NO_THROW
	{
		return m_pIFace == __x.m_pIFace;
	}
	bool operator != (const SService& __x) const NO_THROW
	{
		return m_pIFace != __x.m_pIFace;
	}

	operator _IFace* () const NO_THROW
	{
		return m_pIFace;
	}

	_IFace* get() const NO_THROW
	{
		return m_pIFace;
	}
	_IFace& operator*() const NO_THROW
	{
		//if (!m_pIFace)
		//	throw null_pointer();
		return *m_pIFace;
	}
	_IFace* operator->() const NO_THROW
	{
		//if (!m_pIFace)
		//	throw null_pointer();
		return m_pIFace;
	}
};

#ifdef __BORLANDC__
#pragma warn .inl
#endif

#endif // _LG_INTERFACE_H
