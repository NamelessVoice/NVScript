#include "ScriptLib.h"
#include <lg/properties.h>

extern const char* g_pszDesignNote;

char* GetObjectParamsNoInherit(int iObject)
{
	SInterface<IPropertyManager> pPM(g_pScriptManager);
	IProperty* pProp = pPM->GetPropertyNamed(g_pszDesignNote);
	if (!pProp)
		throw null_pointer();
	if (!pProp->IsSimplyRelevant(iObject))
	{
		pProp->Release();
		return NULL;
	}
	IStringProperty *pDNProp;
	const char* pszDesignNote;
	char* pszRet;
	if (E_NOINTERFACE == pProp->QueryInterface(IID_IStringProperty, reinterpret_cast<void**>(&pDNProp)))
		throw no_interface("IStringProperty");
	pProp->Release();
	pDNProp->GetSimple(iObject, &pszDesignNote);
	if ( pszDesignNote == NULL )
		return NULL;
		
	pszRet = reinterpret_cast<char*>(g_pMalloc->Alloc(strlen(pszDesignNote) + 1));
	if (pszRet)
		strcpy(pszRet, pszDesignNote);
	pDNProp->Release();
	return pszRet;
}

