/**********************
 * LGS Types
 */

#ifndef _LG_TYPES_H
#define _LG_TYPES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/config.h>
#include <lg/objstd.h>

#pragma pack(1)


struct true_bool
{
	unsigned int f;

	true_bool() : f(0) { }
	true_bool(int v) { f = v; }
	
	bool operator== (int v) const
	{
		return (f) ? (v != 0) : (v == 0);
	}
	bool operator!= (int v) const
	{
		return (f) ? (v == 0) : (v != 0);
	}
	bool operator! () const
	{
		return f == 0;
	}

	true_bool& operator= (int v)
	{
		f = v;
		return *this;
	}
	
	operator int () const
	{
		return (f) ? 1 : 0;
	}
	operator bool () const
	{
		return f != 0;
	}
};

struct mxs_vector
{
	float x, y, z;
};

struct mxs_angvec
{
	float x, y, z;
};

class cScrVec : public mxs_vector
{
public:
	cScrVec()
		{ x=0; y=0; z=0; }
	cScrVec(float i, float j, float k)
		{ x=i; y=j; z=k; }
	cScrVec(double i, double j, double k)
		{ x=static_cast<float>(i); y=static_cast<float>(j); z=static_cast<float>(k); }
	cScrVec(int i, int j, int k)
		{ x=static_cast<float>(i); y=static_cast<float>(j); z=static_cast<float>(k); }
	cScrVec(const mxs_vector& rV)
		{ x=rV.x; y=rV.y; z=rV.z; }
	cScrVec& operator= (const mxs_vector& v)
	{
		x = v.x; y = v.y; z = v.z;
		return *this;
	}
	cScrVec operator+ (const mxs_vector& r) const
	{
		return cScrVec(x+r.x, y+r.y, z+r.z);
	}
	cScrVec& operator+= (const mxs_vector& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}
	cScrVec operator- (const mxs_vector& r) const
	{
		return cScrVec(x-r.x, y-r.y, z-r.z);
	}
	cScrVec& operator-= (const mxs_vector& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;
		return *this;
	}
	cScrVec operator- () const
	{
		return cScrVec(-x,-y,-z);
	}
	cScrVec operator* (const mxs_vector& r) const
	{
		return cScrVec(x*r.x, y*r.y, z*r.z);
	}
	cScrVec operator* (float g) const
	{
		return cScrVec(x*g, y*g, z*g);
	}
	cScrVec& operator*= (const mxs_vector& r)
	{
		x *= r.x;
		y *= r.y;
		z *= r.z;
		return *this;
	}
	cScrVec& operator*= (float g)
	{
		x *= g;
		y *= g;
		z *= g;
		return *this;
	}
	cScrVec operator/ (const mxs_vector& r) const
	{
		return cScrVec(x/r.x, y/r.y, z/r.z);
	}
	cScrVec operator/ (float g) const
	{
		return cScrVec(x/g, y/g, z/g);
	}
	cScrVec& operator/= (const mxs_vector& r)
	{
		x /= r.x;
		y /= r.y;
		z /= r.z;
		return *this;
	}
	cScrVec& operator/= (float g)
	{
		x /= g;
		y /= g;
		z /= g;
		return *this;
	}
	void Adjust(float g)
	{
		x += g;
		y += g;
		z += g;
	}
	cScrVec Normal() const;
	void Normalize();
	float Magnitude() const;
	float MagSquared() const;
	float Dot(const mxs_vector& r) const;
	cScrVec Cross(const mxs_vector& r) const;
	static const float epsilon;
	static const cScrVec Zero;
};


// A cScrStr should _never_ be NULL.
class cScrStr
{
	const char* m_pszData;

	static const char* _ChNil;
public:
	cScrStr () : m_pszData(_ChNil)
		{ }
	cScrStr (const char* psz) 
		{ m_pszData = IF_NOT(psz, _ChNil); }

	cScrStr& operator = (char* psz)
		{ m_pszData = IF_NOT(psz, _ChNil); return *this;}

	operator const char* () const
		{ return m_pszData; }

	// Normally, a cScrStr should be considered const
	// In a few cases, you may need to do this, however.
	// No checking is performed, so be absolutely sure when you call this.
	void Free();

	BOOL IsEmpty(void) const
		{ return (m_pszData == NULL) || (*m_pszData == 0); }

	static const cScrStr Null;
};

// A variant of cScrStr that can safely be used for script service functions
// that return strings in non-const string references.
// A cVarScrStr should _never_ be NULL.
class cVarScrStr
{
	char* m_pszData;
protected:
	void Copy(const char *psz);
public:
	cVarScrStr () : m_pszData(NULL)
		{ Copy(""); }
	cVarScrStr (const char* psz) 
		{ Copy(psz); }

	~cVarScrStr();

	cVarScrStr& operator = (const char* psz)
		{ Copy(psz); return *this;}

	operator const char* () const
		{ return m_pszData; }

	BOOL IsEmpty(void) const
		{ return (m_pszData == NULL) || (*m_pszData == 0); }
};

class cAnsiStr
{
	char*	m_pchData;
	int		m_nDataLength;
	int		m_nAllocLength;

	static const char* _ChNil;

public:
	~cAnsiStr();
	cAnsiStr();
	cAnsiStr(int);
	cAnsiStr(const char *);
	cAnsiStr(char);
	cAnsiStr(const char *, int);
	cAnsiStr(const cAnsiStr &);
	cAnsiStr(const cScrStr &);

	operator const char* () const
		{ return m_pchData; }

	cAnsiStr& operator= (const cAnsiStr& rCpy)
		{ Assign(rCpy.m_nDataLength, rCpy.m_pchData); return *this; }
	cAnsiStr& operator= (const char* psz);
	cAnsiStr& operator= (const cScrStr& rCpy)
		{ return *this = static_cast<const char*>(rCpy); }
	cAnsiStr& operator+= (const cAnsiStr& rCat)
		{ Append(rCat.m_nDataLength, rCat.m_pchData); return *this; }
	cAnsiStr& operator+= (const char* psz);
	cAnsiStr& operator+= (const cScrStr& rCat)
		{ return *this += static_cast<const char*>(rCat); }

	friend cAnsiStr operator+ (const cAnsiStr&, const cAnsiStr&);
	friend cAnsiStr operator+ (const cAnsiStr&, const char*);
	friend cAnsiStr operator+ (const char*, const cAnsiStr&);

	char* AllocStr(int);
	char* ReallocStr(char*, int);
	void FreeStr(char*);
	void AllocBuffer(int);

	void Attach(char*, int, int);
	char* Detach(void);
	void BufDone(int nLength, int nAlloc = -1);

	// Creates a new string with an allocated buffer of length + extra
	// and copies length characters from us beginning at start.
	void AllocCopy(cAnsiStr&, int length, int extra, int start) const;

	void Assign(int, const char *);
	void Append(int, const char *);
	void Append(char);
	void ConcatCopy(int, const char*, int, const char*);

	int Insert(const char *, int pos = 0);
	int Insert(char, int pos = 0);
	void Remove(int pos, int length);
	int Replace(const char *, int pos, int length);
	void Empty(void);
	void Trim(void);

	int GetLength(void) const
		{ return m_nDataLength; }
	BOOL IsEmpty(void) const
		{ return m_nDataLength == 0; }
	BOOL IsInitialEmpty(void) const
		{ return m_pchData == _ChNil; }

	char GetAt(int pos) const
		{ return (pos < m_nDataLength) ? m_pchData[pos] : '\0'; }
	char SetAt(int pos, char ch)
		{ return (pos < m_nDataLength) ? m_pchData[pos] = ch : '\0'; }
	char operator [] (int pos) const
		{ return m_pchData[pos]; }
	char operator [] (unsigned int pos) const
		{ return m_pchData[pos]; }

	int Compare(int, const char *) const;
	int cAnsiStr::Compare(const cAnsiStr& rStr) const
		{ return Compare(rStr.m_nDataLength, rStr.m_pchData); }
	int cAnsiStr::Compare(const char *pchData) const
		{ return Compare(strlen(pchData) + 1, pchData); }

	int Find(const char *, int start = 0) const;
	int Find(char, int start = 0) const;
	int ReverseFind(char) const;
	int FindOneOf(const char*, int start = 0) const;

	int SpanIncluding(const char*, int start = 0) const;
	int SpanExcluding(const char*, int start = 0) const;
	int ReverseIncluding(const char*, int start = -1) const;
	int ReverseExcluding(const char*, int start = -1) const;

	enum eQuoteMode
	{
		kOff,
		kDoubleQuotes,
		kEscapeQuotes,
		kQuoteIfWhite,
		kRemoveEmbeddedQuotes
	};
	
	cAnsiStr& Quoted(eQuoteMode);
	void FmtStr(unsigned int, const char*, ...);
	void FmtStr(const char*, ...);
};

cAnsiStr operator+ (const cAnsiStr&, const cAnsiStr&);
cAnsiStr operator+ (const cAnsiStr&, const char*);
cAnsiStr operator+ (const char*, const cAnsiStr&);
inline bool operator == (const cAnsiStr& lStr, const cAnsiStr& rStr)
	{ return lStr.Compare(rStr) == 0; }
inline bool operator == (const cAnsiStr& lStr, const char *rStr)
	{ return lStr.Compare(strlen(rStr) + 1, rStr) == 0; }
inline bool operator != (const cAnsiStr& lStr, const cAnsiStr& rStr)
	{ return lStr.Compare(rStr) != 0; }
inline bool operator != (const cAnsiStr& lStr, const char *rStr)
	{ return lStr.Compare(strlen(rStr) + 1, rStr) != 0; }

/*
	SetAt...
	Compare...
	CompareNoCase...
	Collate...
	Mid...
	MakeUpper...
	MakeLower...
	MakeReverse...
	void FmtStr(unsigned int, const char*, ...);
	void FmtStr(const char*, ...);
	void FmtStr(unsigned int, unsigned short, ...);
	void FmtStr(unsigned short, ...);
	char* GetBuffer(int);
	void ReleaseBuffer(void);
	GetBufferSetLength...
	BufIn
	BufOut
	BufInOut
not really sure why we need this
	void DoGrowBuffer(int);
	int ToStream(class cOStore&) const;
	int FromStream(class cIStore&);
	int LoadString(unsigned short);
*/


// Multi-parm type
enum eMultiParmType
{
	kMT_Undef,
	kMT_Int,
	kMT_Float,
	kMT_String,
	kMT_Vector,
	kMT_Boolean
};

struct sMultiParm
{
	union {
		int i;
		float  f;
		char*  psz;
		mxs_vector*  pVector;
		unsigned int b;
	};
	eMultiParmType type;
};

class cMultiParm : public sMultiParm
{
public:
	~cMultiParm()
		{ Unset(); }

	cMultiParm()
		{ psz = NULL; type = kMT_Undef; }
	cMultiParm(int ival)
		{ i = ival; type = kMT_Int; }
	cMultiParm(long ival)
		{ i = static_cast<int>(ival); type = kMT_Int; }
	cMultiParm(short ival)
		{ i = static_cast<int>(ival); type = kMT_Int; }
	cMultiParm(float fval)
		{ f = fval; type = kMT_Float; }
	cMultiParm(double fval)
		{ f = static_cast<float>(fval); type = kMT_Float; }
	// why the fsck does c++ equate 'char*' with 'bool' ???
	//cMultiParm(bool bval)
	//	{ b = bval; type = kMT_Boolean; }
	//cMultiParm(true_bool& bval)
	//	{ b = static_cast<bool>(bval); type = kMT_Boolean; }
	cMultiParm(const char* pszval);
	cMultiParm(const mxs_vector* pVectorval);
	cMultiParm(const mxs_vector& rVectorval);
	cMultiParm(const sMultiParm& rcpy);
	cMultiParm(const cScrStr& rcpy);

	// Simple assignment methods. 
	void Unset();
	void Set(int ival)
		{ Unset(); i = ival; type = kMT_Int; }
	void Set(float fval)
		{ Unset(); f = fval; type = kMT_Float; }
	void Set(bool bval)
		{ Unset(); b = bval; type = kMT_Boolean; }
	void Set(char* pszval)
		{ Unset(); psz = pszval; type = kMT_String; }
	void Set(mxs_vector* pVectorval)
		{Unset();  pVector = pVectorval; type = kMT_Vector; }

	// Assignment will destroy a string or vector pointer.
	cMultiParm& operator= (int ival)
		{ Set(ival); return *this; }
	cMultiParm& operator= (long ival)
		{ Set(static_cast<int>(ival)); return *this; }
	cMultiParm& operator= (float fval)
		{ Set(fval); return *this; }
	cMultiParm& operator= (double fval)
		{ Set(static_cast<float>(fval)); return *this; }
	cMultiParm& operator= (const char* pszval);
	cMultiParm& operator= (const mxs_vector* pVectorval);
	cMultiParm& operator= (const mxs_vector& rVectorval);
	//cMultiParm& operator= (bool bval)
	//	{ Set(bval); return *this; }
	//cMultiParm& operator= (const true_bool& bval)
	//	{ Set(static_cast<bool>(bval)); return *this; }
	cMultiParm& operator= (sMultiParm& mp);
	cMultiParm& operator= (const cScrStr& str);

	operator int () const;
	operator float () const;
	operator bool () const;
	// These pointers are constant, don't fsck with them.
	operator const char* () const;
	operator const mxs_vector* () const;

	bool operator== (const cMultiParm& rhs);
	
	// Explicit destructor. Use Unset instead.
	//void Free();
};


class object
{
public:
	operator int () const
		{ return id; }
	object& operator= (int i)
		{ id = i; return *this; }
	bool operator== (const object & o)
		{ return id == o.id; }
	bool operator== (int i)
		{ return id == i; }
	bool operator!= (const object & o)
		{ return id != o.id; }
	bool operator!= (int i)
		{ return id != i; }
	object (int iId = 0) : id(iId) { }
	int id;
};


interface ILinkQuery;

struct sLink
{
	int   source;
	int   dest;
	short flavor;
};

class link
{
public:
	/*
	object* To (object&, sLink *);
	cMultiParm* GetData (cMultiParm&, const char * field);
	void SetData (const char *, cMultiParm &);
	*/
	link (long i = 0) : id(i) { }
	link& operator= (long i)
		{ id = i; return *this; }
	operator long () const
		{ return id; }
	long id;
};

class linkkind
{
public:
	/*
	linkkind (linkkind);
	*/
	linkkind (int iId = 0) : id(iId) { }
	bool operator== (const linkkind & f)
		{ return id == f.id; }
	bool operator!= (const linkkind & f)
		{ return id != f.id; }
	operator long () const
		{ return id; }
	long id;
};

/***LG3 -- Return values have changed for these methods. ***/
class linkset
{
public:
	inline linkset& operator=(ILinkQuery* __q);
	linkset() : query(NULL) { }
	inline ~linkset();
	inline link Link() const;
	inline void NextLink();
	inline bool AnyLinksLeft() const;
	inline void* Data() const;
	inline sLink Get() const;


	ILinkQuery* query;
};

class reaction_kind
{
public:
	long id;
	reaction_kind (long iId = 0) : id(iId) { }
	operator long () const
		{ return id; }
};


interface IScript;
typedef IScript* (__cdecl *ScriptFactoryProc)(const char*, int);

struct sScrClassDesc
{
	const char* pszModule;
	const char* pszClass;
	const char* pszBaseClass;
	ScriptFactoryProc pfnFactory;
};

enum eScrTraceAction {
	kNoAction,
	kBreak,
	kSpew
};

DECLARE_HANDLE(tScrIter);


struct sScrDatumTag
{
	int  objId;
	const char *  pszClass;
	const char *  pszName;
};


enum eFieldType
{
	kFieldTypeInt = 0,
	kFieldTypeBoolean,
	kFieldTypeShortInt,
	kFieldTypeBitVector,
	kFieldTypeEnum,
	kFieldTypeString,
	kFieldTypeStringPtr,
	kFieldTypePtr,		// don't use this
	kFieldTypePoint,
	kFieldTypeVector,
	kFieldTypeFloat,
	kFieldTypeFixed,	// 16.16 bits (multiply by 65536)
	kFieldTypeFixVec,
	kFieldTypeDouble,
	kFieldTypeAng,		// 180.0 <-> 32768 
	kFieldTypeAngVec
};

struct sFieldDesc
{
	char name[32];
	eFieldType type;
	unsigned long size;
	unsigned long offset;
	int  flags;
	int  min, max;
	int num_bits;
	const char* * bit_names;
};

struct sStructDesc
{
	char name[32];
	unsigned long size;
	int  zero;
	int  num_fields;
	sFieldDesc* fields;
};


struct sTraitDesc
{
};

enum eHierarchyEvent
{
	kHierarchyAdd = 0,
	kHierarchyRemove
};
struct sHierarchyMsg
{
	eHierarchyEvent event;
	int iSubjId;
	int iObjId;
};

// AI uses this
struct sSoundInfo
{
};

#pragma pack()

#endif // _LG_TYPES_H
