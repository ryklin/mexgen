# Makefile for mexgen mex-file generator
#
# Copyright (C) 2003-2012
# Center for Perceptual Systems
# University of Texas at Austin
#
# jsp Wed Oct 31 15:32:35 CDT 2012

.PHONY: all check clean release

CPP=g++
CPFLAGS=-g -pg # Profile
CRFLAGS=-Wall -O3 -DNDEBUG # Release
CDFLAGS=-Wall -g # Debug
CFLAGS=$(CDFLAGS)

all: mexgen

mexgen: mexgen.tab.cpp mexgen.yy.cpp
	$(CPP) $(CFLAGS) -o mexgen -Wall -Ic:/users/jsp/projects mexgen.tab.cpp mexgen.yy.cpp

mexgen.tab.cpp: mexgen.ypp mexgen.h
	bison --defines=mexgen.tab.h mexgen.ypp

mexgen.yy.cpp: mexgen.lex
	flex -omexgen.yy.cpp mexgen.lex

clean:
	rm -f *.o *.stackdump $(TARGETS) .deps *.exe
	rm -f mexgen mexgen.exe mexgen.yy.cpp mexgen.tab.cpp mexgen.tab.h
	rm -f abchandlers.cpp # output of mexgen -c abc.mg
	rm -f abcentry.cpp # output of mexgen -e abc.mg
	rm -f abcclamp.m abcdonothing.m # output of mexgen -m abc.mg
	rm -f xyzhandlers.cpp # output of mexgen -c xyz.mg
	rm -f xyzentry.cpp # output of mexgen -e xyz.mg
	rm -f xyzinit.m xyzfunction?.m # output of mexgen -m xyz.mg
	rm -f abcentry.* xyzentry.* # other cleanup

check: all
	$(MAKE) -f./Makefile.check EXE='./mexgen' CHECKDIR='.'
	# Run matlab in batch mode
	matlab -nojvm -nosplash -r "mgtest;exit;"
	./mexgen -v

release:
	$(MAKE) CFLAGS="$(CRFLAGS)"
