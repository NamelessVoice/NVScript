/**********************
 * LGS Sound
 */

#ifndef _LG_SOUND_H
#define _LG_SOUND_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <mmsystem.h>

extern  const GUID  IID_ICDPlayer;
interface ICDPlayer : IUnknown
{
	STDMETHOD_(long,Init)(void) PURE;
	STDMETHOD_(long,CDPlayTrack)(int) PURE;
	STDMETHOD_(long,CDStopPlay)(void) PURE;
	STDMETHOD_(long,CDSetBGMTrack)(int) PURE;
	STDMETHOD_(long,CDPlayTransition)(int,unsigned int) PURE;
	STDMETHOD_(long,Update)(void) PURE;
	virtual void * __thiscall d0(unsigned int) = 0;
	STDMETHOD_(long,CDOpen)(LPMCI_OPEN_PARMS) PURE;
	STDMETHOD_(long,CheckBGM)(void) PURE;
};
DEFINE_IIDSTRUCT(ICDPlayer,IID_ICDPlayer)

interface ISong;

extern  const GUID  IID_ISongPlayer;
interface ISongPlayer : IUnknown
{
	STDMETHOD_(int,SetSong)(ISong*) PURE;
	STDMETHOD_(void,GetSong)(ISong**) PURE;
	STDMETHOD_(void,UnloadSong)(void) PURE;
	STDMETHOD_(int,StartPlaying)(void) PURE;
	STDMETHOD_(int,StopPlaying)(void) PURE;
	STDMETHOD_(int,PausePlaying)(void) PURE;
	STDMETHOD_(int,ResumePlaying)(void) PURE;
	STDMETHOD_(void,SetVolume)(int) PURE;
	STDMETHOD_(int,GetVolume)(void) PURE;
	STDMETHOD_(int,SendEvent)(char const*, int) PURE;
	STDMETHOD_(void,SetThemed)(int) PURE;
	STDMETHOD_(int,IsThemed)(void) PURE;
	STDMETHOD_(int,SetTheme)(char const*) PURE;
	STDMETHOD_(void,GetMemento)(void**, int*) PURE;
	STDMETHOD_(int,RestoreMemento)(void*, int) PURE;
};
DEFINE_IIDSTRUCT(ISongPlayer,IID_ISongPlayer)

extern  const GUID  IID_IVoiceOverSys;
interface IVoiceOverSys : IUnknown
{
	STDMETHOD_(int,Play)(int) PURE;
	STDMETHOD_(int,AlreadyPlayed)(int) PURE;
	STDMETHOD_(long,SetAlreadyPlayed)(int,int) PURE;
	STDMETHOD_(long,SetCombatTest)(int (__stdcall *)(void)) PURE;
};

#endif // _LG_SOUND_H
