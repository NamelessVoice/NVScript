/*
[100050F0] = SetTimedMessage
[10005350] = SetScriptVar
[10005140] = GetScriptVar
[100051E0] = GetScriptVar

[10029220] = IScriptManager
[10029228] = ILinkSrv
[10029234] = IObjectSrv
[10029238] = IPhysSrv
[10029240] = IDarkGameSrv
[10029274] = 
[10029280] = IPropertySrv
[10029290] = IQuestSrv
[10029294] = IDamageSrv
[10029414] = IDataSrv
[100294A0] = ISoundScrSrv
[100294AC] = IDarkUISrv

[1001ef60] = str GoalStateVarName(int i)
[1001efa0] = str GoalVisibleVarName(int i)
[1001efe0] = str GoalMinDiffVarName(int i)
[1001f020] = str GoalMaxDiffVarName(int i)

*/

interface cScr_VictoryChecker : public IScr_Room
{
	// IScr_RootScript
	// Responds to CheckVictory and ReSubscribe
	//virtual void OnMessage_(sScrMsg&,cMultiParm*) = 0; // 0x100072C0
	//virtual void OnBeginScript_(sScrMsg&,cMultiParm*) = 0; // 0x10006a70
	//virtual void OnEndScript_(sScrMsg&,cMultiParm*) = 0; // 0x10006d30
	//virtual void OnTimer_(sScrTimerMsg&,cMultiParm*) = 0; // 0x10007110
	//virtual void OnQuestChange_(sQuestMsg&,cMultiParm*) = 0; // 0x10006d60
	//virtual void OnContainer_(sContainerScrMsg&,cMultiParm*) = 0; // 0x10006f10
	//virtual void OnSlain_(sSlayMsg&,cMultiParm*) = 0; // 0x10006de0
	// IScr_Room
	// cScr_VictoryChecker
	// Set the timer EndMission for 4.5 seconds, or from the qvar EndMission
	// If the svar DoFadeToBlack is set, set the timer FadeToBlack for 1 second and set the svar FadeTime.
	// If qvar MISSION_COMPLETE is 0 and svar IDied is 0, display FailedObj from PlayHint.str.
	virtual void __thiscall EndMission(void) = 0; // 0x10004c30
	// Subscribes to GOAL_STATE_n and GOAL_VISIBLE_n qvars. 
	// Stops at the first non-existent GOAL_STATE_n
	virtual void __thiscall SubscribeToGoalChanges(true_bool subscribe) = 0; // 0x100053b0
	virtual void __thiscall ObjectiveSet(int objective, int value) = 0; // 0x100055b0
	virtual void __thiscall ObjectiveShow(int objective, true_bool ShowElseHide) = 0; // 0x10005600
	// Sets the M-GarrettDiffSettings metaprops and adds a HitPointBonus
	virtual void __thiscall ObjectiveInit(int objective) = 0; // 0x10005660
	virtual int __thiscall ObjectiveGet(int objective) = 0; // 0x100056b0
	// Takes into account difficulty
	virtual true_bool __thiscall ObjectiveVisible(int objective) = 0; // 0x10005700
	virtual true_bool __thiscall ObjectiveExists(int objective) = 0; // 0x10005870
	// Return true if GOAL_REVERSE_n or GOAL_OPTIONAL_n
	virtual true_bool __thiscall ObjectiveNotBreach(int objective) = 0; // 0x100058d0
	// Returns 0, 1, or 3
	virtual int __thiscall ObjectivesMet(void) = 0; // 0x10005aa0
	// Returns true if mission is completed or failed.
	// Sets REVERSE and OPTIONAL qvars to final values.
	// Sets MISSION_COMPLETE and calls EndMission.
	virtual true_bool __thiscall CheckComplete(void) = 0; // 0x10005b80
	// Will send a Popup message
	virtual void __thiscall CheckNewGoal(void) = 0; // 0x10005d60
	virtual int __thiscall GoalVisibleMask(void) = 0; // 0x10006290
	virtual void __thiscall ObjectiveScore(int objective, true_bool DoneElseUndone) = 0; // 0x100063d0
	virtual void __thiscall ObjToGoal(object obj, true_bool got) = 0; // 0x10006a30
	virtual void __thiscall LootToGoal(int loot_total, int goldval, int gemsval, int goodsval, int lootbits) = 0; // 0x10006a50
};

interface cScr_HealingWaterFont : public IScr_RootScript
{
	virtual void __thiscall Heal(object Target) = 0; // 0x100074a0
	virtual int __thiscall GetHealAmount(void) = 0; // 0x100076b0
	virtual float __thiscall GetRechargeTime(void) = 0; // 0x100076d0
};

interface cScr_TransmogrifyMachine : public IScr_RootScript
{
	virtual void __thiscall OnTurnOn(void) = 0; // 0x1000c7d0
	virtual void __thiscall OnEnter(int,object) = 0; // 0x1000c7f0
	virtual void __thiscall OnExit(int,object) = 0; // 0x1000c810
};

