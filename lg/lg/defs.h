/************************
 * LGS Definitions
 */

#ifndef _LG_DEFS_H
#define _LG_DEFS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/types.h>


enum eAIMode
{
	kAIM_Asleep,
	kAIM_SuperEfficient,
	kAIM_Efficient,
	kAIM_Normal,
	kAIM_Combat,
	kAIM_Dead,
	kAIM_Num
};

enum eAIScriptAlertLevel
{
	kNoAlert,
	kLowAlert,
	kModerateAlert,
	kHighAlert
};

enum eAIAwareLevel
{
	kNoAwareness,
	kLowAwareness,
	kModerateAwareness,
	kHighAwareness
};

enum eAIScriptSpeed
{
	kSlowSpeed,
	kNormalSpeed,
	kFastSpeed
};

enum eAIAction
{
	kAINoAction,
	kAIGoto,
	kAIFrob,
	kAIManeuver
};

enum eAIActionPriority
{
	kLowPriorityAction,
	kNormalPriorityAction,
	kHighPriorityAction
};

enum eAIResponsePriority
{
	kDefaultPriorityResponse,
	kVeryLowPriorityResponse,
	kLowPriorityResponse,
	kNormalPriorityResponse,
	kHighPriorityResponse,
	kVeryHighPriorityResponse,
	kAbsolutePriorityResponse
};

enum eAIActionResult
{
	kActionDone,
	kActionFailed,
	kActionNotAttempted
};

enum eAIConverseState
{
	kAIConverseStateZero
};

enum eAIConvEventKind
{
	kMotion,
	kSchema,
	kTweqAll,
	kVoiceover,
	kEnd
};

enum eAIResult
{
	kAIResultZero
};

enum eAmbientFlags
{
	kAmbientFlagEnvironmental = 0x01,
	kAmbientFlagNoSharpCurve = 0x02,
	kAmbientFlagTurnedOff = 0x04,
	kAmbientFlagRemoveProp = 0x08,
	kAmbientFlagMusic = 0x10,
	kAmbientFlagSynch = 0x20,
	kAmbientFlagNoFade = 0x40,
	kAmbientFlagDestroyObj = 0x80,
	kAmbientFlagDoAutoOff = 0x100
};

enum eCollisionType
{
	kCollisionTypeBounce = 0x1,
	kCollisionTypeDestroy = 0x02,
	kCollisionTypeSlay = 0x04,
	kCollisionTypeNoSound = 0x08,
	kCollisionTypeNoResult = 0x10,
	kCollisionTypeFullSound = 0x20
};

enum eConstraintKind
{
	kNullConstraint,
	kConstrainBefore,
	kConstrainAfter
};

enum eContainsEvent
{
	kContainQueryAdd,
	kContainQueryCombine,
	kContainAdd,
	kContainRemove,
	kContainCombine
};

enum eContainType
{
	kContainTypeAlternate = -3,
	kContainTypeHand = -2,
	kContainTypeBelt = -1,
	kContainTypeGeneric = 0
};

enum eCritMsgNotification
{
	kCritMsgEnter,
	kCritMsgExit,
	kCritMsgIgnoring,
	kCritMsgTerminating,
	kCritMsgDebugging
};

enum eDoorState
{
	kDoorStateClosed,
	kDoorStateOpen,
	kDoorStateClosing,
	kDoorStateOpening,
	kDoorStateHalted
};

enum eDoorAction
{
	kDoorOpen,
	kDoorClose,
	kDoorOpening,
	kDoorClosing,
	kDoorHalt
};

enum eEnvSoundLoc
{
	kEnvSoundOnObj,
	kEnvSoundAtObjLoc,
	kEnvSoundAmbient
};

enum eFrobLoc
{
	kFrobLocWorld,
	kFrobLocInv,
	kFrobLocTool,
	kFrobLocNone,
	kFrobNumActions = kFrobLocNone
};

enum eInventoryType
{
	kJunk,
	kItem,
	kWeapon
};

enum eKeyUse
{
	kKeyUseDefault,
	kKeyUseOpen,
	kKeyUseClose,
	kKeyUseCheck
};

enum ePhysAxes
{
	NoAxes,
	XAxis = 0x01,
	YAxis = 0x02,
	ZAxis = 0x04,
	NegXAxis = 0x08,
	NegYAxis = 0x10,
	NegZAxis = 0x20
};

enum ePhysForceType
{
	kPFT_Gravity,
	kPFT_Wind,
	kPFT_Current,
	kPFT_Collision
};

enum ePhysModelType
{
	kPMT_OBB,
	kPMT_Sphere,
	kPMT_SphereHat,
	kPMT_NumTypes,
	kPMT_Invalid = -128
};
/* Where is this used?
enum ePhysModelType
{
	kPMT_Sphere,
	kPMT_BSP,
	kPMT_Point,
	kPMT_OBB,
	kPMT_SphereHat,
	kPMT_NumTypes,
	kPMT_Invalid = -128
};
*/

enum eSlayResult
{
	kSlayResultNormal,
	kSlayResultNoEffect,
	kSlayResultTerminate,
	kSlayResultDestroy
};

enum eSoundNetwork
{
	kSoundNetwork0
};

enum eSoundSpecial
{
	kSoundNormal,
	kSoundLoop
};

enum eTrapFlags
{
	kTrapFlagOnce = 1,
	kTrapFlagInvert = 2,
	kTrapFlagNoOn = 4,
	kTrapFlagNoOff = 8
};

enum eTweqAction
{
	kTweqDoDefault,
	kTweqDoActivate,
	kTweqDoHalt,
	kTweqDoReset,
	kTweqDoContinue,
	kTweqDoForward,
	kTweqDoReverse
};

enum eTweqFlag
{
	kTweqFlagOn = 1,
	kTweqFlagReverse = 2,
	kTweqFlagReSynch = 4,
	kTweqFlagGoEdge = 8,
	kTweqFlagLapOne = 16
};

enum eTweqType
{
	kTweqTypeScale,
	kTweqTypeRotate,
	kTweqTypeJoints,
	kTweqTypeModels,
	kTweqTypeDelete,
	kTweqTypeEmitter,
	kTweqTypeFlicker,
	kTweqTypeLock,
	kTweqTypeAll,
	kTweqTypeNull
};

enum eTweqOperation
{
	kTweqOpKillAll,
	kTweqOpRemoveTweq,
	kTweqOpHaltTweq,
	kTweqOpStatusQuo,
	kTweqOpSlayAll,
	kTweqOpFrameEvent
};

enum eTweqDirection
{
	kTweqDirForward,
	kTweqDirReverse
};

enum goaltype
{
	kGoalNone,
	kGoalTake,
	kGoalSlay,
	kGoalLoot,
	kGoalGoTo
};

#if (_DARKGAME <= 2)
// font color styles for IDarkOverlaySrv
enum StyleColorKind
{
   StyleColorFG,                // foreground 
   StyleColorBG,                // background,
   StyleColorText,              // text color
   StyleColorHilite,            // hilight color
   StyleColorBright,            // bright color
   StyleColorDim,               // dim color
   StyleColorFG2,               // alternate foreground 
   StyleColorBG2,               // alternate background
   StyleColorBorder,            // border color
   StyleColorWhite,             // white color
   StyleColorBlack,             // black color
   StyleColorXOR,               // color for xor-ing
   StyleColorBevelLight,        // light bevel color
   StyleColorBevelDark          // dark bevel color
};
#endif

#if (_DARKGAME == 3)
// defs for IShockOverlaySrv

// overlay elements
#define kOverlayInv        0
#define kOverlayFrame      1  
#define kOverlayText       2
#define kOverlayRep        3
#define kOverlayBook       4
#define kOverlayComm       5
#define kOverlayContainer  6
#define kOverlayHRM        7
#define kOverlayRadar      8
#define kOverlayLetterbox  9
#define kOverlayOverload   10
#define kOverlayPsi        11
#define kOverlayYorN       12
#define kOverlayKeypad     13
#define kOverlayLook       14
#define kOverlayAmmo       15
#define kOverlayMeters     16
#define kOverlayHUD        17
#define kOverlayStats      18
#define kOverlaySkills     19
#define kOverlayBuyTraits  20
#define kOverlaySetting    21    
#define kOverlayCrosshair  22    
#define kOverlayResearch   23
#define kOverlayPDA        24
#define kOverlayEmail      25
#define kOverlayMap        26
#define kOverlayAlarm      27
#define kOverlayPsiIcons   28
#define kOverlayHackIcon   29
#define kOverlayRadiation  30
#define kOverlayPoison     31
#define kOverlayMiniFrame  32
#define kOverlaySecurity   33
#define kOverlayTicker     34
#define kOverlayBuyStats   35
#define kOverlayBuyTech    36
#define kOverlayBuyWeapon  37
#define kOverlayBuyPsi     38
#define kOverlayTechSkill  39
#define kOverlayMFDGame    40
#define kOverlayTlucText   41
#define kOverlaySecurComp  42
#define kOverlayHackComp   43
#define kOverlayHRMPlug    44
#define kOverlayMiniMap    45
#define kOverlayElevator   46
#define kOverlayVersion    47
#define kOverlayTurret     48
#define kOverlayMouseMode  49

// cursor modes
#define SCM_NORMAL   0
#define SCM_DRAGOBJ  1
#define SCM_USEOBJ   2
#define SCM_LOOK     3
#define SCM_PSI      4
#define SCM_SPLIT    5

// keycode flags
#define KB_FLAG_DOWN    (1<<(0+8))
#define KB_FLAG_CTRL    (1<<(1+8))
#define KB_FLAG_ALT     (1<<(2+8))
#define KB_FLAG_SPECIAL (1<<(3+8))
#define KB_FLAG_SHIFT   (1<<(4+8))
#define KB_FLAG_2ND     (1<<(5+8))

// non-ascii and special keycodes
#define KEY_BS          0x08
#define KEY_TAB         0x09
#define KEY_ENTER       0x0D
#define KEY_ESC         0x1B
#define KEY_SPACE       0x20
#define KEY_F1          (0x3B | KB_FLAG_SPECIAL)
#define KEY_F2          (0x3C | KB_FLAG_SPECIAL)
#define KEY_F3          (0x3D | KB_FLAG_SPECIAL)
#define KEY_F4          (0x3E | KB_FLAG_SPECIAL)
#define KEY_F5          (0x3F | KB_FLAG_SPECIAL)
#define KEY_F6          (0x40 | KB_FLAG_SPECIAL)
#define KEY_F7          (0x41 | KB_FLAG_SPECIAL)
#define KEY_F8          (0x42 | KB_FLAG_SPECIAL)
#define KEY_F9          (0x43 | KB_FLAG_SPECIAL)
#define KEY_F10         (0x44 | KB_FLAG_SPECIAL)
#define KEY_F11         (0x57 | KB_FLAG_SPECIAL)
#define KEY_F12         (0x58 | KB_FLAG_SPECIAL)
#define KEY_INS         (0x52 | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_DEL         (0x53 | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_HOME        (0x47 | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_END         (0x4F | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_PGUP        (0x49 | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_PGDN        (0x51 | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_LEFT        (0x4B | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_RIGHT       (0x4D | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_UP          (0x48 | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_DOWN        (0x50 | KB_FLAG_SPECIAL | KB_FLAG_2ND)
#define KEY_GREY_SLASH  ('/' | KB_FLAG_2ND)
#define KEY_GREY_STAR   ('*' | KB_FLAG_2ND)
#define KEY_GREY_PLUS   ('+' | KB_FLAG_2ND)
#define KEY_GREY_MINUS  ('-' | KB_FLAG_2ND)
#define KEY_GREY_ENTER  (KEY_ENTER | KB_FLAG_2ND)
#define KEY_PAD_HOME    (0x47 | KB_FLAG_SPECIAL)
#define KEY_PAD_UP      (0x48 | KB_FLAG_SPECIAL)
#define KEY_PAD_PGUP    (0x49 | KB_FLAG_SPECIAL)
#define KEY_PAD_LEFT    (0x4B | KB_FLAG_SPECIAL)
#define KEY_PAD_CENTER  (0x4C | KB_FLAG_SPECIAL)
#define KEY_PAD_RIGHT   (0x4D | KB_FLAG_SPECIAL)
#define KEY_PAD_END     (0x4F | KB_FLAG_SPECIAL)
#define KEY_PAD_DOWN    (0x50 | KB_FLAG_SPECIAL)
#define KEY_PAD_PGDN    (0x51 | KB_FLAG_SPECIAL)
#define KEY_PAD_INS     (0x52 | KB_FLAG_SPECIAL)
#define KEY_PAD_DEL     (0x53 | KB_FLAG_SPECIAL)
#endif


#endif // _LG_DEFS_H
