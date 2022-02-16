/***********************
 * LGS Editor Interface
 */

#ifndef _LG_EDITOR_H
#define _LG_EDITOR_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/iiddef.h>


extern  const GUID  IID_IEditTools;
interface IEditTools : IUnknown
{
	STDMETHOD_(const char*,ObjName)(int) PURE;
	STDMETHOD_(int,GetObjNamed)(const char*) PURE;
	STDMETHOD_(long,EditTypedData)(const char*, const char*, void*) PURE;
};
DEFINE_IIDSTRUCT(IEditTools,IID_IEditTools)

#endif // _LG_EDITOR_H
