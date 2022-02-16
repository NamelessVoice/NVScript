/**********************
 * LGS Script Tools
 */

#ifndef _LG_TOOLS_H
#define _LG_TOOLS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/iiddef.h>

#include <lg/types.h>

interface IStringProperty;


extern  const GUID  IID_IGameStrings;
interface IGameStrings : IUnknown
{
	// filename, stringname, ???, resname
	STDMETHOD_(cAnsiStr*,FetchString)(cAnsiStr&,const char*,const char*,const char*,const char*) PURE;
	// Bind a resname to a property 
	STDMETHOD_(long,RegisterProp)(const char*,IStringProperty*) PURE;
	// object id, resname (as used in RegisterProp)
	STDMETHOD_(cAnsiStr*,FetchObjString)(cAnsiStr&,int,const char*) PURE;
	STDMETHOD_(long,DumpProp)(const char*) PURE;
	STDMETHOD_(long,LoadProp)(const char*) PURE;
	STDMETHOD_(long,StripProp)(const char*) PURE;
	STDMETHOD_(long,ModernizeProp)(const char*) PURE;
};
DEFINE_IIDSTRUCT(IGameStrings,IID_IGameStrings)

extern  const GUID  IID_IGameTools;
typedef int (__cdecl *IsToGameModeGUIDCallbackFunc)(const _GUID&);
interface IGameTools : IUnknown
{
	STDMETHOD_(int,IsToGameModeGUID)(const _GUID&) PURE;
	STDMETHOD_(long,SetIsToGameModeGUIDCallback)(IsToGameModeGUIDCallbackFunc) PURE;
	STDMETHOD_(long,TeleportObject)(int,const mxs_vector*,const mxs_angvec*,int) PURE;
};
DEFINE_IIDSTRUCT(IGameTools,IID_IGameTools)

extern  const GUID  IID_IStructDescTools;
interface IStructDescTools : IUnknown
{
	 STDMETHOD_(const sFieldDesc*,GetFieldNamed)(const sStructDesc*,const char*) PURE;
	 STDMETHOD_(long,ParseField)(const sFieldDesc*,const char*,void*) PURE;
	 STDMETHOD_(long,UnparseField)(const sFieldDesc*,const void*,char*,int) PURE;
	 STDMETHOD_(int,IsSimple)(const sStructDesc*) PURE;
	 STDMETHOD_(int,ParseSimple)(const sStructDesc*,const char*,void*) PURE;
	 STDMETHOD_(int,UnparseSimple)(const sStructDesc*,const void*,char*,int) PURE;
	 STDMETHOD_(int,ParseFull)(const sStructDesc*,const char*,void*) PURE;
	 STDMETHOD_(int,UnparseFull)(const sStructDesc*,const void*,char*,int) PURE;
	 STDMETHOD_(long,Dump)(const sStructDesc*,const void*) PURE;
	 STDMETHOD_(long,SetIntegral)(const sFieldDesc*,long,void*) PURE;
	 STDMETHOD_(long,GetIntegral)(const sFieldDesc*,const void*,long*) PURE;
	 STDMETHOD_(const sStructDesc*,Lookup)(const char*) PURE;
	 STDMETHOD_(long,Register)(const sStructDesc*) PURE;
	 STDMETHOD_(long,ClearRegistry)(void) PURE;
};
DEFINE_IIDSTRUCT(IStructDescTools,IID_IStructDescTools)

#endif // _LG_TOOLS_H
