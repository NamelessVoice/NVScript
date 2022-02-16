
#ifndef NVGENERICTRIGGER_H
#define NVGENERICTRIGGER_H

#include "scriptvars.h"
#include "ScriptNV.h"

/*
 * Generic Script
 *
 */
class cScrNVTrigger
{
public:
	cScrNVTrigger()
		: m_iTrigObjId(0)
	{
		m_szTriggerName = new char[1];
		m_szTriggerName[0] = '\0';
	}
	cScrNVTrigger(const char* pszName,int iHostObjId)
		: m_iTrigObjId(iHostObjId)
	{
		m_szTriggerName = new char[strlen(pszName) + 1];
		strcpy(m_szTriggerName, pszName);
	}

	virtual ~cScrNVTrigger()
	{
		if (m_szTriggerName)
		{
			delete[] m_szTriggerName;
			m_szTriggerName = NULL;
		}
	}

private:
	int m_iTrigObjId;
	char* m_szTriggerName;
	static int StimIterFunc(ILinkSrv* pLS, ILinkQuery* pLQ, IScript* pScript, void* pData);
	long TriggerSingleObject(object oDest, const char* pszMessage, int iMsgTime);
	long StimSingleObject(object oDest, object oStim, float fIntensity);
	long StimSingleObject(IActReactSrv* pARSrv, object oDest, object oStim, float fIntensity);
//	Nothing private

protected:
	virtual void InitScriptData()
	{	
		
	// Message handler registration function has not been inherited from cScript.
	// I think I'll just add this code to all decendants of this script instead.
	//	RegisterMessageHandler("ResetTriggerCount", static_cast<MessageHandlerFunc>(&cScrNVTrigger::OnResetTriggerCount));
		
		return;
	}

	virtual long Trigger(bool bOn, sScrMsg* pMsg, int iDefaultDest = 0, const char* szDefaultOnMessage = "TurnOn", const char* szDefaultOffMessage = "TurnOff");

	void ResetTriggerCount();

public:
//	Nothing public
};

#endif // NVGENERICTRIGGER_H
