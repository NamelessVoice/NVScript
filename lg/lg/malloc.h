/*****************************
 * LGS Malloc Implementation
 */

#ifndef _LG_MALLOC_H
#define _LG_MALLOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/iiddef.h>


/*
interface IMalloc : IUnknown
{
	STDMETHOD_(void*,Alloc)(ULONG) PURE;
	STDMETHOD_(void*,ReAlloc)(void*,ULONG) PURE;
	STDMETHOD_(void,Free)(void*) PURE;
	STDMETHOD_(ULONG,GetSize)(void*) PURE;
	STDMETHOD_(int,DidAlloc)(void*) PURE;
	STDMETHOD_(void,HeapMinimize)(void) PURE;
};
DEFINE_IIDSTRUCT(IMalloc,IID_IMalloc)
*/

extern  const GUID  IID_IDebugMalloc;
interface IDebugMalloc : IMalloc
{
	STDMETHOD_(void*,AllocEx)(unsigned long,const char*,int) PURE;
	STDMETHOD_(void*,Reallocex)(void*,unsigned long,const char*,int) PURE;
	STDMETHOD_(void,FreeEx)(void*,const char*,int) PURE;
	STDMETHOD_(long,VerifyAlloc)(void*) PURE;
	STDMETHOD_(long,VerifyHeap)(void) PURE;
	STDMETHOD_(void,DumpHeapInfo)(void) PURE;
	STDMETHOD_(void,DumpStats)(void) PURE;
	STDMETHOD_(void,DumpBlocks)(void) PURE;
	STDMETHOD_(void,DumpModules)(void) PURE;
	STDMETHOD_(void,PushCredit)(const char*,int) PURE;
	STDMETHOD_(void,PopCredit)(void) PURE;
};
DEFINE_IIDSTRUCT(IDebugMalloc,IID_IDebugMalloc)

/*
extern  const GUID  IID_IScriptModuleAlloc;
interface IScriptModuleAlloc : IMalloc
{
	STDMETHOD_(void*,AllocEx)(unsigned long,const char*,int) PURE;
	STDMETHOD_(void*,Reallocex)(void*,unsigned long,const char*,int) PURE;
	STDMETHOD_(void,FreeEx)(void*,const char*,int) PURE;
	STDMETHOD_(long,VerifyAlloc)(void*) PURE;
	STDMETHOD_(long,VerifyHeap)(void) PURE;
	STDMETHOD_(void,DumpHeapInfo)(void) PURE;
	STDMETHOD_(void,DumpStats)(void) PURE;
	STDMETHOD_(void,DumpBlocks)(void) PURE;
	STDMETHOD_(void,DumpModules)(void) PURE;
	STDMETHOD_(void,PushCredit)(const char*,int) PURE;
	STDMETHOD_(void,PopCredit)(void) PURE;
	virtual void Attach(IMalloc **) PURE;
	void Remove(void);
	int HaveRecord(void*);
	sScriptAllocRecord* VoidToRecord(void*);
	void* RealAddress(void*);
	void GrowBlocks(void);
};
DEFINE_IIDSTRUCT(IScriptModuleAlloc,IID_IScriptModuleAlloc)
*/


#endif // _LG_MALLOC_H
