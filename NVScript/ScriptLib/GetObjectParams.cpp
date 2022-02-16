#include "ScriptLib.h"

extern const char* g_pszDesignNote;

char* GetObjectParams(int iObject)
{
	SService<IPropertySrv> pPS(g_pScriptManager);
	cMultiParm mpDesignNote;
	if (!pPS->Possessed(iObject, g_pszDesignNote))
		return NULL;
	pPS->Get(mpDesignNote, iObject, g_pszDesignNote, NULL);
	// naughty little shortcut to avoid alloc+copy
	mpDesignNote.type = kMT_Undef;
	return (mpDesignNote.psz);
}

