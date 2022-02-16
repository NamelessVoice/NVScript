// Half-written scripts that don't work, were never finished, or otherwise have no reason to exist in the main module

/***********************
 * NVSongTrap
 */
/*
MSGHANDLER cScr_NVSongTrap::OnActivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	DisplayPrintf("Hello world!!!!!");
	const char* pszEvent = ParamGetString(m_iObjId, "NVSongEvent", NULL);
	DisplayPrintf("Event is %s", pszEvent);
	
	SInterface<ISongPlayer> pSongPlayer(g_pScriptManager);
	pSongPlayer->SendEvent(pszEvent, 1);
	DisplayPrintf("Goodbye world!!!");
	return 0;
}

MSGHANDLER cScr_NVSongTrap::OnDeactivate(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	return 0;
}
*/

/***********************
 * NVInfinityStair
 */
/*
MSGHANDLER cScr_NVInfinityStair::OnAscend(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iMax = ParamGetInt(m_iObjId, "NVInfinityStairMax", 0);
	if (m_iLevel < iMax)
		Process(1);
	
	return 0;
}

MSGHANDLER cScr_NVInfinityStair::OnDescend(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	int iMin = ParamGetInt(m_iObjId, "NVInfinityStairMin", 0);
	if (m_iLevel > iMin)
		Process(-1);
		
	return 0;
}

void cScr_NVInfinityStair::Process(int iDirection)
{
	SService<IObjectSrv> pOS(g_pScriptManager);
	
	DisplayPrintf("Going to level %i", static_cast<int>(m_iLevel));	

	cScrVec vPlus(0,0,0), vZero(0,0,0);
	if (iDirection == 1)
		vPlus.z = -ParamGetFloat(m_iObjId, "NVInfinityStairHeight", 16.00f);
	else
		vPlus.z = ParamGetFloat(m_iObjId, "NVInfinityStairHeight", 16.00f);
	
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	SService<IPropertySrv> pPropSrv(g_pScriptManager);
				
	linkset lsLinks;
	pLS->GetAll(lsLinks, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, 0);
	
	for (; lsLinks.AnyLinksLeft(); lsLinks.NextLink())
	{
		sLink sl = lsLinks.Get();
		
		cMultiParm mpReturn;
		pLTS->LinkGetData(mpReturn, lsLinks.Link(), NULL);
		
		
		if (static_cast<int>(mpReturn) == static_cast<int>(m_iLevel))
		{
			pOS->Teleport(sl.dest, vPlus, vZero, sl.dest);
			SimpleSend(sl.dest, m_iObjId, "RemoveFromLevel");
			SimpleSend(sl.dest, m_iObjId, "AddToLevel");
		} else {
			if (static_cast<int>(mpReturn) == (static_cast<int>(m_iLevel) + iDirection))
			{
				pPropSrv->Set(sl.dest, "HasRefs", NULL, true);
			} else {
				pPropSrv->Set(sl.dest, "HasRefs", NULL, false);
			}
		}

	}
	
	if (iDirection == 1)
		m_iLevel++;
	else
		m_iLevel--;
	
	return;
}

MSGHANDLER cScr_NVInfinityStair::OnAddToLevel(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	
	DisplayPrintf("Add to level %i object %i", static_cast<int>(m_iLevel), pMsg->from);
	link lLink;
	pLS->Create(lLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, pMsg->from);
	if ( lLink )
	{
		pLTS->LinkSetData(lLink, NULL, static_cast<int>(m_iLevel));
	}
	return 0;
}

MSGHANDLER cScr_NVInfinityStair::OnRemoveFromLevel(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	link lLink;
	pLS->GetOne(lLink, pLTS->LinkKindNamed("ScriptParams"), m_iObjId, pMsg->from);
	if ( lLink)
	{
		pLS->Destroy(lLink);
	}
	return 0;
}
*/
/***********************
 * NVInfinityStairCacheRoom
 */
/*
MSGHANDLER cScr_NVInfinityStairCacheRoom::OnIn(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sRoomMsg* pRoomMsg = static_cast<sRoomMsg*>(pMsg);
	int iTransObj = pRoomMsg->MoveObjId;

	link lLink;
	sLink slLink;
	
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->GetOne(lLink, pLTS->LinkKindNamed("ControlDevice"), m_iObjId, 0);
	if ( !lLink)
		return 1;

	pLTS->LinkGet(lLink, slLink);
	
	sScrMsg msg;
				msg.from = iTransObj;
				msg.to = slLink.dest;
				msg.time = pMsg->time;
				msg.flags = pMsg->flags; // No one knows what this means.
				msg.message = "AddToLevel";
				
	g_pScriptManager->SendMessage(&msg, pReply);
	
//	DisplayPrintf("Hello, %i!", iTransObj);
	return 0;
}

MSGHANDLER cScr_NVInfinityStairCacheRoom::OnOut(sScrMsg* pMsg, sMultiParm* pReply, eScrTraceAction eTrace)
{
	sRoomMsg* pRoomMsg = static_cast<sRoomMsg*>(pMsg);
	int iTransObj = pRoomMsg->MoveObjId;
	
	link lLink, lLink2;
	sLink slLink;
	
	SService<ILinkSrv> pLS(g_pScriptManager);
	SService<ILinkToolsSrv> pLTS(g_pScriptManager);
	pLS->GetOne(lLink, pLTS->LinkKindNamed("ControlDevice"), m_iObjId, 0);
	if ( !lLink)
		return 1;

	pLTS->LinkGet(lLink, slLink);
	
	sScrMsg msg;
				msg.from = iTransObj;
				msg.to = slLink.dest;
				msg.time = pMsg->time;
				msg.flags = pMsg->flags; // No one knows what this means.
				msg.message = "RemoveFromLevel";
	
	g_pScriptManager->SendMessage(&msg, pReply);
	
//	DisplayPrintf("Goodbye, %i!", iTransObj);
	return 0;
}
*/