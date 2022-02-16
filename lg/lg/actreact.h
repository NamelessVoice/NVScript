/**********************
 * LGS Act/React
 */

#ifndef _LG_ACTREACT_H
#define _LG_ACTREACT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <lg/objstd.h>

struct sPropagatorDesc
{
};

struct sStimSourceDesc
{
};

struct sStimSensorEvent
{
};

struct sStimSourceEvent
{
};

struct sReactionEvent
{
};

struct sReactionParam
{
};

extern  const GUID  IID_IPropagator;
interface IPropagator : IUnknown
{
	STDMETHOD_(const sPropagatorDesc *,Describe)(void) const PURE;
	STDMETHOD_(unsigned long,GetID)(void) PURE;
	STDMETHOD_(long,AddStimulus)(int) PURE;
	STDMETHOD_(long,RemoveStimulus)(int) PURE;
	STDMETHOD_(int,SupportsStimulus)(int) PURE;
	STDMETHOD_(long,InitSource)(sStimSourceDesc*) PURE;
	STDMETHOD_(const sStructDesc *,DescribeShapes)(void) PURE;
	STDMETHOD_(const sStructDesc *,DescribeLifeCycles)(void) PURE;
	STDMETHOD_(long,SourceEvent)(sStimSourceEvent *) PURE;
	STDMETHOD_(float,GetSourceLevel)(unsigned long) PURE;
	STDMETHOD_(long,DescribeSource)(unsigned long,sStimSourceDesc *) PURE;
	STDMETHOD_(int,f11)(void) PURE;
	STDMETHOD_(long,Connect)(IUnknown *) PURE;
	STDMETHOD_(long,Propagate)(unsigned long,unsigned long) PURE;
	STDMETHOD_(long,Reset)(void) PURE;
	STDMETHOD_(long,Start)(void) PURE;
	STDMETHOD_(long,Stop)(void) PURE;
};

extern  const GUID  IID_IPropagatorQuery;
interface IPropagatorQuery : IUnknown
{
	STDMETHOD_(long,Start)(void) PURE;
	STDMETHOD_(int,Done)(void) PURE;
	STDMETHOD_(long,Next)(void) PURE;
	STDMETHOD_(unsigned long,ID)(void) PURE;
	STDMETHOD_(IPropagator*,Propagator)(void) PURE;
};

extern  const GUID  IID_IReactionQuery;
interface IReactionQuery : IUnknown
{
	STDMETHOD_(long,Start)(void) PURE;
	STDMETHOD_(int,Done)(void) PURE;
	STDMETHOD_(long,Next)(void) PURE;
	STDMETHOD_(unsigned long,ID)(void) PURE;
};

extern  const GUID  IID_IStimSensorQuery ;
extern  const GUID  IID_IStimSourceQuery ;

struct sReactionDesc
{
	char	szName[32];
	char	szLongName[64];
	char	szParamType[32];
	int		unknown;
};

extern  const GUID  IID_IReactions;
interface IReactions : IUnknown
{
	STDMETHOD_(unsigned long,Add)(sReactionDesc*,unsigned long (__stdcall *)(sReactionEvent*,const sReactionParam*,void*),void*) PURE;
	STDMETHOD_(unsigned long,GetReactionNamed)(const char*) PURE;
	STDMETHOD_(const sReactionDesc*,DescribeReaction)(unsigned long) PURE;
	STDMETHOD_(IReactionQuery*,QueryAll)(void) PURE;
	STDMETHOD_(unsigned long,React)(unsigned long,sReactionEvent*,const sReactionParam) PURE;
};
DEFINE_IIDSTRUCT(IReactions,IID_IReactions)

extern  const GUID  IID_IStimSensors ;
extern  const GUID  IID_IStimSources ;
extern  const GUID  IID_IStimulator ;
extern  const GUID  IID_IStimuli ;
extern  const GUID  IID_IPeriodicPropagator ;

extern  const GUID  IID_IPropagation ;
interface IPropagation : IUnknown
{
	STDMETHOD_(unsigned long,AddPropagator)(IPropagator*) PURE;
	STDMETHOD_(long,AddPropagatorStimulus)(unsigned long,int) PURE;
	STDMETHOD_(long,RemovePropagatorStimulus)(unsigned long,int) PURE;
	STDMETHOD_(int,SupportsStimulus)(unsigned long,int) PURE;
	STDMETHOD_(IPropagator*,GetPropagator)(unsigned long) PURE;
	STDMETHOD_(IPropagator*,GetPropagatorNamed)(const char*) PURE;
	STDMETHOD_(IPropagatorQuery*,GetPropagators)(int) PURE;
	STDMETHOD_(long,SensorEvent)(sStimSensorEvent*) PURE;
	STDMETHOD_(long,SimpleSensorEvent)(unsigned long,unsigned long) PURE;
	STDMETHOD_(long,SourceEvent)(sStimSourceEvent*) PURE;
	STDMETHOD_(long,SimpleSourceEvent)(unsigned long,unsigned long) PURE;
	STDMETHOD_(long,Propagate)(unsigned long,unsigned long) PURE;
	STDMETHOD_(long,Reset)(void) PURE;
	STDMETHOD_(long,AddStimulus)(int) PURE;
};

#endif // _LG_ACTREACT_H
