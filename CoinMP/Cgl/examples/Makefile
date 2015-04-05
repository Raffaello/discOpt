# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

# $Id: Makefile.in 726 2006-04-17 04:16:00Z andreasw $

##########################################################################
#    You can modify this example makefile to fit for your own program.   #
#    Usually, you only need to change the five CHANGEME entries below.   #
##########################################################################

# To compile other examples, either changed the following line, or
# add the argument DRIVER=filename_without_extension to make
DRIVER = cgl1

# CHANGEME: This should be the name of your executable
EXE = $(DRIVER)

# CHANGEME: Here is the name of all object files corresponding to the source
#           code that you wrote in order to define the problem statement
OBJS =  $(DRIVER).o

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
#  package.                                                              #
##########################################################################

COIN_HAS_PKGCONFIG = TRUE
COIN_CXX_IS_CL = #TRUE
COIN_HAS_SAMPLE = TRUE

# C++ Compiler command
CXX = /usr/bin/g++

# C++ Compiler options
CXXFLAGS = -g3 -gdwarf-2   -DCGL_BUILD

# additional C++ Compiler options for linking
CXXLINKFLAGS =  -Wl,--rpath -Wl,/home/raffa/Documents/discOpt/CoinMP/lib

# Sample data directory
ifeq ($(COIN_HAS_SAMPLE), TRUE)
  ifeq ($(COIN_HAS_PKGCONFIG), TRUE)
    CXXFLAGS += -DSAMPLEDIR=\"`PKG_CONFIG_PATH=/home/raffa/Documents/discOpt/CoinMP/lib64/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/lib/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/share/pkgconfig: pkg-config --variable=datadir coindatasample`\"
  else
    CXXFLAGS += -DSAMPLEDIR=\"\"
  endif
endif

# Include directories (we use the CYGPATH_W variables to allow compilation with Windows compilers)
ifeq ($(COIN_HAS_PKGCONFIG), TRUE)
  INCL = `PKG_CONFIG_PATH=/home/raffa/Documents/discOpt/CoinMP/lib64/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/lib/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/share/pkgconfig: pkg-config --cflags cgl`
else
  INCL = 
endif
INCL += $(ADDINCFLAGS)

# Linker flags
ifeq ($(COIN_HAS_PKGCONFIG), TRUE)
  LIBS = `PKG_CONFIG_PATH=/home/raffa/Documents/discOpt/CoinMP/lib64/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/lib/pkgconfig:/home/raffa/Documents/discOpt/CoinMP/share/pkgconfig: pkg-config --libs cgl`
else
  ifeq ($(COIN_CXX_IS_CL), TRUE)
    LIBS = -link -libpath:`$(CYGPATH_W) /home/raffa/Documents/discOpt/CoinMP/lib` libCgl.lib 
  else
    LIBS = -L/home/raffa/Documents/discOpt/CoinMP/lib -lCgl 
  endif
endif

# The following is necessary under cygwin, if native compilers are used
CYGPATH_W = echo

all: $(EXE)

.SUFFIXES: .cpp .c .o .obj

$(EXE): $(OBJS)
	bla=;\
	for file in $(OBJS); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CXX) $(CXXLINKFLAGS) $(CXXFLAGS) -o $@ $$bla $(ADDLIBS) $(LIBS)

clean:
	rm -rf $(EXE) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCL) -c -o $@ `test -f '$<' || echo '$(SRCDIR)/'`$<


.cpp.obj:
	$(CXX) $(CXXFLAGS) $(INCL) -c -o $@ `if test -f '$<'; then $(CYGPATH_W) '$<'; else $(CYGPATH_W) '$(SRCDIR)/$<'; fi`

.c.o:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ `test -f '$<' || echo '$(SRCDIR)/'`$<


.c.obj:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ `if test -f '$<'; then $(CYGPATH_W) '$<'; else $(CYGPATH_W) '$(SRCDIR)/$<'; fi`
