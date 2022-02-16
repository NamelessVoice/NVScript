#ifndef _LG_QUEST_H
#define _LG_QUEST_H

#if _MSC_VER > 1000
#pragma once
#endif

extern  const GUID  IID_IQuestDataIter;
interface IQuestDataIter : IUnknown
{
	STDMETHOD_(int,Done)(void) PURE;
	STDMETHOD_(long,Next)(void) PURE;
	STDMETHOD_(const char*,Name)(void) PURE;
	STDMETHOD_(int,Value)(void) PURE;
};

extern  const GUID  IID_IQuestData;
interface IQuestData : IUnknown
{
	STDMETHOD_(long,Create)(const char *,int) PURE;
	STDMETHOD_(long,Set)(const char *,int) PURE;
	STDMETHOD_(int,Get)(const char *) PURE;
	STDMETHOD_(int,Exists)(const char *) PURE;
	STDMETHOD_(long,Delete)(const char *) PURE;
	STDMETHOD_(IQuestDataIter*,Iter)(int) PURE;
	STDMETHOD_(long,DeleteAll)(void) PURE;
	STDMETHOD_(long,DeleteAllType)(int) PURE;
	STDMETHOD_(int,Save)(long (__cdecl *)(void *,unsigned int,unsigned int),int) PURE;
	STDMETHOD_(int,Load)(long (__cdecl *)(void *,unsigned int,unsigned int),int) PURE;
	STDMETHOD_(long,SubscribeMsg)(int,const char *,int) PURE;
	STDMETHOD_(long,UnsubscribeMsg)(int,const char *) PURE;
	STDMETHOD_(long,ObjDeleteListener)(int) PURE;
	STDMETHOD_(long,UnsubscribeAll)(void) PURE;
	STDMETHOD_(long,ObjSpewListen)(int) PURE;
	STDMETHOD_(long,Filter)(int (__cdecl *)(const char *,int,int),void *) PURE;
};
DEFINE_IIDSTRUCT(IQuestData,IID_IQuestData)

extern  const GUID  IID_ICampaign ;

#endif // _LG_QUEST_H
