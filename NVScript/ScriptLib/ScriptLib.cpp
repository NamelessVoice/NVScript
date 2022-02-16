#include <lg/config.h>
#include <lg/types.h>
#include <lg/scrmanagers.h>

IMalloc* g_pMalloc;
IScriptMan* g_pScriptManager;

cMultiParm g_mpUndef;

const char* g_pszDesignNote = 
#if (_DARKGAME == 3)
		"ObjList";
#else
		"DesignNote";
#endif

const char* g_pszParamDelim = ";";
const char* g_pszQuotes = "\"'";

const char* g_pszCDLinkFlavor = 
#if (_DARKGAME == 3)
		"SwitchLink";
#else
		"ControlDevice";
#endif

