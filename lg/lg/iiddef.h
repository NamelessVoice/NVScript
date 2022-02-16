/***********************
 * LGS IID Helper class
 */

#ifndef _LG_IIDDEF_H
#define _LG_IIDDEF_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/config.h>
#include <lg/objstd.h>


#define DEFINE_IIDSTRUCT(_iface,_iid)	                 \
		template<> struct IID_Def<_iface>                \
		{                                                \
			static const GUID iid() { return _iid; }            \
			static const char* name() { return #_iface; }       \
		};

template<typename _IFace>
struct IID_Def
{
	static const IID* iid();
	static const char* name();
};

#endif // _LG_IIDDEF_H
