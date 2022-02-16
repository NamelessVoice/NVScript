
#ifndef NVGENERICSCRIPT_H
#define NVGENERICSCRIPT_H

#include "scriptvars.h"
#include "ScriptNV.h"


/*****************
 * Generic Script
 *
 */
class cScrNVGeneric : public cScriptNV
{
public:
	cScrNVGeneric(const char* pszName, int iHostObjId)
		: cScriptNV(pszName, iHostObjId),
		SCRIPT_VAROBJ(m_iUseCount),
		SCRIPT_VAROBJ(m_iOnCapacitor),
		SCRIPT_VAROBJ(m_iOffCapacitor),
		SCRIPT_VAROBJ(m_iOnCapacitorTime),
		SCRIPT_VAROBJ(m_iOffCapacitorTime)
	{ }

	STDMETHOD_(long,ReceiveMessage)(sScrMsg*,sMultiParm*,eScrTraceAction);

private:
//	Nothing private

protected:
	script_int m_iUseCount;
	script_int m_iOnCapacitor;
	script_int m_iOffCapacitor;
	script_int m_iOnCapacitorTime;
	script_int m_iOffCapacitorTime;

	virtual void InitGenericVars(void)
	{
		m_iUseCount.Init();
		m_iOnCapacitor.Init();
		m_iOffCapacitor.Init();
		m_iOnCapacitorTime.Init();
		m_iOffCapacitorTime.Init();
		return;
	}

	virtual bool AnyUsesLeft(bool bOn);
	virtual bool CheckCapacitor(bool bOn, int iTime);
	virtual MSGHANDLER OnPreActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnPreDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual MSGHANDLER OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) = 0;
	virtual MSGHANDLER OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace) = 0;

public:
//	Nothing public
};

#endif // NVGENERICSCRIPT_H
