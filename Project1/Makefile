#
# Created by gmakemake (Ubuntu Jul 25 2014) on Mon Mar  8 10:27:44 2021
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CFLAGS =	-std=c99 -ggdb -Wall -Wextra -pedantic
CLIBFLAGS =	-lm 


########## End of flags from header.mak


CPP_FILES =	
C_FILES =	display.c use_getopt.c wildfire.c
PS_FILES =	
S_FILES =	
H_FILES =	display.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	display.o 

#
# Main targets
#

all:	use_getopt wildfire 

use_getopt:	use_getopt.o $(OBJFILES)
	$(CC) $(CFLAGS) -o use_getopt use_getopt.o $(OBJFILES) $(CLIBFLAGS)

wildfire:	wildfire.o $(OBJFILES)
	$(CC) $(CFLAGS) -o wildfire wildfire.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

display.o:	display.h
use_getopt.o:	
wildfire.o:	display.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) use_getopt.o wildfire.o core

realclean:        clean
	-/bin/rm -f use_getopt wildfire 
