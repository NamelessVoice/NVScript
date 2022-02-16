//#if _DARKGAME != 2
//#error NVDebug is written for Thief2. You can try, but I doubt it.
//#else

// #include <cstdlib>
//#include <cmath>

//#include "lg/interface.h"
#include "lg/scrservices.h"
//#include "lg/scrmanagers.h"
#include "lg/scrmsgs.h"
#include "ScriptLib.h"

#define INITOSM
#include "NVDebug.h"

#include "lg/iids.h"


/***********************
 * NVDifficultyDebug
 */

MSGHANDLER cScr_NVDifficultyDebug::OnSim(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	if (static_cast<sSimMsg*>(pMsg)->fStarting)
	{
		SService<IQuestSrv> pQSrv(g_pScriptManager);
		int iCurrent = -1; // Default value (if QVar doesn't exist)
		if (!pQSrv->Exists("Difficulty"))
		{
			if (pQSrv->Exists("DebugDifficulty"))
			{
				iCurrent = pQSrv->Get("DebugDifficulty"); // Get QVar value
				pQSrv->Set("difficulty", iCurrent, 0);
				
				// Process Difficulty:
				SService<IDebugScrSrv> pDebugSrv(g_pScriptManager);
				pDebugSrv->Command("process_difficulty", "", "", "", "", "", "", "");		
			}
		}

		// Display warning/info (to remind people to delete the object before packing the FM).
#if (_DARKGAME != 3)
		switch ( iCurrent )
		{
		case -1:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nNo difficulty defined.", m_iObjId);
			break;
		case 0:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nDifficulty processed as 'Normal' (%i)", m_iObjId, iCurrent);
			break;
		case 1:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nDifficulty processed as 'Hard' (%i)", m_iObjId, iCurrent);
			break;
		case 2:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nDifficulty processed as 'Expert' (%i)", m_iObjId, iCurrent);
			break;
		default:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nInvalid difficulty specified (%i)", m_iObjId, iCurrent);
		}
#else
switch ( iCurrent )
		{
		case -1:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nNo difficulty defined.", m_iObjId);
			break;
		case 0:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nDifficulty processed as 'Easy' (%i)", m_iObjId, iCurrent);
			break;
		case 1:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nDifficulty processed as 'Normal' (%i)", m_iObjId, iCurrent);
			break;
		case 2:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nDifficulty processed as 'Hard' (%i)", m_iObjId, iCurrent);
			break;
		case 3:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nDifficulty processed as 'Impossible' (%i)", m_iObjId, iCurrent);
			break;
		default:
			DisplayPrintf("Object %i is being used as Difficulty Debug\nInvalid difficulty specified (%i)", m_iObjId, iCurrent);
		}
#endif
	}


	return 0;
}

/***********************
 * NVObjDumper
 */

MSGHANDLER cScr_NVObjDumper::OnBeginScript(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	cScrVec vPosition;
	pOS->Position(vPosition, m_iObjId);
	
	SInterface<ITraitManager> pTraits(g_pScriptManager);
	int iArchetype = pTraits->GetArchetype(m_iObjId); // Make a note of object's archetype.
	
	cScrStr strName;
	pOS->GetName(strName, iArchetype);
	
	DebugPrintf("%s (%i) at (%f, %f, %f)", static_cast<const char*>(strName), m_iObjId, vPosition.x, vPosition.y, vPosition.z);
		
	return 0;
}
//#endif // _DARKGAME == 2
