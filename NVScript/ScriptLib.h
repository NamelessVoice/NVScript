/************************************************
 * General-purpose Utilities for script modules
 */

#ifndef SCRIPTLIB_H
#define SCRIPTLIB_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/config.h>
#include <lg/objstd.h>
#include <lg/interface.h>
#include <lg/types.h>
#include <lg/scrservices.h>
#include <lg/scrmanagers.h>
#include <lg/links.h>

interface IScript;

/****************
 * Globals
 * The hosting app/module must assign these.
 */

extern IMalloc* g_pMalloc;
extern IScriptMan* g_pScriptManager;

/* An unassigned cMultiParm. Useful for filling in default arguments. */
extern cMultiParm g_mpUndef;

#ifndef NO_PARAMCACHE
void InitParamCache(void);
void ReleaseParamCache(void);
#endif

/* Common string constants. Defined externally because they depend on 
 * the value of _DARKGAME.
 */
extern const char* g_pszDesignNote;
extern const char* g_pszCDLinkFlavor;


/*************
 * Properties
 */

// Read design note parameters with inheritance.
// The parameters are cached to improve performance.
// Additionally, string returns are constant, so don't use Free.

const char* ParamGetString(int iObject, const char* pszParam, const char* pszDefault = NULL);
int ParamGetInt(int iObject, const char* pszParam, int iDefault = 0);
float ParamGetFloat(int iObject, const char* pszParam, float fDefault = 0);
int ParamGetObject(int iObject, const char* pszParam, int iDefault = 0);
int ParamGetTime(int iObject, const char* pszParam, int iDefault = 0);
bool ParamGetBool(int iObject, const char* pszParam, bool bDefault = false);
void ParamSetString(int iObject, const char* pszParam, const char* pszValue);
void ParamSetInt(int iObject, const char* pszParam, int iValue);
void ParamSetFloat(int iObject, const char* pszParam, float fValue);
void ParamSetBool(int iObject, const char* pszParam, bool bValue);
void ParamRemove(int iObject, const char* pszParam, bool bNoInherit = false);
bool ParamExists(int iObject, const char* pszParam);
bool ParamExistsDirect(int iObject, const char* pszParam);


// Old-style param functions read the Editor\DesignNote property on-demand
// Strings are allocated using g_pMalloc. Please use Free.

// Returns the whole Design Note.
// Useful to save time if you're reading a lot of params.
char* GetObjectParams(int iObject);
char* GetObjectParamsNoInherit(int iObject);
int SetObjectParams(int iObject, const char* pszParams);

// Returns the param as a particular type.
char* GetObjectParamString(int iObject, const char* pszParam);
int GetObjectParamInt(int iObject, const char* pszParam);
float GetObjectParamFloat(int iObject, const char* pszParam);
int GetObjectParamObject(int iObject, const char* pszParam);
int GetObjectParamTime(int iObject, const char* pszParam);
bool GetObjectParamBool(int iObject, const char* pszParam);

int SetObjectParamString(int iObject, const char* pszParam, const char* pszVal);
int SetObjectParamInt(int iObject, const char* pszParam, int iVal);
int SetObjectParamFloat(int iObject, const char* pszParam, float fVal);
int SetObjectParamBool(int iObject, const char* pszParam, bool bVal);

// Same, but uses the supplied string instead of extracting from the object.
char* GetParamString(const char* pszString, const char* pszParam);
int GetParamInt(const char* pszString, const char* pszParam);
float GetParamFloat(const char* pszString, const char* pszParam);
int GetParamObject(const char* pszString, const char* pszParam);
int GetParamTime(const char* pszString, const char* pszParam);
bool GetParamBool(const char* pszString, const char* pszParam);

char* SetParamString(char* pszString, const char* pszParam, const char* pszVal);
char* SetParamInt(char* pszString, const char* pszParam, int iVal);
char* SetParamFloat(char* pszString, const char* pszParam, float fVal);
char* SetParamBool(char* pszString, const char* pszParam, bool bVal);

// Delete a specific parameter from the Design Note.
// All occurances of the parameter are removed.
int RemoveObjectParam(int iObject, const char* pszParam);


/***********
 * Metaproperties
 */

// These functions take care to not put more than one instance 
// of a metaproperty on an object.
int AddSingleMetaProperty(const char* pszMetaProp, int iObjId);
int AddSingleMetaProperty(int iMetaProp, int iObjId);
int RemoveSingleMetaProperty(const char* pszMetaProp, int iObjId);
int RemoveSingleMetaProperty(int iMetaProp, int iObjId);
int AddMetaProperty(const char* pszMetaProp, int iObjId);
int RemoveMetaProperty(const char* pszMetaProp, int iObjId);


/********
 * Links
 */
typedef int (*LinkIterCallbackFunc)(ILinkSrv*,ILinkQuery*,IScript*,void*);

// GetOne... - return the first link found.
// GetAny... - Randomly select a link from all that exist.
// ...Inheritedxxx - Crawl up the inheritence tree for the source or dest 
//     until a match is found. For GetAny, the crawl will stop at the first 
//     object found with a valid link.

long GetOneLink(const char* pszFlavor, int iSource, int iDest, sLink* pResult);
long GetOneLinkInheritedSrc(const char* pszFlavor, int iSource, int iDest, sLink* pResult);
int GetOneLinkDest(const char* pszFlavor, int iSource);
long GetOneLinkByData(const char* pszFlavor, int iSource, int iDest, sLink* pResult, const void* pvData, int iDataLen);
long GetOneLinkByDataInheritedSrc(const char* pszFlavor, int iSource, int iDest, sLink* pResult, const void* pvData, int iDataLen);
int GetOneLinkByDataDest(const char* pszFlavor, int iSource, const void* pvData, int iDataLen);
long GetAnyLink(const char* pszFlavor, int iSource, int iDest, sLink* pResult);
long GetAnyLinkInheritedSrc(const char* pszFlavor, int iSource, int iDest, sLink* pResult);
int GetAnyLinkDest(const char* pszFlavor, int iSource);
long GetAnyLinkByData(const char* pszFlavor, int iSource, int iDest, sLink* pResult,
			    const void* pvData, int iDataLen);
long GetAnyLinkByDataInheritedSrc(const char* pszFlavor, int iSource, int iDest, sLink* pResult,
			    const void* pvData, int iDataLen);
int GetAnyLinkByDataDest(const char* pszFlavor, int iSource, const void* pvData, int iDataLen);
// The iterator callback should be a static member function. (Or just a regular non-member function.)
long IterateLinks(const char* pszFlavor, int iSource, int iDest,
			    LinkIterCallbackFunc pfnLinkIterCallback, IScript* pScript, void* pData);
long IterateLinksByData(const char* pszFlavor, int iSource, int iDest,
			    const void* pvFilter, int iFilterLen,
			    LinkIterCallbackFunc pfnLinkIterCallback, IScript* pScript, void* pData);
void DeleteAllLinks(const char* pszFlavor, int iSource, int iDest);
void DeleteAllLinksByData(const char* pszFlavor, int iSource, int iDest, const void* pvData, int iDataLen);

// Any links between the source and the object that's the source of a PlayerFactory link
// will be relinked so the source points to the actual Player object.
void FixupPlayerLinks(int iSource, int iPlayer);


/***********
 * Messages
 */

void SimpleSend(int iSource, int iDest, const char* pszMessage,
		    const cMultiParm& mpData1 = g_mpUndef, const cMultiParm& mpData2 = g_mpUndef, const cMultiParm& mpData3 = g_mpUndef);
void SimplePost(int iSource, int iDest, const char* pszMessage,
		    const cMultiParm& mpData1 = g_mpUndef, const cMultiParm& mpData2 = g_mpUndef, const cMultiParm& mpData3 = g_mpUndef);
long CDSend(const char* pszMessage, int iSource, const cMultiParm& mpData = g_mpUndef);


/*************
 * MultiParms
 */

void MultiParmAssignInt(sMultiParm* pParm, int iValue);
void MultiParmAssignFloat(sMultiParm* pParm, float fValue);
void MultiParmAssignString(sMultiParm* pParm, const char* pszValue);
void MultiParmAssignVector(sMultiParm* pParm, const mxs_vector* pvValue);


/*************
 * Objects
 */

int StrToObject(const char* pszName);


/***********
 * User
 */

int CalcTextTime(const char* pszText, int iWordTime = 500);
int ShowBook(int iObject, bool bArt, int iTime = 0);
void ShowString(const char* pszStr, int iTime, COLORREF color = 0);
__attribute__((format (printf,1,2)))
	void DebugPrintf(const char* pszFormat, ...);
__attribute__((format (printf,1,2)))
	void DisplayPrintf(const char* pszFormat, ...);
__attribute__((format (printf,2,3)))
	void DisplayPrintf(COLORREF iColour, const char* pszFormat, ...);


/************
 * Utils
 */
__attribute__((nonnull (1,2)))
	char* strsep(char** str, const char* delim);
__attribute__((nonnull (1,2,3)))
	char* strqsep(char** str, const char* delim, const char* quotes);

int strtocolor(const char* str);
int strtotime(const char* str);

extern const char g_cHexTable[256];
extern const char g_cHexChars[16];

__attribute__((nonnull (1))) inline 
	char hexbyte(const char* hex)
{
	return (g_cHexTable[unsigned(hex[0])] << 4)
			| g_cHexTable[unsigned(hex[1])];
}
__attribute__((nonnull (1))) inline 
	short hexshort(const char* hex)
{
	return (short(hexbyte(hex)) << 8) | short((unsigned char)hexbyte(hex+2));
}
__attribute__((nonnull (1))) inline 
	long hexlong(const char* hex)
{
	return (long(hexshort(hex)) << 16) | long((unsigned short)hexshort(hex+4));
}

__attribute__((nonnull (1))) inline 
	void bytehex(char* hex, char i)
{
	hex[0] = g_cHexChars[unsigned((i >> 4) & 0xF)];
	hex[1] = g_cHexChars[unsigned(i & 0xF)];
	hex[2] = 0;
}
__attribute__((nonnull (1))) inline 
	void shorthex(char* hex, short i)
{
	bytehex(hex, (i >> 8) & 0xFF);
	bytehex(hex+2, i & 0xFF);
}
__attribute__((nonnull (1))) inline 
	void longhex(char* hex, long i)
{
	shorthex(hex, (i >> 16) & 0xFFFF);
	shorthex(hex+4, i & 0xFFFF);
}

#endif // SCRIPTLIB_H
