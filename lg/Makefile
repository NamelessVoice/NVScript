# Makefile for lg library
# GCC in Cygwin/Mingw32 

.SUFFIXES:
.SUFFIXES: .o .cpp

srcdir = .


SH = sh

CC = g++-3
AR = ar
DLLWRAP = dllwrap

# _DARKGAME is not used here. The implementation of lgLib is universal
DEFINES = -DWINVER=0x0400 -D_WIN32_WINNT=0x0400 -DWIN32_LEAN_AND_MEAN

ARFLAGS = rc
LDFLAGS = -mno-cygwin -mwindows -L.
LIBS = -lstdc++ -luuid
INCLUDEDIRS = -I. -I$(srcdir)
# If you care for this... # -Wno-unused-variable 
# A lot of the callbacks have unused parameters, so I turn that off.
CXXFLAGS =  -W -Wall -Wno-unused-parameter -Wno-pmf-conversions \
			$(INCLUDEDIRS) -mno-cygwin -masm=att \
			-fno-pcc-struct-return -mms-bitfields
LDFLAGS = -mno-cygwin -mwindows -L. -llg
DLLFLAGS = --def script.def --add-underscore --target i386-mingw32

ifdef DEBUG
CXXDEBUG = -g -DDEBUG 
LDDEBUG = -g
else
CXXDEBUG = -O2 -DNDEBUG
LDDEBUG =
endif

LG_HEADERS = lg/actreact.h \
	lg/ai.h \
	lg/config.h \
	lg/convict.h \
	lg/defs.h \
	lg/dynarray.h \
	lg/dynarray.hpp \
	lg/editor.h \
	lg/gen.h \
	lg/graphics.h \
	lg/iiddef.h \
	lg/iids.h \
	lg/input.h \
	lg/interface.h \
	lg/links.h \
	lg/malloc.h \
	lg/miss16.h \
	lg/objects.h \
	lg/objstd.h \
	lg/propdefs.h \
	lg/properties.h \
	lg/quest.h \
	lg/res.h \
	lg/script.h \
	lg/scrmanagers.h \
	lg/scrmsgs.h \
	lg/scrservices.h \
	lg/sound.h \
	lg/tools.h \
	lg/types.h \
	lg/win.h

LG_LIB = liblg.a
LG_SRCS = lg.cpp scrmsgs.cpp iids.cpp
LG_OBJS = lg.o  scrmsgs.o iids.o

LG_LIBD = liblg-d.a
LG_OBJSD = lg-d.o scrmsgs-d.o iids.o

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) -o $@ -c $<

%-d.o: %.cpp
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) -o $@ -c $<


ALL:	stamp $(LG_LIB) $(LG_LIBD)

clean:
	$(RM) $(LG_OBJS)

stamp: $(LG_SRCS) $(LG_HEADERS)
	$(RM) lg/stamp-*
	$(SH) timestamp.sh lg lg $(LG_SRCS)

$(LG_LIB): $(LG_OBJS)
	$(AR) $(ARFLAGS) $@ $?

$(LG_LIBD): CXXDEBUG = -g -DDEBUG
$(LG_LIBD): LDDEBUG = -g
$(LG_LIBD): $(LG_OBJSD)
	$(AR) $(ARFLAGS) $@ $?

iids.o: iids.cpp
lg.o: lg.cpp lg/types.h lg/dynarray.h lg/dynarray.hpp
scrmsgs.o: scrmsgs.cpp lg/scrmsgs.h
lg-d.o: lg.cpp lg/types.h lg/dynarray.h lg/dynarray.hpp
scrmsgs-d.o: scrmsgs.cpp lg/scrmsgs.h
