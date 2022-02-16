/**********************
 * LGS Windows Host
 */

#ifndef _LG_WIN_H
#define _LG_WIN_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "lg/objstd.h"
#include <ddraw.h>
#include "lg/graphics.h"


interface IApplication : IUnknown
{
};
/*
int IWindowsApplication::CriticalMessageBox(const char*,const char*,int);
int IWindowsApplication::MessageBoxA(const char*,const char*,int);
long IWindowsApplication::Unadvise(unsigned long);
long IWindowsApplication::QueryQuit(void);
const char* IWindowsApplication::GetCaption(void);
const char* IWindowsApplication::GetDefaultFilePath(void);
const char* IWindowsApplication::GetPath(void);
const char* IWindowsApplication::GetFullName(void);
void IWindowsApplication::AppCommand(unsigned int);
void IWindowsApplication::Abort(const char*);
void IWindowsApplication::SetCaption(const char*);
void IWindowsApplication::SetDefaultFilePath(const char*);
void IWindowsApplication::Quit(void);
*/

extern  const GUID  IID_IWinApp;

enum eWinPumpResult
{
	kWinPumpResult0
};
enum eWinPumpDuration
{
	kWinPumpDuration0
};

interface IWinApp : IUnknown
{
	STDMETHOD_(HWND,GetMainWnd)(void) PURE;
	STDMETHOD_(HWND,SetMainWnd)(HWND) PURE;
	STDMETHOD_(HINSTANCE,GetResourceInstance)(void) PURE;
	virtual int f1() = 0;
	STDMETHOD_(const char*,GetCommandLine)(void) PURE;
	STDMETHOD_(int,GetCommandShow)(void) PURE;
	STDMETHOD_(void,SetRegistryKey)(const char*) PURE;
	STDMETHOD_(const char*,GetRegistryKey)(void) PURE;
	STDMETHOD_(void,SetProfileName)(const char*) PURE;
	STDMETHOD_(const char*,GetProfileName)(void) PURE;
	STDMETHOD_(void,SetDialogBkColor)(unsigned long,unsigned long) PURE;
	STDMETHOD_(eWinPumpResult,PumpEvents)(int,eWinPumpDuration) PURE;
	STDMETHOD_(long,ModalDialogBox)(const char*,DLGPROC) PURE;
	virtual int f2() = 0;
	virtual int f3() = 0;
};
DEFINE_IIDSTRUCT(IWinApp,IID_IWinApp)

/*
	STDMETHOD_(int,BreakLock)(void) PURE;
	STDMETHOD_(void,GetKind2)(eDisplayDeviceKind*,int*,_GUID**) PURE;
	STDMETHOD_(int,SetGamma)(double) PURE;
	STDMETHOD_(long,Open)(grs_drvcap*,int) PURE;
	STDMETHOD_(int,SetMonitor)(void**,eDisplayDeviceMonitorFlag) PURE;
	STDMETHOD_(void,SetKind2)(eDisplayDeviceKind,_GUID*) PURE;
	STDMETHOD_(long,GetRenderTargets)(sGrRenderTargets*) PURE;
	STDMETHOD_(int,SetFlip)(int) PURE;
	STDMETHOD_(void,RestoreLock)(int) PURE;
	STDMETHOD_(int,IsLocked)(void) PURE;
	STDMETHOD_(void,FlushRect)(int,int,int,int) PURE;
*/
extern  const GUID  IID_IWinDisplayDevice;
struct sWinDispDevCallbackInfo
{
};
typedef void (__cdecl *TaskSwitchCallbackFunc)(sWinDispDevCallbackInfo*);
interface IWinDispDevice : IUnknown
{
	STDMETHOD_(int,ProcessMessage)(unsigned int,unsigned int,long,long*) PURE;
	STDMETHOD_(void,GetWidthHeight)(unsigned int*,unsigned int*) PURE;
	STDMETHOD_(void,OnTaskSwitch)(int) PURE;
	STDMETHOD_(int,GetDirectDraw)(IDirectDraw4**) PURE;
	STDMETHOD_(void,WaitForMutex)(void) PURE;
	STDMETHOD_(void,ReleaseMutex)(void) PURE;
	STDMETHOD_(void,CursorBltFromGDIScreen)(grs_bitmap*,int,int) PURE;
	STDMETHOD_(void,CursorBltToGDIScreen)(grs_bitmap*,int,int) PURE;
	STDMETHOD_(int,GetBitmapSurface)(grs_bitmap*,IDirectDrawSurface4**) PURE;
	STDMETHOD_(void,AddTaskSwitchCallback)(TaskSwitchCallbackFunc) PURE;
	STDMETHOD_(void,RemoveTaskSwitchCallback)(int) PURE;
};
DEFINE_IIDSTRUCT(IWinDispDevice,IID_IWinDisplayDevice)

extern  const GUID  IID_IGameShell;
interface IWinGameShell : IUnknown
{
	STDMETHOD_(void,BeginFrame)(void) PURE;
	STDMETHOD_(void,EndFrame)(void) PURE;
	STDMETHOD_(long,BringToFront)(void) PURE;
};

#endif // _LG_WIN_H
