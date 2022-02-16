/**********************
 * LGS AI
 */

#ifndef _LG_AI_H
#define _LG_AI_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/scrmanagers.h>
#include <lg/dynarray.h>

interface IInternalAI;

extern  const GUID  IID_IAIPathfindControl;
extern  const GUID  IID_IAIPathQuery;

class cAIPathEdge;

extern  const GUID  IID_IAIPath;
interface IAIPath : IUnknown
{
	virtual void* f0(void) =0;
	STDMETHOD_(void,Clear)(void) PURE;
	STDMETHOD_(long,Advance)(unsigned char) PURE;
	STDMETHOD_(int,Backup)(void) PURE;
	STDMETHOD_(int,GetPathEdgeCount)(void) const PURE;
	STDMETHOD_(void,GetPathEdge)(int,cAIPathEdge *) const PURE;
	STDMETHOD_(const cAIPathEdge*,GetCurEdge)(void) const PURE;
	STDMETHOD_(void,GetFinalDestination)(mxs_vector *) const PURE;
	STDMETHOD_(int,MoreEdges)(void) const PURE;
	STDMETHOD_(int,IsDestVisible)(const mxs_vector &,unsigned int,unsigned char) const PURE;
	STDMETHOD_(int,UpdateTargetEdge)(const mxs_vector &,const mxs_angvec &,unsigned int,unsigned char) PURE;
	STDMETHOD_(float,ComputeXYLength)(const mxs_vector &) const PURE;
	STDMETHOD_(void,SetStressBits)(unsigned char) PURE;
	STDMETHOD_(unsigned char,GetStressBits)(void) PURE;
	STDMETHOD_(int,IsCellInPath)(unsigned int) PURE;
};

class cAIActions;
class cAIGoal;

struct sAIAction;
struct sAIAlertness;
struct sAISoundType;

interface IAIActor;

extern  const GUID  IID_IAIAction;
interface IAIAction : IUnknown
{
	STDMETHOD_(void,Describe)(cAnsiStr *) PURE;
	STDMETHOD_(int,InProgress)(void) PURE;
	STDMETHOD_(unsigned long,DataSize)(void) PURE;
	STDMETHOD_(eAIResult,GetResult)(void) PURE;
	STDMETHOD_(unsigned long,GetData)(unsigned int) PURE;
	STDMETHOD_(const sAIAction*,Access)(void) PURE;
	STDMETHOD_(int,Done)(void) PURE;
	STDMETHOD_(IAIAction *,GetInnerAction)(void) PURE;
	STDMETHOD_(IAIAction *,GetTrueAction)(void) PURE;
	STDMETHOD_(IAIActor *,GetTrueOwner)(void) PURE;
	STDMETHOD_(eAIResult,Update)(void) PURE;
	STDMETHOD_(eAIResult,Enact)(unsigned long) PURE;
	STDMETHOD_(eAIResult,End)(void) PURE;
	STDMETHOD_(int,Save)(ITagFile *) PURE;
	STDMETHOD_(int,Load)(ITagFile *) PURE;
};

enum eAIPropUpdateKind
{
	kAIPropUpdateZero
};

struct sAIDefendPoint;
struct sAISignal;
struct sAIMoveGoal;
class cAIMoveSuggestions;

struct sPhysListenMsg;
struct sPropertyListenMsg;

extern  const GUID  IID_IAIComponent;
interface IAIComponent : IUnknown
{
	STDMETHOD_(void,GetSubComponents)(cDynArray<IAIComponent *> *) PURE;
	STDMETHOD_(void,ConnectComponent)(IInternalAI *,unsigned int * *) PURE;
	STDMETHOD_(void,Init)(void) PURE;
	STDMETHOD_(void,End)(void) PURE;
	STDMETHOD_(int,Object)(void) PURE;
	STDMETHOD_(const char*,GetName)(void) PURE;
	STDMETHOD_(void,OnSimStart)(void) PURE;
	STDMETHOD_(void,OnSimEnd)(void) PURE;
	STDMETHOD_(void,OnBeginFrame)(void) PURE;
	STDMETHOD_(void,OnEndFrame)(void) PURE;
	STDMETHOD_(void,OnUpdateProperties)(eAIPropUpdateKind) PURE;
	STDMETHOD_(void,OnActionProgress)(IAIAction *) PURE;
	STDMETHOD_(void,OnGoalProgress)(const cAIGoal *) PURE;
	STDMETHOD_(void,OnModeChange)(eAIMode,eAIMode) PURE;
	STDMETHOD_(void,OnGoalChange)(const cAIGoal *,const cAIGoal *) PURE;
	STDMETHOD_(void,OnActionChange)(IAIAction *,IAIAction *) PURE;
	STDMETHOD_(void,OnAwareness)(int,eAIAwareLevel,const sAIAlertness *) PURE;
	STDMETHOD_(void,OnAlertness)(int,eAIAwareLevel,eAIAwareLevel,const sAIAlertness *) PURE;
	STDMETHOD_(void,OnHighAlert)(int,eAIAwareLevel,eAIAwareLevel,const sAIAlertness *) PURE;
	STDMETHOD_(void,OnDamage)(const sDamageMsg *,int) PURE;
	STDMETHOD_(void,OnDeath)(const sDamageMsg *) PURE;
	STDMETHOD_(void,OnWeapon)(int,int,int) PURE;
	STDMETHOD_(void,OnSound)(const sSoundInfo *,const sAISoundType *) PURE;
	STDMETHOD_(void,OnFastObj)(int,int,const mxs_vector &) PURE;
	STDMETHOD_(void,OnFoundBody)(int) PURE;
	STDMETHOD_(void,OnFoundSuspicious)(int) PURE;
	STDMETHOD_(void,OnScriptCommand)(unsigned int,void *) PURE;
	STDMETHOD_(void,OnProperty)(IProperty *,const sPropertyListenMsg *) PURE;
	STDMETHOD_(void,OnDefend)(const sAIDefendPoint *) PURE;
	STDMETHOD_(void,OnSignal)(const sAISignal *) PURE;
	STDMETHOD_(void,OnWitnessCombat)(void) PURE;
	STDMETHOD_(void,OnGoalsReset)(const cAIGoal *) PURE;
	STDMETHOD_(void,OnCollision)(const sPhysListenMsg *) PURE;
	STDMETHOD_(void,OnTripwire)(int,int) PURE;
	STDMETHOD_(void,OnFrustration)(int,int,IAIPath *) PURE;
	STDMETHOD_(void,OnGameEvent)(void *) PURE;
	STDMETHOD_(int,PreSave)(void) PURE;
	STDMETHOD_(int,Save)(ITagFile *) PURE;
	STDMETHOD_(int,PostSave)(void) PURE;
	STDMETHOD_(int,PreLoad)(void) PURE;
	STDMETHOD_(int,Load)(ITagFile *) PURE;
	STDMETHOD_(int,PostLoad)(void) PURE;
};
/*
	STDMETHOD_(int,SaveActions(ITagFile *,cAIActions *)
	STDMETHOD_(int,LoadActions(ITagFile *,cAIActions *)
	STDMETHOD_(long,SuggestActions(cAIGoal *,const cAIActions &,cAIActions *)
	STDMETHOD_(void,ConnectAbility(unsigned int * *)
	STDMETHOD_(int,SaveGoal(ITagFile *,cAIGoal *)
	STDMETHOD_(int,LoadGoal(ITagFile *,cAIGoal * *)
*/

extern  const GUID  IID_IAIConversation;
interface IAIConversation : IUnknown
{
	STDMETHOD_(void,OnStateChange)(int,eAIConverseState,eAIConverseState) PURE;
	STDMETHOD_(void,OnAlertness)(eAIAwareLevel) PURE;
	STDMETHOD_(void,Frame)(void) PURE;
	STDMETHOD_(unsigned long,ID)(void) PURE;
};

struct sAIPsdScrAct;

extern  const GUID  IID_IAIConverse;
interface IAIConverse : IUnknown
{
	STDMETHOD_(void,Start)(IAIConversation *,int) PURE;
	STDMETHOD_(void,NewActions)(sAIPsdScrAct *,unsigned int) PURE;
	STDMETHOD_(void,Terminate)(void) PURE;
	STDMETHOD_(void,f3)(long) PURE;
	STDMETHOD_(void,OnSpeechEnd)(int) PURE;
	STDMETHOD_(void,f5)(void *) PURE;
	STDMETHOD_(int,GetCurrentAction)(void) PURE;
};

extern  const GUID  IID_IAIAbility;
extern  const GUID  IID_IAIAppServices;
extern  const GUID  IID_IAIInform;
extern  const GUID  IID_IAIMoveRegulator;
interface IAIMoveAction;
interface IAILocoAction;
interface IAIMotionAction;
interface IAISoundAction;
interface IAIOrientAction;
interface IAIFrobAction;
interface IAIFollowAction;
interface IAIInvestAction;
interface IAIWanderAction;
interface IAIPsdScrAction;
interface IAIWaitAction;
interface IAILaunchAction;
interface IAIJointRotateAction;
interface IAIJointSlideAction;
interface IAIJointScanAction;

class cAIActor;

extern  const GUID  IID_IAIBehaviorSet;
interface IAIBehaviorSet : IUnknown
{
	STDMETHOD_(const char*,GetName)(void) PURE;
	STDMETHOD_(IAI*,CreateAI)(int) PURE;
	STDMETHOD_(long,CreateComponents)(cDynArray<IAIComponent *> *) PURE;
	STDMETHOD_(IAIMoveAction*,CreateMoveAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAILocoAction*,CreateLocoAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIMotionAction*,CreateMotionAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAISoundAction*,CreateSoundAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIOrientAction*,CreateOrientAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIFrobAction*,CreateFrobAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIFollowAction*,CreateFollowAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIInvestAction*,CreateInvestAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIWanderAction*,CreateWanderAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIPsdScrAction*,CreatePsdScrAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIWaitAction*,CreateWaitAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAILaunchAction*,CreateLaunchAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIJointRotateAction*,CreateJointRotateAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIJointSlideAction*,CreateJointSlideAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIJointScanAction*,CreateJointScanAction)(cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIAction*,CreateGestureAction)(unsigned int,cAIActor *,unsigned long) PURE;
	STDMETHOD_(IAIAction*,CreateCustomAction)(unsigned int,cAIActor *,unsigned long) PURE;
	STDMETHOD_(void,EnactProxyLaunchAction)(IAI *,void *) PURE;
	STDMETHOD_(void,EnactProxyJointRotateAction)(IAI *,void *) PURE;
	STDMETHOD_(void,EnactProxyJointSlideAction)(IAI *,void *) PURE;
	STDMETHOD_(void,EnactProxyJointScanAction)(IAI *,void *) PURE;
	STDMETHOD_(void,EnactProxyCustomAction)(unsigned int,IAI *,void *) PURE;
	STDMETHOD_(void,SaveAction)(ITagFile *,IAIAction *) PURE;
	STDMETHOD_(IAIAction*,LoadAndCreateAction)(ITagFile *,cAIActor *) PURE;
};

extern  const GUID  IID_IAIMoveEnactor;
extern  const GUID  IID_IAISoundEnactor;
extern  const GUID  IID_IAISenses;
extern  const GUID  IID_IAIPathfinder;

interface IAIMoveEnactor;
interface IAISoundEnactor;
interface IAISenses;
interface IAIPathfinder;

struct sAIAwareness;

extern  const GUID  IID_IAI;
interface IAI : IUnknown
{
	STDMETHOD_(int,GetID)(void) PURE;
	STDMETHOD_(void,NotifyGameEvent)(void *) PURE;
	STDMETHOD_(void,NotifyTripwire)(int,int) PURE;
	STDMETHOD_(void,Kill)(void) PURE;
	STDMETHOD_(void,Resurrect)(void) PURE;
	STDMETHOD_(int,IsDead)(void) PURE;
	STDMETHOD_(int,IsDying)(void) PURE;
	STDMETHOD_(void,Freeze)(unsigned long) PURE;
	STDMETHOD_(void,Unfreeze)(void) PURE;
	STDMETHOD_(IInternalAI*,AccessOtherAI)(int) PURE;
	STDMETHOD_(void,AccessAIs)(IInternalAI * * *,int *) PURE;
	STDMETHOD_(long,Init)(int,IAIBehaviorSet*) PURE;
	STDMETHOD_(long,End)(void) PURE;
	STDMETHOD_(long,GetComponent)(const char *,IAIComponent * *) PURE;
	STDMETHOD_(IAIComponent*,GetFirstComponent)(tAIIter *) PURE;
	STDMETHOD_(IAIComponent*,GetNextComponent)(tAIIter *) PURE;
	STDMETHOD_(void,GetComponentDone)(tAIIter *) PURE;
	STDMETHOD_(IAIMoveEnactor*,AccesMoveEnactor)(void) PURE;
	STDMETHOD_(IAISoundEnactor*,AccessSoundEnactor)(void) PURE;
	STDMETHOD_(IAISenses*,AccessSenses)(void) PURE;
	STDMETHOD_(IAIPathfinder*,AccessPathfinder)(void) PURE;
	STDMETHOD_(IAIBehaviorSet*,AccessBehaviorSet)(void) PURE;
	// ???
	STDMETHOD_(void*,Describe)(void) PURE;
	STDMETHOD_(unsigned int,UpdatePathCell)(void) PURE;
	STDMETHOD_(void,GetRecentCells)(cDynArray<unsigned int> *) PURE;
	STDMETHOD_(float,GetGroundOffset)(void) PURE;
	STDMETHOD_(int,GetClosestPlayer)(void) PURE;
	STDMETHOD_(const sAIAwareness *,GetAwareness)(int) PURE;
	STDMETHOD_(void,NotifySimStart)(void) PURE;
	STDMETHOD_(void,NotifySimEnd)(void) PURE;
	STDMETHOD_(unsigned long,NotifyDamage)(const sDamageMsg *) PURE;
	STDMETHOD_(void,NotifyWeapon)(int,int,int) PURE;
	STDMETHOD_(void,NotifyFastObj)(int,int,const mxs_vector &) PURE;
	STDMETHOD_(void,NotifySound)(const sSoundInfo *,const sAISoundType *) PURE;
	STDMETHOD_(void,NotifyFoundBody)(int) PURE;
	STDMETHOD_(void,NotifyFoundSuspicious)(int) PURE;
	STDMETHOD_(void,NotifyScriptCommand)(unsigned int,void *) PURE;
	STDMETHOD_(void,NotifyProperty)(IProperty *,const sPropertyListenMsg *) PURE;
	STDMETHOD_(void,NotifyDefend)(const sAIDefendPoint *) PURE;
	STDMETHOD_(void,NotifySignal)(const sAISignal *) PURE;
	STDMETHOD_(void,NotifyWitnessCombat)(void) PURE;
	STDMETHOD_(void,NotifyCollision)(const sPhysListenMsg *) PURE;
	STDMETHOD_(long,f0)(void) PURE;
	STDMETHOD_(void,StartProxyAction)(IAIAction *,unsigned long) PURE;
	STDMETHOD_(IAIAction *,GetTheActionOfType)(unsigned int) PURE;
	STDMETHOD_(void,StopActionOfType)(unsigned int) PURE;
	STDMETHOD_(void,SetGoal)(cAIGoal *) PURE;
	STDMETHOD_(void,SetModeAndNotify)(eAIMode) PURE;
	STDMETHOD_(void,SetIsBrainsOnly)(int) PURE;
	STDMETHOD_(int,PreSave)(void) PURE;
	STDMETHOD_(int,Save)(ITagFile *) PURE;
	STDMETHOD_(int,PostSave)(void) PURE;
	STDMETHOD_(int,PreLoad)(void) PURE;
	STDMETHOD_(int,Load)(ITagFile *) PURE;
	STDMETHOD_(int,PostLoad)(void) PURE;
	STDMETHOD_(void,DebugDraw)(void) PURE;
	STDMETHOD_(void,DebugSetPrimitiveMove)(const cAIMoveSuggestions *,const sAIMoveGoal *) PURE;
	STDMETHOD_(void,DebugGetPrimitiveMove)(const cAIMoveSuggestions * *,const sAIMoveGoal * *) PURE;
};
DEFINE_IIDSTRUCT(IAI,IID_IAI)

#endif // _LG_AI_H
