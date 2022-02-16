/************************************************
 * General-purpose Utilities for script modules
 */

#ifndef NVSCRIPTLIB_H
#define NVSCRIPTLIB_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>

#if (_DARKGAME == 3)
	#define QVARPROP "QBName"
#else
	#define QVARPROP "TrapQVar"
#endif


/*************
 * Objects
 */

// Convert a string to an object reference. Understands [Me], [Source], and ^ (closest)
int NVStrToObject(const char* pszName, sScrMsg* pMsg);
int NVStrToObject(const char* pszName, int iSource, int iMe);

std::vector<int> GetObjects(const char* pszDest, int iObj, sScrMsg* pMsg);

// Get the distance between two points, using the Theorem of Pythagoras.
float GetDistance(const mxs_vector* vSource, const mxs_vector* vDest);
// Get the distance between two points on the X and Y axes only. The Z axis is ignored, using the Theorem of Pythagoras.
float GetDistanceXY(const mxs_vector* vSource, const mxs_vector* vDest);

// Returns the first object contained by iContainer that inherits from iType.
int GetSinglePossession(int iContainer, int iType);

// Gets a random ScriptParams link, treating data as a 'weight'.
long GetAnyWeightedLink(int iSource, int iDest, sLink* pResult);

int ParamGetNVObject(int iObject, const char* pszParam, int iDefault = 0);

int NVParamGetInt(int iObject, const char* pszScriptName, const char* pszParam, int iDefault);
void NVParamSetInt(int iObject, const char* pszScriptName, const char* pszParam, int iValue);
float NVParamGetFloat(int iObject, const char* pszScriptName, const char* pszParam, float fDefault);
const char* NVParamGetString(int iObject, const char* pszScriptName, const char* pszParam, const char* sDefault);
int NVParamGetNVObject(int iObject, const char* pszScriptName, const char* pszParam, int iDefault);

// Get the descriptive name (either its name, or the name of its archetype) of an object
cScrStr GetObjectDescriptiveName(int objId);

void ErrorPrintf(const char* pszMessage, const char* pszFormat, ...);

bool GetQVarInfo(int iQVarObjId, char* cOperand, int* iValue, char** pszQVar, int* iValueLength);
bool GetQVarInfo(char* pszQVarString, char* cOperand, int* iValue, char** pszQVar, int* iValueLength);

#endif // NVSCRIPTLIB_H
