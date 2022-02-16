/**********************
 * LGS Script Managers
 */

#ifndef _LG_SCRMANAGERS_H
#define _LG_SCRMANAGERS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/iiddef.h>

#include <lg/types.h>
#include <lg/defs.h>


// Forward declarations.
interface IAI;
interface IAIBehaviorSet;
interface IAIConversationManager;
interface IMotor;
interface IInputDevice;
interface ILinkQuery;
interface ILinkQueryDatabase;
interface ILoopClient;
interface ILoopClientFactory;
interface ILoopMode;
interface IObjectQuery;
interface IProperty;
interface IRelation;
interface IRes;
interface IResType;
interface ISearchPath;
interface IStore;
interface IStoreFactory;
interface ITagFile;
interface ITrait;


extern  const GUID  IID_IAIManager;
DECLARE_HANDLE(tAIIter);
interface IAIManager : IUnknown
{
	STDMETHOD_(long,InstallBehaviorSet)(IAIBehaviorSet *) PURE;
	STDMETHOD_(IAI*,GetAI)(int) PURE;
	STDMETHOD_(int,CreateAI)(int,const char *) PURE;
	STDMETHOD_(int,DestroyAI)(int) PURE;
	STDMETHOD_(int,SelfSlayAI)(int) PURE;
	virtual int f5(void) =0;
	STDMETHOD_(IAI*,GetFirst)(tAIIter*) PURE;
	STDMETHOD_(IAI*,GetNext)(tAIIter*) PURE;
	STDMETHOD_(void,EndIter)(tAIIter*) PURE;
	STDMETHOD_(long,SetMotor)(int,IMotor *) PURE;
	STDMETHOD_(long,ResetMotionTags)(int) PURE;
	STDMETHOD_(long,NotifyFastObj)(int,int,const mxs_vector &) PURE;
	STDMETHOD_(long,NotifySound)(const sSoundInfo *) PURE;
	STDMETHOD_(void,SetIgnoreSoundTime)(unsigned long) PURE;
	STDMETHOD_(int,StartConversation)(int) PURE;
	STDMETHOD_(IAIConversationManager*,GetConversationManager)(void) PURE;
	STDMETHOD_(void,ListenToProperty)(IProperty *) PURE;
	STDMETHOD_(void,ListenForCollisions)(int) PURE;
	STDMETHOD_(void,UnlistenForCollissions)(int) PURE;
	/*
	virtual void* __thiscall d0(unsigned int) =0;
	virtual void __thiscall OnNormalFrame(void) =0;
	virtual void __thiscall DatabaseReset(void) =0;
	*/
};
/*  IAIManager- debris
	virtual unsigned int CountAIs(void) ;
	virtual long  End(void) ;
	virtual long  Init(void) ;
	virtual int  ReceiveMessage(int,struct tLoopMessageData__  *) ;
	virtual int  GetPlayers(void *, int) ;
	virtual long  GetAITransferInfo(int, struct sAINetTransfer * *) ;
	virtual unsigned long  NetMsgHandlerID(void) ;
	virtual long  MakeFullAI(int, struct sAINetTransfer const *) ;
	virtual int  Networking(void) ;
	virtual long  MakeProxyAI(int, int) ;
	virtual int  IsProxy(int) ;
	virtual int  GetTargetVel(int, struct sAIImpulse *) ;
	virtual int  TransferAI(int, int) ;
	virtual void  SetObjImpulse(int, float, float, float, float, int) ;
 */
DEFINE_IIDSTRUCT(IAIManager,IID_IAIManager)

extern  const GUID  IID_IAIConversationManager;
struct IAIConversationManager : IUnknown
{
	STDMETHOD_(int,Start)(int) PURE;
	STDMETHOD_(void,SetActorObj)(int,int,int) PURE;
	STDMETHOD_(int,GetActorObj)(int,int,int *) PURE;
	STDMETHOD_(void,RemoveActorObj)(int,int) PURE;
	STDMETHOD_(int,GetNumActors)(int) PURE;
	STDMETHOD_(int,GetActorIDs)(int,int *) PURE;
	STDMETHOD_(void,Frame)(void) PURE;
	STDMETHOD_(void,ListenConversationEnd)(void (__cdecl *)(int)) PURE;
	STDMETHOD_(void,NotifyConversationEnd)(int) PURE;
	virtual void* d0(unsigned int) =0;
	virtual void f1(void) =0;
	STDMETHOD_(void,Save)(ITagFile *) PURE;
	STDMETHOD_(void,Load)(ITagFile *) PURE;
	STDMETHOD_(long,Reset)(void) PURE;
};
DEFINE_IIDSTRUCT(IAIConversationManager,IID_IAIConversationManager)

extern  const GUID  IID_IInputManager;
struct sInputDeviceIter
{
};
interface IInputManager : IUnknown
{
	STDMETHOD_(void,IterStart)(sInputDeviceIter *,_GUID) PURE;
	STDMETHOD_(void,IterNext)(sInputDeviceIter *) PURE;
	STDMETHOD_(IInputDevice*,IterGet)(sInputDeviceIter *) PURE;
	STDMETHOD_(int,IterFinished)(sInputDeviceIter *) PURE;
};
DEFINE_IIDSTRUCT(IInputManager,IID_IInputManager)

extern  const GUID  IID_ILinkManager;
struct sRelationIter
{
    int unknown;
};
interface ILinkManager : IUnknown
{
	// The functions below get a relation, which is a type of link
	STDMETHOD_(IRelation*,GetRelation)(short) PURE;
	STDMETHOD_(IRelation*,GetRelationNamed)(const char *) PURE;
	// I don't know what Notify does
	STDMETHOD_(long,Notify)(unsigned long,void *) PURE;
	// the next two probably iterate through relation types for DromEd.  We don't need them.   Or so I thought earlier.
	STDMETHOD_(long,IterStart)(sRelationIter *) PURE;
	STDMETHOD_(int,IterNext)(sRelationIter *,short *) PURE;
	// SShock2 
	STDMETHOD_(int,IsRelevant)(int) PURE;
/*** Add - Add a link between two objects.
 *  	= long - The ID of the created link.
 *  	: int - ID of the source object.
 *  	: int - ID of the destination object.
 *  	: short - Flavor of the link.
 */
	STDMETHOD_(long,Add)(int,int,short) PURE;
/*** AddFull - Add a link with data between two objects.
 *  	= long - The ID of the created link.
 *  	: int - ID of the source object.
 *  	: int - ID of the destination object.
 *  	: short - Flavor of the link.
 *  	: void * - Pointer to the data for the link. See lg/linkdefs.h
 */
	STDMETHOD_(long,AddFull)(int,int,short,void *) PURE;
/*** Remove - Deletes a link.
 *  	= long - ???.
 *  	: long - ID of the link to delete.
 */
	STDMETHOD_(long,Remove)(long) PURE;
/*** Get - Retrieve information about a link.
 *  	= int - ???. (Non-zero if info is valid?)
 *  	: long - ID of the link to query.
 *  	: sLink * - Address that the information will be copied to.
 */
	STDMETHOD_(int,Get)(long,sLink *) const PURE;
/*** SetData - Set the associated data for a link.
 *  	= long - ???.
 *  	: long - ID of the link to change.
 *  	: void * - Pointer to the data that will be copied into the link.
 */
	STDMETHOD_(long,SetData)(long,void *) PURE;
	// Is this pointer Alloced? Are we supposed to Free?
/*** GetData - Retreive the associated data for a link.
 *  	= void * - Pointer to the link data. (Is it alloced? Are we supposed to free it?)
 *  	: long - ID of the link to query.
 */
	STDMETHOD_(void*,GetData)(long) PURE;
/*** Query - Get all links of a particular flavor between two objects.
 *  	= ILinkQuery * - The query instance.
 *  	: int - ID of the source object.
 *  	: int - ID of the destination object.
 *  	: short - Flavor of the link(s).
 */
	STDMETHOD_(ILinkQuery*,Query)(int,int,short) PURE;
	// Don't touch this function!
	STDMETHOD_(long,RemapOnLoad)(long) PURE;
	// Can touch the ones below, but I recommend using IRelation
	STDMETHOD_(long,GetSingleLink)(short,int,int) PURE;
	STDMETHOD_(int,AnyLinks)(short,int,int) PURE;
	// Important little bugger
	STDMETHOD_(short,AddRelation)(IRelation *) PURE;
	STDMETHOD_(long,RemoveRelation)(short) PURE;
	STDMETHOD_(long,ReplaceRelation)(short,IRelation *) PURE;
	STDMETHOD_(long,AddQueryDB)(ILinkQueryDatabase *,unsigned long) PURE;
	STDMETHOD_(long,RemoveQueryDB)(ILinkQueryDatabase *,unsigned long) PURE;
	STDMETHOD_(int,Lock)(void) PURE;
	STDMETHOD_(long,Unlock)(int) PURE;
	STDMETHOD_(long,GetLockCount)(void) PURE;
	STDMETHOD_(int,LinkValid)(long,int) PURE;
	STDMETHOD_(long,LinkBirth)(long) PURE;
	STDMETHOD_(int,LinkDeath)(long) PURE;
};
DEFINE_IIDSTRUCT(ILinkManager,IID_ILinkManager)

extern  const GUID  IID_ILinkManagerInternal;

extern  const GUID  IID_ILoopManager;
DECLARE_HANDLE(tLoopClientData);
struct sLoopModeDesc
{
};
interface ILoopManager : IUnknown
{
	STDMETHOD_(long,AddClient)(ILoopClient *,unsigned long *) PURE;
	STDMETHOD_(long,RemoveClient)(unsigned long) PURE;
	STDMETHOD_(long,AddClientFactory)(ILoopClientFactory *,unsigned long *) PURE;
	STDMETHOD_(long,RemoveClientFactory)(unsigned long) PURE;
	STDMETHOD_(long,GetClient)(const _GUID *,tLoopClientData,ILoopClient **) PURE;
	STDMETHOD_(long,AddMode)(const sLoopModeDesc *) PURE;
	STDMETHOD_(ILoopMode*,GetMode)(const _GUID *) PURE;
	STDMETHOD_(long,RemoveMode)(const _GUID *) PURE;
	STDMETHOD_(long,SetBaseMode)(const _GUID *) PURE;
	STDMETHOD_(ILoopMode*,GetBaseMode)(void) PURE;
};
DEFINE_IIDSTRUCT(ILoopManager,IID_ILoopManager)

/*
extern  const GUID  IID_INetManager;
interface INetManager : IUnknown
{
	STDMETHOD_(int,Networking)(void) PURE;
	STDMETHOD_(int,IsNetworkGame)(void) PURE;
	STDMETHOD_(unsigned long,NumPlayers)(void) PURE;
	STDMETHOD_(unsigned long,MyPlayerNum)(void) PURE;
	STDMETHOD_(unsigned long,ObjToPlayerNum)(int) PURE;
	STDMETHOD_(int,PlayerNumToObj)(unsigned long) PURE;
	STDMETHOD_(void,SetPlayerName)(const char*) PURE;
	STDMETHOD_(const char*,GetPlayerName)(int) PURE;
	STDMETHOD_(const char*,GetPlayerAddress)(int) PURE;
	STDMETHOD_(int,Host)(const char*,const char*) PURE;
	STDMETHOD_(int,Join)(const char*,const char*,const char*) PURE;
	STDMETHOD_(unsigned long,GetTimeout)(void) PURE;
	STDMETHOD_(void,SetTimeout)(unsigned long) PURE;
	STDMETHOD_(void,SuspendMessaging)(void) PURE;
	STDMETHOD_(int,Suspended)(void) PURE;
	STDMETHOD_(void,ResumeMessaging)(void) PURE;
	STDMETHOD_(void,Send)(int.void*,unsigned long,int) PURE;
	STDMETHOD_(void,Broadcast)(void*,unsigned long,int) PURE;
	STDMETHOD_(void,MetagameBroadcast)(void*,unsigned long,int) PURE;
	STDMETHOD_(int,DefaultHost)(void) PURE;
	STDMETHOD_(int,AmDefaultHost)(void) PURE;
	STDMETHOD_(const char*,GetDefaultHostName)(void) PURE;
	STDMETHOD_(unsigned char,RegisterMessageParser)(void (__stdcall *)(const sNetMsg_Generic*,unsigned long,int,void*),const char*,unsigned long,void*) PURE;
	STDMETHOD_(void,NetPlayerIterStart)(sPropertyObjIter*) const PURE;
	STDMETHOD_(int,NetPlayerIterNext)(sPropertyObjIter*,int*) const PURE;
	STDMETHOD_(int,Listen)(void (__stdcall *)(unsigned long,unsigned long,void*),unsigned long,void*) PURE;
	STDMETHOD_(sGlobalObjID,ToGlobalObjID)(int) PURE;
	STDMETHOD_(int,FromGlobalObjID)(sGlobalObjID*) PURE;
	STDMETHOD_(int,OriginatingPlayer)(void) PURE;
	STDMETHOD_(void,StartSynch)(void) PURE;
	STDMETHOD_(void,NonNetworkLevel)(void) PURE;
	STDMETHOD_(void,NormalLevel)(void) PURE;
	STDMETHOD_(void,PreFlush)(void) PURE;
	STDMETHOD_(int,SynchFlush)(int (__stdcall *)(void*),void*) PURE;
	STDMETHOD_(const char*,GetPlayerNameByNum)(int) PURE;
	STDMETHOD_(const char*,GetPlayerAddressByNum)(int) PURE;
	STDMETHOD_(void,Leave)(void) PURE;
	STDMETHOD_(void,Unlisten)(int) PURE;
	STDMETHOD_(void,SetMaxPlayers)(int) PURE;
	virtual void* d0(unsigned int) = 0;
	STDMETHOD_(unsigned long,ToNetPlayerID)(int) PURE;
	STDMETHOD_(int,FromNetPlayerID)(unsigned long) PURE;
};
*/

extern  const GUID  IID_IPropertyManager;
struct sPropertyIter
{
	int unknown;
};
interface IPropertyManager : IUnknown
{
	STDMETHOD_(IProperty*,GetProperty)(int) const PURE;
	STDMETHOD_(IProperty*,GetPropertyNamed)(const char *) const PURE;
	STDMETHOD_(long,Notify)(unsigned long, void *) PURE;
	STDMETHOD_(long,BeginIter)(sPropertyIter *) PURE;
	STDMETHOD_(IProperty*,NextIter)(sPropertyIter *) PURE;
	STDMETHOD_(int,AddProperty)(IProperty *,int *) PURE;
	STDMETHOD_(int,DelProperty)(IProperty *) PURE;
};
DEFINE_IIDSTRUCT(IPropertyManager,IID_IPropertyManager)

extern  const GUID  IID_IResMan;
interface IResMan : IUnknown
{
	STDMETHOD_(ISearchPath*,NewSearchPath)(const char *) PURE;
	STDMETHOD_(void,SetDefaultPath)(ISearchPath *) PURE;
	STDMETHOD_(void,SetGlobalContext)(ISearchPath *) PURE;
	STDMETHOD_(void,SetDefaultVariants)(ISearchPath *) PURE;
	STDMETHOD_(IRes*,Bind)(const char *,const char *,ISearchPath *,const char *,unsigned int) PURE;
	STDMETHOD_(void,BindAll)(const char *,const char *,ISearchPath *,void (__stdcall *)(IRes *,IStore *,void *),void *,const char *,unsigned int) PURE;
	STDMETHOD_(IRes*,BindSpecific)(const char *,const char *,IStore *,IStore *,unsigned int) PURE;
	STDMETHOD_(IRes *,Retype)(IRes *,const char *,unsigned int) PURE;
	STDMETHOD_(IRes*,Lookup)(const char *,const char *,const char *) PURE;
	STDMETHOD_(IStore*,GetStore)(const char *) PURE;
	STDMETHOD_(int,RegisterResType)(IResType *) PURE;
	STDMETHOD_(int,RegisterStoreFactory)(IStoreFactory *) PURE;
	STDMETHOD_(void,UnregisterResType)(IResType *) PURE;
	STDMETHOD_(void,MarkForRefresh)(IRes *) PURE;
	STDMETHOD_(void,GlobalRefresh)(void) PURE;
};
DEFINE_IIDSTRUCT(IResMan,IID_IResMan)

extern  const GUID  IID_IResManHelper;
interface IResManHelper : IUnknown
{
	STDMETHOD_(void*,LockResource)(IRes *) PURE;
	STDMETHOD_(void*,ExtractResource)(IRes *,void *) PURE;
	STDMETHOD_(void*,FindResource)(IRes *,long *) PURE;
	STDMETHOD_(void,UnlockResource)(IRes *) PURE;
	STDMETHOD_(unsigned int,GetResourceLockCount)(IRes *) PURE;
	STDMETHOD_(int,DropResource)(IRes *) PURE;
	STDMETHOD_(long,GetResourceSize)(IRes *) PURE;
	STDMETHOD_(void,UnregisterResource)(IRes *,unsigned long) PURE;
	STDMETHOD_(int,AsyncLock)(IRes *,int) PURE;
	STDMETHOD_(int,AsyncExtract)(IRes *,int,void *,long) PURE;
	STDMETHOD_(int,AsyncPreload)(IRes *) PURE;
	STDMETHOD_(int,IsAsyncFulfilled)(IRes *) PURE;
	STDMETHOD_(long,AsyncKill)(IRes *) PURE;
	STDMETHOD_(long,GetAsyncResult)(IRes *,void * *) PURE;
};
/*
	virtual void Dump(const char *);
	virtual long End(void);
	virtual long Init(void);
	virtual void SetMode(enum eResStatMode, int);
	virtual void EnablePaging(int);
	virtual void DumpSnapshot(const char *);
	virtual void SetDefaultVariants(struct ISearchPath *);
	virtual void Compact(void);
*/
DEFINE_IIDSTRUCT(IResManHelper,IID_IResManHelper)

extern  const GUID  IID_IResManHelper;

extern  const GUID  IID_IScriptMan;
enum eScrTimedMsgKind {
	kSTM_OneShot,
	kSTM_Periodic
};
DECLARE_HANDLE(tScrTimer);
struct Datapath
{
};
class cScrTraceHashKey
{
	long m_Combo; // unknown type
};
class cScrTrace
{
	int m_HostObjID;
	eScrTraceAction m_TraceAction;
	int m_iTraceLine;
	cScrTraceHashKey m_TraceHashKey;
};
struct sScrMsg;
typedef int (__cdecl *ScrSaveLoadFunc)(void*,void*,unsigned int);

interface IScriptMan : IUnknown
{
	STDMETHOD_(long,GameInit)(void) PURE;
	STDMETHOD_(long,GameEnd)(void) PURE;
	STDMETHOD_(long,BeginScripts)(void) PURE;
	STDMETHOD_(long,EndScripts)(void) PURE;
	STDMETHOD_(long,SetModuleDatapath)(const Datapath *) PURE;
	STDMETHOD_(long,AddModule)(const char *) PURE;
	STDMETHOD_(long,RemoveModule)(const char *) PURE;
	STDMETHOD_(long,ClearModules)(void) PURE;
	STDMETHOD_(long,ExposeService)(IUnknown *,const _GUID &) PURE;
	STDMETHOD_(IUnknown *,GetService)(const _GUID &) PURE;
	STDMETHOD_(const sScrClassDesc*,GetFirstClass)(tScrIter*) PURE;
	STDMETHOD_(const sScrClassDesc*,GetNextClass)(tScrIter*) PURE;
	STDMETHOD_(void,EndClassIter)(tScrIter*) PURE;
	STDMETHOD_(const sScrClassDesc*,GetClass)(const char *) PURE;
	STDMETHOD_(long,SetObjScripts)(int,const char * *,unsigned int) PURE;
/*** ForgetObj - Drop the scripts for a single object. The scripts will be restarted when the Scripts property is reloaded.
 *  	= long -
 *  	: int - Object ID
 */
	STDMETHOD_(long,ForgetObj)(int) PURE;
/*** ForgetAllObjs - Terminates all scripts.  
 *  	= long -
 */
	STDMETHOD_(long,ForgetAllObjs)(void) PURE;
	STDMETHOD_(int,WantsMessage)(int,const char *) PURE;
/*** SendMessage - Send a message to another object. Returns when the message has been completely processed.
 *  	= long - 
 *  	: sScrMsg * - The message that will be sent. Can be allocated on the stack.
 *  	: sMultiParm * - The return result of the message. This is the pRet argument in ReceiveMessage. A string or vector type should be freed by the caller.
 */
	STDMETHOD_(long,SendMessage)(sScrMsg *,sMultiParm *) PURE;
/*** KillTimedMessage - Abort a message that was sent with SetTimedMessage2.
 *  	: tScrTimer - The pointer returned from SetTimedMessage2.
 */
	STDMETHOD_(void,KillTimedMessage)(tScrTimer) PURE;
	STDMETHOD_(int,PumpMessages)(void) PURE;
/*** PostMessage - Send a message without waiting for a reply.
 *  	: sScrMsg * - The message to post. Must be allocated on the heap.
 */
	STDMETHOD_(void,PostMessage)(sScrMsg *) PURE;
/*** SetTimedMessage - Send a message at some future time.
 *  	= tScrTimer - Pointer to the timer struct for the queued message.
 *  	: sScrMsg * - The message to send. Must be allocated on the heap. It doesn't have to be, but most timed messages are "Timer" in a sScrTimerMsg struct.
 *  	: unsigned long - How many milliseconds to wait before sending the message.
 *  	: eScrTimedMsgKind - Whether to send the message just once, or every time the period lapses.
 */
	STDMETHOD_(tScrTimer,SetTimedMessage)(sScrMsg *,unsigned long,eScrTimedMsgKind) PURE;
	// Aggregate return, is the psz union member Alloced?
/*** SendMessage2 - Construct and send a message.
 *  	= cMultiParm - The reply from the message. Aggregate return. A string or vector type should be freed by the caller.
 *  	: int - The source object. sScrMsg::from
 *  	: int - The destination object. sScrMsg::to
 *  	: const char * - The message string. sScrMsg::message
 *  	: const cMultiParm & - Extra data. sScrMsg::data
 *  	: const cMultiParm & - Extra data. sScrMsg::data2
 *  	: const cMultiParm & - Extra data. sScrMsg::data3
 */
	STDMETHOD_(cMultiParm*,SendMessage2)(cMultiParm &,int,int,const char *,const cMultiParm &,const cMultiParm &,const cMultiParm &) PURE;
/*** PostMessage2 - Construct and post a message. The parameters are copied, so they can be on the stack.
 *  	: int - The source object. sScrMsg::from
 *  	: int - The destination object. sScrMsg::to
 *  	: const char * - The message string. sScrMsg::message
 *  	: const cMultiParm & - Extra data. sScrMsg::data
 *  	: const cMultiParm & - Extra data. sScrMsg::data2
 *  	: const cMultiParm & - Extra data. sScrMsg::data3
 *  	: unsigned long - Message options. Not in Thief 1. sScrMsg::flags
 */
#if (_DARKGAME==1)
	STDMETHOD_(void,PostMessage2)(THIS_ int,int,const char *,const cMultiParm &,const cMultiParm &,const cMultiParm &) PURE;
#else
	STDMETHOD_(void,PostMessage2)(THIS_ int,int,const char *,const cMultiParm &,const cMultiParm &,const cMultiParm &,unsigned long) PURE;
#endif
/*** SetTimedMessage2 - Construct and send a "Timer" message.
 *  	= tScrTimer - Pointer to the timer struct for the queued message.
 *  	: int - The object to send the message to.
 *  	: const char * - The message name. sScrTimedMsg::name
 *  	: unsigned long - How many milliseconds to wait before sending the message.
 *  	: eScrTimedMsgKind - Whether to send the message just once, or every time the period lapses.
 *  	: const cMultiParm & - Extra data. sScrMsg::data
 */
	STDMETHOD_(tScrTimer,SetTimedMessage2)(int,const char *,unsigned long,eScrTimedMsgKind,const cMultiParm &) PURE;
/*** IsScriptDataSet - Checks if the persistent script data is valid.
 *  	= int - Non-zero if the data exists.
 *  	: const sScrDatumTag * - Description of the data to check for.
 */
	STDMETHOD_(int,IsScriptDataSet)(const sScrDatumTag *) PURE;
/*** GetScriptData - Returns the data stored in the persistent script database.
 *	= long - 
 *	: const sScrDatumTag * - Description of the data to retrieve.
 *	: sMultiParm * - Address that the data will be stored in. String and vector types will have allocated members.
 */
	STDMETHOD_(long,GetScriptData)(const sScrDatumTag *,sMultiParm *) PURE;
/*** SetScriptData - Copies arbitrary data into a persistent database.
 *	= long - 
 *	: const sScrDatumTag * - Description of the data being saved.
 *	: const sMultiParm * - The data that will be saved.
 */
	STDMETHOD_(long,SetScriptData)(const sScrDatumTag *,const sMultiParm *) PURE;
/*** ClearScriptData - Removes a data item from the persistent script database.
 *	= long - 
 *	: const sScrDatumTag * - Description of the data being deleted.
 *	: sMultiParm * - Receives the data that was stored in the database.
 */
	STDMETHOD_(long,ClearScriptData)(const sScrDatumTag *,sMultiParm *) PURE;
	STDMETHOD_(long,AddTrace)(int,char *,eScrTraceAction,int) PURE;
	STDMETHOD_(long,RemoveTrace)(int,char *) PURE;
	STDMETHOD_(int,GetTraceLine)(int) PURE;
	STDMETHOD_(void,SetTraceLine)(int,int) PURE;
	STDMETHOD_(int,GetTraceLineMask)(void) PURE;
	STDMETHOD_(void,SetTraceLineMask)(int) PURE;
	STDMETHOD_(const cScrTrace*,GetFirstTrace)(tScrIter*) PURE;
	STDMETHOD_(const cScrTrace*,GetNextTrace)(tScrIter*) PURE;
	STDMETHOD_(void,EndTraceIter)(tScrIter*) PURE;
	STDMETHOD_(long,SaveLoad)(ScrSaveLoadFunc,void *,int) PURE;
	STDMETHOD_(void,PostLoad)(void) PURE;
};
DEFINE_IIDSTRUCT(IScriptMan,IID_IScriptMan)

struct sDispatchListenerDesc;
struct sDispatchMsg;

typedef int (__cdecl *DispatchListenFunc)(const sDispatchMsg*, const sDispatchListenerDesc*);
struct sDispatchListenerDesc
{
	const IID*	pIID_Listener;
	DWORD	dwFlags;
	DispatchListenFunc	pfnListener;
	void*	pData;
};
struct sDispatchMsg
{
	DWORD	dwEventId;
	DWORD	dwDispatchId;
};
enum eSimEvent
{
	kSimStart   = 1,
	kSimStop    = 2,
	kSimSuspend = 4,
	kSimResume  = 8
};

extern  const GUID  IID_ISimManager;
interface ISimManager : IUnknown
{
	// IBaseSimMan
	STDMETHOD_(long,Listen)(const sDispatchListenerDesc *) PURE;
	STDMETHOD_(long,Unlisten)(const _GUID *) PURE;
	STDMETHOD_(long,SendMsg)(const sDispatchMsg *) PURE;
	STDMETHOD_(long,StartSim)(void) PURE;
	STDMETHOD_(long,SuspendSim)(void) PURE;
	STDMETHOD_(long,StopSim)(void) PURE;
	STDMETHOD_(unsigned long,LastMsg)(void) PURE;
	STDMETHOD_(long,DatabaseMsg)(unsigned long,ITagFile *) PURE;
};
DEFINE_IIDSTRUCT(ISimManager,IID_ISimManager)

extern  const GUID  IID_IStorageManager;
interface IStorageManager : IUnknown
{
	STDMETHOD_(void,RegisterFactory)(IStoreFactory *) PURE;
	STDMETHOD_(IStore*,GetStore)(const char *,int) PURE;
	STDMETHOD_(int,HeteroStoreExists)(IStore *,char * const *) PURE;
	STDMETHOD_(IStore*,CreateSubstore)(IStore *,const char *) PURE;
	STDMETHOD_(void,SetGlobalContext)(ISearchPath *) PURE;
	STDMETHOD_(void,SetDefaultVariants)(ISearchPath *) PURE;
	STDMETHOD_(ISearchPath*,NewSearchPath)(const char *) PURE;
	STDMETHOD_(void,Close)(void) PURE;
};
DEFINE_IIDSTRUCT(IStorageManager,IID_IStorageManager)

extern  const GUID  IID_IThreadManager;

extern  const GUID  IID_ITraitManager;
struct sTraitPredicate
{
};
enum eTraitQuery
{
	kTraitQueryFull = 0x80000000,
	kTraitQueryDefault = 0,
	kTraitQueryMetaProps = 1,
	kTraitQueryChildren = 2,
};
typedef void (__stdcall *TraitListenFunc)(const sHierarchyMsg*,void*);
interface ITraitManager : IUnknown
{
	STDMETHOD_(long,CreateBaseArchetype)(const char *,int *) PURE;
	STDMETHOD_(int,CreateArchetype)(const char *,int) PURE;
	STDMETHOD_(int,CreateConcreteArchetype)(const char *,int) PURE;
	STDMETHOD_(int,GetArchetype)(int) PURE;
	STDMETHOD_(long,SetArchetype)(int,int) PURE;
	STDMETHOD_(int,IsArchetype)(int) PURE;
	STDMETHOD_(int,RootMetaProperty)(void) PURE;
	STDMETHOD_(int,CreateMetaProperty)(const char *,int) PURE;
	STDMETHOD_(int,IsMetaProperty)(int) PURE;
	STDMETHOD_(long,AddObjMetaProperty)(int,int) PURE;
	STDMETHOD_(long,AddObjMetaPropertyPrioritized)(int,int,long) PURE;
	STDMETHOD_(long,RemoveObjMetaProperty)(int,int) PURE;
	STDMETHOD_(long,RemoveObjMetaPropertyPrioritized)(int,int,long) PURE;
	STDMETHOD_(int,ObjHasDonor)(int,int) PURE;
	STDMETHOD_(int,ObjHasDonorIntrinsically)(int,int) PURE;
	STDMETHOD_(int,ObjIsDonor)(int) PURE;
	STDMETHOD_(IObjectQuery*,Query)(int,unsigned long) PURE;
	STDMETHOD_(ITrait*,CreateTrait)(const sTraitDesc *,const sTraitPredicate *) PURE;
	STDMETHOD_(long,Listen)(TraitListenFunc,void *) PURE;
	STDMETHOD_(long,AddObject)(int,int) PURE;
	STDMETHOD_(long,RemoveObject)(int) PURE;
	STDMETHOD_(long,Notify)(unsigned long,void *) PURE;
};
DEFINE_IIDSTRUCT(ITraitManager,IID_ITraitManager)


#endif // _LG_SCRMANAGERS_H
