# Makefile for custom scripts
# Makefile for NVScript. No one knows what this does. Don't touch it - it WORKS! 

.SUFFIXES:
.SUFFIXES: .o .cpp .rc _osm.cpp _res.o

srcdir = .
LGDIR = ../lg
DHDIR = ../DH2

GAME = 2

GAMESUFFIX =
SSINCLUDEH =
SSINCLUDEO =
SSINCLUDEC =

# For some reason this whole thing doesn't work anymore
ifeq (3, $(GAME))
GAMESUFFIX =-SS2
endif

ifeq (1, $(GAME))
GAMESUFFIX =-T1
endif

ifeq (3, $(GAME))
SSINCLUDEH = NVScript-SS2.h NVScript-ThiefShock.h
SSINCLUDEO = NVScript-SS2.o NVScript-ThiefShock.o
SSINCLUDEC = NVScript-SS2.cpp NVScript-ThiefShock.cpp
endif

CC = g++-3
AR = ar
LD = g++-3
DLLTOOL = dlltool
RC = windres
# GNU ar updates the symbols automatically.
# Set this if you need to do it yourself
RANLIB = echo

ifdef BUILD
	B = -D_BUILD=1
else
	B = 
endif

DEFINES = -DWINVER=0x0400 -D_WIN32_WINNT=0x0400 -DWIN32_LEAN_AND_MEAN -D_DARKGAME=$(GAME) $(B)

ifdef DEBUG
D = -d
else
D =
endif

ARFLAGS = rc

LIBS = -lScript$(GAMESUFFIX)$(D) -ldh2 -llg$(D) -lstdc++ -luuid -lwinmm

LDFLAGS = -mno-cygwin -mwindows -mdll -L. -L$(DHDIR) -L$(LGDIR)
INCLUDEDIRS = -I. -I$(srcdir) -I$(DHDIR) -I$(LGDIR)
# If you care for this... # -Wno-unused-variable 
# A lot of the callbacks have unused parameters, so I turn that off.
CXXFLAGS =  -W -Wall -Wno-unused-parameter -Wno-pmf-conversions \
			$(INCLUDEDIRS) -mno-cygwin -masm=att \
			-fno-pcc-struct-return -mms-bitfields
#DLLFLAGS = --def script.def --add-underscore --target i386-mingw32 --image-base=0x13000000
DLLFLAGS = --add-underscore

ifdef DEBUG
CXXDEBUG = -g -DDEBUG -DDEBUG_PARAMCACHE
LDDEBUG = -g
else
CXXDEBUG = -O3 -DNDEBUG
LDDEBUG = -s
endif

SCRLIB_SRCS = ScriptLib/ScriptLib.cpp \
	ScriptLib/GlobalParamCache.cpp \
	ScriptLib/ParamCache.cpp \
	ScriptLib/AddMetaProperty.cpp \
	ScriptLib/AddSingleMetaProperty.cpp \
	ScriptLib/CalcTextTime.cpp \
	ScriptLib/CDSend.cpp \
	ScriptLib/DebugPrintf.cpp \
	ScriptLib/DeleteAllLinks.cpp \
	ScriptLib/DeleteAllLinksByData.cpp \
	ScriptLib/DisplayPrintf.cpp \
	ScriptLib/FixupPlayerLinks.cpp \
	ScriptLib/GetAnyLink.cpp \
	ScriptLib/GetAnyLinkByData.cpp \
	ScriptLib/GetAnyLinkByDataDest.cpp \
	ScriptLib/GetAnyLinkByDataInheritedSrc.cpp \
	ScriptLib/GetAnyLinkDest.cpp \
	ScriptLib/GetAnyLinkInheritedSrc.cpp \
	ScriptLib/GetObjectParamBool.cpp \
	ScriptLib/GetObjectParamFloat.cpp \
	ScriptLib/GetObjectParamInt.cpp \
	ScriptLib/GetObjectParamObject.cpp \
	ScriptLib/GetObjectParams.cpp \
	ScriptLib/GetObjectParamsNoInherit.cpp \
	ScriptLib/GetObjectParamString.cpp \
	ScriptLib/GetObjectParamTime.cpp \
	ScriptLib/GetOneLink.cpp \
	ScriptLib/GetOneLinkByData.cpp \
	ScriptLib/GetOneLinkByDataDest.cpp \
	ScriptLib/GetOneLinkByDataInheritedSrc.cpp \
	ScriptLib/GetOneLinkDest.cpp \
	ScriptLib/GetOneLinkInheritedSrc.cpp \
	ScriptLib/GetParamBool.cpp \
	ScriptLib/GetParamFloat.cpp \
	ScriptLib/GetParamInt.cpp \
	ScriptLib/GetParamObject.cpp \
	ScriptLib/GetParamString.cpp \
	ScriptLib/GetParamTime.cpp \
	ScriptLib/HexTable.cpp \
	ScriptLib/IterateLinks.cpp \
	ScriptLib/IterateLinksByData.cpp \
	ScriptLib/MultiParmAssignFloat.cpp \
	ScriptLib/MultiParmAssignInt.cpp \
	ScriptLib/MultiParmAssignString.cpp \
	ScriptLib/MultiParmAssignVector.cpp \
	ScriptLib/ParamExists.cpp \
	ScriptLib/ParamExistsDirect.cpp \
	ScriptLib/ParamGetBool.cpp \
	ScriptLib/ParamGetFloat.cpp \
	ScriptLib/ParamGetInt.cpp \
	ScriptLib/ParamGetObject.cpp \
	ScriptLib/ParamGetString.cpp \
	ScriptLib/ParamGetTime.cpp \
	ScriptLib/ParamRemove.cpp \
	ScriptLib/ParamSetBool.cpp \
	ScriptLib/ParamSetFloat.cpp \
	ScriptLib/ParamSetInt.cpp \
	ScriptLib/ParamSetString.cpp \
	ScriptLib/RemoveObjectParam.cpp \
	ScriptLib/RemoveMetaProperty.cpp \
	ScriptLib/RemoveSingleMetaProperty.cpp \
	ScriptLib/SetObjectParamBool.cpp \
	ScriptLib/SetObjectParamFloat.cpp \
	ScriptLib/SetObjectParamInt.cpp \
	ScriptLib/SetObjectParams.cpp \
	ScriptLib/SetObjectParamString.cpp \
	ScriptLib/SetParamBool.cpp \
	ScriptLib/SetParamFloat.cpp \
	ScriptLib/SetParamInt.cpp \
	ScriptLib/SetParamString.cpp \
	ScriptLib/ShowBook.cpp \
	ScriptLib/ShowString.cpp \
	ScriptLib/SimplePost.cpp \
	ScriptLib/SimpleSend.cpp \
	ScriptLib/strqsep.cpp \
	ScriptLib/strsep.cpp \
	ScriptLib/strtocolor.cpp \
	ScriptLib/StrToObject.cpp \
	ScriptLib/strtotime.cpp
SCRLIB_OBJS = $(SCRLIB_SRCS:%.cpp=%.o)
SCRLIB_OBJSD = $(SCRLIB_SRCS:%.cpp=%-d.o)

COMMON_SRCS =  Script.cpp ScriptNV.cpp ScriptModule.cpp ScrInstance.cpp NVScriptLib.cpp scriptvars.cpp
COMMON_OBJS =  Script.o ScriptNV.o ScriptModule.o ScrInstance.o NVScriptLib.o scriptvars.o exports.o

LIBSA = libScript$(GAMESUFFIX).a
LIBSAD = libScript$(GAMESUFFIX)-d.a

GEN_SRCS = NVGenericScript.cpp NVGenericTrigger.cpp
# previously included:
# GEN_SRCS = GenericScript.cpp GenericTrap.cpp GenericScale.cpp GenericControl.cpp GenericTrigger.cpp
GEN_OBJS = NVGenericScript.o NVGenericTrigger.o
# previously included:
# GEN_OBJS = GenericScript.o GenericTrap.o GenericScale.o GenericControl.o GenericTrigger.o

SRCS = NVScript.cpp NVScript-CoSaS.cpp NVScript-Flames.cpp $(SSINCLUDEC) NVScript_osm.cpp \
       NVDebug.cpp Citadel.cpp

OBJS =  NVScript.o NVScript-CoSaS.o NVScript-Flames.o $(SSINCLUDEO)
	   
SCRLIB = libScript$(GAMESUFFIX)$(D).a

OSMS = NVScript.osm NVDebug.osm

ifeq (3, $(GAME))
	# SS2
	OSMS += Citadel.osm
endif


.PRECIOUS: %_osm.cpp %.o

%_osm.cpp: %.h
	perl $(srcdir)/genosm.pl $(DEFINES) $(INCLUDEDIRS) -o $@ $^

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) -o $@ -c $<

%-d.o: %.cpp
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) -o $@ -c $<

%_res.o: %.rc version.rc
	$(RC) $(DEFINES) -o $@ -i $<

%.osm: %.o %_osm.o exports.o
	$(LD) $(LDFLAGS) $(LDDEBUG) -o $@ $^ $(LIBS)


ALL:	$(OSMS)
	
libs:	$(LIBSA) $(LIBSAD)
	
clean:
	$(RM) *.osm *.o *.obj


deps:
	$(CC) -MM $(CXXFLAGS) $(DEFINES) $(SRCS) $(NV_SRCS) $(GEN_SRCS) $(COMMON_SRCS) $(LG_SRCS) >$(srcdir)/deps-NVScript.mak

.INTERMEDIATE: exports.o

exports.o: ScriptModule.o
	$(DLLTOOL) $(DLLFLAGS) --output-exp $@ $^

$(LIBSA): CXXDEBUG = -O3 -DNDEBUG
$(LIBSA): LDDEBUG =
$(LIBSA): $(SCRLIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

$(LIBSAD): CXXDEBUG = -g -DDEBUG -DDEBUG_PARAMCACHE
$(LIBSAD): LDDEBUG = -g
$(LIBSAD): $(SCRLIB_OBJSD)
	$(AR) $(ARFLAGS) $@ $?
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1
	
ScriptModuleNP.o: ScriptModule.cpp ScriptModule.h ScriptLib.h
	$(CC) $(CXXFLAGS) $(CXXDEBUG) -DNO_PARAMCACHE $(DEFINES) -o $@ -c $<
	
	
NVScript.osm: NVScript.o NVScript-CoSaS.o NVScript-Flames.o $(SSINCLUDEO) NVScript_osm.o $(GEN_OBJS) $(COMMON_OBJS) NVScript_res.o
	$(LD) $(LDFLAGS) -Wl,--image-base=0x13100000 $(LDDEBUG) -o $@ $^ $(LIBS)
	
NVDebug.osm: NVDebug.o NVDebug_osm.o $(COMMON_OBJS) NVDebug_res.o
	$(LD) $(LDFLAGS) -Wl,--image-base=0x13000000 $(LDDEBUG) -o $@ $^ $(LIBS)

Citadel.osm: Citadel.o Citadel_osm.o $(COMMON_OBJS) Citadel_res.o
	$(LD) $(LDFLAGS) -Wl,--image-base=0x13200000 $(LDDEBUG) -o $@ $^ $(LIBS)
#NVT2X.osm: NVT2X.o NVT2X_osm.o $(COMMON_OBJS) NVT2X_res.o
#	$(LD) $(LDFLAGS) -Wl,--image-base=0x13220000 $(LDDEBUG) -o $@ $^ $(LIBS)

NVScript_osm.cpp: NVScript.h NVScript-CoSaS.h NVScript-Flames.h $(SSINCLUDEH)
	perl $(srcdir)/genosm.pl $(DEFINES) $(INCLUDEDIRS) -o $@ $^

NVDebug_osm.cpp: NVDebug.h
	perl $(srcdir)/genosm.pl $(DEFINES) $(INCLUDEDIRS) -o $@ $^
	
Citadel_osm.cpp: Citadel.h
	perl $(srcdir)/genosm.pl $(DEFINES) $(INCLUDEDIRS) -o $@ $^
#NVT2X_osm.cpp: NVT2X.h
#	perl $(srcdir)/genosm.pl $(DEFINES) $(INCLUDEDIRS) -o $@ $^
	
NVScript_res.o: NVScript.rc
	$(RC) $(DEFINES) -o $@ -i $<
	
NVDebug_res.o: NVDebug.rc
	$(RC) $(DEFINES) -o $@ -i $<
	
#NVT2X_res.o: NVT2X.rc
#	$(RC) $(DEFINES) -o $@ -i $<
Citadel_res.o: Citadel.rc
	$(RC) $(DEFINES) -o $@ -i $<
	
	
include deps-NVScript.mak

