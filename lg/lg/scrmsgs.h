/************************
 * LGS Script Messages
 */

/*
 *  This is difficult.
 *  sScrMsg is one of the few (the only?) classes a script is likely to handle
 *  that (a) has multiple bases, and (b) uses MSVC calling semantics.
 *  The declaration is:
 *    struct sScrMsg : public sScrMsgBase, public sPersistent;
 *  I think. To be honest, I really have no fscking idea what the hell is
 *  going on. It's just a three-way collision between ANSI C++, MSVC, and GCC.
 *  What it boils down to is that it's impossible to implement this class in any 
 *  reasonably portable way.
 *
 *  What do we do about it?
 *  Well, what I'm going to do is build the sScrMsg instances the hard way, 
 *  by manually constructing the vtable. Calls to members of sPersistent 
 *  will be handled with cdecl glue functions.
 *  
 *  MSVC++ users may be able to get it to work directly.  I wouldn't know.
 *
 *  Oh, and to make things even more interesting, the GayleSaver scripts 
 *  occasionally send messages with an invalid sPersistent vtable.
 *
 */

#ifndef _LG_SCRMSGS_H
#define _LG_SCRMSGS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <unknwn.h>

#include <lg/config.h>
#include <lg/types.h>
#include <lg/defs.h>


struct sScrMsgBase : IUnknown
{
	sScrMsgBase()
		{ m_iRef = 1; }
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
	int  m_iRef;
};

// These are __thiscall, not __stdcall
// Which means they are subject to myriad incompatibilities
#ifdef _MSC_VER
struct sPersistent
{
	virtual ~sPersistent()
		{ }
	virtual int __thiscall Persistence()
		{ return 1; }
	virtual const char* __thiscall GetName() const = 0;
};
#else
typedef void (*fnPersistent_dtor)(void*);
typedef int (*fnPersistent_Persistence)();
typedef const char* (*fnPersistent_GetName)();
struct sPersistent_vtable
{
	fnPersistent_dtor thunk_dtor;
	fnPersistent_Persistence thunk_Persistence;
	fnPersistent_GetName thunk_GetName;
};
#endif // _MSC_VER

// I don't know if this will work. It's worth a try...
#ifdef _MSC_VER
struct sScrMsg : public sScrMsgBase, public sPersistent 
{
#else
struct sScrMsg : public sScrMsgBase
{
	// For non-MS compilers
	sPersistent_vtable *persistent_hack;
#endif

	int   from;
	int   to;
	const char* message;
	int   time;
	int   flags;
	// I've reverted these back to cMultiParm 
	// to take advantage of operator overloading 
	cMultiParm data;
	cMultiParm data2;
	cMultiParm data3;

	sScrMsg();
	virtual ~sScrMsg();

	// These have been moved here from what is supposed to be the
	// sPersistent base class.  The hacked vtable will call the 
	// real function.
	virtual int __thiscall Persistence() {
		return 0;
	}
	virtual const char* __thiscall GetName() const {
		return "sScrMsg";
	}

#ifndef _MSC_VER
	// Functions that make sScrMsg work, against all odds.
	int Persistent_Persistence(void);
	const char* Persistent_GetName(void) const;
#endif
};

enum eScrMsgFlags
{
	kScrMsgSent = 1,
	kScrMsgAbort = 2,
	kScrMsgSendToProxy = 4,
	kScrMsgPostToOwner = 8
};


// The classes below are what we actually instantiate.

struct sGenericScrMsg : sScrMsg
{
	virtual const char* __thiscall GetName() const {
		return "sScrMsg";
	}
};

// "ScriptPtrQuery"
struct sPtrQueryMsg : sScrMsg
{
	const char*   pszDestClass;
	void**     pScriptReceptacle;

	virtual const char* __thiscall GetName() const { return "sPtrQueryMsg"; }
};

// "Sim"
// was actually a true_bool, but who gives a fsck...
struct sSimMsg : sScrMsg
{
	BOOL fStarting;

	virtual const char* __thiscall GetName() const { return "sSimMsg"; }
};

// "DarkGameModeChange"
struct sDarkGameModeScrMsg : sScrMsg
{
	BOOL fEntering;

	virtual const char* __thiscall GetName() const { return "sDarkGameModeScrMsg"; }
};

// "AIModeChange"
struct sAIModeChangeMsg : sScrMsg
{
	eAIMode mode;
	eAIMode previous_mode;

	virtual const char* __thiscall GetName() const { return "sAIModeChangeMsg"; }
};

// "Alertness"
struct sAIAlertnessMsg : sScrMsg
{
	eAIScriptAlertLevel  level;
	eAIScriptAlertLevel  oldLevel;

	virtual const char* __thiscall GetName() const { return "sAIAlertnessMsg"; }
};

// "HighAlert"
struct sAIHighAlertMsg: sScrMsg 
{
	eAIScriptAlertLevel  level;
	eAIScriptAlertLevel  oldLevel;

	virtual const char* __thiscall GetName() const { return "sAIHighAlertMsg"; }
};

struct sAIResultMsg : sScrMsg
{
	eAIAction action;
	eAIActionResult result;
	cMultiParm result_data;

	virtual const char* __thiscall GetName() const { return "sAIResultMsg"; }
};

// "ObjActResult"
struct sAIObjActResultMsg : sAIResultMsg
{
	int target;

	virtual const char* __thiscall GetName() const { return "sAIObjActResultMsg"; }
};

// "PatrolPoint"
struct sAIPatrolPointMsg : sScrMsg
{
	int   patrolObj;

	virtual const char* __thiscall GetName() const { return "sAIPatrolPointMsg"; }
};

// "SignalAI"
struct sAISignalMsg : sScrMsg
{
	cScrStr  signal;

	virtual const char* __thiscall GetName() const { return "sAISignalMsg"; }
};

// "StartAttack", "EndAttack", "StartWindup"
struct sAttackMsg : sScrMsg
{
	int weapon;

	virtual const char* __thiscall GetName() const { return "sAttackMsg"; }
};

// "Combine"
struct sCombineScrMsg : sScrMsg
{
	int  combiner;

	virtual const char* __thiscall GetName() const { return "sCombineScrMsg"; }
};
// "Contained"
struct sContainedScrMsg : sScrMsg
{
	int    event;
	int    container;

	virtual const char* __thiscall GetName() const { return "sContainedScrMsg"; }
};
// "Container"
struct sContainerScrMsg : sScrMsg
{
	int    event;
	int    containee;

	virtual const char* __thiscall GetName() const { return "sContainerScrMsg"; }
};

// "Damage"
struct sDamageScrMsg : sScrMsg
{
	int kind;
	int damage;
	int culprit;

	virtual const char* __thiscall GetName() const { return "sDamageScrMsg"; }
};

// "Difficulty"
struct sDiffScrMsg : sScrMsg
{
	int  difficulty;

	virtual const char* __thiscall GetName() const { return "sDiffScrMsg"; }
};

// "Door..."
struct sDoorMsg : sScrMsg
{
	eDoorAction ActionType;
	eDoorAction PrevActionType;

	virtual const char* __thiscall GetName() const { return "sDoorMsg"; }
};

// "Frob...Begin", "Frob...End", et. al.
struct sFrobMsg : sScrMsg
{
	int SrcObjId, DstObjId;
	int Frobber;
	eFrobLoc SrcLoc;
	eFrobLoc DstLoc;
	float   Sec;
	int     Abort;

	virtual const char* __thiscall GetName() const { return "sFrobMsg"; }
};

// "MotionStart", "MotionEnd", "MotionFlagReached"
struct sBodyMsg : sScrMsg
{
	enum eBodyAction
	{
		kMotionStart,
		kMotionEnd,
		kMotionFlagReached
	};
	eBodyAction ActionType;
	cScrStr MotionName;
	int FlagValue;

	virtual const char* __thiscall GetName() const { return "sBodyMsg"; }
};

// "PickStateChange" Sent when AdvPickCfg is in effect.
struct sPickStateScrMsg : sScrMsg
{
	int PrevState;
	int NewState;

	virtual const char* __thiscall GetName() const { return "sPickStateScrMsg"; }
};

// "PressurePlate..."
struct sPressurePlateMsg: sScrMsg
{
	// no data... go fig.

	virtual const char* __thiscall GetName() const { return "sPressurePlateMsg"; }
};

// "Phys..."
struct sPhysMsg : sScrMsg
{
	int Submod;
	int collType;
	int collObj;
	int collSubmod;
	float collMomentum;
	mxs_vector collNormal;
	mxs_vector collPt;
	int contactType;
	int contactObj;
	int contactSubmod;
	int transObj;
	int transSubmod;

	virtual const char* __thiscall GetName() const { return "sPhysMsg"; }
};

// "ReportMessage"
struct sReportMsg : sScrMsg
{
	int  WarnLevel;
	int  Flags;
	int  Types;
	const char* TextBuffer;

	virtual const char* __thiscall GetName() const { return "sReportMsg"; }
};

// "...RoomEnter", "...RoomExit"
struct sRoomMsg : sScrMsg
{
	enum eTransType
	{
		kEnter,
		kExit,
		kRoomTransit
	};
	enum eObjType
	{
		kPlayer,
		kRemotePlayer,
		kCreature,
		kObject,
		kNull
	};
	int FromObjId;
	int ToObjId;
	int MoveObjId;
	eObjType ObjType;
	eTransType TransitionType;

	virtual const char* __thiscall GetName() const { return "sRoomMsg"; }
};

// "Slain"
struct sSlayMsg : sScrMsg
{
	int culprit;
	int kind;

	virtual const char* __thiscall GetName() const { return "sSlayMsg"; }
};

// "SchemaDone"
struct sSchemaDoneMsg : sScrMsg
{
	mxs_vector coordinates;
	int targetObject;
	cScrStr  name;

	virtual const char* __thiscall GetName() const { return "sSchemaDoneMsg"; }
};
// "SoundDone"
struct sSoundDoneMsg : sScrMsg
{
	mxs_vector coordinates;
	int targetObject;
	cScrStr  name;

	virtual const char* __thiscall GetName() const { return "sSoundDoneMsg"; }
};

// "...Stimulus"
struct sStimMsg : sScrMsg
{
	object stimulus;
	float intensity;
	long   sensor;	// StimSensor (tSensorCount)
	long   source;	// arSrc (sStimSourceData)

	virtual const char* __thiscall GetName() const { return "sStimMsg"; }
};

// "Timer"
struct sScrTimerMsg : sScrMsg
{
	cScrStr name;

	virtual const char* __thiscall GetName() const { return "sScrTimerMsg"; }
};

// "TweqComplete"
struct sTweqMsg : sScrMsg
{
	eTweqType       Type;
	eTweqOperation  Op;
	eTweqDirection  Dir;

	virtual const char* __thiscall GetName() const { return "sTweqMsg"; }
};

// "WaypointReached" and "MovingTerrainWaypoint"
struct sWaypointMsg : sScrMsg
{
	int  moving_terrain;

	virtual const char* __thiscall GetName() const { return "sWaypointMsg"; }
};
struct sMovingTerrainMsg : sScrMsg
{
	int  waypoint;

	virtual const char* __thiscall GetName() const { return "sMovingTerrainMsg"; }
};

// "QuestChange"
struct sQuestMsg : sScrMsg
{
	const char* m_pName;
	int  m_oldValue;
	int  m_newValue;

	virtual const char* __thiscall GetName() const { return "sQuestMsg"; }
};

// "MediumTransition"
// Solid = 0, Air = 1, Water = 2
struct sMediumTransMsg : sScrMsg
{
	int nFromType;
	int nToType;

	virtual const char* __thiscall GetName() const { return "sMediumTransMsg"; }
};

/* These messages are only used in Sshock2 (and make a cameo in T1/G)
 * To allow liblg to be universal, I'm not going to #ifdef them.
 */
// "YorNDone"
struct sYorNMsg : sScrMsg
{
	bool YorN;

	virtual const char* __thiscall GetName() const { return "sYorNMsg"; }
};

// "KeypadDone"
struct sKeypadMsg : sScrMsg
{
	int code;

	virtual const char* __thiscall GetName() const { return "sKeypadMsg"; }
};


#endif // _LG_SCRMSGS_H
