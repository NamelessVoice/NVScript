#ifndef _LG_GEN_H_
#define _LG_GEN_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/config.h>
#include <lg/script.h>
#include <lg/scrmsgs.h>
#include <lg/types.h>

/*
 * This sucks.
 * Internal routines pass the class instance pointer in ECX,
 * and push the rest of the arguments.
 * So I have to resort to using inline assembly.
 * I doubt anything other than GCC3 will be able to handle this.
 */

#ifdef __GNUC__
typedef void (*pfScriptCall)(IScript*);

#define _DoScriptCall(__Fn,__p) \
({  pfScriptCall __pf = (pfScriptCall)((__p)->*(&__Fn)); \
	asm volatile ( "movl %1,%%ecx\n\tcall *%0" : : "r" (__pf) , "g" (__p) : "ecx" ); \
})
#define _DoScriptCall1(__Fn,__p,__a) \
({  pfScriptCall __pf = (pfScriptCall)((__p)->*(&__Fn)); \
	asm volatile ( "push %2\n\tmovl %1,%%ecx\n\tcall *%0" \
		: : "r" (__pf) , "g" (__p) , "g" (__a) : "ecx" , "memory" ); \
})
#define _DoScriptCall1a(__Fn,__p,__a) \
({  typeof(__a) __r; \
	pfScriptCall __pf = (pfScriptCall)((__p)->*(&__Fn)); \
	asm volatile ( "push %3\n\tmovl %2,%%ecx\n\tcall *%1\n\t" : "=a" (__r) \
		: "r" (__pf) , "g" (__p) , "g" (__a) : "ecx" , "memory" ); \
	__r; \
})
#define _DoScriptCall2(__Fn,__p,__a,__b) \
({  pfScriptCall __pf = (pfScriptCall)((__p)->*(&__Fn)); \
	asm volatile ( "push %3\n\tpush %2\n\tmovl %1,%%ecx\n\tcall *%0" \
		: : "r" (__pf) , "g" (__p) , "g" (__a) , "g" (__b) : "ecx" , "memory" ); \
})
#define _DoScriptCallRet(__Fn,__p,__r) \
({  pfScriptCall __pf = (pfScriptCall)((__p)->*(&__Fn)); \
	asm volatile ( "movl %2,%%ecx\n\tcall *%1" \
		: "=a" (__r) : "r" (__pf) , "g" (__p) : "ecx" ); \
	__r; \
})
#define _DoScriptCallRet1(__Fn,__p,__r,__a) \
({  pfScriptCall __pf = (pfScriptCall)((__p)->*(&__Fn)); \
	asm volatile ( "push %3\n\tmovl %2,%%ecx\n\tcall *%1" \
		: "=a" (__r) : "r" (__pf) , "g" (__p) , "g" (__a) : "ecx" , "memory" ); \
	__r; \
})
#define _DoScriptCallRet2(__Fn,__p,__r,__a,__b) \
({  pfScriptCall __pf = (pfScriptCall)((__p)->*(&__Fn)); \
	asm volatile ( "push %4\n\tpush %3\n\tmovl %2,%%ecx\n\tcall *%1" \
		: "=a" (__r) : "r" (__pf) , "g" (__p) , "g" (__a) , "g" (__b) : "ecx" , "memory" ); \
	__r; \
})
#endif
#ifdef _MSC_VER
/* MSVC++ should be able to call __thiscall functions directly. */

#define _DoScriptCall(__Fn,__p) ((__p)->__Fn())
#define _DoScriptCall1(__Fn,__p,__a) ((__p)->__Fn((__a)))
#define _DoScriptCall1a(__Fn,__p,__a) ((__p)->__Fn((__a)))
#define _DoScriptCall2(__Fn,__p,__a,__b) ((__p)->__Fn((__a),(__b)))
#define _DoScriptCall2a(__Fn,__p,__a,__b) ((__p)->__Fn((__a),(__b)))
#define _DoScriptCallRet(__Fn,__p,__r) (__r = (__p)->__Fn())
#define _DoScriptCallRet1(__Fn,__p,__r,__a) (__r = (__p)->__Fn((__a)))
#define _DoScriptCallRet2(__Fn,__p,__r,__a,__b) (__r = (__p)->__Fn((__a),(__b)))

#endif


interface IScr_RootScript : public IScript
{
	// cScript
	virtual void __thiscall fn1() = 0; // Init?
	virtual void __thiscall DoAddMsgMaps() = 0;
	virtual void __thiscall OnMessage_(sScrMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnBeginScript_(sScrMsg&,cMultiParm*) = 0; // +20
	virtual void __thiscall OnEndScript_(sScrMsg&,cMultiParm*) = 0; // +24
	virtual void __thiscall OnTimer_(sScrTimerMsg&,cMultiParm*) = 0; // +28
	virtual void __thiscall fn2() = 0; // +2C
	// cScr_EngineRootScript
	virtual void __thiscall OnTweqComplete_(sTweqMsg&,cMultiParm*) = 0; // +30
	virtual void __thiscall OnNowLocked_(sScrMsg&,cMultiParm*) = 0; // +34
	virtual void __thiscall OnNowUnlocked_(sScrMsg&,cMultiParm*) = 0; // +38
	virtual void __thiscall OnSoundDone_(sSoundDoneMsg&,cMultiParm*) = 0; // +3C
	virtual void __thiscall OnSchemaDone_(sSchemaDoneMsg&,cMultiParm*) = 0; // +40
	virtual void __thiscall OnQuestChange_(sQuestMsg&,cMultiParm*) = 0; // +44
	virtual void __thiscall OnSim_(sSimMsg&,cMultiParm*) = 0; // +48
	virtual void __thiscall OnMotionStart_(sBodyMsg&,cMultiParm*) = 0; // +4C
	virtual void __thiscall OnMotionEnd_(sBodyMsg&,cMultiParm*) = 0; // +50
	virtual void __thiscall OnMotionFlagReached_(sBodyMsg&,cMultiParm*) = 0; // +54
	virtual void __thiscall OnCreate_(sScrMsg&,cMultiParm*) = 0; // +58
	virtual void __thiscall OnDestroy_(sScrMsg&,cMultiParm*) = 0; // +5C
	virtual void __thiscall OnFrobToolBegin_(sFrobMsg&,cMultiParm*) = 0; // +60
	virtual void __thiscall OnFrobToolEnd_(sFrobMsg&,cMultiParm*) = 0; // +64
	virtual void __thiscall OnFrobWorldBegin_(sFrobMsg&,cMultiParm*) = 0; // +68
	virtual void __thiscall OnFrobWorldEnd_(sFrobMsg&,cMultiParm*) = 0; // +6C
	virtual void __thiscall OnFrobInvBegin_(sFrobMsg&,cMultiParm*) = 0; // +70
	virtual void __thiscall OnFrobInvEnd_(sFrobMsg&,cMultiParm*) = 0; // +74
	virtual void __thiscall OnWorldFocus_(sScrMsg&,cMultiParm*) = 0; // +78
	virtual void __thiscall OnWorldDeFocus_(sScrMsg&,cMultiParm*) = 0; // +7C
	virtual void __thiscall OnWorldSelect_(sScrMsg&,cMultiParm*) = 0; // +80
	virtual void __thiscall OnWorldDeSelect_(sScrMsg&,cMultiParm*) = 0; // +84
	virtual void __thiscall OnInvFocus_(sScrMsg&,cMultiParm*) = 0; // +88
	virtual void __thiscall OnInvDeFocus_(sScrMsg&,cMultiParm*) = 0; // +8C
	virtual void __thiscall OnInvSelect_(sScrMsg&,cMultiParm*) = 0; // +90
	virtual void __thiscall OnInvDeSelect_(sScrMsg&,cMultiParm*) = 0; // +94
	virtual void __thiscall OnCombine_(sCombineScrMsg&,cMultiParm*) = 0; // +98
	virtual void __thiscall OnContainer_(sContainerScrMsg&,cMultiParm*) = 0; // +9C
	virtual void __thiscall OnContained_(sContainedScrMsg&,cMultiParm*) = 0; // +A0
	// cScr_RootScript
	virtual void __thiscall OnDamage_(sDamageScrMsg&,cMultiParm*) = 0; // +A4
	virtual void __thiscall OnSlain_(sSlayMsg&,cMultiParm*) = 0; // +A8
	virtual void __thiscall OnReportMessage_(sReportMsg&,cMultiParm*) = 0; // +AC
	virtual void __thiscall OnDifficulty_(sDiffScrMsg&,cMultiParm*) = 0; // +B0
#if (_DARKGAME == 2)
	virtual void __thiscall OnPickStateChange_(sPickStateScrMsg&,cMultiParm*) = 0; // +B4
	virtual void __thiscall OnDarkGameModeChange_(sDarkGameModeScrMsg&,cMultiParm*) = 0; // +B8
#endif
};

interface IScr_AI : public IScr_RootScript
{
	virtual void __thiscall OnSignalAI_(sAISignalMsg&,cMultiParm*) = 0; // +BC
	virtual void __thiscall OnPatrolPoint_(sAIPatrolPointMsg&,cMultiParm*) = 0; // +C0
	virtual void __thiscall OnAIModeChange_(sAIModeChangeMsg&,cMultiParm*) = 0; // +C4
	virtual void __thiscall OnAlertness_(sAIAlertnessMsg&,cMultiParm*) = 0; // +C8
	virtual void __thiscall OnHighAlert_(sAIHighAlertMsg&,cMultiParm*) = 0; // +CC
	virtual void __thiscall OnObjActResult_(sAIObjActResultMsg&,cMultiParm*) = 0; // +D0
	virtual true_bool* __thiscall MakeGotoObjLoc(true_bool&,const object&,eAIScriptSpeed,eAIActionPriority,const cMultiParm&) = 0;
	virtual true_bool* __thiscall FrobObj(true_bool&,const object&,const object&,eAIActionPriority,const cMultiParm&) = 0;
	virtual true_bool* __thiscall FrobObj(true_bool&,const object&,eAIActionPriority,const cMultiParm&) = 0;
	virtual eAIScriptAlertLevel __thiscall GetAlertLevel(void) = 0;
	virtual void __thiscall SetMinimumAlert(eAIScriptAlertLevel) = 0;
	virtual void __thiscall ClearGoals(void) = 0;
	virtual void __thiscall SetScriptFlags(int) = 0;
	virtual void __thiscall ClearAlertness(void) = 0;
	virtual void __thiscall Signal(const cScrStr&) = 0;
};

interface IScr_Attack : public IScr_RootScript
{
	virtual void __thiscall OnStartWindup_(sAttackMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnStartAttack_(sAttackMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnEndAttack_(sAttackMsg&,cMultiParm*) = 0;
};

interface IScr_Door : public IScr_RootScript
{
	virtual void __thiscall OnDoorOpen_(sDoorMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnDoorClose_(sDoorMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnDoorOpening_(sDoorMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnDoorClosing_(sDoorMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnDoorHalt_(sDoorMsg&,cMultiParm*) = 0;
};

interface IScr_PressurePlate : public IScr_RootScript
{
	virtual void __thiscall OnPressurePlateInactive_(sPressurePlateMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnPressurePlateActive_(sPressurePlateMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnPressurePlateDeactivating_(sPressurePlateMsg&,cMultiParm*) = 0;
	virtual void __thiscall OnPressurePlateActivating_(sPressurePlateMsg&,cMultiParm*) = 0;
};

interface IScr_Physics : public IScr_RootScript
{
	virtual void __thiscall OnPhysFellAsleep_(sPhysMsg&,cMultiParm*) = 0; // +BC
	virtual void __thiscall OnPhysWokeUp_(sPhysMsg&,cMultiParm*) = 0; // +C0
	virtual void __thiscall OnPhysMadePhysical_(sPhysMsg&,cMultiParm*) = 0; // +C4
	virtual void __thiscall OnPhysMadeNonPhysical_(sPhysMsg&,cMultiParm*) = 0; // +C8
	virtual void __thiscall OnPhysCollision_(sPhysMsg&,cMultiParm*) = 0; // +CC
	virtual void __thiscall OnPhysContactCreate_(sPhysMsg&,cMultiParm*) = 0; // +D0
	virtual void __thiscall OnPhysContactDestroy_(sPhysMsg&,cMultiParm*) = 0; // +D4
	virtual void __thiscall OnPhysEnter_(sPhysMsg&,cMultiParm*) = 0; // +D8
	virtual void __thiscall OnPhysExit_(sPhysMsg&,cMultiParm*) = 0; // +DC
};

interface IScr_Room : public IScr_RootScript
{
	virtual void __thiscall OnPlayerRoomEnter_(sRoomMsg&,cMultiParm*) = 0; // +BC
	virtual void __thiscall OnPlayerRoomExit_(sRoomMsg&,cMultiParm*) = 0; // +C0
	virtual void __thiscall OnRemotePlayerRoomEnter_(sRoomMsg&,cMultiParm*) = 0; // +C4
	virtual void __thiscall OnRemotePlayerRoomExit_(sRoomMsg&,cMultiParm*) = 0; // +C8
	virtual void __thiscall OnCreatureRoomEnter_(sRoomMsg&,cMultiParm*) = 0; // +CC
	virtual void __thiscall OnCreatureRoomExit_(sRoomMsg&,cMultiParm*) = 0; // +D0
	virtual void __thiscall OnObjectRoomEnter_(sRoomMsg&,cMultiParm*) = 0; // +D4
	virtual void __thiscall OnObjectRoomExit_(sRoomMsg&,cMultiParm*) = 0; // +D8
	virtual void __thiscall OnObjRoomTransit_(sRoomMsg&,cMultiParm*) = 0; // +DC
};


interface IScr_StdTrap : public IScr_RootScript
{
	virtual void __thiscall OnSwitch(true_bool on, object from) = 0;
	virtual void __thiscall OnTurnOnOff(true_bool on, object from) = 0;
};

interface IScr_AnimLight : public IScr_RootScript
{
	virtual void __thiscall InitModes() = 0; // +BC
	virtual void __thiscall AmbientHack(true_bool) = 0; // +C0
	virtual true_bool* __thiscall IsLightOn(true_bool&) = 0; // +C4
	virtual void __thiscall ChangeMode(true_bool) = 0; // +C8
	virtual void __thiscall TurnOn() = 0; // +CC
	virtual void __thiscall TurnOff() = 0; // +D0
	virtual void __thiscall Toggle() = 0; // +D4
};

interface IScr_StdDoor : public IScr_Door
{
	/*
	 OnSim: If starting ( ScanDoubles; set("Sim") = 1;} else clear("Sim");
	 OnNowLocked: if get("Sim") == 1 { PingDoubles; IDoorSrv::CloseDoor; }
	 OnNowUnlocked: if get("Sim") == 1 { PingDoubles; IDoorSrv::OpenDoor; }
	 OnTimer("Close"): StopCloseTimer(false); IDoorSrv::CloseDoor;
	 OnFrobWorldEnd:
	   if locked && closed && !LockCheat:
	     PlayEnvSchema(m_ObjId, "Event Reject, Operation OpenDoor", m_ObjId, 0, 0, 0);
	   else:
	     if frobber = "Player": set("PlayerFrob");
		 if isset("BeforeHalt"):
		   beforehalt = get("BeforeHalt");
		   clear("BeforeHalt");
		   if beforehalt == Closing:
		     IDoorSrv::CloseDoor;
		   else:
		     IDoorSrv::OpenDoor;
	     else:
		   IDoorSrv::ToggleDoor;
	 OnDoorHalt: 
	   set("BeforeHalt") = previousstate; 
	   PlayEnvSchema(m_ObjId, StateChangeTags, m_ObjId, 0, 0, 0);
	   SetCloseTimer;
	 OnDoorOpening:
	   PingDoubles
	   IDarkGameSrv::FoundObject
	   PlayEnvSchema(m_ObjId, StateChangeTags, m_ObjId, 0, 0, 0);
	 OnDoorClosing:
	   PingDoubles;
	   PlayEnvSchema(m_ObjId, StateChangeTags, m_ObjId, 0, 0, 0);
	   StopCloseTimer(true);
	 OnDoorClose:
	   ISoundScrSrv::Halt(m_ObjId, "", 0);
	   PlayEnvSchema(m_ObjId, StateChangeTags, m_ObjId, 0, 0, 0);
	 OnDoorOpen:
	   ISoundScrSrv::Halt(m_ObjId, "", 0);
	   PlayEnvSchema(m_ObjId, StateChangeTags, m_ObjId, 0, 0, 0);
	   SetCloseTimer;
	 OnSlain:
	   IDamageSrv::Resurrect(m_ObjId, 0);
	   IPropertySrv::Set(m_ObjId, "HitPoints" 1);
	   for ILinkSrv::GetAll("Lock",m_ObjId,0): ILinkSrv::Destroy;
	   if Possessed("Locked"): Set("Locked",0); Remove("Locked");
	   if Possessed("KeyDst"): Remove("KeyDst");
	   IDoorSrv::OpenDoor;
	 OnMessage:
	   switch(message):
	     "TurnOn" | "Open": IDoorSrv::OpenDoor; break;
		 "TurnOff" | "Close": IDoorSrv::CloseDoor; break;
		 "PlayerToolFrob": set("PlayerFrob") = 0; break;
		 "SynchUp":
		   target = TargState(IDoorSrv::GetDoorState(from));
		   state = TargState(IDoorSrv::GetDoorState(m_ObjId));
		   if state != target:
		     if target == Closed:
			   IDoorSrv::CloseDoor;
			 else:
			   IDoorSrv::OpenDoor;
		   if IPropertySrv::Possessed(from,"Locked") && IPropertySrv::Possessed(m_ObjId,"Locked"):
		     if IPropertySrv::Get(from,"Locked") != IPropertySrv::Get(m_ObjId,"Locked"):
			   IPropertySrv::CopyFrom(m_ObjId, "Locked", from);
		   break;
	 */
	// Sent "SynchUp" to ScriptParams and ~ScriptParams w/ "Double"
	virtual void __thiscall PingDoubles() = 0; // +D0
	// Closing => Closed, Opening => Open, otherwise identity.
	virtual int __thiscall TargState(int state) = 0; // +D4
	// "Event StateChange, OpenState %s, OldOpenState %s"
	// + ", CreatureType Player" (if PlayerFrob)
	// Clears PlayerFrob if status is open, closed, or halted
	virtual cScrStr* __thiscall StateChangeTags(cScrStr&, int status, int oldstatus) = 0; // +D8
#if (_DARKGAME == 2)
	virtual void __thiscall StopCloseTimer(true_bool) = 0; // +DC
	// If ScriptTiming exists, stop any previous timers and
	// set the timer "Close" for ScriptTiming milliseconds.
	virtual void __thiscall SetCloseTimer() = 0; // +E0
	// Fixes the bug where an AI will get stuck if it tries to walk directly through 
	// the middle of a double-door pair. Only operates when door2 is greater than 
	// door1 (since it will be called twice for each pair). 
	// As long as the doors border the same rooms, and they touch at the midpoint 
	// between the doors, a marker with AI_ObjAvoid\Flags set to 2 is created between them.
	virtual void __thiscall PathAvoidDoubles(object& door1, object& door2) = 0; // +E4
	// Calls PathAvoidDoubles for each ScriptParams link from this object,
	// and for ScriptParams to the object, but only if there isn't a ScriptParams 
	// from it to the destination. (So it won't do the same object more than once, 
	// if it can help it.)
	virtual void __thiscall ScanDoubles() = 0; // +E8
	// Fills r1 and r2 with the rooms on opposite sides of the door.
	// Returns true if successful. False if it can't do it.
	// (Rooms are invalid or object isn't a door.)
	// r1 is guaranteed to be greater than r2 (Why?)
	virtual true_bool* __thiscall GetRooms(true_bool&, object& door, object& r1, object& r2) = 0; // +EC
#endif
};

interface IScr_TrigOBB : public IScr_Physics
{
	virtual void __thiscall Census(object who) = 0; // +E0
	virtual void __thiscall DeCensus(object who) = 0; // +E4
	virtual void __thiscall TrigOBBActivate() = 0; // +E8
	virtual void __thiscall TrigOBBDeactivate() = 0; // +EC
	virtual void __thiscall UpdatePop() = 0; // +F0
};

/*
ControllerActivate(true_bool) // 10012ba0
TweqAnimStateHasVal(true_bool&,obj,InputMask) // 10012d60
OnBeginScript // 10012e60

StdTwoState {
  OnMessage // 100137f0
    TurnOn || GoForward: GoTowardsState(0)
	TurnOff || GoBackward: GoTowardsState(1)
	Toggle: TweqActivate()
	ToggleNoSE: TweqActivate()
	Target?: reply TargetState(this)
	SynchUp: GoTowardsState(TargetState(from))
  OnTweqComplete // 10013a20
    if joint || rotate: PostTweqActivate(dir)
  // returns 0 or 1
  int TargetState(object) // 10013180
  // returns "Forward" or "Reverse"
  DirectionTags(cScrStr&,int) // 10013220
  PreTweqActivate(int) // 10013270
  PostTweqActivate(int) // 10013290
  // PlayEnvSchema(this, "Event StateChange, DirectionState " + DirectionTags(this,TargetState(this)),
  //               this, None, 1, 0)
  // tweq_control(joint || rotate, DoDefault)
  // SynchUp on ~FrobProxy
  TweqActivateNoSE() // 100132b0
  // TweqActivateNoSE(); ToggleNoSE on ScriptParams w/ SideEffect
  TweqActivate()  // 10013710
  // if not TargetState TweqActivate
  GoTowardsState(int) // 10013790
}

StdLever {
  OnFrobWorldEnd // 10013be0
  PostTweqActivate(int) // 10013ad0
  // 10013b40
}

MovingTerrain {
}

StdElevator {
  OnMessage // 100163a0
  // activate or halt tweq depending on the state of MovingTerrain
  OnBeginScript // 100161f0
  // Sends "Starting" if not already there
  // activates or halts the joint tweq
  // Link to the object with TPathNext
  // activate the moving terrain
  SeekPoint(object) // 10015040
  // Returns the other end of the given link kind from the given object,
  // or object if it can't find it
  NextPoint(object&,object,linkkind) // 100155c0
  // Follows TPath links looking for the object with the highest or lowest Z position
  // Returns the script object if it can't find it.
  FindPoint(object&,uint8 highest) // 10015690
  GoUpOrDown(true_bool) // 10015a50
  NextPath(object&,object,object) // 10015aa0
  SeekPath(object) // 100160b0
}
*/

#endif // _LG_GEN_H_
