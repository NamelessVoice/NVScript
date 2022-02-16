/**************************
 * NVSongTrap
 * (NVTrap)
 *
 *
 * Messages: TurnOn, TurnOff (Specifiable)
 * Parameters: NVVOQVar, NVPoliteVO
 * Links: None
 */
/*
DECLARE_SCRIPT(cScr_NVSongTrap, cScrNVGeneric)
{
public:
	DECLARE_FACTORY(NVSongTrap,NVGenericTrap);
	cScr_NVSongTrap(const char* pszName, int iHostObjId)
		: cScrNVGeneric(pszName,iHostObjId)
	{ }

private:
	virtual long OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);
	virtual long OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace);

protected:
	virtual void InitScriptData()
	{
		cScrNVGeneric::InitScriptData();
  }

};
*/

/**************************
 * NVInfinityStair
 *
 * Infinite stairs
 *
 *
 * Messages: Descend, Ascend
 * Parameters: None
 * Links: None
 */
/*
DECLARE_SCRIPT(cScr_NVInfinityStair,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInfinityStair,CustomScript);
	cScr_NVInfinityStair(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId),
		SCRIPT_VAROBJ(m_iLevel)
	{ }

private:
	DECLARE_MSGHANDLER(OnAscend);
	DECLARE_MSGHANDLER(OnDescend);
	DECLARE_MSGHANDLER(OnAddToLevel);
	DECLARE_MSGHANDLER(OnRemoveFromLevel);
	void Process(int iDirection);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("Ascend", cScr_NVInfinityStair::OnAscend);
		REGISTER_MSGHANDLER("Descend", cScr_NVInfinityStair::OnDescend);
		REGISTER_MSGHANDLER("AddToLevel", cScr_NVInfinityStair::OnAddToLevel);
		REGISTER_MSGHANDLER("RemoveFromLevel", cScr_NVInfinityStair::OnRemoveFromLevel);
	}
	
	script_int m_iLevel;
	
	virtual void InitVars(void)
	{
		m_iLevel.Init();
		return;
	}
	
	
};

*/
/**************************
 * NVInfinityStairCacheRoom
 *
 * Infinite stairs
 *
 *
 * Messages: Descend, Ascend
 * Parameters: None
 * Links: None
 */
/*
DECLARE_SCRIPT(cScr_NVInfinityStairCacheRoom,cScriptNV)
{
public:
	DECLARE_FACTORY(NVInfinityStairCacheRoom,CustomScript);
	cScr_NVInfinityStairCacheRoom(const char* pszName, int iHostObjId)
		: cScriptNV(pszName,iHostObjId)
	{ }

private:
	DECLARE_MSGHANDLER(OnIn);
	DECLARE_MSGHANDLER(OnOut);

protected:

	virtual void InitScriptData()
	{
		cScriptNV::InitScriptData();
		REGISTER_MSGHANDLER("PlayerRoomEnter", cScr_NVInfinityStairCacheRoom::OnIn);
		REGISTER_MSGHANDLER("ObjectRoomEnter", cScr_NVInfinityStairCacheRoom::OnIn);
		REGISTER_MSGHANDLER("PlayerRoomExit", cScr_NVInfinityStairCacheRoom::OnOut);
		REGISTER_MSGHANDLER("ObjectRoomExit", cScr_NVInfinityStairCacheRoom::OnOut);
	}	
};

*/