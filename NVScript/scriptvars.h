
#ifndef SCRIPTVARS_H
#define SCRIPTVARS_H

#include "lg/types.h"
#include "lg/scrmanagers.h"

extern IScriptMan* g_pScriptManager;
extern IMalloc* g_pMalloc;


#define SCRIPT_VAR(className,varName) varName(#className, #varName)
#define SCRIPT_VAROBJ(varName) varName(m_szName, #varName, iHostObjId)


class script_var
{
protected:
	bool			m_bDynName;
	sScrDatumTag	m_tag;

public:
	~script_var()
	{
		if (m_bDynName)
			delete[] m_tag.pszName;
	}
	script_var()
	{
		m_bDynName = false;
	}
	script_var(const char* pszScriptName, const char* pszVarName)
	{
		m_bDynName = false;
		m_tag.pszClass = pszScriptName;
		m_tag.pszName = pszVarName;
	}
	script_var(const char* pszScriptName, const char* pszVarName, int iObj)
	{
		m_bDynName = false;
		m_tag.pszClass = pszScriptName;
		m_tag.pszName = pszVarName;
		m_tag.objId = iObj;
	}
	script_var(const script_var& rCpy)
		: m_bDynName(false), m_tag(rCpy.m_tag)
	{
		if (rCpy.m_bDynName)
			SetDynTag(rCpy.m_tag.pszName);
	}

	script_var& operator= (const script_var& rCpy)
	{
		if (m_bDynName)
		{
			delete[] m_tag.pszName;
			m_bDynName = false;
		}
		m_tag = rCpy.m_tag;
		if (rCpy.m_bDynName)
		{
			SetDynTag(rCpy.m_tag.pszName);
		}
		return *this;
	}

	void SetObj(int iObj)
	{
		m_tag.objId = iObj;
	}

	void SetDynTag(const char* pszDynName);
	void Clear();
	bool Valid() const
	{
		return (g_pScriptManager->IsScriptDataSet(&m_tag) != 0);
	}
};


class script_int : public script_var
{
private:
	int m_iVal;

	void _ReSynch();
	void _Synch();

public:
	script_int() : script_var(), m_iVal(0)
	{ }
	script_int(const char* pszScriptName, const char* pszVarName)
		: script_var(pszScriptName,pszVarName), m_iVal(0)
	{ }
	script_int(const char* pszScriptName, const char* pszVarName, int iObj)
		: script_var(pszScriptName,pszVarName,iObj), m_iVal(0)
	{ }
	script_int(const script_int& rCpy) : script_var(rCpy), m_iVal(rCpy.m_iVal)
	{ }

	void Init()
	{
		Init(m_iVal);
	}
	void Init(int iVal)
	{
		if (Valid())
			_ReSynch();
		else
			*this = iVal;
	}
	void Clear()
	{
		script_var::Clear();
		m_iVal = 0;
	}

	operator int() const
	{
		return m_iVal;
	}
	script_int& operator= (int iVal)
	{
		m_iVal = iVal;
		_Synch();
		return *this;
	}
	script_int& operator+= (int iVal)
	{
		return *this = m_iVal + iVal;
	}
	script_int& operator-= (int iVal)
	{
		return *this = m_iVal - iVal;
	}
	script_int& operator++ ()
	{
		return *this = m_iVal + 1;
	}
	script_int operator++ (int iPF)
	{
		script_int __tmp(*this);
		*this = m_iVal + 1;
		return __tmp;
	}
	script_int& operator-- ()
	{
		return *this = m_iVal - 1;
	}
	script_int operator-- (int iPF)
	{
		script_int __tmp(*this);
		*this = m_iVal - 1;
		return __tmp;
	}

	script_int& operator= (const script_int& rCpy)
	{
		script_var::operator=(rCpy);
		m_iVal = rCpy.m_iVal;
		return *this;
	}
};


class script_float : public script_var
{
private:
	float m_fVal;

	void _ReSynch();
	void _Synch();

public:
	script_float() : script_var(), m_fVal(0.0f)
	{ }
	script_float(const char* pszScriptName, const char* pszVarName)
		: script_var(pszScriptName,pszVarName), m_fVal(0.0f)
	{ }
	script_float(const char* pszScriptName, const char* pszVarName, int iObj)
		: script_var(pszScriptName,pszVarName,iObj), m_fVal(0.0f)
	{ }
	script_float(const script_float& rCpy) : script_var(rCpy), m_fVal(rCpy.m_fVal)
	{ }

	void Init()
	{
		Init(m_fVal);
	}
	void Init(float fVal)
	{
		if (Valid())
			_ReSynch();
		else
			*this = fVal;
	}
	void Clear()
	{
		script_var::Clear();
		m_fVal = 0.0f;
	}

	operator float() const
	{
		return m_fVal;
	}
	script_float& operator= (float fVal)
	{
		m_fVal = fVal;
		_Synch();
		return *this;
	}
	script_float& operator+= (float fVal)
	{
		return *this = m_fVal + fVal;
	}
	script_float& operator-= (float fVal)
	{
		return *this = m_fVal - fVal;
	}
	script_float& operator++ ()
	{
		return *this = m_fVal + 1;
	}
	script_float operator++ (int iPF)
	{
		script_float __tmp(*this);
		*this = m_fVal + 1;
		return __tmp;
	}
	script_float& operator-- ()
	{
		return *this = m_fVal - 1;
	}
	script_float operator-- (int iPF)
	{
		script_float __tmp(*this);
		*this = m_fVal - 1;
		return __tmp;
	}

	script_float& operator= (const script_float& rCpy)
	{
		script_var::operator=(rCpy);
		m_fVal = rCpy.m_fVal;
		return *this;
	}
};


class script_str : public script_var
{
	cScrStr m_szVal;

	void _ReSynch();
	void _Synch(const char* pszVal);

public:
	~script_str()
	{
		m_szVal.Free();
	}
	script_str() : script_var(), m_szVal()
	{ }
	script_str(const char* pszScriptName, const char* pszVarName)
		: script_var(pszScriptName,pszVarName), m_szVal()
	{ }
	script_str(const char* pszScriptName, const char* pszVarName, int iObj)
		: script_var(pszScriptName,pszVarName,iObj), m_szVal()
	{ }
	script_str(const script_str& rCpy) : script_var(rCpy), m_szVal()
	{ }

	void Init(const char* pszVal = NULL)
	{
		if (!Valid())
			*this = pszVal;
	}

	operator const char*()
	{
		_ReSynch();
		return m_szVal;
	}

	script_str& operator= (const char* pszVal)
	{
		_Synch(pszVal);
		return *this;
	}
};


class script_vec : public script_var
{
	cScrVec m_vVal;

	void _ReSynch();
	void _Synch();

public:
	script_vec() : script_var(), m_vVal()
	{ }
	script_vec(const char* pszScriptName, const char* pszVarName)
		: script_var(pszScriptName,pszVarName), m_vVal()
	{ }
	script_vec(const char* pszScriptName, const char* pszVarName, int iObj)
		: script_var(pszScriptName,pszVarName,iObj), m_vVal()
	{ }
	script_vec(const script_vec& rCpy) : script_var(rCpy), m_vVal(rCpy.m_vVal)
	{ }

	void Init(const mxs_vector* pvVal = NULL)
	{
		if (Valid())
			_ReSynch();
		else
			*this = pvVal;
	}

	operator const mxs_vector*()
	{
		return &m_vVal;
	}

	script_vec& operator= (const mxs_vector* pvVal)
	{
		cScrVec v;
		if (pvVal)
			m_vVal = *pvVal;
		else
		{
			m_vVal.x = 0.0f; m_vVal.y = 0.0f; m_vVal.z = 0.0f;
		}
		_Synch();
		return *this;
	}

	script_vec& operator= (const script_vec& rCpy)
	{
		script_var::operator=(rCpy);
		m_vVal = rCpy.m_vVal;
		return *this;
	}

	void Clear()
	{
		script_var::Clear();
		m_vVal.x = 0.0f; m_vVal.y = 0.0f; m_vVal.z = 0.0f;
	}
};


template<typename _Type>
class script_handle : public script_var
{
public:
	script_handle() : script_var()
	{ }
	script_handle(const char* pszScriptName, const char* pszVarName)
		: script_var(pszScriptName,pszVarName)
	{ }
	script_handle(const char* pszScriptName, const char* pszVarName, int iObj)
		: script_var(pszScriptName,pszVarName,iObj)
	{ }
	script_handle(const script_handle& rCpy) : script_var(rCpy)
	{ }

	// [Telliamed] gcc is being problematic with type casting.
	// so I figure that we want to initialize handles to NULL anyway
	void Init()
	{
		if (!Valid())
		{
			sMultiParm param;
			param.type = kMT_Int;
			param.i = 0;
			g_pScriptManager->SetScriptData(&m_tag, &param);
		}
	}

	operator _Type()
	{
		sMultiParm param;
		param.type = kMT_Undef;
		g_pScriptManager->GetScriptData(&m_tag, &param);
		return reinterpret_cast<_Type>(param.i);
	}
	script_handle<_Type>& operator= (_Type hVal)
	{
		sMultiParm param;
		param.type = kMT_Int;
		param.i = reinterpret_cast<int>(hVal);
		g_pScriptManager->SetScriptData(&m_tag, &param);
		return *this;
	}
};


#endif // SCRIPTVARS_H
