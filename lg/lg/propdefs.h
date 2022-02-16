/***************************
 * LGS Property Definitions
 */

#ifndef _LG_PROPDEFS_H
#define _LG_PROPDEFS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/types.h>
#include <lg/defs.h>

#define AITEAM_GOOD		0 
#define AITEAM_NEUTRAL	1

#define AIAWARE_SEEN		0x00000001
#define AIAWARE_HEARD		0x00000002
#define AIAWARE_CANRAYCAST	0x00000004
#define AIAWARE_HAVELOS		0x00000008
#define AIAWARE_BLIND		0x00000010
#define AIAWARE_DEAF		0x00000020
#define AIAWARE_HIGHEST		0x00000040
#define AIAWARE_FIRSTHAND	0x00000080

// This should really be in linkdefs.h
struct sAIAwareness {
	int  i;
	int  flags;
	int  level;
	int  PeakLevel;
	int  LevelEnterTime;
	int  TimeLastContact;
	mxs_vector PosLastContact;
	int  i2;
	int  VisCone;
	int  TimeLastUpdate;
	int  TimeLastUpdateLOS;
	int  i3;
	int  TimeLastFirstHand;
	int  Freshness;
#if (_DARKGAME == 2)
	int  i4;
#endif
};

struct  sAIAlertCap {
	int  iMax, iMin, iMinRelaxAfterPeak;
};


struct sLoot {
	int  iGold;
	int  iGems;
	int  iArt;
	int  iSpecial;
};

struct sMovingTerrain
{
	unsigned long	unknown;
	unsigned long	active;
};


#define PHYS_CONTROL_AXISVELS	1
#define PHYS_CONTROL_VELS		2
#define PHYS_CONTROL_ROTVELS	4

struct sPhysControl {
	int uiActive;
	mxs_vector axis_vels;
	mxs_vector vels;
	mxs_vector rot_vels;
};

struct sPhysState {
	mxs_vector loc;
	mxs_vector facing;
	mxs_vector vels;
	mxs_vector rot_vels;
};

#pragma pack(push,1)
struct sRenderFlash
{
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
	unsigned char	active;
	unsigned long	worldduration;
	unsigned long	screenduration;
	unsigned long	effectduration;
	float	time;
	float	range;
	unsigned long	starttime;
};
#pragma pack(pop)

enum eAIPsdScrAct
{
	kPS_Nothing,
	kPS_ScrMsg,
	kPS_Alert,
	kPS_GoHostile,
	kPS_Investigate,
	kPS_GotoObject,
	kPS_FrobObject,
	kPS_Wait,
	kPS_MPrint,
	kPS_AddRemMetaProp,
	kPS_AddLink,
	kPS_RemLink,
	kPS_Face,
	kPS_DestScrMsg
};

struct sAIPsdScr
{
	eAIPsdScrAct	action;
	char	argument1[64];
	char	argument2[64];
	char	argument3[64];
	char	pad[64];
};

enum eAIWatchKind
{
	kAIWatchPlayerIntrustion,
	kAIWatchSelfEntry
};

enum eAIVisibility
{
	kNoVisibility,
	kLineOfSight,
	kRaycast
};

struct sAIWatchPoint
{
	char 	pad1[60];
	eAIWatchKind	WatchKind;
	eAIAwareLevel	ReqAwareness;
	eAIVisibility	ReqVisibility;
	int 	pad2;
	unsigned long	LinkKill;
	unsigned long	KillLikeLinks;
	unsigned long	NoTestOnceTriggered;
	int		ReuseTime;
	int		ResetTime;
	eAIScriptAlertLevel	MinAlertness;
	eAIScriptAlertLevel	MaxAlertness;
	eAIResponsePriority	Priority;
	int		TriggerRadius;
	int		TriggerHeight;
	sAIPsdScr	Response[8];
#if (_DARKGAME == 2)
	sAIPsdScr	Response2[8];
#endif
};

struct sFlinders
{
	int	Count;
	float	Impulse;
	unsigned long	Scatter;
	mxs_vector	Offset;
};

#pragma pack(push,1)
struct sKeyProp
{
	unsigned char bMasterBit;
#if (_DARKGAME == 2)
	unsigned long iRegionMask;
#else
	unsigned short iRegionMask;
#endif
	unsigned char iLockID;
};
#pragma pack(pop)

#define ANIMLIGHT_FLIPMINMAX 0
#define ANIMLIGHT_SLIDESMOOTHLY 1
#define ANIMLIGHT_RANDOM 2
#define ANIMLIGHT_MINBRIGHTNESS 3
#define ANIMLIGHT_MAXBRIGHTNESS 4
#define ANIMLIGHT_ZEROBRIGHTNESS 5
#define ANIMLIGHT_SMOOTHLYBRIGHTEN 6
#define ANIMLIGHT_SMOOTHLYDIM 7
#define ANIMLIGHT_RANDOMBUTCOHERENT 8
#define ANIMLIGHT_FLICKERMINMAX 9

struct sAnimLight
{
	int pad1;
	mxs_vector Offset;
	int pad2;
	unsigned short CellIndex;
	unsigned short CellCount;
	short LightID;
	unsigned short Mode;
	int TimeToBrighten;
	int TimeToDim;
	float MinBrightness;
	float MaxBrightness;
	float CurrentBrightness;
	unsigned long IsRising;
	int Countdown;
	unsigned long IsDisabled;
	float Radius;
	int pad3;
	unsigned long QuadLit;
	float InnerRadius;
};

#endif // _LG_PROPS_H
