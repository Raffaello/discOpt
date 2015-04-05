# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

# $Id: Makefile.in 726 2006-04-17 04:16:00Z andreasw $

##########################################################################
#    You can modify this example makefile to fit for your own program.   #
#    Usually, you only need to change the five CHANGEME entries below.   #
##########################################################################

EXNAME = example

# CHANGEME: This should be the name of your executable
EXE = $(EXNAME)

# CHANGEME: Here is the name of all object files corresponding to the source
#           code that you wrote in order to define the problem statement
OBJS =  $(EXNAME).o

# CHANGEME: Additional libraries
ADDLIBS =

# CHANGEME: Additional flags for compilation (e.g., include flags)
ADDINCFLAGS =

# CHANGEME: Directory to the sources for the (example) problem definition
# files
SRCDIR = .


##########################################################################
#  Usually, you don't have to change anything below.  Note that if you   #
#  change certain compiler options, you might have to recompile the      #
#  COIN package.                                                         #
##########################################################################

COIN_HAS_PKGCONFIG = TRUE
COIN_CXX_IS_CL = #TRUE
COIN_HAS_DATASTOCHASTIC = @COIN_HAS_DATASTOCHASTIC_TRUE@TRUE

# C Compiler command
CC = gcc

# C Compiler options
CFLAGS = -O3 -pipe -DNDEBUG -pedantic-errors -Wimplicit -Wparentheses -Wsequence-point -Wreturn-type -Wcast-qual -Wall -Wno-unknown-pragmas -Wno-long-long   -DCOINMP_BUILD

# additional C Compiler options for linking
CLINKFLAGS =  -Wl,--rpath -Wl,/home/raffa/Documents/discOpt/CoinMP/lib

# Include directories (we use the CYGPATH_W variables to allow compilation with Windows compilers)
ifeq ($(COIN_HAS_PKGCONFIG), TRUE)
  INCL = `PKG_CONFIG_PATH=/home/raffa/Documents/discOpt/CoinMP/lib64/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/lib/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/share/pkgconfig: pkg-config --cflags coinmp`
else
  INCL = 
endif
INCL += $(ADDINCFLAGS)

# Linker flags
ifeq ($(COIN_HAS_PKGCONFIG), TRUE)
  LIBS = `PKG_CONFIG_PATH=/home/raffa/Documents/discOpt/CoinMP/lib64/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/lib/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/share/pkgconfig: pkg-config --libs coinmp`
else
  ifeq ($(COIN_CXX_IS_CL), TRUE)
    LIBS = -link -libpath:`$(CYGPATH_W) /home/raffa/Documents/discOpt/CoinMP/lib` libCoinMP.lib 
  else
    LIBS = -L/home/raffa/Documents/discOpt/CoinMP/lib -lCoinMP 
  endif
endif

# The following is necessary under cygwin, if native compilers are used
CYGPATH_W = echo

all: $(EXE)

.SUFFIXES: .c .o .obj

$(EXE): $(OBJS)
	bla=;\
	for file in $(OBJS); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CC) $(CLINKFLAGS) $(CFLAGS) -o $@ $$bla $(LIBS) $(ADDLIBS)

clean:
	rm -rf $(EXE) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ `test -f '$<' || echo '$(SRCDIR)/'`$<

.c.obj:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ `if test -f '$<'; then $(CYGPATH_W) '$<'; else $(CYGPATH_W) '$(SRCDIR)/$<'; fi`
