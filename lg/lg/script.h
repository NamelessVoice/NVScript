/*******************************
 * LGS Script and Script Module
 */

#ifndef _LG_SCRIPT_H
#define _LG_SCRIPT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/iiddef.h>

#include <lg/types.h>
#include <lg/scrmsgs.h>


extern  const GUID  IID_IScriptModule;
interface IScriptModule : IUnknown
{
	STDMETHOD_(const char*,GetName)(void) PURE;
	STDMETHOD_(const sScrClassDesc*,GetFirstClass)(tScrIter*) PURE;
	STDMETHOD_(const sScrClassDesc*,GetNextClass)(tScrIter*) PURE;
	STDMETHOD_(void,EndClassIter)(tScrIter*) PURE;
};
DEFINE_IIDSTRUCT(IScriptModule,IID_IScriptModule)


extern  const GUID  IID_IScript;
interface IScript : IUnknown
{
	STDMETHOD_(const char*,GetClassName)(void) PURE;
	STDMETHOD_(long,ReceiveMessage)(sScrMsg *,sMultiParm *,eScrTraceAction) PURE;
};
DEFINE_IIDSTRUCT(IScript,IID_IScript)

#endif // _LG_SCRIPT_H
