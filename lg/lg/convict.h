#include <lg/gen.h>

interface cScr_VictoryChecker : public IScr_Room
{
	// IScr_RootScript
	// On CheckVictory: Call CheckComplete.
	// On ReSubscribe: Call SubscribeToGoalChanges(TRUE)
	//virtual void OnMessage_(sScrMsg&,cMultiParm*) = 0; // 0x100072C0
	// SubscribeToGoalChanges(TRUE)
	// Sets the M-GarrettDiffSettings metaprops
	// If variable HitPointBonus exists, add it to HitPoints and MAX_HP
	// Instantiate AirSupply
	//virtual void OnBeginScript_(sScrMsg&,cMultiParm*) = 0; // 0x10006A70
	// Does nothing on VictoryChecker
	//virtual void OnEndScript_(sScrMsg&,cMultiParm*) = 0; // 0x10006D30
	// On EndMission: Call IDarkGameSrv::EndMission
	// On FadeToBlack: Call IDarkGameSrv::FadeToBlack with svar FadeTime
	//virtual void OnTimer_(sScrTimerMsg&,cMultiParm*) = 0; // 0x10007110
	// If newValue != oldValue, calls CheckComplete then CheckNewGoal.
	//virtual void OnQuestChange_(sQuestMsg&,cMultiParm*) = 0; // 0x10006D60
	// Calls ObjToGoal. If picking up object with Loot propety, call LootToGoal
	//virtual void OnContainer_(sContainerScrMsg&,cMultiParm*) = 0; // 0x10006F10
	// Calls IDarkGameSrv::KillPlayer, sets MISSION_COMPLETE to 0
	// Sets svar DoFadeToBlack to 1 and svar IDied to 1
	// Calls EndMission
	//virtual void OnSlain_(sSlayMsg&,cMultiParm*) = 0; // 0x10006DE0
	// IScr_Room
	// cScr_VictoryChecker
	// Set the timer EndMission for 4.5 seconds, or from the qvar EndingTime
	// If the svar DoFadeToBlack is set, set the timer FadeToBlack for 1 second and set the svar FadeTime.
	// If qvar MISSION_COMPLETE is 0 and svar IDied is not 1, display FailedObj from PlayHint.str.
	virtual void __thiscall EndMission(void) = 0; // 0x10004C30
	// Subscribes to GOAL_STATE_n and GOAL_VISIBLE_n qvars. 
	// Stops at the first non-existent GOAL_STATE_n
	virtual void __thiscall SubscribeToGoalChanges(true_bool subscribe) = 0; // 0x100053B0
	// Sets the GOAL_STATE_n
	virtual void __thiscall ObjectiveSet(int objective, int value) = 0; // 0x100055B0
	// Sets GOAL_VISIBLE_n
	virtual void __thiscall ObjectiveShow(int objective, true_bool ShowElseHide) = 0; // 0x10005600
	// ObjectiveSet(objective, 0); ObjectiveShow(objective, TRUE); 
	virtual void __thiscall ObjectiveInit(int objective) = 0; // 0x10005660
	// Returns GOAL_STATE_n
	virtual int __thiscall ObjectiveGet(int objective) = 0; // 0x100056B0
	// Returns true if the objective is valid at this difficulty and is visible.
	virtual true_bool* __thiscall ObjectiveVisible(true_bool&, int objective) = 0; // 0x10005700
	// Reterns true if GOAL_STATE_n exists
	virtual true_bool* __thiscall ObjectiveExists(true_bool&, int objective) = 0; // 0x10005870
	// Return true if GOAL_REVERSE_n is non-zero or GOAL_OPTIONAL_n is non-zero
	virtual true_bool* __thiscall ObjectiveNotBreach(true_bool&, int objective) = 0; // 0x100058D0
	// Returns 3 if any GOAL_STATE_n is 3.
	// Otherwise, return 1 if all non-optional, non-reverse goals are non-zero, else 0.
	virtual int __thiscall ObjectivesMet(void) = 0; // 0x10005AA0
	// Returns true if mission is completed or failed.
	// If completed, sets any incomplete optional goals to 2,
	// and incomplete non-optional goals (implied no-breach) to 1,
	// then sets MISSION_COMPLETE to 1 and calls EndMission.
	// If failed, sets MISSION_COMPLETE to 0 and calls EndMission.
	virtual true_bool* __thiscall CheckComplete(true_bool&) = 0; // 0x10005B80
	// Will send a Popup message
	// If a GOAL_VISIBLE_n qvar changes from 0 to 1, sends a "Popup" message with data "NewGoal"
	// and displays the "NewGoal" text from PlayHint.str and plays the schema "new_obj".
	// If the variable "goal_notify" is set and 
	// a GOAL_STATE_n qvar changes from 0 to 1, sends a "Popup" message with data "DoneGoal"
	// and displays the "DoneGoal" text from PlayHint.str and plays the schema "new_obj".
	virtual void __thiscall CheckNewGoal(cScrStr qvar, int old, int new) = 0; // 0x10005D60
	// Calls ObjectiveScore(n,1) for each goal where GOAL_SATISFIED_n != 0 && GOAL_STATE_n != 0
	virtual void __thiscall ScoreObjectives(void) = 0; // 0x10006290
	// Will not undo an objective if GOAL_IRREVERSIBLE_n is set.
	// Does nothing if objective is GOAL_STATE_n is 2
	// If GOAL_REVERSE_n && DoneElseUndone then fail the goal.
	// If GOAL_FINAL_n != 0, then set GOAL_SATISFIED_n and
	// ... If DoneElseUndone and ObjectivesMet is true disregarding this objective,
	// ... and MISSION_COMPLETE is 1, 
	// ... then set GOAL_IRREVERSIBLE_n to 1 and set the goal state
	// ... Otherwise, leave the objective state as-is.
	// Otherwise, set the goal state and calls ScoreObjectives
	// Reveals a GOAL_BONUS_n
	virtual void __thiscall ObjectiveScore(int objective, true_bool DoneElseUndone) = 0; // 0x100063D0
	// Does nothing in VictoryChecker
	virtual void __thiscall ObjToGoal(object obj, true_bool got) = 0; // 0x10006A30
	// Does nothing in VictoryChecker
	virtual void __thiscall LootToGoal(int loot_total, int goldval, int gemsval, int goodsval, int lootbits) = 0; // 0x10006A50
};

enum goaltype
{
	kGoalNone,
	kGoalTake,
	kGoalSlay,
	kGoalLoot,
	kGoalGoTo
};

interface IScr_VictoryCheck : public IScr_VictoryChecker
{
	// IScr_RootScript
	// On J'Accuse: ScanGoals(kGoalSlay, from, 1);
	//virtual void OnMessage_(sScrMsg&,cMultiParm*) = 0; // 0x10007BD0
	// Calls NoKillGoalCheat
	//virtual void OnBeginScript_(sScrMsg&,cMultiParm*) = 0; // 0x10007AB0
	// IScr_Room
	// ScanGoals(kGoalGoTo, FromObjId, 0); ScanGoals(kGoalGoTo, ToObjId, 1);
	//virtual void OnObjRoomTransit_(sRoomMsg&,cMultiParm*) = 0; // 0x10007B40
	// IScr_VictoryChecker
	// ScanGoals(kGoalTake, obj, got)
	//virtual void ObjToGoal(object obj, true_bool got) = 0; // 0x10007A80
	// Checks the first 31 goals that are loot objectives.
	// if goal_special_n !=0 :: goal_specials_n |= 1 << ((goal_special_n - 1) & 0xFF)
	// score = loot_total > goal_loot && goldval > goal_gold && gemsval > goal_gems && goodsval > goal_goods && (lootbits & goal_specials == goal_specials)
	// Call ObjectiveScore
	//virtual void LootToGoal(int loot_total, int goldval, int gemsval, int goodsval, int lootbits) = 0; // 0x100078C0
	// Returns GOAL_TYPE_n
	virtual int __thiscall GoalType(int objective) = 0; // 0x100073A0
	// Sets all non-reversed slay objectives to 2 if the variable "NoKillGoalCheat" is set.
	// Only checks the first 31 goals. (most of these other functions are virtually unlimited)
	virtual void __thiscall NoKillGoalCheat(void) = 0; // 0x10007480
	// Returns an object with id GOAL_TARGET_n. Does not verify the id.
	virtual object* __thiscall GoalTarget(object& target, int objective) = 0; // 0x10007580
	// Returns the loot requirement. loottype is "loot", "gold", "gems", "goods", "specials", "special".
	// Actually, it just looks for "goal_loottype_n".
	virtual int __thiscall GoalLoot(int objective, cScrStr loottype) = 0; // 0x10007670
	// Checks the first 31 goals regardless of validity.
	// With a non-zero target, each goal that matches typefilter
	// and the goal_target is a type of target, calls ObjectiveScore(objective, score)
	virtual void __thiscall ScanGoals(goaltype typefilter, object target, true_bool score) = 0; // 0x100077A0
};

