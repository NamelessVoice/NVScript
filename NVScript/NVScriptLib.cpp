#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <vector>
#include <cmath>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include <lg/interface.h>
#include <lg/scrservices.h>
#include <lg/scrmanagers.h>
#include <lg/scrmsgs.h>
#include <lg/objects.h>

#include "NVScriptLib.h"
#include "ParamCache.h"
#include "ScriptLib.h"
#include "NVMaths.h"

int NVStrToObject(const char* pszName, int iSource, int iMe)
{
	if (!pszName || !*pszName)
		return 0;
	object obj = 0;
	if ( !stricmp(pszName, "[me]") )
	{
		obj = iMe;
		return obj;
	}
	if ( !stricmp(pszName, "[source]") )
	{
		obj = iSource;
		return obj;
	}
	SService<IObjectSrv> pOS(g_pScriptManager);
	if ( pszName[0] == '^' )
	{
		float fNearDistance = 0.0f;
		int itemsFound = 0;

		boolean distanceLimited = false;
		char modifierChar = 0;
		char *pszName2 = new char[strlen(pszName)+1];
		itemsFound = sscanf(pszName+1, "%c%f:%[^\n]", &modifierChar, &fNearDistance, pszName2);

		if (itemsFound == 3 && (modifierChar == '<' || modifierChar == '{')) {
			distanceLimited = true;
		} else {
			strcpy(pszName2, pszName+1);
		}

		#if (_DARKGAME == 2)
			if (!distanceLimited) {
				pOS->FindClosestObjectNamed(obj, iMe, pszName2);
			} else {
		#endif
			cScrVec vPosition, vDestPos;

			int iChild = 0;
			object oNearest = 0;
			float fDistance = 0.00f, fNearestDistance = 0.00f;

			object oDest = NVStrToObject(pszName2, iSource, iMe);

			SInterface<ITraitManager> pTraits(g_pScriptManager);

			if ( oDest > 0 )
			{
				oDest = pTraits->GetArchetype(oDest); // Make a note of object's archetype.
			}
			// Use the ITraitManager to affect all children of this archetype.
			IObjectQuery* pInheritance = pTraits->Query(oDest, kTraitQueryChildren | kTraitQueryFull);
			if (!pInheritance) {
				delete[] pszName2;
				return 0;
			}

			pOS->Position(vPosition, iMe);

			while (! pInheritance->Done())
			{
				iChild = pInheritance->Object();
				if ( iChild > 0)
				{
					pOS->Position(vDestPos, iChild);

					if (!distanceLimited || modifierChar == '<') {
						fDistance = GetDistanceXY(&vPosition, &vDestPos);
					} else {
						fDistance = GetDistance(&vPosition, &vDestPos);
					}
					if ( ( oNearest == 0 ) || ( fDistance < fNearestDistance ) )
					{
						if (!distanceLimited || fDistance <= fNearDistance) {
							fNearestDistance = fDistance;
							oNearest = iChild;
						}
					}
				}
				pInheritance->Next();
			}
			pInheritance->Release();

			delete[] pszName2;
			return oNearest;

//			DisplayPrintf("NVSCRIPT ERROR: 'The closest object that inherits from' function is only available in Thief 2!");
		#if (_DARKGAME == 2)
			}
		#endif

		delete[] pszName2;
	} else {
		pOS->Named(obj, pszName);
	}

	if (obj == 0)
	{
		char *p = NULL;
		obj = strtol(pszName, &p, 10);
		true_bool _p;
		if (! *pOS->Exists(_p, obj))
			return 0;
	}
	return obj;
}

int NVStrToObject(const char* pszName, sScrMsg* pMsg)
{
	int iSource = pMsg->from;
	if ( strstr(pMsg->message, "Stimulus") )
	{
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);
		sLink slSource;

		pLTS->LinkGet(iSource = static_cast<sStimMsg*>(pMsg)->source, slSource);

		iSource = slSource.source;
	}

	return NVStrToObject(pszName, iSource, pMsg->to);
}

float GetDistance(const mxs_vector* vSource, const mxs_vector* vDest)
{
	float fX, fY, fZ;
	fX = vDest->x - vSource->x;
	fY = vDest->y - vSource->y;
	fZ = vDest->z - vSource->z;
	return sqrt((fX * fX)+(fY * fY)+(fZ * fZ));
}
float GetDistanceXY(const mxs_vector* vSource, const mxs_vector* vDest)
{
	float fX, fY;
	fX = vDest->x - vSource->x;
	fY = vDest->y - vSource->y;
	return sqrt((fX * fX)+(fY * fY));
}

int GetSinglePossession(int iContainer, int iType)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	true_bool bBool;

	#if ( _DARKGAME == 3 )
		SService<ILinkToolsSrv> pLTS(g_pScriptManager);

		SService<ILinkSrv> pLS(g_pScriptManager);
		linkset lsLinks;
		pLS->GetAll(lsLinks, pLTS->LinkKindNamed("Contains"), iContainer, 0);

		for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
		{
			sLink sl = lsLinks.Get();

			pOS->InheritsFrom(bBool, sl.dest, iType);
			if ( bBool )
			{
				return sl.dest;
			}
		}
	# else
		SInterface<IContainSys> pContainSys(g_pScriptManager);

		sContainIter* pContainIter;
		pContainIter = pContainSys->IterStart(iContainer);
		while (pContainIter )
		{
			pOS->InheritsFrom(bBool, pContainIter->Object, iType);
			if ( bBool )
			{
				int gotObj = pContainIter->Object;
				pContainSys->IterEnd(pContainIter);
				return gotObj;
			}

			if ( !pContainSys->IterNext(pContainIter) )
				break;
		}
		pContainSys->IterEnd(pContainIter);
	#endif

	return 0;
}

long GetAnyWeightedLink(int iSource, int iDest, sLink* pResult)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);

	std::vector<long> vLinks;
	linkset lsLinks;
	linkkind lkFlavor = pLTS->LinkKindNamed("ScriptParams");
	pLS->GetAll(lsLinks, lkFlavor, iSource, iDest);
	cMultiParm mpReturn;
	int iLinkWeight;
	int i;
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		pLTS->LinkGetData(mpReturn, lsLinks.Link(), NULL);
		iLinkWeight = static_cast<int>(mpReturn);
		if (!iLinkWeight)
			iLinkWeight = 1;
		for (i=1; i <= iLinkWeight; i++)
		{
			vLinks.insert(vLinks.begin(), lsLinks.query->ID());
		}
	}

	long iLinkID = 0;
	if (!vLinks.empty())
	{

	#if (_DARKGAME != 3)
		SService<IDataSrv> pDS(g_pScriptManager);
		iLinkID = vLinks.at(pDS->RandInt(0, vLinks.size() - 1));
	# else
		SService<IShockGameSrv> pShockGame(g_pScriptManager);
		iLinkID = vLinks.at(pShockGame->RandRange(0, vLinks.size() - 1));
	#endif

		if (iLinkID && pResult)
			pLTS->LinkGet(iLinkID, *pResult);
	}
	return iLinkID;
}

extern cParamCache* g_pGlobalParamCache;

int ParamGetNVObject(int iObject, const char* pszParam, int iDefault)
{
	if (!g_pGlobalParamCache)
		return iDefault;
	const std::string* psParam = g_pGlobalParamCache->Retrieve(iObject, pszParam);
	if (!psParam)
		return iDefault;
	return NVStrToObject(psParam->c_str(), iObject, iObject);
}

int NVParamGetInt(int iObject, const char* pszScriptName, const char* pszParam, int iDefault)
{
	char *pszParamName = new char[strlen(pszParam) + strlen(pszScriptName) + 1];
	sprintf(pszParamName, "%s%s", pszScriptName, pszParam);
	int iReturn = ParamGetInt(iObject, pszParamName, iDefault);

	delete[] pszParamName;
	return iReturn;
}

void NVParamSetInt(int iObject, const char* pszScriptName, const char* pszParam, int iValue)
{
	char *pszParamName = new char[strlen(pszParam) + strlen(pszScriptName) + 1];
	sprintf(pszParamName, "%s%s", pszScriptName, pszParam);
	ParamSetInt(iObject, pszParamName, iValue);

	delete[] pszParamName;
}

float NVParamGetFloat(int iObject, const char* pszScriptName, const char* pszParam, float fDefault)
{
	char *pszParamName = new char[strlen(pszParam) + strlen(pszScriptName) + 1];
	sprintf(pszParamName, "%s%s", pszScriptName, pszParam);
	float fReturn = ParamGetFloat(iObject, pszParamName, fDefault);

	delete[] pszParamName;
	return fReturn;
}

const char* NVParamGetString(int iObject, const char* pszScriptName, const char* pszParam, const char* sDefault)
{
	char *pszParamName = new char[strlen(pszParam) + strlen(pszScriptName) + 1];
	sprintf(pszParamName, "%s%s", pszScriptName, pszParam);
	const char* sReturn = ParamGetString(iObject, pszParamName, sDefault);

	delete[] pszParamName;
	return sReturn;
}

int NVParamGetNVObject(int iObject, const char* pszScriptName, const char* pszParam, int iDefault)
{
	char *pszParamName = new char[strlen(pszParam) + strlen(pszScriptName) + 1];
	sprintf(pszParamName, "%s%s", pszScriptName, pszParam);
	int iReturn = ParamGetNVObject(iObject, pszParamName, iDefault);

	delete[] pszParamName;
	return iReturn;
}


std::vector<int> GetObjects(const char* pszDest, int iObj, sScrMsg* pMsg)
{
	std::vector<int> oList;

	if ( pszDest )
	{
		bool NearestUseXYZ = false;
		object oDest = 0;
		long iLinkFlavour = 0;
		float fNearDistance = 0.00f;

		int iGetTraitTree = 0;
		switch ( pszDest[0] )
		{
			case '&':
			{
				SService<ILinkToolsSrv> pLTS(g_pScriptManager);
				SService<ILinkSrv> pLS(g_pScriptManager);
				const char* pszLinkName = (pszDest +1);

				if ( strlen(pszLinkName) > 0 )
				{
					if ( pszLinkName[0] == '?' )
					{ // Choose one random link.
						sLink slDest;
						pszLinkName++;
						long lDest = 0;

						if (!stricmp(pszLinkName, "Weighted"))
						{
							lDest = GetAnyWeightedLink(iObj, 0, &slDest);
						} else {
							lDest = GetAnyLink(pszLinkName, iObj, 0, &slDest);
						}
						if ( lDest )
						{
							oDest = slDest.dest;
							oList.push_back(oDest);
		//					if ( iKillLinks )
		//					{
		//						pLS->Destroy(lDest);
		//					}
						}
					} else {
						iLinkFlavour = pLTS->LinkKindNamed(pszLinkName);

						// Go through all the links and add them to the list
						if ( iLinkFlavour )
						{
							cScrStr sLinkName;
							pLTS->LinkKindName(sLinkName, iLinkFlavour);
							// IterateLinks won't work from outside an cScript, so we have to do things differently
							linkset lsLinks;
							pLS->GetAll(lsLinks, iLinkFlavour, iObj, 0);
							for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
							{
								sLink sl = lsLinks.Get();

								oList.push_back(sl.dest);
							}
						}
					}
				}

				break;
			}
			case '{':
				NearestUseXYZ = true;
			case '<':
			{
				char *pszDest2 = new char[strlen(pszDest)+1];
				int found = sscanf(pszDest+1, "%f:%[^\n]", &fNearDistance, pszDest2);
				if (found != 2) {
					ErrorPrintf("Invalid search criteria for distance-based matcher: '%s' on %s (%i)", pszDest, static_cast<const char*>(GetObjectDescriptiveName(iObj)), iObj);
					delete[] pszDest2;
					return oList;
				}
				oDest = NVStrToObject(pszDest2, pMsg);
				delete[] pszDest2;
			}
			case '@':
				iGetTraitTree = kTraitQueryFull;
			case '*':
			{
				// Affect all objects of a specific type.
				if ( (pszDest[0] != '<' ) && (pszDest[0] != '{' ) )
					oDest = NVStrToObject(pszDest+1, pMsg);

				SInterface<ITraitManager> pTraits(g_pScriptManager);

				if ( oDest > 0 )
				{
					oDest = pTraits->GetArchetype(iObj); // Make a note of object's archetype.
				}
				// Use the ITraitManager to affect all children of this archetype.
				IObjectQuery* pInheritance = pTraits->Query(oDest, kTraitQueryChildren | iGetTraitTree);
				if (!pInheritance)
				{
					break;
				}

				SService<IObjectSrv> pOS(g_pScriptManager);

				cScrVec vPosition, vDestPos;

				// Don't bother trying to get the position of an object if it is not necessary.
				// This is mainly a cheap way to avoid a crash when getting the position of an
				// object during BeginScript, when it hasn't been positioned properly yet.
				if ( fNearDistance > 0.00f )
				{
					pOS->Position(vPosition, iObj);
				} else {
					vPosition = cScrVec(0.0f, 0.0f, 0.0f);
				}

				while (! pInheritance->Done())
				{
					int iChild = pInheritance->Object();
					if ( iChild > 0)
					{
						if ( fNearDistance > 0.00f )
						{
							true_bool bExists;
							pOS->Exists(bExists, iObj);
							if (bExists)
							{
								pOS->Position(vDestPos, iChild);

								if ( (( NearestUseXYZ == false ) && ( GetDistanceXY(&vPosition, &vDestPos) > fNearDistance ))
								|| (( NearestUseXYZ == true ) && ( GetDistance(&vPosition, &vDestPos) > fNearDistance )) )
								{
									pInheritance->Next();
									continue;
								}
							} else {
								ErrorPrintf("Attempting to get location of object which has already been destroyed, for nearest-object trigger using '%s' on %s (%i).", pszDest, static_cast<const char*>(GetObjectDescriptiveName(iObj)), iObj);
							}
						}
						oList.push_back(iChild);
					}
					pInheritance->Next();
				}
				pInheritance->Release();

				break;
			}
			default: // Affect a single object
				oDest = NVStrToObject(pszDest, pMsg);
				oList.push_back(oDest);
		}
	}

	return oList;
}

cScrStr GetObjectDescriptiveName(int objId)
{
	// Get the name of the object
		SInterface<ITraitManager> pTraits(g_pScriptManager);
		int iArchetype = 0;

		SService<IObjectSrv> pOS(g_pScriptManager);
		cScrStr strToName;
		pOS->GetName(strToName, objId);

		if ( strToName.IsEmpty() )
		{	// If the object has no name, use its archetype name
			iArchetype = pTraits->GetArchetype(objId); // Make a note of object's archetype.
			pOS->GetName(strToName, iArchetype);
		}

		return strToName;
}

void ErrorPrintf(const char* pszMessage, const char* pszFormat, ...)
{
		char *pszMessageWithPrefix = new char[strlen(pszMessage)+17];
		snprintf(pszMessageWithPrefix, strlen(pszMessage)+16, "NVScript Error: %s", pszMessage);
		DisplayPrintf(0x0000FF, pszMessageWithPrefix, pszFormat);
		DebugPrintf(pszFormat);
		delete[] pszMessageWithPrefix;
}

/***********************
 * Functions for getting info about a qvar
 */

bool GetQVarInfo(int iQVarObjId, char* cOperand, int* iValue, char** pszQVar, int* iValueLength)
{
	SService<IPropertySrv> pPropSrv(g_pScriptManager);

	if (!pPropSrv->Possessed(iQVarObjId, QVARPROP))
	{
		return false; // This object doesn't have Trap->Quest Var - abort.
	}

	cMultiParm mpQVar;
	pPropSrv->Get(mpQVar, iQVarObjId, QVARPROP, NULL);

	GetQVarInfo(mpQVar.psz, cOperand, iValue, pszQVar, iValueLength);
	return true;
}

bool GetQVarInfo(char* pszQVarString, char* cOperand, int* iValue, char** pszQVar, int* iValueLength)
{
	if (*pszQVar != NULL)
	{
		delete[] *pszQVar;
		*pszQVar = NULL;
	}

	if (strlen(pszQVarString) < 5)
		return false;	// Pointless. It can't possibly contain an op, value, colon and qvar.

	*iValue = 0; // Default value, if no value or QVar is specified.
	*cOperand = pszQVarString[0];

	char* pszQuestVar = strchr(pszQVarString+1, ':');	// Get pointer to the string starting with the colon
	if ( !pszQuestVar )
	{
		// Ooops, no colon.
		*pszQVar = NULL;
		return false;
	}

	pszQuestVar[0] = '\0';
	pszQuestVar++; // We're not interested in the position of the colon, but of what's after it.

	*pszQVar = new char[strlen(pszQuestVar) +1];
	strcpy(*pszQVar, pszQuestVar);
//	DebugPrintf("pszQVar is %s\npszQuestVar is %s", *pszQVar, pszQuestVar);


	// Maths time!
	NVMathsOperations* m = NVMathsOperations::getInstance();

	*iValue = m->calculate(pszQVarString + 1 );

	// Calculate the length of the number (used for the " operator)
	int digits = 0;
	if ( *iValue == 0 )
	{
		digits = 1;
	} else {
		int x2 = *iValue;
		while ( x2 > 0 )
		{
			++digits;
			x2 /= 10;
		}
	}

	*iValueLength = digits;

	return true;
}
