#ifndef _LG_OBJSTD_H
#define _LG_OBJSTD_H

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <objbase.h>

#undef SendMessage
#undef PostMessage
#undef DispatchMessage
#undef GetClassName
#undef GetCommandLine
#undef UnlockResource
#undef FindResource

#endif // _LG_OBJSTD_H
