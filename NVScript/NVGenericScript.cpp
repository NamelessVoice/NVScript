//#include "lg/interface.h"
#include "lg/scrservices.h"
#include "ScriptLib.h"
#include "NVGenericScript.h"

long __stdcall cScrNVGeneric::ReceiveMessage(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	/******************
	 * Override the main ReceiveMessage function:
	 * This function first sends messages to the main message handling hub.
	 * It then catches the specified On and Off messages and passes them on
	 * to their appropriate function.
	 * The "ResetCount" message is also handled directly here.
	 */
	long iRet = 1;
	try
	{
		if (!stricmp(pMsg->message, "BeginScript")) {
			InitGenericVars();
		}

		// Call parent ReceiveMessage:
		iRet = cScript::ReceiveMessage(pMsg, pReply, eTrace);
		
/*		
		char* pszError = new char[strlen(pMsg->message) + 40];
		sprintf(pszError, "Message: %s (%i)", pMsg->message, m_iObjId);
		DisplayPrintf(pszError);
		DebugPrintf(pszError);
		delete[] pszError;
		//return iRet;
		
		if (!stricmp(pMsg->message, "BeginScript" ))
		{
			DisplayPrintf("BeginScript message ignored.");
			DebugPrintf("BeginScript message ignored.");
//			return iRet;
		}
*/

		if (!stricmp(pMsg->message, "Sim")) {
			if (!static_cast<sSimMsg*>(pMsg)->fStarting) {
				return iRet;
			}
		}

		char* pszParam = new char[strlen(m_szName) + 4];
			sprintf(pszParam, "%sOn", m_szName);
			const char* pszMessageOn = ParamGetString(m_iObjId, pszParam, "TurnOn");
				
			sprintf(pszParam, "%sOff", m_szName);
			const char* pszMessageOff = ParamGetString(m_iObjId, pszParam, "TurnOff");	
		delete[] pszParam;
		
		if (!stricmp(pMsg->message, pszMessageOn) )
		{
			// Null is supposed to be reserved for 'no effect', not a to be an actual message in its own right!
			if (stricmp(pszMessageOn, "null"))
			{
				OnPreActivate(pMsg, pReply, eTrace);
			}
		} //else
		if (!stricmp(pMsg->message, pszMessageOff) )
		{
			// Null is supposed to be reserved for 'no effect', not a to be an actual message in its own right!
			if (stricmp(pszMessageOff, "null"))
			{
				OnPreDeactivate(pMsg, pReply, eTrace);
			}
		}
		if ( !stricmp(pMsg->message, "ResetCount") )
		{
			m_iUseCount = 0;
		}
	}
	catch (std::exception& err)
	{
		DebugString("Exception caught: \n", err.what());
	}
	return iRet;
}

MSGHANDLER cScrNVGeneric::OnPreActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( CheckCapacitor(true, pMsg->time) )
		if ( AnyUsesLeft(true) )
			OnActivate(pMsg, pReply, eTrace);
			
	return 0;
}
MSGHANDLER cScrNVGeneric::OnPreDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if ( CheckCapacitor(false, pMsg->time	) )
		if ( AnyUsesLeft(false) )
			OnDeactivate(pMsg, pReply, eTrace);

	return 0;
}

bool cScrNVGeneric::CheckCapacitor(bool bOn, int iTime)
{
	char* pszParam = new char[strlen(m_szName) + 14];
	char* pszParam2 = new char[strlen(m_szName) + 22];
	
		if ( bOn )
		{
			sprintf(pszParam, "%sOnCapacitor", m_szName);
			sprintf(pszParam2, "%sOnCapacitorFalloff", m_szName);
		} else {
			sprintf(pszParam, "%sOffCapacitor", m_szName);
			sprintf(pszParam2, "%sOffCapacitorFalloff", m_szName);
		}
			
		int iCapacitor = ParamGetInt(m_iObjId, pszParam, 0);
		int iCapacitorFalloff = ParamGetInt(m_iObjId, pszParam2, 0);
		
		if ( iCapacitor == 0 )
		{
			sprintf(pszParam, "%sCapacitor", m_szName);
			iCapacitor = ParamGetInt(m_iObjId, pszParam, 0);
		}
		
		if ( iCapacitorFalloff == 0 )
		{
			sprintf(pszParam2, "%sCapacitorFalloff", m_szName);
			iCapacitorFalloff = ParamGetInt(m_iObjId, pszParam2, 0);
		}
		
		delete[] pszParam;
		delete[] pszParam2;
		
		if ( iCapacitor == 0 )
			return true;
			
		if ( iCapacitorFalloff > 0 )
		{
			int iFalloff = iTime;
			
			if ( bOn )
			{
				iFalloff -= static_cast<int>(m_iOnCapacitorTime);
				m_iOnCapacitor -= (iFalloff / iCapacitorFalloff);
//				DisplayPrintf("On Capacitor falloff reduces capacitor by %i", (iFalloff / iCapacitorFalloff));
				
				if ( m_iOnCapacitor < 0 )
					m_iOnCapacitor = 0;
			} else {
				iFalloff -= static_cast<int>(m_iOffCapacitorTime);
				m_iOffCapacitor -= (iFalloff / iCapacitorFalloff);
//				DisplayPrintf("Off Capacitor falloff reduces capacitor by %i", (iFalloff / iCapacitorFalloff));
				
				if ( m_iOffCapacitor < 0 )
					m_iOffCapacitor = 0;
			}
				
		}
			
		if ( bOn )
		{
			m_iOnCapacitor++;
//			DisplayPrintf("%s: On Capacitor is %i of %i", m_szName, static_cast<int>(m_iOnCapacitor), iCapacitor);
//			DebugPrintf("%s: On Capacitor is %i of %i", m_szName, static_cast<int>(m_iOnCapacitor), iCapacitor);
			m_iOnCapacitorTime = iTime;
			if (m_iOnCapacitor >= iCapacitor )
			{
				m_iOnCapacitor = 0;
				return true;
			}
		} else {
			m_iOffCapacitor++;
//			DisplayPrintf("%s: Off Capacitor is %i of %i", m_szName, static_cast<int>(m_iOffCapacitor), iCapacitor);
//			DebugPrintf("%s: Off Capacitor is %i of %i", m_szName, static_cast<int>(m_iOffCapacitor), iCapacitor);
			m_iOffCapacitorTime = iTime;
			if (m_iOffCapacitor >= iCapacitor )
			{
				m_iOffCapacitor = 0;
				return true;
			}
		}
			
	return false;

}

bool cScrNVGeneric::AnyUsesLeft(bool bOn)
{
	char* pszParam = new char[strlen(m_szName) + 10];
		sprintf(pszParam, "%sCount", m_szName);
		int iMaxUses = ParamGetInt(m_iObjId, pszParam, 0);

		sprintf(pszParam, "%sCountOnly", m_szName);
		int iCountOnly = ParamGetInt(m_iObjId, pszParam, 0);
	delete[] pszParam;
	
	if ( iMaxUses )
	{
		if ( (1 + bOn) != iCountOnly) // If bOn is -1 then it will match 2, otherwise it will match 1.
		{
			if ( m_iUseCount < iMaxUses )
			{
//				DisplayPrintf("%s Ok: Uses: %i of %i", m_szName, static_cast<int>(m_iUseCount), iMaxUses);
//				DebugPrintf("%s Ok: Uses: %i of %i", m_szName, static_cast<int>(m_iUseCount), iMaxUses);
				m_iUseCount++;
				return true;
			} else {
//				DisplayPrintf("%s Fail: Uses: %i of %i", m_szName, static_cast<int>(m_iUseCount), iMaxUses);
//				DebugPrintf("%s Fail: Uses: %i of %i", m_szName, static_cast<int>(m_iUseCount), iMaxUses);
				return false;
			}
		}
	}
	
	return true;
}
