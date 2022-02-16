/**********************
 * LGS Dialog Interfaces
 */

#ifndef _LG_DLGS_H
#define _LG_DLGS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/iiddef.h>

#include <lg/types.h>

struct sObjEditorDesc
{
	char title[32];
	unsigned long flags;
};

struct sLinkEditorDesc
{
	char title[64];
	unsigned long flags;
};

struct sActReactEditorDesc
{
	int objid;
	int stimulus;
};

enum eHierarchyRootFlag
{
	kHierarchyRootShowConcrete = 1,
	kHierarchyRootNoCreate = 8
};

struct sHierarchyRootDesc
{
	char label[32];
	char name[16];
	unsigned long flags;
	// Any of these callbacks can be NULL. In which case, 
	// the dialog should handle the event itself (if it can)
	int (*CreateCallback)(int,const char*);	// retuns 10 or 1
	int (*DeleteCallback)(int);	// returns 1 or 8
	int (*BrushCallback)(int);	// returns 5
};

struct sHierarchyEditorDesc
{
	char title[36];
	sHierarchyRootDesc* root;
};

// You could use a union to read the embedded structs,
// but why not take advantage or IStructDescTools
struct sStimSource
{
	int type;
	float intensity;
	unsigned long flags;
	char shape[32];
	char lifecycle[32];
};

struct sStimSensor
{
	int ordinal;
	float minintensity;
	float maxintensity;
	unsigned long flags;
	int effect;
	int target;
	int agent;
	char data[32];
};

extern  const GUID  IID_IStructEditor;
interface IStructEditor : IUnknown
{
	STDMETHOD_(BOOL,Show)(BOOL) PURE;
	// Set...something or other
	STDMETHOD(f01)(int,void*) PURE;
	STDMETHOD_(char const*,GetTitle)(VOID) PURE;
	STDMETHOD_(char const* const*,GetStructDesc)(VOID) PURE;
	STDMETHOD_(void const*,GetData)(VOID) PURE;
};
DEFINE_IIDSTRUCT(IStructEditor,IID_IStructEditor)

extern  const GUID  IID_IObjEditor;
interface IObjEditor : IUnknown
{
	STDMETHOD_(BOOL,Show)(BOOL) PURE;
	// Set...something or other
	STDMETHOD(f01)(int,void*) PURE;
	STDMETHOD_(sObjEditorDesc*,GetDesc)(VOID) PURE;
	STDMETHOD_(int,GetID)(VOID) PURE;
};
DEFINE_IIDSTRUCT(IObjEditor,IID_IObjEditor)

// plural?
extern  const GUID  IID_IActReactEditors;
interface IActReactEditor : IUnknown
{
	STDMETHOD(EditSource)(int,sActReactEditorDesc*,sStimSource*) PURE;
	STDMETHOD(EditReceptron)(int,sActReactEditorDesc*,sStimSensor*) PURE;
	STDMETHOD(ListReceptrons)(int,sActReactEditorDesc*) PURE;
	STDMETHOD(ListSources)(int,sActReactEditorDesc*) PURE;
};
DEFINE_IIDSTRUCT(IActReactEditor,IID_IActReactEditors)

#endif // _LG_DLGS_H
