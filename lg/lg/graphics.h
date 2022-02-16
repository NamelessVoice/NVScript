/**********************
 * LGS Graphics interface
 */

#ifndef _LG_GRAPHICS_H
#define _LG_GRAPHICS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>

enum eDisplayDeviceKind
{
	kUnknownDisplayDeviceKind = -1
};

struct grs_sys_info
{
};

struct grs_mode_info
{
	short	mode_num;
	short	unknown1;
	unsigned short	width;
	unsigned short	height;
	unsigned char	depth;
	unsigned char	unknown2;
	unsigned short	unknown3;
};

struct sGrModeInfoEx
{
};

struct grs_bitmap
{
	char *data;
	short	unknown1;
	short	unknown2;
	unsigned short	width, height, row_size;
	unsigned short unknown3;
};

extern  const GUID  IID_IDisplayDevice;
interface IDisplayDevice : IUnknown
{
	STDMETHOD_(eDisplayDeviceKind,GetKind)(void) PURE;
	STDMETHOD_(void,SetKind)(eDisplayDeviceKind,int) PURE;
	STDMETHOD_(void,GetInfo)(grs_sys_info*,grs_mode_info*) PURE;
	STDMETHOD_(long,Close)(void) PURE;
	STDMETHOD_(long,SetMode)(int,int) PURE;
	STDMETHOD_(int,GetMode)(sGrModeInfoEx*) PURE;
	STDMETHOD_(int,StatHTrace)(void) PURE;
	STDMETHOD_(int,StatVTrace)(void) PURE;
	STDMETHOD_(int,SetPalette)(unsigned int,unsigned int,const unsigned char*) PURE;
	STDMETHOD_(int,GetPalette)(unsigned int,unsigned int,unsigned char*) PURE;
	STDMETHOD_(long,PageFlip)(void) PURE;
	STDMETHOD_(int,Lock)(void) PURE;
	STDMETHOD_(int,Unlock)(void) PURE;
	STDMETHOD_(void,Flush)(void) PURE;
	STDMETHOD_(void,PauseFlush)(int) PURE;
};
DEFINE_IIDSTRUCT(IDisplayDevice,IID_IDisplayDevice)

#endif
