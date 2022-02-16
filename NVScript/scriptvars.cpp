#include "scriptvars.h"
#include <cstring>

extern IScriptMan* g_pScriptManager;
extern IMalloc* g_pMalloc;

void script_var::SetDynTag(const char* pszDynName)
{
	char* pszNew = new char[strlen(pszDynName)+1];
	if (pszNew)
	{
		strcpy(pszNew, pszDynName);
		if (m_bDynName)
			delete[] m_tag.pszName;
		m_tag.pszName = pszNew;
		m_bDynName = true;
	}
}
void script_var::Clear()
{
	cMultiParm param;
	g_pScriptManager->ClearScriptData(&m_tag, &param);
}

void script_int::_ReSynch()
{
	cMultiParm param;
	g_pScriptManager->GetScriptData(&m_tag, &param);
	m_iVal = param;
}
void script_int::_Synch()
{
	cMultiParm param(m_iVal);
	g_pScriptManager->SetScriptData(&m_tag, &param);
}

void script_float::_ReSynch()
{
	cMultiParm param;
	g_pScriptManager->GetScriptData(&m_tag, &param);
	m_fVal = param;
}
void script_float::_Synch()
{
	cMultiParm param(m_fVal);
	g_pScriptManager->SetScriptData(&m_tag, &param);
}

void script_str::_ReSynch()
{
	sMultiParm param;
	param.type = kMT_Undef;
	g_pScriptManager->GetScriptData(&m_tag, &param);
	m_szVal.Free();
	m_szVal = (param.type == kMT_String) ? param.psz : NULL;
}
void script_str::_Synch(const char* pszVal)
{
	cMultiParm param(pszVal);
	g_pScriptManager->SetScriptData(&m_tag, &param);
}

void script_vec::_ReSynch()
{
	cMultiParm param;
	g_pScriptManager->GetScriptData(&m_tag, &param);
	m_vVal = *static_cast<const mxs_vector*>(param);
}
void script_vec::_Synch()
{
	cMultiParm param(m_vVal);
	g_pScriptManager->SetScriptData(&m_tag, &param);
}
