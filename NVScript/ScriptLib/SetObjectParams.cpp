#include "ScriptLib.h"

int SetObjectParams(int iObject, const char* pszParams)
{
	SService<IPropertySrv> pPS(g_pScriptManager);
	cMultiParm mpDesignNote;
	mpDesignNote.Set(const_cast<char*>(pszParams));
	pPS->Add(iObject, g_pszDesignNote);
	register int iRet = pPS->SetSimple(iObject, g_pszDesignNote, mpDesignNote);
	mpDesignNote.type = kMT_Undef;
	return iRet;
}

