/**********************
 * LGS Script Services
 */

#ifndef _LG_SCRSERVICES_H
#define _LG_SCRSERVICES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>
#include <lg/iiddef.h>

#include <lg/types.h>
#include <lg/defs.h>


interface IScriptServiceBase : IUnknown
{
	STDMETHOD_(void,Init)(void) PURE;
	STDMETHOD_(void,End)(void) PURE;
};

extern  const GUID  IID_IActReactScriptService;
interface IActReactSrv : IScriptServiceBase
{
/*** React - Execute a reaction response.
 *  	= long - Returns 0 on success.
 *  	: reaction_kind - ID of the reaction to execute.
 *  	: float - Intensity of the reaction.
 *  	: object - The source object.
 *  	: object - The destination object.
 *  	: cMultiParm & - Argument for the reaction.
 *  	: ...
 */
	STDMETHOD_(long,React)(reaction_kind,float,object,object,const cMultiParm &,const cMultiParm &,const cMultiParm &,const cMultiParm &,const cMultiParm &,const cMultiParm &,const cMultiParm &,const cMultiParm &) PURE;
#if (_DARKGAME == 1)
/*** Stimulate - Stimulate an object.
 *  	= long - Returns 0 on success.
 *  	: object - The object to be stimulated.
 *  	: object - The stimulus object.
 *  	: float - The intensity of the stimulus.
 */
	STDMETHOD_(long,Stimulate)(object,object,float) PURE;
#else
/*** Stimulate - Stimulate an object.
 *  	= long - Returns 0 on success.
 *  	: object - The object to be stimulated.
 *  	: object - The stimulus object.
 *  	: float - The intensity of the stimulus.
 *  	: object - The source of the stimulus.
 */
	STDMETHOD_(long,Stimulate)(object,object,float,object) PURE;
#endif
/*** GetReactionNamed - Return the reaction ID given its name.
 *  	= int - ID of the reaction.
 *  	: const char * - The reaction name.
 */
	STDMETHOD_(int,GetReactionNamed)(const char *) PURE;
/*** GetReactionName - Return the name of a reaction given its ID.
 *  	= cScrStr - Name of the reaction.
 *  	: long - The reaction ID.
 */
	STDMETHOD_(cScrStr*,GetReactionName)(cScrStr &,long) PURE;
/*** SubscribeToStimulus - Register an object to receive stimulus messages of a particular type.
 *  	= long - Returns 0 on success.
 *  	: object - The object that will receive the messages.
 *  	: object - The type of stimulus.
 */
	STDMETHOD_(long,SubscribeToStimulus)(object,object) PURE;
/*** UnsubscribeToStimulus - Stop listening for stimulus messages.
 *  	= long - Returns 0 on success.
 *  	: object - The object that was previously registered.
 *  	: object - The type of stimulus.
 */
	STDMETHOD_(long,UnsubscribeToStimulus)(object,object) PURE;
/*** BeginContact - Initiate a stimulus contact on an object.
 *  	= long - Returns 0 on success.
 *  	: object - The stimulus object.
 *  	: object - The destination object.
 */
	STDMETHOD_(long,BeginContact)(object,object) PURE;
/*** EndContact - Terminate stimulus contact on an object.
 *  	= long - Returns 0 on success.
 *  	: object - The stimulus object.
 *  	: object - The destination object.
 */
	STDMETHOD_(long,EndContact)(object,object) PURE;
/*** SetSingleSensorContact - Briefly touch an object.
 *  	= long - Returns 0 on success.
 *  	: object - The stimulus object.
 *  	: object - The destination object.
 */
	STDMETHOD_(long,SetSingleSensorContact)(object,object) PURE;
};
DEFINE_IIDSTRUCT(IActReactSrv,IID_IActReactScriptService)

extern  const GUID  IID_IAIScriptService;
interface IAIScrSrv : IScriptServiceBase
{
/*** MakeGotoObjLoc - Instruct an AI to go to the current location of an object.
 *  	= true_bool - Success.
 *  	: int - Object ID of the AI.
 *  	: const object - The target object.
 *  	: eAIScriptSpeed - How quickly the AI will move.
 *  	: eAIActionPriority - Priority level the action will be evaluated at.
 *  	: const cMultiParm & - Arbitrary data that will be passed with the ObjActResult message.
 */
	STDMETHOD_(true_bool*,MakeGotoObjLoc)(true_bool &,int,const object &,eAIScriptSpeed,eAIActionPriority,const cMultiParm &) PURE;
/*** MakeFrobObjWith - Instruct an AI to frob an object with another object.
 *  	= true_bool - Success.
 *  	: int - Object ID of the AI.
 *  	: const object - The target object.
 *  	: const object - Object that the target will be frobbed with.
 *  	: eAIActionPriority - Priority level the action will be evaluated at.
 *  	: const cMultiParm & - Arbitrary data that will be passed with the ObjActResult message.
 */
	STDMETHOD_(true_bool*,MakeFrobObjWith)(true_bool &,int,const object &,const object &,eAIActionPriority,const cMultiParm &) PURE;
/*** MakeFrobObj - Instruct an AI to frob an object.
 *  	= true_bool - Success.
 *  	: int - Object ID of the AI.
 *  	: const object - The target object.
 *  	: eAIActionPriority - Priority level the action will be evaluated at.
 *  	: const cMultiParm & - Arbitrary data that will be passed with the ObjActResult message.
 */
	STDMETHOD_(true_bool*,MakeFrobObj)(true_bool &,int,const object &,eAIActionPriority,const cMultiParm &) PURE;
/*** GetAlertLevel - Return the current alert level of the AI.
 *  	= eAIScriptAlertLevel - The current level.
 *  	: int - Object ID of the AI.
 */
	STDMETHOD_(eAIScriptAlertLevel,GetAlertLevel)(int) PURE;
/*** SetMinimumAlert - Restrict the alert level of the AI.
 *  	: int - Object ID of the AI.
 *  	: eAIScriptAlertLevel - The lowest alert level allowable for the AI.
 */
	STDMETHOD_(void,SetMinimumAlert)(int,eAIScriptAlertLevel) PURE;
/*** ClearGoals - Make the AI forget what it's doing.
 *  	: int - Object ID of the AI.
 */
	STDMETHOD_(void,ClearGoals)(int) PURE;
/*** SetScriptFlags - Disable AI broadcasts.
 *  	: int - Object ID of the AI.
 *  	: int - Halt speech if 1. Clear flags if 0.
 */
	STDMETHOD_(void,SetScriptFlags)(int,int) PURE;
/*** ClearAlertness - Reset the alert level to it's lowest allowable value.
 *  	: int - Object ID of the AI.
 */
	STDMETHOD_(void,ClearAlertness)(int) PURE;
/*** Signal - Send a SignalAI message with a given string to the AI.
 *  	: int - Object ID of the AI.
 *  	: const cScrStr & - A string that will be sent with the signal.
 */
	STDMETHOD_(void,Signal)(int,const cScrStr  &) PURE;
/*** StartConversation - Run a conversation pseudo-script.
 *  	= true_bool - Success.
 *  	: int - The object with the conversation script.
 */
	STDMETHOD_(true_bool*,StartConversation)(true_bool &,int) PURE;
};
DEFINE_IIDSTRUCT(IAIScrSrv,IID_IAIScriptService)

extern  const GUID  IID_IAnimTextureScriptService;
interface IAnimTextureSrv : IScriptServiceBase
{
/*** ChangeTexture - Swap one texture on nearby polygons.
 *  	= long - Returns 0 on success.
 *  	: object - The host object.
 *  	: const char * - The path to the texture family of the old texture. May be NULL.
 *  	: const char * - The name of the old texture. If the path is NULL, this must be a full path.
 *  	: const char * - The path to the texture family of the new texture. May be NULL.
 *  	: const char * - The name of the new texture. If the path is NULL, this must be a full path.
 */
	STDMETHOD_(long,ChangeTexture)(object,const char *,const char *,const char *,const char *) PURE;
};
DEFINE_IIDSTRUCT(IAnimTextureSrv,IID_IAnimTextureScriptService)

extern  const GUID  IID_IBowScriptService;
interface IBowSrv : IScriptServiceBase
{
/*** Equip - Set the bow as the current weapon.
 *  	= long - Returns 1 on success.
 */
	STDMETHOD_(long,Equip)(void) PURE;
/*** UnEquip - Clear the current weapon if it was the bow.
 *  	= long - Returns 1 on success.
 */
	STDMETHOD_(long,UnEquip)(void) PURE;
/*** IsEquipped - Check if the bow is the current weapon.
 *  	= int - Non-zero if the bow is the current weapon.
 */
	STDMETHOD_(int,IsEquipped)(void) PURE;
/*** StartAttack - Begin drawing the bow.
 *  	= long - Returns 1 on success.
 */
	STDMETHOD_(long,StartAttack)(void) PURE;
/*** FinishAttack - Release the bow.
 *  	= long - Returns 1 on success.
 */
	STDMETHOD_(long,FinishAttack)(void) PURE;
/*** AbortAttack - Relax the bow without firing an arrow.
 *  	= long - Returns 1 on success.
 */
	STDMETHOD_(long,AbortAttack)(void) PURE;
/*** SetArrow - Make an object the current projectile for the bow.
 *  	= int - Returns 1 on success.
 *  	: object - The object that will be fired from the bow.
 */
	STDMETHOD_(int,SetArrow)(object) PURE;
};
DEFINE_IIDSTRUCT(IBowSrv,IID_IBowScriptService)

extern  const GUID  IID_ICameraScriptService;
interface ICameraSrv : IScriptServiceBase
{
/*** StaticAttach - Attach camera to object. Do not allow freelook.
 *  	= long - Returns 0.
 *  	: object - Object to attach to.
 */
	STDMETHOD_(long,StaticAttach)(object) PURE;
/*** DynamicAttach - Attach camera to object. Allow freelook.
 *  	= long - Returns 0.
 *  	: object - Object to attach to.
 */
	STDMETHOD_(long,DynamicAttach)(object) PURE;
/*** CameraReturn - Return the camera to Player if it's still attached to the object.
 *  	= long - Returns 0 if the camera was returned.
 *  	: object - Object to detach from. Does nothing if camera is attached to a different object.
 */
	STDMETHOD_(long,CameraReturn)(object) PURE;
/*** ForceCameraReturn - Return the camera to Player unconditionally.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,ForceCameraReturn)(void) PURE;

	//
	// new functions in T2 v1.22 / SS2 v2.43:
	//

	// get the object the camera is currently attached to
	STDMETHOD_(object*,GetCameraParent)(object &) PURE;

	// returns true if GetCameraParent() != PlayerObject()
	STDMETHOD_(true_bool, IsRemote)() PURE;

	// get location/rotation for the camera
	STDMETHOD_(cScrVec*,GetPosition)(cScrVec &) PURE;
	STDMETHOD_(cScrVec*,GetFacing)(cScrVec &) PURE;
};
DEFINE_IIDSTRUCT(ICameraSrv,IID_ICameraScriptService)

extern  const GUID  IID_IContainerScriptService;
interface IContainSrv : IScriptServiceBase
{
/*** Add - Move an object into a container.
 *  	= long - Returns 0 on success.
 *  	: object - The object to be contained.
 *  	: object - The container.
 *  	: int - The contain type.
 *  	: int - Combine the object if set to 1.
 */
	STDMETHOD_(long,Add)(object,object,int,int) PURE;
/*** Remove - Take an object out of a container.
 *  	= long - Returns 0.
 *  	: object - The object that was contained.
 *  	: object - The container.
 */
	STDMETHOD_(long,Remove)(object,object) PURE;
/*** MoveAllContents - Move all the objects in one container into another.
 *  	= long - Returns 0.
 *  	: object - The container to take the objects from.
 *  	: object - The container to place the objects in.
 *  	: int - Attempt to combine similar objects if set to 1.
 */
	STDMETHOD_(long,MoveAllContents)(object,object,int) PURE;
#if (_DARKGAME != 1)
/*** StackAdd - Modify the stack count of an object.
 *  	= long - The actual amount added.
 *  	: object - The object to add to.
 *  	: int - Amount to add. May be negative.
 */
	STDMETHOD_(long,StackAdd)(object,int) PURE;
/*** IsHeld - Check whether an object is contained by another.
 *  	= int - Returns the contain type or MAXLONG if the object is not contained.
 *  	: object - The container to check.
 *  	: object - The possibly contained object.
 */
	STDMETHOD_(int,IsHeld)(object,object) PURE;
#endif
};
DEFINE_IIDSTRUCT(IContainSrv,IID_IContainerScriptService)

extern  const GUID  IID_IDamageScriptService;
interface IDamageSrv : IScriptServiceBase
{
/*** Damage - Cause damage to an object.
 *  	= long - Returns 0.
 *  	: object - Victim
 *  	: object - Culprit
 *  	: int - Intensity of damage
 *  	: int - Type of damage
 */
	STDMETHOD_(long,Damage)(object,object,int,int) PURE;
/*** Slay - Slay an object.
 *  	= long - Returns 0.
 *  	: object - Victim
 *  	: object - Culprit
 */
	STDMETHOD_(long,Slay)(object,object) PURE;
/*** Resurrect - Reverse the effects of slaying an object.
 *  	= long - Returns 0.
 *  	: object - Un-Victim
 *  	: object - Culprit
 */
	STDMETHOD_(long,Resurrect)(object,object) PURE;
};
DEFINE_IIDSTRUCT(IDamageSrv,IID_IDamageScriptService)

extern  const GUID  IID_IDarkGameScriptService;
interface IDarkGameSrv : IScriptServiceBase
{
/*** KillPlayer - Garrett dies. Ignored if no_endgame is set.
 *  	= long - Returns 0 on success.
 */
	STDMETHOD_(long,KillPlayer)(void) PURE;
/*** EndMission - Immediately stop the current mission. Ignored if no_endgame is set.
 *  	= long - Returns 0 on success.
 */
	STDMETHOD_(long,EndMission)(void) PURE;
/*** FadeToBlack - Gradually fade the screen. Ignored if no_endgame is set.
 *  	= long - Returns 0 on success.
 *  	: float - Speed of the fade, in seconds. If negative, the screen will immediately return to full visibility.
 */
	STDMETHOD_(long,FadeToBlack)(float) PURE;
/*** FoundObject - Rings the secret bell.
 *  	= long - Returns 0.
 *  	: int - object ID. (Will only work once per object?)
 */
	STDMETHOD_(long,FoundObject)(int) PURE;
/*** ConfigIsDefined - Test if a config variable has been set.
 *  	= int - Is non-zero if the variable has been set.
 *  	: const char * - The config variable to test for.
 */
	STDMETHOD_(int,ConfigIsDefined)(const char *) PURE;
/*** ConfigGetInt - Return the value of a config variable interpreted as an integer.
 *  	= int - Returns non-zero on success.
 *  	: const char * - The config variable to retrieve.
 *  	: int * - Address of a variable that will recieve the value.
 */
	STDMETHOD_(int,ConfigGetInt)(const char *,int *) PURE;
/*** ConfigGetFloat - Return the value of a config variable interpreted as an floating-point number.
 *  	= int - Returns non-zero on success.
 *  	: const char * - The config variable to retrieve.
 *  	: float * - Address of a variable that will recieve the value.
 */
	STDMETHOD_(int,ConfigGetFloat)(const char *,float *) PURE;
/*** BindingGetFloat - Return the floating-point value of a key-binding variable.
 *  	= float - The value of the variable.
 *  	: const char * - The name of the variable.
 */
	STDMETHOD_(float,BindingGetFloat)(const char *) PURE;
/*** GetAutomapLocationVisited - Return whether a region of the auto-map is marked as visited.
 *  	= int - Non-zero if the region was visited.
 *  	: int - The page number of the location.
 *  	: int - The room number of the location.
 */
	STDMETHOD_(int,GetAutomapLocationVisited)(int,int) PURE;
/*** SetAutomapLocationVisited - Mark a region of the auto-map as having been visited.
 *  	= long - Returns 0.
 *  	: int - The page number of the location.
 *  	: int - The room number of the location.
 */
	STDMETHOD_(long,SetAutomapLocationVisited)(int,int) PURE;

	//
	// new functions in v1.19:
	//

	// set/change the next mission that will follow current mission (normally the next mission is cur+1 or if
	// "missflag.str" contains a miss_%d_next the next mission is defined by that)
	STDMETHOD_(void, SetNextMission)(int mission) PURE;

	// get current mission number
	STDMETHOD_(int, GetCurrentMission)() PURE;
};
DEFINE_IIDSTRUCT(IDarkGameSrv,IID_IDarkGameScriptService)

extern  const GUID  IID_IDarkUIScriptService;
interface IDarkUISrv : IScriptServiceBase
{
/*** TextMessage - Display a string on the screen.
 *  	= long - Returns 0.
 *  	: const char * - The message to display.
 *  	: int - The color of the text. Equivalent to Win32 COLORREF. If this is zero, uses the default color, which is white.
 *  	: int - The maximum time, in milliseconds, to display the message. Passing -1001 will use the default time of 5 secs. (Actual time may be shorter if another call to TextMessage is made.)
 */
	STDMETHOD_(long,TextMessage)(const char *,int,int) PURE;
/*** ReadBook - Enter book mode.
 *  	= long - Returns 0.
 *  	: const char * - The resource name of the book.
 *  	: const char * - The background image that will be shown.
 */
	STDMETHOD_(long,ReadBook)(const char *,const char *) PURE;
/*** InvItem - Get the currently selected inventory item.
 *  	= object - The inventory item.
 */
	STDMETHOD_(object*,InvItem)(object &) PURE;
/*** InvWeapon - Get the currently selected weapon.
 *  	= object - Object ID of the weapon.
 */
	STDMETHOD_(object*,InvWeapon)(object &) PURE;
/*** InvSelect - Set an object to be the active inventory selection.
 *  	= long - Returns 0 on success.
 *  	: object - Object ID of the item.
 */
	STDMETHOD_(long,InvSelect)(object) PURE;
/*** IsCommandBound - Test if a command string is bound to a key sequence.
 *  	= true_bool - Non-zero if the command is bound. Aggregate return.
 *  	: const cScrStr & - The command string to check for.
 */
	STDMETHOD_(true_bool*,IsCommandBound)(true_bool &,const cScrStr &) PURE;
/*** DescribeKeyBinding - Return the key-binding for a particular command.
 *  	= cScrStr - The keys bound to this command. Aggregate return.
 *  	: const cScrStr & - The command string.
 */
	STDMETHOD_(cScrStr*,DescribeKeyBinding)(cScrStr &,const cScrStr &) PURE;
};
DEFINE_IIDSTRUCT(IDarkUISrv,IID_IDarkUIScriptService)

extern  const GUID  IID_IDataScriptService;
interface IDataSrv : IScriptServiceBase
{
/*** GetString - Retrieve a string from a resource file.
 *  	= cScrStr - Aggregate return. Caller should free.
 *  	: const char * - The name of the resource file.
 *  	: const char * - The string name to get.
 *  	: const char * - unused
 *  	: const char * - The resource directory to look in. ("Books" is not a normal resource, use "..\Books\file" for the filename and "strings" here.)
 */
	STDMETHOD_(cScrStr*,GetString)(cScrStr &,const char *,const char *,const char *,const char *) PURE;
/*** GetObjString - Retrieve one of the standard object strings. Uses the GameName property.
 *  	= cScrStr - Aggregate return. Caller should free.
 *  	: int - The object ID to get the string of.
 *  	: const char * - The resource name to look in. "objnames" or "objdescs"
 */
	STDMETHOD_(cScrStr*,GetObjString)(cScrStr &,int,const char *) PURE;
/*** DirectRand - Return a (psuedo) random number in the range [0,32767]
 *  	= int - A randomly generated number.
 */
	STDMETHOD_(int,DirectRand)(void) PURE;
/*** RandInt - Return a (psuedo) random number in a given range. The range is inclusive on both ends.
 *  	= int - A randomly generated number.
 *  	: int - The low end of the range. The generated number will not be less than this.
 *  	: int - The high end of the range. The generated number will not be greater than this.
 */
	STDMETHOD_(int,RandInt)(int,int) PURE;
/*** RandFlt0to1 - Return a (psuedo) random floating-point number in the range [0,1].
 *  	= float - A randomly generated number.
 */
	STDMETHOD_(float,RandFlt0to1)(void) PURE;
/*** RandFltNeg1to1 - Return a (psuedo) random floating-point number in the range [-1,1).
 *  	= float - A randomly generated number.
 */
	STDMETHOD_(float,RandFltNeg1to1)(void) PURE;
};
DEFINE_IIDSTRUCT(IDataSrv,IID_IDataScriptService)

extern  const GUID  IID_IDebugScriptService;
interface IDebugScrSrv : IScriptServiceBase
{
/*** MPrint - Send string(s) to the mono.
 *  	= long - Returns 0.
 *  	: cScrStr - A string to print.
 *  	: ... 
 */
	STDMETHOD_(long,MPrint)(const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &) PURE;
/*** Command - Execute commands.
 *  	= long - Returns 0.
 *  	: cScrStr - Command string. A space will be inserted between this and the rest of the arguments.
 *  	: cScrStr - Command argument.
 *  	: ... 
 */
	STDMETHOD_(long,Command)(const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &,const cScrStr &) PURE;
/*** Break - Raise a debugger interrupt.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,Break)(void) PURE;

	//
	// new functions in T2 v1.19 / SS2 v2.4:
	//

	// log file output (works in game exe too)
	STDMETHOD (Log)(const cScrStr &, const cScrStr &, const cScrStr &, const cScrStr &, const cScrStr &, const cScrStr &, const cScrStr &, const cScrStr &) PURE;
};
DEFINE_IIDSTRUCT(IDebugScrSrv,IID_IDebugScriptService)

extern  const GUID  IID_IDoorScriptService;
interface IDoorSrv : IScriptServiceBase
{
/*** CloseDoor - Closes a door.
 *  	= int - Non-zero on success.
 *  	: object - The door to close.
 */
	STDMETHOD_(int,CloseDoor)(object) PURE;
/*** OpenDoor - Open a door.
 *  	= int - Non-zero on success.
 *  	: object - The door to open.
 */
	STDMETHOD_(int,OpenDoor)(object) PURE;
/*** GetDoorState - Return whether a door is open or closed (or something else).
 *  	= int - The state of the door. See eDoorState in lg/defs.h. Returns 5 if the object is not a door.
 *  	: object - The door.
 */
	STDMETHOD_(int,GetDoorState)(object) PURE;
/*** ToggleDoor - Switch the state of a door.
 *  	= long - Non-zero on success.
 *  	: object - The door to open/close.
 */
	STDMETHOD_(long,ToggleDoor)(object) PURE;
#if (_DARKGAME == 2)
/*** SetBlocking - Set whether a door can block sound.
 *  	= long - Non-zero on success.
 *  	: object - The door to change.
 *  	: int - The blocking state. Zero to not block sound, non-zero to block.
 */
	STDMETHOD_(long,SetBlocking)(object,int) PURE;
/*** GetSoundBlocking - Return if a door blocks sound.
 *  	= int - Non-zero if the door blocks sound.
 *  	: object - The door to check..
 */
	STDMETHOD_(int,GetSoundBlocking)(object) PURE;
#endif
};
DEFINE_IIDSTRUCT(IDoorSrv,IID_IDoorScriptService)

extern  const GUID  IID_IDrkInvScriptService;
enum eDrkInvCap
{
	kDrkInvCapCycle,
	kDrkInvCapWorldFrob,
	kDrkInvCapWorldFocus,
	kDrkInvCapInvFrob
};
enum eDrkInvControl
{
	kDrkInvControlOn,
	kDrkInvControlOff,
	kDrkInvControlToggle
};
interface IDarkInvSrv : IScriptServiceBase
{
/*** CapabilityControl - 
 *  	: eDrkInvCap - 
 *  	: eDrkInvControl - 
 */
	STDMETHOD_(void,CapabilityControl)(eDrkInvCap,eDrkInvControl) PURE;
/*** AddSpeedControl - Change the speed of Player.
 *  	: char * - ID of the control. Speed controls can be stacked and removed out-of-order.
 *  	: float - Amount to multiply the speed by.
 *  	: float - Multiplier for turning speed. Appears to be ineffective.
 */
	STDMETHOD_(void,AddSpeedControl)(const char *,float,float) PURE;
/*** RemoveSpeedControl - Remove a previously applied speed control.
 *  	: char * - ID of the control.
 */
	STDMETHOD_(void,RemoveSpeedControl)(const char *) PURE;
};
DEFINE_IIDSTRUCT(IDarkInvSrv,IID_IDrkInvScriptService)

extern  const GUID  IID_IDrkPowerupsScriptService;
interface IDarkPowerupsSrv : IScriptServiceBase
{
/*** TriggerWorldFlash - Create a blinding flash.
 *  	: object - The flash will occur at the location of this object. Any AI with a line-of-sight to the object will be blinded. A RenderFlash link on the object's archetype determines how the flash will appear on screen.
 */
	STDMETHOD_(void,TriggerWorldFlash)(object) PURE;
/*** ObjTryDeploy - Create a new object at the location of another object.
 *  	= int - ID of created object.
 *  	: object - Type of object to create.
 *  	: object - Emitter object.
 */
	STDMETHOD_(int,ObjTryDeploy)(object,object) PURE;
/*** CleanseBlood - Slay nearby objects with Prox\Blood
 *  	: object - Culprit object.
 *  	: float - The radius around the object that will be cleaned.
 */
	STDMETHOD_(void,CleanseBlood)(object,float) PURE;
};
DEFINE_IIDSTRUCT(IDarkPowerupsSrv,IID_IDrkPowerupsScriptService)

extern  const GUID  IID_IKeyScriptService;
interface IKeySrv : IScriptServiceBase
{
/*** TryToUseKey - Use or test a key on an object.
 *  	= int - Non-zero if the operation was successful.
 *  	: const object & - The key object.
 *  	: const object & - The object to use the key on.
 *  	: eKeyUse - The operation to attempt.
 */
	STDMETHOD_(int,TryToUseKey)(const object &,const object &,eKeyUse) PURE;
};
DEFINE_IIDSTRUCT(IKeySrv,IID_IKeyScriptService)

extern  const GUID  IID_ILightScriptService;
interface ILightScrSrv : IScriptServiceBase
{
/*** Set - Set the mode and min/max values of the light.
 *  	: const object & - The light object.
 *  	: int - The light mode.
 *  	: float - Minimum brightness of the light.
 *  	: float - Maximum brightness of the light.
 */
	STDMETHOD_(void,Set)(const object &,int,float,float) PURE;
/*** SetMode - Set the current mode of the light.
 *  	: const object & - The light object.
 *  	: int - The light mode.
 */
	STDMETHOD_(void,SetMode)(const object &,int) PURE;
/*** Activate - Clear the inactive flag on the light.
 *  	: const object & - The light object.
 */
	STDMETHOD_(void,Activate)(const object &) PURE;
/*** Deactivate - Set the inactive flag on the light.
 *  	: const object & - The light object.
 */
	STDMETHOD_(void,Deactivate)(const object &) PURE;
/*** Subscribe - Listen for LightChange messages.
 *  	: const object & - The light object.
 */
	STDMETHOD_(void,Subscribe)(const object &) PURE;
/*** Unsubscribe - Stop listening for LightChange messages.
 *  	: const object & - The light object.
 */
	STDMETHOD_(void,Unsubscribe)(const object &) PURE;
/*** GetMode - Return the current light mode.
 *  	= int - The current mode.
 *  	: const object & - The light object.
 */
	STDMETHOD_(int,GetMode)(const object &) PURE;
};
DEFINE_IIDSTRUCT(ILightScrSrv,IID_ILightScriptService)

extern  const GUID  IID_ILinkScriptService;
interface ILinkSrv : IScriptServiceBase
{
/*** Create - Add a link between two objects.
 *  	= link - The created link. Aggregate return.
 *  	: linkkind - The flavor of the link to create.
 *  	: object - The source of the link.
 *  	: object - The destination of the link.
 */
	STDMETHOD_(link*,Create)(link &,linkkind,object,object) PURE;
/*** Destroy - Removes a link.
 *  	= long - Non-zero on success.
 *  	: link - The link to remove.
 */
	STDMETHOD_(long,Destroy)(link) PURE;
/*** AnyExist - Check if there is a link of a certain flavor between two objects.
 *  	= true_bool - True if at least one link exists. Aggregate return.
 *  	: linkkind - The flavor of the link to look for.
 *  	: object - The source of the link.
 *  	: object - The destination of the link.
 */
	STDMETHOD_(true_bool*,AnyExist)(true_bool &,linkkind,object,object) PURE;
/*** GetAll - Get all the links of certain flavor between two objects.
 *  	= linkset - The query of the requested links. Aggregate return.
 *  	: long - The link flavor requested. Not a linkkind for some buggy reason.
 *  	: object - The source of the links.
 *  	: object - The destination of the links.
 */
	STDMETHOD_(linkset*,GetAll)(linkset &,long,object,object) PURE;
/*** GetOne - Get just one link of certain flavor between two objects.
 *  	= link - The link. Aggregate return.
 *  	: linkkind - The flavor of the link requested.
 *  	: object - The source of the link.
 *  	: object - The destination of the link.
 */
	STDMETHOD_(link*,GetOne)(link &,linkkind,object,object) PURE;
/*** BroadcastOnAllLinks - Send a message to the destination of links from an object.
 *  	= long - Returns 0.
 *  	: const object & - The source of the link(s). Also the source of the message(s).
 *  	: const char * - The name of the message to send.
 *  	: linkkind - The flavor of the link(s) to send the message accross.
 */
	STDMETHOD_(long,BroadcastOnAllLinks)(const object &,const char *,linkkind) PURE;
/*** BroadcastOnAllLinksData - Send a message to the destination of links from an object, comparing data.
 *  	= long - Returns 0.
 *  	: const object & - The source of the link(s). Also the source of the message(s).
 *  	: const char * - The name of the message to send.
 *  	: linkkind - The flavor of the link(s) to send the message accross.
 *  	: const cMultiParm & - The message is sent if the link data matches this exactly. It is copied to the first data parameter of the message.
 */
	STDMETHOD_(long,BroadcastOnAllLinksData)(const object &,const char *,linkkind,const cMultiParm &) PURE;
/*** CreateMany - Add links between many different objects.
 *  	= long - Returns 0 on success.
 *  	: linkkind - The flavor of the links to create.
 *  	: const cScrStr & - The sources of the links.
 *  	: const cScrStr & - The destinations of the links.
 */
	STDMETHOD_(long,CreateMany)(linkkind,const cScrStr &,const cScrStr &) PURE;
/*** DestroyMany - Remove multiple links from objects.
 *  	= long - Returns 0 on success.
 *  	: linkkind - The flavor of the links to destroy.
 *  	: const cScrStr & - The sources of the links.
 *  	: const cScrStr & - The destinations of the links.
 */
	STDMETHOD_(long,DestroyMany)(linkkind,const cScrStr &,const cScrStr &) PURE;
/*** GetAllInherited - Retrieve links between objects and their descendants.
 *  	= linkset - The links between the objects. Aggregate return.
 *  	: linkkind - The flavor of the links to retrieve.
 *  	: object - The parent source of the links.
 *  	: object - The parent destination of the links.
 */
	STDMETHOD_(linkset*,GetAllInherited)(linkset &,linkkind,object,object) PURE;
/*** GetAllInheritedSingle - Retrieve links between objects and their descendants.
 *  	= linkset - The links between the objects. Aggregate return.
 *  	: linkkind - The flavor of the links to retrieve.
 *  	: object - The parent source of the links.
 *  	: object - The parent destination of the links.
 */
	STDMETHOD_(linkset*,GetAllInheritedSingle)(linkset &,linkkind,object,object) PURE;
};
DEFINE_IIDSTRUCT(ILinkSrv,IID_ILinkScriptService)

extern  const GUID  IID_ILinkToolsScriptService;
interface ILinkToolsSrv : IScriptServiceBase
{
/*** LinkKindNamed - Get the ID of a link flavor
 *  	= long - The flavor ID.
 *  	: const char * - The name of the flavor.
 */
	STDMETHOD_(long,LinkKindNamed)(const char *) PURE;
/*** LinkKindName - Get the name of a link flavor.
 *  	= cScrStr - The flavor name. Aggregate return.
 *  	: long - The ID of the flavor.
 */
	STDMETHOD_(cScrStr*,LinkKindName)(cScrStr &,long) PURE;
/*** LinkGet - Fill a structure with information about a link.
 *  	= long - Returns 0.
 *  	: long - The link ID.
 *  	: sLink & - Address of the structure that will recieve the information.
 */
	STDMETHOD_(long,LinkGet)(long,sLink &) PURE;
/*** LinkGetData - Retrieve the data associated with a link.
 *  	= cMultiParm - Aggregate return. Caller should free.
 *  	: long - The link ID.
 *  	: const char * - Name of the structure field to retrieve.
 */
	STDMETHOD_(cMultiParm*,LinkGetData)(cMultiParm &,long,const char *) PURE;
/*** LinkSetData - Set the data associated with a link.
 *  	= long - Returns 0.
 *  	: long - The link ID.
 *  	: const char * - The structure field to modify.
 *  	: const cMultiParm & - The data to assign.
 */
	STDMETHOD_(long,LinkSetData)(long,const char *,const cMultiParm &) PURE;
};
DEFINE_IIDSTRUCT(ILinkToolsSrv,IID_ILinkToolsScriptService)

extern  const GUID  IID_ILockedScriptService;
interface ILockSrv : IScriptServiceBase
{
/*** IsLocked - Test whether an object has the Locked property set.
 *  	= int - Non-zero if the object is locked.
 *  	: const object & - The object to check.
 */
	STDMETHOD_(int,IsLocked)(const object &) PURE;
};
DEFINE_IIDSTRUCT(ILockSrv,IID_ILockedScriptService)

extern  const GUID  IID_INetworkingScriptService;
interface INetworkingSrv : IScriptServiceBase
{
#if (_DARKGAME == 3) || ((_DARKGAME == 2) && (_NETWORKING == 1))
/*** Broadcast - Send a message to over the network.
 *  	= long - Non-zero on success.
 *  	: const object & - The object to send to.
 *	: const char * - The message name.
 *	: int - Send the message to the local object if non-zero.
 *	: const cMultiParm & - Message data.
 */
	STDMETHOD_(long,Broadcast)(const object &,const char*,int,const cMultiParm &) PURE;
/*** SendToProxy - Send a message to an object on another computer.
 *  	= long - Non-zero on success.
 *  	: const object & - The source object, and the computer that will send the message.
 *	: const object & - The destination of the message.
 *	: const char * - The message name.
 *	: const cMultiParm & - Message data.
 */
	STDMETHOD_(long,SendToProxy)(const object &,const object &,const char*,const cMultiParm &) PURE;
/*** TakeOver - Become the host of an object.
 *  	= long - Non-zero on success.
 *  	: const object & - Object to grab.
 */
	STDMETHOD_(long,TakeOver)(const object &) PURE;
/*** GiveTo - Let an object be hosted by a different object.
 *  	= long - Non-zero on success.
 *  	: const object & - Object to give.
 *  	: const object & - The new host.
 */
	STDMETHOD_(long,GiveTo)(const object &,const object &) PURE;
/*** IsPlayer - Check if an object is one of the players.
 *  	= int - Non-zero if the object is a player.
 *  	: const object & - Object to test.
 */
	STDMETHOD_(int,IsPlayer)(const object &) PURE;
/*** IsMultiplayer - Check if the game is running in multi-player mode.
 *  	= int - Non-zero if multi-player mode.
 */
	STDMETHOD_(int,IsMultiplayer)(void) PURE;
/*** SetProxyOneShotTimer - Send a timed message to the proxy for an object.
 *  	= int - Timer handle.
 *  	: const object & - The destination object.
 *	: const char * - The (Timer?) message name.
 *	: float - Time to delay the message, in seconds.
 *	: const cMultiParm & - Message data.
 */
	STDMETHOD_(int,SetProxyOneShotTimer)(const object &,const char *,float,const cMultiParm &) PURE;
/*** FirstPlayer - Begin an iteration of all players.
 *  	= object - The object ID of the first player. Aggregate return.
 */
	STDMETHOD_(object*,FirstPlayer)(object &) PURE;
/*** NextPlayer - Continue an iteration of all players.
 *  	= object - The object ID of another player. Aggregate return.
 */
	STDMETHOD_(object*,NextPlayer)(object &) PURE;
/*** Suspend - Prevent network messages from propagating.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,Suspend)(void) PURE;
/*** Resume - Allow network communication again. (Possibly flushes queued messages?)
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,Resume)(void) PURE;
/*** HostedHere - Check if an object is hosted by this computer.
 *  	= int - Non-zero if the object is hosted here.
 *  	: const object & - Object to test.
 */
	STDMETHOD_(int,HostedHere)(const object &) PURE;
/*** IsProxy - Check if an object is hosted somewhere else.
 *  	= int - Non-zero if the object is a remote proxy.
 *  	: const object & - Object to test.
 */
	STDMETHOD_(int,IsProxy)(const object &) PURE;
/*** LocalOnly - Check if an object is locally specific.
 *  	= int - Non-zero if the object is not on the network.
 *  	: const object & - Object to test.
 */
	STDMETHOD_(int,LocalOnly)(const object &) PURE;
/*** IsNetworking - Check if the computer is connected to a network.
 *  	= int - Non-zero if networking is active.
 */
	STDMETHOD_(int,IsNetworking)(void) PURE;
#if (_DARKGAME == 2)
/*** Owner - Get the object that hosts an object.
 *  	= object - The host object. Aggregate return.
 *  	: const object & - The hosted object.
 */
	STDMETHOD_(object*,Owner)(object &,const object &) PURE;
#endif
#endif
};
DEFINE_IIDSTRUCT(INetworkingSrv,IID_INetworkingScriptService)

extern  const GUID  IID_INullScriptService;

extern  const GUID  IID_IObjectScriptService;
interface IObjectSrv : IScriptServiceBase
{
/*** BeginCreate - Do the first half of a two-stage object creation.
 *  	= object - The new object. Aggregate return.
 *  	: object - The parent archetype for the new object.
 */
	STDMETHOD_(object*,BeginCreate)(object &,object) PURE;
/*** EndCreate - Finish creating an object returned from BeginCreate.
 *  	= long - Returns 0 on success.
 *  	: object - The created object.
 */
	STDMETHOD_(long,EndCreate)(object) PURE;
/*** Create - Add an object to the database.
 *  	= object - The new object. Aggregate return.
 *  	: object - The parent archetype for the new object.
 */
	STDMETHOD_(object*,Create)(object &,object) PURE;
/*** Destroy - Remove an object from the database.
 *  	= long - Returns 0 on success.
 *  	: object - The object to destroy.
 */
	STDMETHOD_(long,Destroy)(object) PURE;
/*** Exists - Check if an object is in the database.
 *  	= true_bool - True if the object is valid. Aggregate return.
 *  	: object - The object to look for.
 */
	STDMETHOD_(true_bool*,Exists)(true_bool &,object) PURE;
/*** SetName - Change an object's name.
 *  	= long - Returns 0.
 *  	: object - The object to change.
 *  	: const char * - The new name of the object.
 */
	STDMETHOD_(long,SetName)(object,const char *) PURE;
/*** GetName - Return an object's name.
 *  	= cScrStr - The name of the object. Aggregate return. Should be freed by the caller.
 *  	: object - The object.
 */
	STDMETHOD_(cScrStr*,GetName)(cScrStr &,object) PURE;
/*** Named - Find an object given a name.
 *  	= object - The found object. Aggregate return.
 *  	: const char * - The name of the object.
 */
	STDMETHOD_(object*,Named)(object &,const char *) PURE;
/*** AddMetaProperty - Add a metaproperty to an object.
 *  	= long - Returns 0.
 *  	: object - The object to add the metaproperty to.
 *  	: object - The metaproperty to add.
 */
	STDMETHOD_(long,AddMetaProperty)(object,object) PURE;
/*** RemoveMetaProperty - Add a metaproperty to an object.
 *  	= long - Returns 0.
 *  	: object - The object to remove the metaproperty from.
 *  	: object - The metaproperty to remove.
 */
	STDMETHOD_(long,RemoveMetaProperty)(object,object) PURE;
/*** HasMetaProperty - Check if a metaproperty is applied to an object.
 *  	= true_bool - True if the object is linked with the metaproperty. Aggregate return.
 *  	: object - The object that might have the metaproperty.
 *  	: object - The metaproperty to look for.
 */
	STDMETHOD_(true_bool*,HasMetaProperty)(true_bool &,object,object) PURE;
/*** InheritsFrom - Check if an object inherits properties from another object. May be a direct ancestor or a metaproperty.
 *  	= true_bool - True if the object inherits from the archetype. Aggregate return.
 *  	: object - The object that might be a descendant.
 *  	: object - The ancestor object.
 */
	STDMETHOD_(true_bool*,InheritsFrom)(true_bool &,object,object) PURE;
/*** IsTransient - Check if an object is transient.
 *  	= true_bool - True if the object is transient. Aggregate return.
 *  	: object - The object to check.
 */
	STDMETHOD_(true_bool*,IsTransient)(true_bool &,object) PURE;
/*** SetTransience - Make an object transient, or not.
 *  	= long - Returns 0.
 *  	: object - The object to change.
 *  	: true_bool - Whether or not the object should be transient.
 */
	STDMETHOD_(long,SetTransience)(object,true_bool) PURE;
/*** Position - Get the location of an object.
 *  	= cScrVec - The object's XYZ vector. Aggregate return.
 *  	: object - The object to query.
 */
	STDMETHOD_(cScrVec*,Position)(cScrVec &,object) PURE;
/*** Facing - Get the direction an object is facing.
 *  	= cScrVec - The object's HPB vector. Aggregate return.
 *  	: object - The object to query.
 */
	STDMETHOD_(cScrVec*,Facing)(cScrVec &,object) PURE;
/*** Teleport - Move an object to a different location.
 *  	= long - Returns 0.
 *  	: object - The object to move.
 *  	: const cScrVec & - The new location for the object.
 *  	: const cScrVec & - The new facing for the object.
 *  	: object - An object to measure the new location and facing relative from. 0 for absolute positioning.
 */
	STDMETHOD_(long,Teleport)(object,const cScrVec  &,const cScrVec  &,object) PURE;
#if (_DARKGAME == 2) || (_DARKGAME == 3)
/*** IsPositionValid - Check if the object is inside the worldrep.
 *  	= true_bool - True if the object is not in solid space. Aggregate return.
 *  	: object - The object to query.
 */
	STDMETHOD_(true_bool*,IsPositionValid)(true_bool &,object) PURE;
#endif
#if (_DARKGAME == 2)
/*** FindClosestObjectNamed - Locate the object that is physically closest to a reference object.
 *  	= object - The nearest object, if any. Aggregate return.
 *  	: int - The object to center the search about.
 *  	: const char * - The name of an archetype that the located object should inherit from.
 */
	STDMETHOD_(object*,FindClosestObjectNamed)(object &, int,const char *) PURE;
#endif
/*** AddMetaPropertyToMany - Add a metaproperty to multiple objects.
 *  	= int - Number of objects affected.
 *  	: object - The metaproperty to add.
 *  	: const cScrStr & - The objects to modify.
 */
	STDMETHOD_(int,AddMetaPropertyToMany)(object,const cScrStr  &) PURE;
/*** RemoveMetaPropertyFromMany - Remove a metaproperty from multiple objects.
 *  	= int - Number of objects affected.
 *  	: object - The metaproperty to remove.
 *  	: const cScrStr & - The objects to modify.
 */
	STDMETHOD_(int,RemoveMetaPropertyFromMany)(object,const cScrStr  &) PURE;
/*** RenderedThisFrame - Check if the object was included in the last rendering pass.
 *  	= true_bool - True if the object is visible. Aggregate return.
 *  	: object - The object to query.
 */
	STDMETHOD_(true_bool*,RenderedThisFrame)(true_bool &,object) PURE;

	//
	// new functions in T2 v1.19 / SS2 v2.4:
	//

#if (_DARKGAME == 3)
	STDMETHOD_(object*,FindClosestObjectNamed)(object &, object objId, const char* name) PURE;
#endif

	// transform a pos from object space to world space, ObjectToWorld(vector(0,0,0)) would be the same as Position()
	STDMETHOD_(cScrVec*,ObjectToWorld)(cScrVec &, object obj, const cScrVec &obj_pos) PURE;
};
DEFINE_IIDSTRUCT(IObjectSrv,IID_IObjectScriptService)

extern  const GUID  IID_IPGroupScriptService;
interface IPGroupSrv : IScriptServiceBase
{
/*** SetActive - Turn a particle SFX on or off.
 *  	= long - Returns 1.
 *  	: int - Object ID of the particle group.
 *  	: int - 1 to activate, 0 to deactivate.
 */
	STDMETHOD_(long,SetActive)(int,int) PURE;
};
DEFINE_IIDSTRUCT(IPGroupSrv,IID_IPGroupScriptService)

enum ePhysMsgType {
	kPhysCollision = 1,
	kPhysContact = 2,
	kPhysEnterExit = 4,
	kPhysFellAsleep = 8
};
extern  const GUID  IID_IPhysicsScriptService;
interface IPhysSrv : IScriptServiceBase
{
/*** SubscribeMsg - Register an object to receive certain messages.
 *  	= long - Returns 1.
 *  	: object - Object that will recieve the messages.
 *  	: int - A bit mask specifying which messages to begin listening for.
 */
	STDMETHOD_(long,SubscribeMsg)(object,int) PURE;
/*** UnsubscribeMsg - Stop listening for certain messages.
 *  	= long - Returns 1.
 *  	: object - Object that recieved the messages.
 *  	: int - A bit mask specifying which messages to stop listening for.
 */
	STDMETHOD_(long,UnsubscribeMsg)(object,int) PURE;
/*** LaunchProjectile - Emit an object.
 *  	= object - The emitted object. Aggregate return.
 *  	: object - Object to launch from.
 *  	: object - Archetype to emit.
 *  	: float - Velocity scale of projectile.
 *  	: int - 1=NoPhysics, 2=PushOut, 4=RelativeVelocity, 8=Gravity.
 *  	: const cScrVec & - Randomize velocity.
 */
	STDMETHOD_(object*,LaunchProjectile)(object &,object,object,float,int,const cScrVec &) PURE;
/*** SetVelocity - Set the current speed of an object.
 *  	= long - Returns 0.
 *  	: object - The object to modify.
 *  	: const cScrVec & - The new velocity of the object.
 */
	STDMETHOD_(long,SetVelocity)(object,const cScrVec &) PURE;
/*** GetVelocity - Retrieve the current speed of an object.
 *  	= long - Returns 0.
 *  	: object - The object to query.
 *  	: cScrVec & - Address of a variable that will receive the velocity.
 */
	STDMETHOD_(long,GetVelocity)(object,cScrVec  &) PURE;
#if (_DARKGAME == 2)
/*** ControlVelocity - Lock the velocity of an object.
 *  	= long - Returns 0.
 *  	: object - The object to modify.
 *  	: const cScrVec & - The new velocity of the object.
 */
	STDMETHOD_(long,ControlVelocity)(object,const cScrVec &) PURE;
/*** StopControlVelocity - Unlock the velocity of an object.
 *  	= long - Returns 0.
 *  	: object - The object to modify.
 */
	STDMETHOD_(long,StopControlVelocity)(object) PURE;
#endif
/*** SetGravity - Set the gravity of an object.
 *  	= long - Returns 0.
 *  	: object - The object to modify.
 *  	: float - The new gravity of the object.
 */
	STDMETHOD_(long,SetGravity)(object,float) PURE;
/*** GetGravity - Retrieve the gravity of an object.
 *  	= float - The current gravity.
 *  	: object - The object to query.
 */
	STDMETHOD_(float,GetGravity)(object) PURE;

/*** HasPhysics - Check if an object has a physics model.
 *  	= int - Non-zero if the object has a physics model.
 *  	: object - The object to query.
 */
	STDMETHOD_(int,HasPhysics)(object) PURE;
/*** IsSphere - Check if an object is a sphere.
 *  	= int - Non-zero if the object has a sphere-type physics model.
 *  	: object - The object to query.
 */
	STDMETHOD_(int,IsSphere)(object) PURE;
/*** IsOBB - Check if an object is an oriented bounding-box.
 *  	= int - Non-zero if the object has an OBB-type physics model.
 *  	: object - The object to query.
 */
	STDMETHOD_(int,IsOBB)(object) PURE;
/*** ControlCurrentLocation - Lock the location of an object to its current values.
 *  	= long - Returns 0 if the object is not 0.
 *  	: object - The object to lock.
 */
	STDMETHOD_(long,ControlCurrentLocation)(object) PURE;
/*** ControlCurrentRotation - Lock the facing of an object to its current values.
 *  	= long - Returns 0 if the object is not 0.
 *  	: object - The object to lock.
 */
	STDMETHOD_(long,ControlCurrentRotation)(object) PURE;
/*** ControlCurrentPosition - Lock the location and facing of an object to its current values.
 *  	= long - Returns 0 if the object is not 0.
 *  	: object - The object to lock.
 */
	STDMETHOD_(long,ControlCurrentPosition)(object) PURE;
/*** DeregisterModel - Remove the physics model of an object.
 *  	= long - Returns 0 if the object is not 0.
 *  	: object - The object to modify.
 */
	STDMETHOD_(long,DeregisterModel)(object) PURE;
/*** PlayerMotionSetOffset - Nudge the player.
 *  	: int - Must be zero.
 *  	: cScrVec & - Offset.
 */
	STDMETHOD_(void,PlayerMotionSetOffset)(int,cScrVec &) PURE;
/*** Activate - Wake up an object at rest.
 *  	= long - Returns 0 on success.
 *  	: object - The object to activate.
 */
	STDMETHOD_(long,Activate)(object) PURE;
/*** ValidPos - Test if an object is not in solid space.
 *  	= int - Non-zero if position is valid.
 *  	: object - The object to test.
 */
	STDMETHOD_(int,ValidPos)(object) PURE;


	//
	// new functions in T2 v1.19 / SS2 v2.4:
	//

	// is object a rope
	STDMETHOD_(BOOL, IsRope)(const object &obj) PURE;

	// get climbable object that 'climber' is currently attached to, OBJ_NULL if none
	STDMETHOD_(void, GetClimbingObject)(const object &climber, object &climbobj) PURE;
};
DEFINE_IIDSTRUCT(IPhysSrv,IID_IPhysicsScriptService)

extern  const GUID  IID_IPickLockScriptService;
interface IPickLockSrv : IScriptServiceBase
{
/*** Ready - Prepare a pick.
 *  	= int - Non-zero on success.
 *  	: object - The host object.
 *  	: object - The pick.
 */
	STDMETHOD_(int,Ready)(object,object) PURE;
/*** UnReady - Release a pick.
 *  	= int - Non-zero on success.
 *  	: object - The host object.
 *  	: object - The pick.
 */
	STDMETHOD_(int,UnReady)(object,object) PURE;
/*** StartPicking - Begin using a pick on an object.
 *  	= int - Non-zero on success.
 *  	: object - The host object.
 *  	: object - The pick.
 *  	: object - The object being picked.
 */
	STDMETHOD_(int,StartPicking)(object,object,object) PURE;
/*** FinishPicking - Stop picking an object.
 *  	= int - Non-zero on success.
 *  	: object - The pick.
 */
	STDMETHOD_(int,FinishPicking)(object) PURE;
/*** CheckPick - Test a pick.
 *  	= int - Returns the pick state.
 *  	: object - The pick.
 *  	: object - The object being picked.
 *  	: int - A bit mask of some sort.
 */
	STDMETHOD_(int,CheckPick)(object,object,int) PURE;
/*** DirectMotion - Move the pick arm. Not used.
 *  	= int - Non-zero on success.
 *  	: int - The pick.
 */
	STDMETHOD_(int,DirectMotion)(int) PURE;
};
DEFINE_IIDSTRUCT(IPickLockSrv,IID_IPickLockScriptService)

extern  const GUID  IID_IPlayerLimbsScriptService;
interface IPlayerLimbsSrv : IScriptServiceBase
{
/*** Equip - Display the player arm.
 *  	= long - Non-zero on success.
 *  	: object - Host object of the arm.
 */
	STDMETHOD_(long,Equip)(object) PURE;
/*** UnEquip - Hide the player arm.
 *  	= long - Non-zero on success.
 *  	: object - Host object of the arm.
 */
	STDMETHOD_(long,UnEquip)(object) PURE;
/*** StartUse - Begin moving the player arm.
 *  	= long - Non-zero on success.
 *  	: object - Host object of the arm.
 */
	STDMETHOD_(long,StartUse)(object) PURE;
/*** FinishUse - End an arm motion.
 *  	= long - Non-zero on success.
 *  	: object - Host object of the arm.
 */
	STDMETHOD_(long,FinishUse)(object) PURE;
};
DEFINE_IIDSTRUCT(IPlayerLimbsSrv,IID_IPlayerLimbsScriptService)

extern  const GUID  IID_IPropertyScriptService;
interface IPropertySrv : IScriptServiceBase
{
/*** Get - Retrieve some data from a property.
 *  	= cMultiParm - Aggregate return. Caller should free if string or vector.
 *  	: object - The object to query.
 *  	: const char * - The property name.
 *  	: const char * - The field name. NULL if the property is only a single field, otherwise it's the exact name that's displayed in the editor dialog in DromEd.
 */
	STDMETHOD_(cMultiParm*,Get)(cMultiParm &,object,const char *,const char *) PURE;
/*** Set - Write a value into a field of a property.
 *  	= long - Returns 0 on success.
 *  	: const char * - The property name.
 *  	: const char * - The field name.
 *  	: const cMultiParm & - The data to write.
 */
	STDMETHOD_(long,Set)(object,const char *,const char *,const cMultiParm &) PURE;
/*** SetSimple - Write a value into a simple property. Identical to calling Set with a NULL field name.
 *  	= long - Returns 0 on success.
 *  	: const char * - The property name.
 *  	: const cMultiParm & - The data to write.
 */
	STDMETHOD_(long,SetSimple)(object,const char *,const cMultiParm &) PURE;
#if (_DARKGAME == 2) || (_DARKGAME == 3)
/*** SetLocal - Write a value into a field of a property without propogating across the network.
 *  	= long - Returns 0 on success.
 *  	: const char * - The property name.
 *  	: const char * - The field name.
 *  	: const cMultiParm & - The data to write.
 */
	STDMETHOD_(long,SetLocal)(object,const char *,const char *,const cMultiParm &) PURE;
#endif
/*** Add - Create an instance of a property on an object.
 *  	= long - Returns 0 on success.
 *  	: object - The object to receive the property.
 *  	: const char * - The property name.
 */
	STDMETHOD_(long,Add)(object,const char *) PURE;
/*** Remove - Delete an instance of a property from an object.
 *  	= long - Returns 0 on success.
 *  	: object - The object to modify.
 *  	: const char * - The property name.
 */
	STDMETHOD_(long,Remove)(object,const char *) PURE;
/*** CopyFrom - Copy the contents of a property from one object to another.
 *  	= long - Returns 0 on success.
 *  	: object - The destination object.
 *  	: const char * - The property name.
 *  	: object - The source object.
 */
	STDMETHOD_(long,CopyFrom)(object,const char *,object) PURE;
/*** Possessed - Test if an object has an instance of a property.
 *  	= int - Non-zero if the property is on the object.
 *  	: object - The object to query.
 *  	: const char * - The property name.
 */
	STDMETHOD_(int,Possessed)(object,const char *) PURE;
};
DEFINE_IIDSTRUCT(IPropertySrv,IID_IPropertyScriptService)

extern  const GUID  IID_IPuppetScriptService;
interface IPuppetSrv : IScriptServiceBase
{
/*** PlayMotion - Play a motion schema on an object.
 *  	= true_bool - True if the motion played. Aggregate return.
 *  	: object - The host object.
 *  	: const char * - The schema name.
 */
	STDMETHOD_(true_bool*,PlayMotion)(true_bool &,object,const char *) PURE;
};
DEFINE_IIDSTRUCT(IPuppetSrv,IID_IPuppetScriptService)

extern  const GUID  IID_IQuestScriptService;
interface IQuestSrv : IScriptServiceBase
{
/*** SubscribeMsg - Register an object to receive notifications when a quest variable changes.
 *  	= int - Returns 1.
 *  	: object - The object that will receive the messages.
 *  	: const char * - The quest variable name.
 *  	: int - Flags of unknown relevance. Usually set to 2.
 */
	STDMETHOD_(int,SubscribeMsg)(object,const char *,int) PURE;
/*** UnsubscribeMsg - Stop listening for quest variable messages.
 *  	= int - Returns 1.
 *  	: object - The object that was previously registered.
 *  	: const char * - The quest variable name.
 */
	STDMETHOD_(int,UnsubscribeMsg)(object,const char *) PURE;
/*** Set - Change the value of a quest variable.
 *  	= long -  Returns 0.
 *  	: const char * - The quest variable name.
 *  	: int - The new value of the variable.
 *  	: int - The database to modify. 0 is mission, 1 is campaign.
 */
	STDMETHOD_(long,Set)(const char *,int,int) PURE;
/*** Get - Retrieve the value of a quest variable.
 *  	= int - The value of the variable. Returns 0 if the variable doesn't exist.
 *  	: const char * - The quest variable name.
 */
	STDMETHOD_(int,Get)(const char *) PURE;
/*** Exists - Test if a quest variable is defined.
 *  	= int - Non-zero if the variable exists.
 *  	: const char * - The quest variable name.
 */
	STDMETHOD_(int,Exists)(const char *) PURE;
/*** Delete - Remove a quest variable from either database.
 *  	= int - Returns 1 if the variable doesn't exist. Otherwise 0.
 *  	: const char * - The quest variable name.
 */
	STDMETHOD_(int,Delete)(const char *) PURE;
};
DEFINE_IIDSTRUCT(IQuestSrv,IID_IQuestScriptService)

extern  const GUID  IID_IShockGameScriptService;
interface IShockGameSrv : IScriptServiceBase
{
#if (_DARKGAME == 3) || (_SHOCKINTERFACES == 1)
/*** DestroyCursorObj - Destroy the current cursor object.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,DestroyCursorObj)(void) PURE;
/*** DestroyInvObj - Destroy an object which may be in the player's inventory.
 *  	= long - Returns 1.
 *  	: const object & - Object to destroy.
 */
	STDMETHOD_(long,DestroyInvObj)(const object &) PURE;
/*** HideInvObj - Takes an object out of the player's inventory.
 *  	= long - Returns 1.
 *  	: const object & - Inventory object.
 */
	STDMETHOD_(long,HideInvObj)(const object &) PURE;
	/* [20] */
/*** SetPlayerPsiPoints - Modify the player's psi points.
 *  	= long - Returns 1.
 *  	: int - New value for psi points.
 */
	STDMETHOD_(long,SetPlayerPsiPoints)(int) PURE;
/*** GetPlayerPsiPoints - Get the player's psi points.
 *  	= int - Current psi points.
 */
	STDMETHOD_(int,GetPlayerPsiPoints)(void) PURE;
/*** AttachCamera - Set the camera to a named object.
 *  	= long - Returns 0.
 *  	: const cScrStr & - Name of the object to attach to.
 */
	STDMETHOD_(long,AttachCamera)(const cScrStr &) PURE;
/*** CutSceneModeOn - Enter scripted camera mode.
 *  	= long - Returns 0.
 *  	: const cScrStr & - Scene name.
 */
	STDMETHOD_(long,CutSceneModeOn)(const cScrStr &) PURE;
	/* [30] */
/*** CutSceneModeOff - Exit scripted camera mode.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,CutSceneModeOff)(void) PURE;
/*** CreatePlayerPuppet - Create an object that acts like the player. Uses a custom model.
 *  	= int - Object ID of the new player.
 *  	: const cScrStr & - Model name for the puppet.
 */
	STDMETHOD_(int,CreatePlayerPuppet)(const cScrStr &) PURE;
/*** CreatePlayerPuppetDefault - Create an object that acts like the player. Uses the default object model.
 *  	= int - Object ID of the new player.
 */
	STDMETHOD_(int,CreatePlayerPuppetDefault)(void) PURE;
/*** DestroyPlayerPuppet - Destroy the current player puppet object.
 *  	= long - Returns 1.
 */
	STDMETHOD_(long,DestroyPlayerPuppet)(void) PURE;
	/* [40] */
/*** Replicator - Display the replicator interface.
 *  	= long - Returns 0.
 *  	: const object & - The replicator object.
 */
	STDMETHOD_(long,Replicator)(const object &) PURE;
/*** Container - Display the container interface.
 *  	= long - Returns 0.
 *  	: const object & - The container object.
 */
	STDMETHOD_(long,Container)(const object &) PURE;
#if (_DARKGAME == 1)
	virtual int __stdcall f13() = 0;
#endif
/*** YorN - Display a yes-or-no propmpt.
 *  	= long - Returns 0.
 *  	: const object & - Object that will recieve the result of the prompt.
 *  	: const cScrStr & - Message to display.
 */
	STDMETHOD_(long,YorN)(const object &,const cScrStr &) PURE;
/*** Keypad - Display the keypad interface.
 *  	= long - Returns 0.
 *  	: const object & - The keypad object.
 */
	STDMETHOD_(long,Keypad)(const object &) PURE;
#if (_DARKGAME != 1)
	/* [50] */
/*** HRM - Display the hack/repair/modify/maintain/research interface.
 *  	= long - Returns 0 on success.
 *  	: int - Type of action. 0=Hack, 1=Repair, 2=Modify, 3=Maintain, 4=Research.
 *  	: const object & - Object being modified.
 *  	: int - Non-zero if psi is being used.
 */
	STDMETHOD_(long,HRM)(int,const object &,int) PURE;
/*** TechTool - Display an object's HRM interface.
 *  	= long - Returns 0 on success.
 *  	: const object & - Object being modified.
 */
	STDMETHOD_(long,TechTool)(const object &) PURE;
#endif
/*** UseLog - Read an object log.
 *  	= long - Return 0 on success.
 *  	: const object & - Log object. Is destroyed after use.
 *  	: int - If non-zero, display the log even if it was already read.
 */
	STDMETHOD_(long,UseLog)(const object &,int) PURE;
/*** TriggerLog - Displays a log message.
 *  	= int - Non-zero on success.
 *  	: int - Type of log. 0=email, 1=log, 2=?, 3=help, 4=research
 *  	: int - Deck level.
 *  	: int - Log number.
 *  	: int - Always show.
 */
	STDMETHOD_(int,TriggerLog)(int,int,int,int) PURE;
	/* [60] */
/*** FindLogData - Retrieve log data from an object.
 *  	= long - Returns 0 on success.
 *  	: const object & - Log object.
 *  	: int - Type of log.
 *  	: int * - Deck level.
 *  	: int * - Log number.
 */
	STDMETHOD_(long,FindLogData)(const object &,int,int *,int *) PURE;
/*** PayNanites - Attempt to subtract nanites from the player.
 *  	= long - Non-zero if the player doesn't have enough nanites.
 *  	: int - Amount to subtract. May be negative to add nanites.
 */
	STDMETHOD_(long,PayNanites)(int) PURE;
/*** OverlayChange - Change the status of an overlay panel.
 *  	= long - Returns 0.
 *  	: int - Overlay ID.
 *  	: int - Display state. 0=Off, 1=On, 2=Toggle
 */
	STDMETHOD_(long,OverlayChange)(int,int) PURE;
/*** Equipped - Get the object contained in a particular slot.
 *  	= object - Contained object. Aggregate return.
 *  	: int - Slot to look in. The Contains link type minus 1000.
 */
	STDMETHOD_(object*,Equipped)(object &,int) PURE;
	/* [70] */
/*** LevelTransport - Change the mission map.
 *  	= long - Returns 0 on success.
 *  	: const char * - Mission name.
 *  	: int - Location ID.
 *  	: unsigned int - Flags. 1=Transport elevatorable objects.
 */
	STDMETHOD_(long,LevelTransport)(const char *,int,unsigned int) PURE;
/*** CheckLocked - Determine if a creature can open an object.
 *  	= int - Non-zero if the object is locked.
 *  	: const object & - Object to check.
 *  	: int - Use keys if non-zero.
 *  	: const object & - Player or AI attempting to use the object. Required when using keys.
 */
	STDMETHOD_(int,CheckLocked)(const object &,int,const object &) PURE;
/*** AddText - Display text on a player's screen.
 *  	= long - Returns 0.
 *  	: const char * - Text to display.
 *  	: const object & - Player that will see the text, or 0 to display (locally/everywhere)?.
 *  	: int - Time the text will be visible, in milliseconds.
 */
	STDMETHOD_(long,AddText)(const char *,const object &,int) PURE;
/*** AddTranslatableText - Display a string from a resource.
 *  	= long - Returns 0.
 *  	: const char * - String name.
 *  	: const char * - Resource name.
 *  	: const object & - Host object.
 *  	: int - Time in milliseconds.
 */
	STDMETHOD_(long,AddTranslatableText)(const char *,const char *,const object &,int) PURE;
	/* [80] */
/*** AmmoLoad - Add or change the ammunition in a gun.
 *  	= long - Returns 0 on success.
 *  	: const object & - Gun.
 *  	: const object & - Ammo.
 */
	STDMETHOD_(long,AmmoLoad)(const object &,const object &) PURE;
#if (_DARKGAME != 1)
/*** GetClip - Get the size of a clip.
 *  	= int - Number of rounds.
 *  	: const object & - Ammo.
 */
	STDMETHOD_(int,GetClip)(const object &) PURE;
#endif
/*** AddExp - Give experience cookies.
 *  	= long - Returns 0.
 *  	: const object & - Recipient. Can be 0 to give to the player.
 *  	: int - Amount.
 *  	: int - Add cyber modules if non-zero.
 */
	STDMETHOD_(long,AddExp)(const object &,int,int) PURE;
/*** HasTrait - Check if a player has a trait.
 *  	= int - Non-zero if the trait is active.
 *  	: const object & - Object to check.
 *  	: int - Trait ID to look for.
 */
	STDMETHOD_(int,HasTrait)(const object &,int) PURE;
	/* [90] */
/*** HasImplant - Check if a player is using an implant.
 *  	= int - Non-zero if the implant is active.
 *  	: const object & - Object to check.
 *  	: int - Implant ID to look for.
 */
	STDMETHOD_(int,HasImplant)(const object &,int) PURE;
/*** HealObj - Add hit points to an object.
 *  	= long - Returns 0 on success.
 *  	: const object & - Object to heal.
 *  	: int - Number of hit points to add. May be negative.
 */
	STDMETHOD_(long,HealObj)(const object &,int) PURE;
/*** OverlaySetObj - Associate an object with an interface panel.
 *  	= long - Returns 0.
 *  	: int - The overlay ID.
 *  	: const object & - Object to that will receive the messages.
 */
	STDMETHOD_(long,OverlaySetObj)(int,const object &) PURE;
/*** Research - Display the research interface.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,Research)(void) PURE;
	/* [A0] */
/*** GetArchetypeName - Get the name of an object's archetype.
 *  	= cScrStr - Archetype name. Aggregate return.
 *  	: const object & - The child object.
 */
	STDMETHOD_(cScrStr*,GetArchetypeName)(cScrStr &,const object &) PURE;
/*** OverlayOn - Check if an overlay panel is currently displayed.
 *  	= int - Returns non-zero if the overlay is on.
 *  	: int - The overlay ID.
 */
	STDMETHOD_(int,OverlayOn)(int) PURE;
/*** FindSpawnPoint - Get the nearest spawn point.
 *  	= object - The spawn point. Aggregate return.
 *  	: const object & - Object to center search around.
 *  	: unsigned int - Spawn flags. See the property.
 */
	STDMETHOD_(object*,FindSpawnPoint)(object &,const object &,unsigned int) PURE;
/*** CountEcoMatching - Find physical objects of an ecology type.
 *  	= int - Number of matching objects.
 *  	: int - Eco type.
 */
	STDMETHOD_(int,CountEcoMatching)(int) PURE;
	/* [B0] */
/*** GetStat - Get a player stat.
 *  	= int - Stat value.
 *  	: const object & - Object to check.
 *  	: int - Stat ID. 0=STR, 1=END, 2=PSI, 3=AGI, 4=CYB
 */
	STDMETHOD_(int,GetStat)(const object &,int) PURE;
/*** GetSelectedObj - Return the currently selected object.
 *  	= object - The hilighted object. Aggregate return.
 */
	STDMETHOD_(object*,GetSelectedObj)(object &) PURE;
/*** AddInvObj - Try to put an object in the player's inventory.
 *  	= int - Non-zero on success.
 *  	: const object & - Object to add.
 */
	STDMETHOD_(int,AddInvObj)(const object &) PURE;
/*** RecalcStats - Adjust an objects state after having modified its properties.
 *  	= long - Returns 0.
 *  	: const object & - Object to update.
 */
	STDMETHOD_(long,RecalcStats)(const object &) PURE;
	/* [C0] */
/*** PlayVideo - Display a movie.
 *  	= long - Returns 0.
 *  	: const char * - Filename of the movie.
 */
	STDMETHOD_(long,PlayVideo)(const char *) PURE;
#if (_DARKGAME != 1)
/*** ClearRadiation - Cancel the radiation hazard.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,ClearRadiation)(void) PURE;
/*** SetPlayerVolume - Change the rate of the player's movement.
 *  	: float - Movement scale.
 */
	STDMETHOD_(void,SetPlayerVolume)(float) PURE;
/*** RandRange - [low, high]
 *  	= int - A (psuedo) random number.
 *  	: int - The lowest number to return.
 *  	: int - The highest number to return.
 */
	STDMETHOD_(int,RandRange)(int,int) PURE;
	/* [D0] */
/*** LoadCursor - Set the cursor object.
 *  	= int - Non-zero on success.
 *  	: const object & - Object that will be the cursor.
 */
	STDMETHOD_(int,LoadCursor)(const object &) PURE;
/*** AddSpeedControl - Change the speed of Player.
 *  	: const char * - ID of the control. Speed controls can be stacked and removed out-of-order.
 *  	: float - Amount to multiply the speed by.
 *  	: float - Multiplier for turning speed. Appears to be ineffective.
 */
	STDMETHOD_(void,AddSpeedControl)(const char *,float,float) PURE;
/*** RemoveSpeedControl - Remove a previously applied speed control.
 *  	: char * - ID of the control.
 */
	STDMETHOD_(void,RemoveSpeedControl)(const char *) PURE;
/*** PreventSwap - Avoid changing the cursor object when an inventory object is selected.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,PreventSwap)(void) PURE;
	/* [E0] */
/*** GetDistantSelectedObj - Find the object that the player is looking at.
 *  	= object - An object in the player's crosshairs. Aggregate return.
 */
	STDMETHOD_(object*,GetDistantSelectedObj)(object &) PURE;
/*** Equip - Put an object in an inventory slot.
 *  	= long - Returns 0.
 *  	: int - The slot ID. The Contains type is the ID plus 1000.
 *  	: const object & - The object to equip.
 */
	STDMETHOD_(long,Equip)(int,const object &) PURE;
/*** OverlayChangeObj - Set the state and associated object of an interface panel.
 *  	= long - Returns 0.
 *  	: int - Overlay ID.
 *  	: int - Display state.
 *  	: const object & - Object that will recieve the messages.
 */
	STDMETHOD_(long,OverlayChangeObj)(int,int,const object &) PURE;
/*** SetObjState - Change the state of an object.
 *  	= long - Returns 0.
 *  	: const object & - Object to modify.
 *  	: int - New state. 5=Hacked.
 */
	STDMETHOD_(long,SetObjState)(const object &,int) PURE;
	/* [F0] */
/*** RadiationHack - Set radiation to 0 on all objects except the player.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,RadiationHack)(void) PURE;
/*** DestroyAllByName - Destroy all objects that descend from an archetype.
 *  	= long - Returns 0 on success.
 *  	: const char * - Archetype name.
 */
	STDMETHOD_(long,DestroyAllByName)(char const *) PURE;
/*** AddTextObjProp - Display the resource string from an object's property.
 *  	= long - Returns 0.
 *  	: const object & - Object to read the property from.
 *  	: const char * - The property name. Should be a label.
 *  	: const object & - The host.
 *  	: int - Time in milliseconds.
 */
	STDMETHOD_(long,AddTextObjProp)(const object &,const char *,const object &,int) PURE;
/*** DisableAlarmGlobal - Turn off all alarms.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,DisableAlarmGlobal)(void) PURE;
	/* [100] */
/*** Frob - Make the player frob the currently selected item.
 *  	: int - Ignore blocking objects if non-zero. Use this for inventory objects.
 */
	STDMETHOD_(void,Frob)(int) PURE;
/*** TweqAllByName - Activate the tweqs on all objects that descend from an archetype.
 *  	= long - Returns 0 on success.
 *  	: const char * - Archetype name.
 *  	: int - 1=Activate, 0=Halt.
 */
	STDMETHOD_(long,TweqAllByName)(const char *,int) PURE;
/*** SetExplored - Set whether a location has been explored.
 *  	= long - Returns 0.
 *  	: int - Location ID.
 *  	: char - Non-zero if the location is explored.
 */
	STDMETHOD_(long,SetExplored)(int,char) PURE;
/*** RemoveFromContainer - Remove an object from a container.
 *  	= long - Returns 0.
 *  	: const object & - Contained object.
 *  	: const object & - Container object.
 */
	STDMETHOD_(long,RemoveFromContainer)(const object &,const object &) PURE;
	/* [110] */
/*** ActivateMap - Set a quest variable with the name "Map" plus the name of the current map.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,ActivateMap)(void) PURE;
/*** SimTime - Get time the game has been active.
 *  	= int - Current sim time.
 */
	STDMETHOD_(int,SimTime)(void) PURE;
/*** StartFadeIn - Set the screen to a solid color, then gradually fade to full visibility.
 *  	: int - Time in milliseconds.
 *  	: unsigned char - Red.
 *  	: unsigned char - Green.
 *  	: unsigned char - Blue.
 */
	STDMETHOD_(void,StartFadeIn)(int,unsigned char,unsigned char,unsigned char) PURE;
/*** StartFadeOut - Gradually transition to a solid color.
 *  	: int - Time in milliseconds.
 *  	: unsigned char - Red.
 *  	: unsigned char - Green.
 *  	: unsigned char - Blue.
 */
	STDMETHOD_(void,StartFadeOut)(int,unsigned char,unsigned char,unsigned char) PURE;
	/* [120] */
/*** GrantPsiPower - Give a psi ability to a player.
 *  	= long - Returns 0.
 *  	: const object & - Recipient.
 *  	: int - Psi power.
 */
	STDMETHOD_(long,GrantPsiPower)(const object &,int) PURE;
/*** ResearchConsume - Attempt to use an object in the current research process.
 *  	= int - Non-zero if object was used.
 *  	: const object & - Object to use.
 */
	STDMETHOD_(int,ResearchConsume)(const object &) PURE;
/*** PlayerMode - Set the player mode with side-effects.
 *  	= long - Returns 0.
 *  	: int - Mode.
 */
	STDMETHOD_(long,PlayerMode)(int) PURE;
/*** EndGame - Stop playing the game.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,EndGame)(void) PURE;
	/* [130] */
/*** AllowDeath - Return whether the player dying should end the game. Checks for the config variable "undead".
 *  	= int - Non-zero if the game should not end.
 */
	STDMETHOD_(int,AllowDeath)(void) PURE;
/*** AddAlarm - Set the alarm timeout.
 *  	= long - Returns 0.
 *  	: int - The alarm will expire in this many milliseconds. Overrides any previously set alarm.
 */
	STDMETHOD_(long,AddAlarm)(int) PURE;
/*** RemoveAlarm - Immediately cancel the security alarm.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,RemoveAlarm)(void) PURE;
/*** GetHazardResistance - Get the damage multiplier for the endurance level.
 *  	= float - Amount to scale the damage.
 *  	: int - END stat.
 */
	STDMETHOD_(float,GetHazardResistance)(int) PURE;
	/* [140] */
/*** GetBurnDmg - Get the amount of damage done by a psi overload.
 *  	= int - Hit points to damage the player by.
 */
	STDMETHOD_(int,GetBurnDmg)(void) PURE;
/*** PlayerGun - Get the gun being wielded by the player.
 *  	= object - Current weapon. Aggregate return.
 */
	STDMETHOD_(object*,PlayerGun)(object &) PURE;
/*** IsPsiActive - Check if a psi power is being used.
 *  	= int - Non-zero if active.
 *  	: int - Psi power.
 */
	STDMETHOD_(int,IsPsiActive)(int) PURE;
/*** PsiRadarScan - Do a pass of the psi radar.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,PsiRadarScan)(void) PURE;
	/* [150] */
/*** PseudoProjectile - Create an object at a distance from another object.
 *  	= object - Emitted object. Aggregate return.
 *  	: const object & - Firer.
 *  	: const object & - Projectile archetype.
 */
	STDMETHOD_(object*,PseudoProjectile)(object &,const object &,const object &) PURE;
/*** WearArmor - Set or clear the armor.
 *  	= long - Returns 0.
 *  	: const object & - The armor to use, or 0 for none.
 */
	STDMETHOD_(long,WearArmor)(const object &) PURE;
/*** SetModify - Change the modification state of a gun.
 *  	= long - Returns 0.
 *  	: const object & - The object to modify.
 *  	: int - Modification state.
 */
	STDMETHOD_(long,SetModify)(const object &,int) PURE;
/*** Censored - Return whether blood should not be shown. Checks the config variable "menschveracht".
 *  	= int - Non-zero if the game is censored.
 */
	STDMETHOD_(int,Censored)(void) PURE;
	/* [160] */
/*** DebriefMode - Switch to the debriefing screen.
 *  	= long - Returns 0.
 *  	: int - Mission number.
 */
	STDMETHOD_(long,DebriefMode)(int) PURE;
/*** TlucTextAdd - Display a string in an overlay at the center of the screen.
 *  	= long - Returns 0.
 *  	: const char * - String name.
 *  	: const char * - Resource name.
 *  	: int - A number that is appended to the end of the string name.
 */
	STDMETHOD_(long,TlucTextAdd)(const char *,const char *,int) PURE;
/*** Mouse - Change the mouse mode.
 *  	= long - Returns 0.
 *  	: int - Activate mouse mode if non-zero.
 *  	: int - Drop the cursor object if non-zero.
 */
	STDMETHOD_(long,Mouse)(int,int) PURE;
/*** RefreshInv - Update the state of the inventory.
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,RefreshInv)(void) PURE;
	/* [170] */
/*** TreasureTable - Adds random objects to a container. Make sure that guaranteed loot is available.
 *  	= long - Returns 0.
 *  	: const object & - The loot object
 */
	STDMETHOD_(long,TreasureTable)(const object &) PURE;
/*** OverlayGetObj - Get the object associated with an overlay.
 *  	= object - The overlay object. Aggregate return.
 */
	STDMETHOD_(object*,OverlayGetObj)(object &) PURE;
/*** VaporizeInv - Destroy (almost) all objects in the player's inventory. Saves objects in slot 10 and the "Magic Ball".
 *  	= long - Returns 0.
 */
	STDMETHOD_(long,VaporizeInv)(void) PURE;
/*** ShutoffPsi - Deactivate all psi powers.
 *  	= long - Returns 0 on success.
 */
	STDMETHOD_(long,ShutoffPsi)(void) PURE;
	/* [180] */
/*** SetQBHacked - Set the value of a quest variable. Handles variable names that begin with "note" specially.
 *  	= long - Returns 0 on success.
 *  	: const cScrStr & - Quest variable name.
 *  	: int - The value of the variable.
 */
	STDMETHOD_(long,SetQBHacked)(const cScrStr &,int) PURE;
/*** GetPlayerMaxPsiPoints - Return the maximum psi points the player can have.
 *  	= int - The maximum psi points.
 */
	STDMETHOD_(int,GetPlayerMaxPsiPoints)(void) PURE;
/*** SetLogTime - Mark the time that a log is read.
 *  	= long - Returns 0.
 *  	: int - Deck level.
 *  	: int - Type of log.
 *  	: int - Log number.
 */
	STDMETHOD_(long,SetLogTime)(int,int,int) PURE;
/*** AddTranslatableTextInt - Display a string with a replaceable parameter. Identical to AddTranslatableTextIndexInt with the index -1.
 *  	= long - Returns 0.
 *  	: const char * - String name.
 *  	: const char * - Resource name.
 *  	: const object & - Host object.
 *  	: int - Parameter value.
 *  	: int - Time in milliseconds.
 */
	STDMETHOD_(long,AddTranslatableTextInt)(const char *,const char *,const object &,int,int) PURE;
	/* [190] */
/*** ZeroControls - Stops the player's movement.
 *  	= long - Returns 0.
 *  	: const object & - Player object.
 *  	: int - Poll keys if non-zero.
 */
	STDMETHOD_(long,ZeroControls)(const object &,int) PURE;
/*** SetSelectedPsiPower - Set the current psi power.
 *  	= long - Returns 0.
 *  	: int - Psi power.
 */
	STDMETHOD_(long,SetSelectedPsiPower)(int) PURE;
/*** ValidGun - Check if an object is a gun.
 *  	= int - Non-zero if the object has a gun state, is not a melee weapon, and is not the psi amp.
 *  	: const object & - Object to check.
 */
	STDMETHOD_(int,ValidGun)(const object &) PURE;
/*** AddTranslatableTextIndexInt - Display an indexed string with a replaceable parameter.
 *  	= long - Returns 0.
 *  	: const char * - String name.
 *  	: const char * - Resource name.
 *  	: const object & - Host object.
 *  	: int - Number that will be appended to the end of the string name.
 *  	: int - Parameter value.
 *  	: int - Time in milliseconds.
 */
	STDMETHOD_(long,AddTranslatableTextIndexInt)(const char *,const char *,const object &,int,int,int) PURE;
	/* [1A0] */
/*** IsAlarmActive - Check if the security alarm is active.
 *  	= int - Non-zero if there is an alarm.
 */
	STDMETHOD_(int,IsAlarmActive)(void) PURE;
/*** SlayAllByName - Slay all objects that descend from an archetype.
 *  	= long - Returns 0 on success.
 *  	: const char * - Archetype name.
 */
	STDMETHOD_(long,SlayAllByName)(const char *) PURE;
/*** NoMove - Prevents or allow the player to move. Turning and free-look are still allowed.
 *  	= long - Returns 0.
 *  	: int - Non-zero to freeze the player.
 */
	STDMETHOD_(long,NoMove)(int) PURE;
/*** PlayerModeSimple - Just change the player mode.
 *  	= long - Returns 0.
 *  	: int - Mode. 7=Dead, 2=Swimming?, 3=Climbing?, 1=Running?
 */
	STDMETHOD_(long,PlayerModeSimple)(int) PURE;
	/* [1B0] */
/*** UpdateMovingTerrainVelocity - Manually adjust the speed of moving terrain.
 *  	= long - Returns 0.
 *  	: object - Moving terrain object.
 *  	: object - The TerrPt to move to.
 *  	: float - New velocity.
 */
	STDMETHOD_(long,UpdateMovingTerrainVelocity)(object,object,float) PURE;
/*** MouseCursor - Check if the cursor is on-screen.
 *  	= int - Non-zero if visible.
 */
	STDMETHOD_(int,MouseCursor)(void) PURE;
	virtual int __stdcall f107() = 0;
	virtual int __stdcall f108() = 0;
	virtual int __stdcall f109() = 0;
	// Does nothing.
	//STDMETHOD_(long,LevelTransport)(const char *,object &,unsigned int) PURE;
	virtual int __stdcall f110() = 0;
/*** SpewLockData - Show access message for a lock.
 *  	= long - Returns 0.
 *  	: const object & - Locked object.
 *  	: int - The lock has been used if non-zero.
 */
	STDMETHOD_(long,SpewLockData)(const object &,int) PURE;
#endif

#endif
};
DEFINE_IIDSTRUCT(IShockGameSrv,IID_IShockGameScriptService)

extern  const GUID  IID_IShockObjScriptService;
interface IShockObjSrv : IScriptServiceBase
{
#if (_DARKGAME == 3) || (_SHOCKINTERFACES == 1)
/*** FindScriptDonor - Get the object from which a particular script is inherited from.
 *  	= int - The inherited object ID that has the script.
 *  	: int - Descendant object ID.
 *  	: cScrStr - Name of the script to look for. Passed by value. Method will call free().
 */
	STDMETHOD_(int,FindScriptDonor)(int,cScrStr) PURE;
#endif
};
DEFINE_IIDSTRUCT(IShockObjSrv,IID_IShockObjScriptService)


/* These arguments were originally an enum. 
 */
extern  const GUID  IID_IShockPsiScriptService;
interface IShockPsiSrv : IScriptServiceBase
{
#if (_DARKGAME == 3) || (_SHOCKINTERFACES == 1)
/*** OnDeactivate - Stop using a psi power.
 *  	= long - Returns 0.
 *  	: int - Psi power.
 */
	STDMETHOD_(long,OnDeactivate)(int) PURE;
/*** GetActiveTime - Get how long a psi power has been active.
 *  	= unsigned long - Time in milliseconds.
 *  	: int - Psi power.
 */
	STDMETHOD_(unsigned long,GetActiveTime)(int) PURE;
#if (_DARKGAME != 1)
/*** IsOverloaded - Check if a psi power went past its maximum activation level.
 *  	= int - Non-zero when overloaded.
 *  	: int - Psi power.
 */
	STDMETHOD_(int,IsOverloaded)(int) PURE;
#endif
#endif
};
DEFINE_IIDSTRUCT(IShockPsiSrv,IID_IShockPsiScriptService)

extern  const GUID  IID_IShockAIScriptService;
interface IShockAISrv : IScriptServiceBase
{
#if (_DARKGAME == 3) || (_SHOCKINTERFACES == 1)
/*** Stun - Interrupt an AI temporarily.
 *  	= int - Non-zero if successful.
 *  	: object - The AI.
 *  	: cScrStr - Tags. Passed by value. Method will call free().
 *  	: cScrStr - Motion tags. Passed by value. Method will call free().
 *  	: float - Time in seconds.
 */
	STDMETHOD_(int,Stun)(object,cScrStr,cScrStr,float) PURE;
/*** IsStunned - Is an AI unable to move?
 *  	= int - Non-zero if the AI is immobile.
 *  	: object - The AI.
 */
	STDMETHOD_(int,IsStunned)(object) PURE;
/*** UnStun - Restore the AI state.
 *  	= int - Non-zero if successful.
 *  	: object - The AI.
 */
	STDMETHOD_(int,UnStun)(object) PURE;
/*** Freeze - Make an AI immobile.
 *  	= int - Non-zero if successful.
 *  	: object - The AI.
 *  	: float - Time in seconds.
 */
	STDMETHOD_(int,Freeze)(object,float) PURE;
	virtual int __stdcall f4() = 0;
/*** UnFreeze - Let an AI move again.
 *  	= int - Non-zero if successful.
 *  	: object - The AI.
 */
	STDMETHOD_(int,UnFreeze)(object) PURE;
/*** NotifyEnterTripwire - Tell an AI that it is approaching a door.
 *  	: object - The AI.
 *  	: object - The door.
 */
	STDMETHOD_(void,NotifyEnterTripwire)(object,object) PURE;
/*** NotifyExitTripwire - Tell an AI that it is leaving the vicinity of a door.
 *  	: object - The AI.
 *  	: object - The door.
 */
	STDMETHOD_(void,NotifyExitTripwire)(object,object) PURE;
/*** ObjectLocked - Test if an object is locked. Ignores keys.
 *  	= int - Zero if the object can be opened.
 *  	: object - The object to test.
 */
	STDMETHOD_(int,ObjectLocked)(object) PURE;
/*** ValidateSpawn - Associate an object with the spawn point that created it.
 *  	: object - New object. Must already exist.
 *  	: object - The creator.
 */
	STDMETHOD_(void,ValidateSpawn)(object,object) PURE;
#endif
};
DEFINE_IIDSTRUCT(IShockAISrv,IID_IShockAIScriptService)

extern  const GUID  IID_IShockWeaponScriptService;
interface IShockWeaponSrv : IScriptServiceBase
{
#if (_DARKGAME == 3) || (_SHOCKINTERFACES == 1)
/*** SetWeaponModel - Change the current weapon.
 *  	: const object & - The weapon.
 */
	STDMETHOD_(void,SetWeaponModel)(const object &) PURE;
/*** GetWeaponModel - Get the currently active weapon.
 *  	= object - The weapon. Aggregate return.
 */
	STDMETHOD_(object*,GetWeaponModel)(object &) PURE;
/*** TargetScan - Find an object in the path of another object.
 *  	= object - The target object. Aggregate return.
 *  	: object - Object to scan from.
 */
	STDMETHOD_(object*,TargetScan)(object &,object) PURE;
/*** Home - Turn an object to face another object.
 *  	: object - The object to turn.
 *  	: object - The target object.
 */
	STDMETHOD_(void,Home)(object,object) PURE;
/*** DestroyMelee - Stop using a melee weapon.
 *  	: const object & - The weapon.
 */
	STDMETHOD_(void,DestroyMelee)(const object &) PURE;
#endif
};
DEFINE_IIDSTRUCT(IShockWeaponSrv,IID_IShockWeaponScriptService)

extern  const GUID  IID_ISoundScriptService;
#if (_DARKGAME != 1)
#define SOUND_NET  ,eSoundNetwork
#else
#define SOUND_NET
#endif
interface ISoundScrSrv : IScriptServiceBase
{
/*** PlayAtLocation - Play a schema at a specific location.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema tags.
 *  	: const cScrVec & - The location the schema will originate from.
 *  	: eSoundSpecial - Loop option.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlayAtLocation)(true_bool&,object,const cScrStr &,const cScrVec &,eSoundSpecial SOUND_NET) PURE;
/*** PlayAtObject - Play a schema at the location of an object.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema tags.
 *  	: object - The object at which location the schema will be played.
 *  	: eSoundSpecial - Loop option.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlayAtObject)(true_bool&,object,const cScrStr &,object,eSoundSpecial SOUND_NET) PURE;
/*** Play - Play a schema.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema tags.
 *  	: eSoundSpecial - Loop option.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,Play)(true_bool&,object,const cScrStr &,eSoundSpecial SOUND_NET) PURE;
/*** PlayAmbient - Play an ambient schema.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema tags.
 *  	: eSoundSpecial - Loop option.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlayAmbient)(true_bool&,object,const cScrStr &,eSoundSpecial SOUND_NET) PURE;
/*** PlaySchemaAtLocation - Play a schema object at a specific location.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: object - The schema.
 *  	: const cScrVec & - The location the schema will originate from.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlaySchemaAtLocation)(true_bool &,object,object,const cScrVec & SOUND_NET) PURE;
/*** PlaySchemaAtObject - Play a schema object at the location of an object.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: object - The schema.
 *  	: object - The object at which location the schema will be played.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlaySchemaAtObject)(true_bool &,object,object,object SOUND_NET) PURE;
/*** PlaySchema - Play a schema object.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: object - The schema.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlaySchema)(true_bool &,object,object SOUND_NET) PURE;
/*** PlaySchemaAmbient - Play an ambient schema object.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: object - The schema.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlaySchemaAmbient)(true_bool &,object,object SOUND_NET) PURE;
/*** PlayEnvSchema - Play an environmental schema.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema tags.
 *  	: object - The source object.
 *  	: object - Second source object.
 *  	: eEnvSoundLoc - Where the schema will be played.
 *  	: eSoundNetwork - Network option.
 */
	STDMETHOD_(true_bool*,PlayEnvSchema)(true_bool &,object,const cScrStr &,object,object,eEnvSoundLoc SOUND_NET) PURE;
/*** PlayVoiceOver - Play a voice-over schema.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: object - The schema.
 */
	STDMETHOD_(true_bool*,PlayVoiceOver)(true_bool &,object,object) PURE;
/*** Halt - Stop playing a schema.
 *  	= int - The number of schemas halted.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema name. Stops all schemas if blank.
 *  	: object - The source object.
 */
	STDMETHOD_(int,Halt)(object,const cScrStr &,object) PURE;
/*** HaltSchema - Stop playing a schema.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema tags.
 *  	: object - The source object.
 */
	STDMETHOD_(true_bool*,HaltSchema)(true_bool&,object,const cScrStr &,object) PURE;
/*** HaltSpeech - Prevent an AI from speaking.
 *  	= long - Returns 0.
 *  	: object - The AI.
 */
	STDMETHOD_(long,HaltSpeech)(object) PURE;
/*** PreLoad - Read the waveform data for a schema before having to play it.
 *  	= true_bool - Success.
 *  	: object - The host object.
 *  	: const cScrStr & - The schema tags.
 */
	STDMETHOD_(true_bool*,PreLoad)(true_bool &,const cScrStr &) PURE;
};
#undef SOUND_NET
DEFINE_IIDSTRUCT(ISoundScrSrv,IID_ISoundScriptService)

extern  const GUID  IID_IWeaponScriptService;
interface IWeaponSrv : IScriptServiceBase
{
/*** Equip - Select an object as the current weapon.
 *  	= long - Non-zero on success.
 *  	: object - The weapon.
 *  	: int - The type of weapon. 0 for a slashing weapon, 1 for a bashing weapon.
 */
	STDMETHOD_(long,Equip)(object,int) PURE;
/*** UnEquip - Clear the current weapon, if selected.
 *  	= long - Non-zero on success.
 *  	: object - The weapon.
 */
	STDMETHOD_(long,UnEquip)(object) PURE;
/*** IsEquipped - Check if a particular object is the current weapon.
 *  	= long - Non-zero on success.
 *  	: object - The host object.
 *  	: object - The weapon.
 */
	STDMETHOD_(int,IsEquipped)(object,object) PURE;
/*** StartAttack - Begin swinging the weapon.
 *  	= long - Non-zero on success.
 *  	: object - The host object.
 *  	: object - The weapon.
 */
	STDMETHOD_(long,StartAttack)(object,object) PURE;
/*** FinishAttack - Stop swinging the weapon.
 *  	= long - Non-zero on success.
 *  	: object - The host object.
 *  	: object - The weapon.
 */
	STDMETHOD_(long,FinishAttack)(object,object) PURE;
};
DEFINE_IIDSTRUCT(IWeaponSrv,IID_IWeaponScriptService)

// NewDark

extern  const GUID  IID_IVersionScriptService;
interface IVersionSrv : IScriptServiceBase
{
	// returns an app name-version string in the form of "Thief 2 Final 1.19" if title_only is FALSE
	// or an app name string in the form of "Thief 2"/"DromEd"/"System Shock 2" if title_only is TRUE
	STDMETHOD_(void,GetAppName)(BOOL title_only, cVarScrStr &result) PURE;

	// get version numbers
	STDMETHOD_(void,GetVersion)(int &major, int &minor) PURE;

	// returns 0 if the running executable is NOT the editor, 1 if editor in edit mode, 2 if editor in game mode
	STDMETHOD_(int,IsEditor)() PURE;

	// returns the "game" string (set in cam.cfg to select game, i.e. "dark/"shock")
	STDMETHOD_(void,GetGame)(cVarScrStr &result) PURE;

	// returns the current gamesys filename (including ".gam" extension)
	STDMETHOD_(void,GetGamsys)(cVarScrStr &result) PURE;

	// returns the current map/mission filename (including ".mis" extension)
	STDMETHOD_(void,GetMap)(cVarScrStr &result) PURE;

	// get the current FM name, returns empty string and S_FALSE if none is active
	STDMETHOD(GetCurrentFM)(cVarScrStr &result) PURE;

	// get the current FM path name as "rootpath/name" (roopath can be relative to CWD), returns empty string and S_FALSE if none is active
	STDMETHOD(GetCurrentFMPath)(cVarScrStr &result) PURE;

	// returns the 'relpath' with the current FM path prefixed if an FM is active, otherwise the unmodified 'relpath'
	STDMETHOD_(void, FMizeRelativePath)(const char *relpath, cVarScrStr &result) PURE;

	// checks if path is absolute, if so returns it as-is, otherwise behaves the same as FMizeRelativePath
	STDMETHOD_(void, FMizePath)(const char *path, cVarScrStr &result) PURE;
};
DEFINE_IIDSTRUCT(IVersionSrv,IID_IVersionScriptService)

extern  const GUID  IID_IEngineScriptService;
interface IEngineSrv : IScriptServiceBase
{
	// returns TRUE if config var is defined
	STDMETHOD_(BOOL, ConfigIsDefined)(const char *name) PURE;

	// get config var as int value
	STDMETHOD_(BOOL, ConfigGetInt)(const char *name, int &value) PURE;

	// get config var as float value
	STDMETHOD_(BOOL, ConfigGetFloat)(const char *name, float &value) PURE;

	// get config var value as raw text
	STDMETHOD_(BOOL, ConfigGetRaw)(const char *name, cVarScrStr &value) PURE;

	// get bind variable as float
	STDMETHOD_(float, BindingGetFloat)(const char *name) PURE;

	// search for a file in paths defined by a path config var (like "script_module_path"), 'fullname' is
	// set if return value is TRUE
	STDMETHOD_(BOOL, FindFileInPath)(const char *path_config_var, const char *filename, cVarScrStr &fullname) PURE;

	// returns TRUE if running game in legacy DX6 mode
	STDMETHOD_(BOOL,IsRunningDX6)() PURE;

	// get display resolution (more specifically the main 2D surface, which is the only thing relevant as far as drawing concerns)
	STDMETHOD_(void,GetCanvasSize)(int &width, int &height) PURE;

	// aspect ratio of current display mode (w/h)
	STDMETHOD_(float,GetAspectRatio)() PURE;

	// get global fog settings (dist 0 means fog disabled)
	STDMETHOD_(void,GetFog)(int &r, int &g, int &b, float &dist) PURE;
	// set global fog settings (dist 0 means fog disabled)
	STDMETHOD_(void,SetFog)(int r, int g, int b, float dist) PURE;

	// get fog settings for a fog zone (iZone must be a number 1 to 8 or function silently fails)
	STDMETHOD_(void,GetFogZone)(int iZone, int &r, int &g, int &b, float &dist) PURE;
	// set fog settings for a fog zone (iZone must be a number 1 to 8 or function silently fails)
	STDMETHOD_(void,SetFogZone)(int iZone, int r, int g, int b, float dist) PURE;

	// get weather parameters
	STDMETHOD_(void,GetWeather)(int &precip_type, float &precip_freq, float &precip_speed, float &vis_dist,
				float &rend_radius, float &alpha, float &brightness, float &snow_jitter,
				float &rain_len, float &splash_freq, float &splash_radius, float &splash_height,
				float &splash_duration, cVarScrStr &texture, cScrVec &wind) PURE;
	// set weather parameters
	STDMETHOD_(void,SetWeather)(int precip_type, float precip_freq, float precip_speed, float vis_dist,
				float rend_radius, float alpha, float brightness, float snow_jitter,
				float rain_len, float splash_freq, float splash_radius, float splash_height,
				float splash_duration, const char *texture, const cScrVec &wind) PURE;

	// perform a raycast on the worldrep (terrain only), returns TRUE if something was hit and hit_location contains
	// the hit location (not quite as expensive as ObjRaycast but don't use excessively)
	STDMETHOD_(BOOL,PortalRaycast)(const cScrVec &from, const cScrVec &to, cScrVec &hit_location) PURE;

	// perform a raycast on objects and terrain (expensive, don't use excessively)
	//   'ShortCircuit' - if 1, the raycast will return immediately upon hitting an object, without determining if there's
	//                    any other object hit closer to ray start
	//                    if 2, the raycast will return immediately upon hitting any terrain or object (most efficient
	//                    when only determining if there is a line of sight or not)
	//   'bSkipMesh'    - if TRUE the raycast will not include mesh objects (ie. characters) in the cast
	//   'ignore1'      - is an optional object to exclude from the raycast (useful when casting from the location of
	//                    an object to avoid the cast hitting the source object)
	//   'ignore2'      - is an optional object to exclude from the raycast (useful in combination with ignore2 when
	//                    testing line of sight between two objects, to avoid raycast hitting source or target object)
	// returns 0 if nothing was hit, 1 for terrain, 2 for an object, 3 for mesh object (ie. character)
	// for return types 2 and 3 the hit object will be returned in 'hit_object'
	STDMETHOD_(int,ObjRaycast)(const cScrVec &from, const cScrVec &to, cScrVec &hit_location, object &hit_object,
								int ShortCircuit, BOOL bSkipMesh, object ignore1, object ignore2) PURE;

	//
	// new functions in T2 v1.20 / SS2 v2.41:
	//

	// set texture for environment zone (iZone must be a number 0 to 63 or function silently fails, if texture name
	// is empty or NULL the specified env zone map is cleared)
	STDMETHOD_(void,SetEnvMapZone)(int iZone, const char *texture) PURE;
};
DEFINE_IIDSTRUCT(IEngineSrv,IID_IEngineScriptService)

#if (_DARKGAME == 3)
interface IShockOverlayHandler
{
	// handler called by the engine so script implementation of IShockOverlayHandler can draw HUD elements
	STDMETHOD_(void,DrawHUD)(THIS) PURE;

	// handler called by the engine so script implementation of IShockOverlayHandler can update and draw transparent
	// overlay elements (transparent overlay elements are bitmaps drawn directly to the 3D buffer with alpha blending,
	// like the security icon)
	STDMETHOD_(void,DrawTOverlay)(THIS) PURE;

	// handler called by the engine when dark initializes UI components, this is also called when resuming game after
	// in-game menu and other UI screens and is a good place to update positioning of HUD elements in case display
	// resolution changed
	STDMETHOD_(void,OnUIEnterMode)(THIS) PURE;

	// return FALSE if script wants to prevent a native overlay item, that's about to be enabled, to be enabled(/displayed)
	STDMETHOD_(BOOL,CanEnableElement)(THIS_ int which) PURE;

	//
	// mouse event handlers
	//

	// return TRUE if mouse is over a HUD element that interacts with mouse
	STDMETHOD_(BOOL,IsMouseOver)(THIS_ int x, int y) PURE;

	// return TRUE if mouse click was processed/intercepted
	STDMETHOD_(BOOL,MouseClick)(THIS_ int x, int y) PURE;

	// return TRUE if mouse double-click was processed/intercepted
	STDMETHOD_(BOOL,MouseDblClick)(THIS_ int x, int y) PURE;

	// return TRUE if event was processed/intercepted (cursor_mode is the same as ShockOverlay::GetCursorMode)
	STDMETHOD_(BOOL,MouseDragDrop)(THIS_ int x, int y, BOOL start_drag, int cursor_mode) PURE;
};

extern  const GUID  IID_IShockOverlayScriptService;
interface IShockOverlaySrv : IScriptServiceBase
{
	// set the current overlay handler, there can only be one handler set at any given time, NULL clears handler
	STDMETHOD_(void,SetHandler)(IShockOverlayHandler *pHandler) PURE;

	// enable/disable keyboard input capture by overlay handler, use with care because it overrides key bindings
	// and grabs all keyboard input, keep following in mind:
	//     - only enable capture if no native HUD element that captures input is active (like keypad), or remove that first
	//     - when a native HUD element that captures input is about to be displayed the script has to either release
	//       capture or prevent the native element from activating (both done through CanEnableElement)
	//     - when OnUIEnterMode is called any previously enabled capture is lost and has to be re-captured if necessary
	//     - only enable capture for the duration that an overlay item needs keyboard input and is active
	STDMETHOD_(void,SetKeyboardInputCapture)(BOOL bCapture) PURE;

	// get/load a bitmap that can be used for HUD drawing (max 128 bitmaps can be loaded, cleared when db resets)
	// returns a handle that can be used in subsequent bitmap functions or -1 if failed to load
	STDMETHOD_(int,GetBitmap)(const char *name, const char *path /*= "iface\\"*/) PURE;

	// discard a no longer used bitmap handle, only needs to be called when using a lot of bitmaps to stay below 128
	STDMETHOD_(void,FlushBitmap)(int handle) PURE;

	// get size of a loaded bitmap
	STDMETHOD_(void,GetBitmapSize)(int handle, int &width, int &height) PURE;

	// set custom font ('index' is 0 to 3 for which custom font slot to use, if a slot already has a font that one
	// will be replaced with the new font)
	STDMETHOD_(BOOL,SetCustomFont)(int index, const char *name, const char *path /*= "fonts\\"*/) PURE;

	// get the rect for an overlay element
	STDMETHOD_(void,GetOverlayRect)(int which, int &left, int &top, int &right, int &bottom) PURE;

	// get cursor mode (cursor_mode is one of the SCM_* defines below)
	STDMETHOD_(int,GetCursorMode)() PURE;

	// reset cursor mode to normal (also resets any ongoing drag and drop)
	STDMETHOD_(void,ClearCursorMode)() PURE;

	// set cursor bitmap (resets to default if 'name' is NULL or bitmap load failed), returns TRUE if new bitmap was set
	STDMETHOD_(BOOL,SetCursorBitmap)(const char *name, const char *path /*= "iface\\"*/) PURE;

	// set an object the mouse is currently over (for example the mouse is over an inventory item in the inventory)
	STDMETHOD_(void,SetInterfaceMouseOverObject)(const object &obj) PURE;

	// get the current interface focus object
	STDMETHOD_(void,GetInterfaceFocusObject)(object &obj) PURE;

	// open "look" overlay (HUD element with info about an object)
	STDMETHOD_(void,OpenLookPopup)(const object &obj) PURE;

	// toggle "look" cursor mode ("look" cursor mode is with the ? icon)
	STDMETHOD_(void,ToggleLookCursor)() PURE;

	// start drag and drop operation of 'obj', returns TRUE on success
	STDMETHOD_(BOOL,StartObjectDragDrop)(const object &obj) PURE;

	// play a schema for UI sound
	STDMETHOD_(void,PlaySound)(const char *schema_name) PURE;

	//
	// coordinate mapping (may ONLY be called inside the ShockOverlayHandler::DrawHUD and DrawTOverlay handlers)
	//

	// map a 3D position in the world to a screen coordinate, returns FALSE if off-screen
	STDMETHOD_(BOOL,WorldToScreen)(const cScrVec &pos, int &x, int &y) PURE;

	// get the screen space bounding rect of an object, returns FALSE if entirely off-screen
	STDMETHOD_(BOOL,GetObjectScreenBounds)(const object &obj, int &x1, int &y1, int &x2, int &y2) PURE;

	//
	// transparent overlay management
	//

	// create a transparent overlay item (like the security icon), alpha is 0 to 255
	// its contents have to be updated using Begin/EndTOverlayUpdate and draw calls, note that constant updating
	// isn't too optimal, so it should only be done when necessary and avoided on larger items
	// returns an overlay handle, or -1 if failed
	STDMETHOD_(int,CreateTOverlayItem)(int x, int y, int width, int height, int alpha, BOOL trans_bg) PURE;

	// create a transparent overlay item from a bitmap, does not require its contents to be updated
	// (max 64 overlays can be created, cleared when db resets)
	// returns an overlay handle, or -1 if failed (will fail if bm_handle isn't a valid bitmap)
	STDMETHOD_(int,CreateTOverlayItemFromBitmap)(int x, int y, int alpha, int bm_handle, BOOL trans_bg) PURE;

	// destroy a transparent overlay item, not necessary to call but frees resources and overlay slot before a db reset
	STDMETHOD_(void,DestroyTOverlayItem)(int handle) PURE;

	// change alpha of a transparent overlay
	STDMETHOD_(void,UpdateTOverlayAlpha)(int handle, int alpha) PURE;

	// change position of a transparent overlay
	STDMETHOD_(void,UpdateTOverlayPosition)(int handle, int x, int y) PURE;

	// change display size of a transparent overlay (for scaled overlay items)
	STDMETHOD_(void,UpdateTOverlaySize)(int handle, int width, int height) PURE;

	//
	// methods that may ONLY be called inside the ShockOverlayHandler::DrawHUD handler
	// or inside a ShockOverlayHandler::Begin/EndTOverlayUpdate pair
	//

	// draw a loaded bitmap at the given position, bitmap will be drawn unscaled
	STDMETHOD_(void,DrawBitmap)(int handle, int x, int y) PURE;

	// draw a sub-rect of a loaded bitmap at the given position
	// src_x/src_y are relative to the bitmap (0,0 would be the upper left of the bitmap)
	STDMETHOD_(void,DrawSubBitmap)(int handle, int x, int y, int src_x, int src_y, int src_width, int src_height) PURE;

	// draw an object's icon
	STDMETHOD_(void,DrawObjectIcon)(const object &obj, int x, int y) PURE;

	// set current font, font_type is 0 - default, 1 - mono font, 2 - custom font 0, 3 - custom font 1 etc.
	STDMETHOD_(void,SetFont)(int font_type) PURE;

	// set current text color with explicit RGB values (if R is < 0 then the default text color is set)
	// not all font types support custom colors, FONTT_FLAT8 font type has color native in font and is unaffected
	STDMETHOD_(void,SetTextColor)(int r, int g, int b) PURE;

	// get string extents (with current font)
	STDMETHOD_(void,GetStringSize)(const char *text, int &width, int &height) PURE;

	// draw text string (with current font and text color)
	STDMETHOD_(void,DrawString)(const char *text, int x, int y) PURE;

	// draw line (with current text color)
	STDMETHOD_(void,DrawLine)(int x1, int y1, int x2, int y2) PURE;

	//
	// methods that may ONLY be called inside a ShockOverlayHandler::Begin/EndTOverlayUpdate pair
	//

	// fill contents of a transparent overlay item to palette index color (0 is black), 'alpha' sets alpha component of
	// the image data and should not be confused with the alpha specified in the CreateOverlayItem functions,
	// which is applied on top of the alpha in image data (normally it's set to 255)
	STDMETHOD_(void,FillTOverlay)(int color_idx /*= 0*/, int alpha /*= 255*/) PURE;

	//
	// methods that may ONLY be called inside the ShockOverlayHandler::DrawTOverlay handler
	//

	// must be called before updating a transparent overlay item contents with draw calls
	// make sure the return value is TRUE before drawing anything
	STDMETHOD_(BOOL,BeginTOverlayUpdate)(int handle) PURE;

	// end update of a transparent overlay item, may only be called after a successful BeginTOverlayUpdate
	STDMETHOD_(void,EndTOverlayUpdate)() PURE;

	// draw a transparent overlay item, handle is an overlay item handle returned by one of the CreateTOverlayItem functions
	STDMETHOD_(void,DrawTOverlayItem)(int handle) PURE;
};
DEFINE_IIDSTRUCT(IShockOverlaySrv,IID_IShockOverlayScriptService)
#endif

#if (_DARKGAME <= 2)
interface IDarkOverlayHandler
{
	// handler called by the engine so script implementation of IDarkOverlayHandler can draw HUD elements
	STDMETHOD_(void,DrawHUD)(THIS) PURE;

	// handler called by the engine so script implementation of IDarkOverlayHandler can update and draw transparent
	// overlay elements (transparent overlay elements are bitmaps drawn directly to the 3D buffer with alpha blending,
	// like the security icon in shock)
	STDMETHOD_(void,DrawTOverlay)(THIS) PURE;

	// handler called by the engine when dark initializes UI components, this is also called when resuming game after
	// in-game menu and other UI screens and is a good place to update positioning of HUD elements in case display
	// resolution changed
	STDMETHOD_(void,OnUIEnterMode)(THIS) PURE;
};

extern  const GUID  IID_IDarkOverlayScriptService;
interface IDarkOverlaySrv : IScriptServiceBase
{
	// set the current overlay handler, there can only be one handler set at any given time, NULL clears handler
	STDMETHOD_(void,SetHandler)(IDarkOverlayHandler *pHandler) PURE;

	// get/load a bitmap that can be used for HUD drawing (max 128 bitmaps can be loaded, cleared when db resets)
	// returns a handle that can be used in subsequent bitmap functions or -1 if failed to load
	STDMETHOD_(int,GetBitmap)(const char *name, const char *path /*= "intrface\\"*/) PURE;

	// discard a no longer used bitmap handle, only needs to be called when using a lot of bitmaps to stay below 128
	STDMETHOD_(void,FlushBitmap)(int handle) PURE;

	// get size of a loaded bitmap
	STDMETHOD_(void,GetBitmapSize)(int handle, int &width, int &height) PURE;

	//
	// coordinate mapping (may ONLY be called inside the ShockOverlayHandler::DrawHUD and DrawTOverlay handlers)
	//

	// map a 3D position in the world to a screen coordinate, returns FALSE if off-screen
	STDMETHOD_(BOOL,WorldToScreen)(const cScrVec &pos, int &x, int &y) PURE;

	// get the screen space bounding rect of an object, returns FALSE if entirely off-screen
	STDMETHOD_(BOOL,GetObjectScreenBounds)(const object &obj, int &x1, int &y1, int &x2, int &y2) PURE;

	//
	// transparent overlay management
	//

	// create a transparent overlay item (like the security icon in shock), alpha is 0 to 255
	// its contents have to be updated using Begin/EndTOverlayUpdate and draw calls, note that constant updating
	// isn't too optimal, so it should only be done when necessary and avoided on larger items
	// returns an overlay handle, or -1 if failed
	STDMETHOD_(int,CreateTOverlayItem)(int x, int y, int width, int height, int alpha, BOOL trans_bg) PURE;

	// create a transparent overlay item from a bitmap, does not require its contents to be updated
	// (max 64 overlays can be created, cleared when db resets)
	// returns an overlay handle, or -1 if failed (will fail if bm_handle isn't a valid bitmap)
	STDMETHOD_(int,CreateTOverlayItemFromBitmap)(int x, int y, int alpha, int bm_handle, BOOL trans_bg) PURE;

	// destroy a transparent overlay item, not necessary to call but frees resources and overlay slot before a db reset
	STDMETHOD_(void,DestroyTOverlayItem)(int handle) PURE;

	// change alpha of a transparent overlay
	STDMETHOD_(void,UpdateTOverlayAlpha)(int handle, int alpha) PURE;

	// change position of a transparent overlay
	STDMETHOD_(void,UpdateTOverlayPosition)(int handle, int x, int y) PURE;

	// change display size of a transparent overlay (for scaled overlay items)
	STDMETHOD_(void,UpdateTOverlaySize)(int handle, int width, int height) PURE;

	//
	// methods that may ONLY be called inside the DarkOverlayHandler::DrawHUD handler
	// or inside a DarkOverlayHandler::Begin/EndTOverlayUpdate pair
	//

	// draw a loaded bitmap at the given position, bitmap will be drawn unscaled
	STDMETHOD_(void,DrawBitmap)(int handle, int x, int y) PURE;

	// draw a sub-rect of a loaded bitmap at the given position
	// src_x/src_y are relative to the bitmap (0,0 would be the upper left of the bitmap)
	STDMETHOD_(void,DrawSubBitmap)(int handle, int x, int y, int src_x, int src_y, int src_width, int src_height) PURE;

	// set current text color from a StyleColorKind
	// not all font types support custom colors, FONTT_FLAT8 font type has color native in font and is unaffected
	STDMETHOD_(void,SetTextColorFromStyle)(int style_color) PURE;

	// set current text color with explicit RGB values
	// not all font types support custom colors, FONTT_FLAT8 font type has color native in font and is unaffected
	STDMETHOD_(void,SetTextColor)(int r, int g, int b) PURE;

	// get string extents (with current font)
	STDMETHOD_(void,GetStringSize)(const char *text, int &width, int &height) PURE;

	// draw text string (with current font and text color)
	STDMETHOD_(void,DrawString)(const char *text, int x, int y) PURE;

	// draw line (with current text color)
	STDMETHOD_(void,DrawLine)(int x1, int y1, int x2, int y2) PURE;

	//
	// methods that may ONLY be called inside a DarkOverlayHandler::Begin/EndTOverlayUpdate pair
	//

	// fill contents of a transparent overlay item to palette index color (0 is black), 'alpha' sets alpha component of
	// the image data and should not be confused with the alpha specified in the CreateOverlayItem functions,
	// which is applied on top of the alpha in image data (normally it's set to 255)
	STDMETHOD_(void,FillTOverlay)(int color_idx /*= 0*/, int alpha /*= 255*/) PURE;

	//
	// methods that may ONLY be called inside the DarkOverlayHandler::DrawTOverlay handler
	//

	// must be called before updating a transparent overlay item contents with draw calls
	// make sure the return value is TRUE before drawing anything
	STDMETHOD_(BOOL,BeginTOverlayUpdate)(int handle) PURE;

	// end update of a transparent overlay item, may only be called after a successful BeginTOverlayUpdate
	STDMETHOD_(void,EndTOverlayUpdate)() PURE;

	// draw a transparent overlay item, handle is an overlay item handle returned by one of the CreateTOverlayItem functions
	STDMETHOD_(void,DrawTOverlayItem)(int handle) PURE;
};
DEFINE_IIDSTRUCT(IDarkOverlaySrv,IID_IDarkOverlayScriptService)
#endif


#endif // _LG_SCRSERVICES_H
