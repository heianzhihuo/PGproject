#-------------------------------------------------------------------------
#
# Makefile.inc--
#    Makefile for bin/psql
#
# Copyright (c) 1994, Regents of the University of California
#
#
# IDENTIFICATION
#    $Header: /usr/local/cvsroot/postgres95/src/bin/psql/Makefile,v 1.19 1997/01/25 23:53:59 scrappy Exp $
#
#-------------------------------------------------------------------------

SRCDIR= ../..
include ../../Makefile.global

INCLUDE_OPT:= \
               -I$(LIBPQDIR) \
               -I../../include 

CFLAGS+= $(INCLUDE_OPT)

#USE_READLINE is set in Makefile.global
# 
ifeq ($(USE_READLINE), true)
   CFLAGS+= $(READLINE_INC) 

   LD_ADD+= $(READLINE_LIB) $(CURSES_LIB)

   ifeq ($(PORTNAME), ultrix4)
    LD_ADD+= -ltermcap
   else
     ifeq ($(PORTNAME), sparc)
       LD_ADD+= -ltermcap
     else
       ifeq ($(PORTNAME), linux)
         LD_ADD+= -ltermcap
       else
         ifeq ($(PORTNAME), next)
           LD_ADD+= -ltermcap
         else
           ifeq ($(PORTNAME), bsdi)
             LD_ADD+= -ltermcap
           else
             ifeq ($(PORTNAME), BSD44_derived)
               LD_ADD+= -ltermcap
             endif
           endif
         endif
       endif
     endif
   endif
endif

OBJS= psql.o stringutils.o  

ifeq ($(PORTNAME), ultrix4)
OBJS+= ../../utils/strdup.o
endif

all: submake psql

psql: $(OBJS) $(LIBPQDIR)/libpq.a
	$(CC) $(LDFLAGS) -o psql -L$(LIBPQDIR) $(OBJS) -lpq $(LD_ADD)

../../utils/strdup.o:
	$(MAKE) -C ../../utils strdup.o

.PHONY: submake
submake:
	$(MAKE) -C $(LIBPQDIR) libpq.a

install: psql
	$(INSTALL) $(INSTL_EXE_OPTS) psql $(DESTDIR)$(BINDIR)/psql

depend dep:
	$(CC) -MM $(INCLUDE_OPT) *.c >depend

clean: 
	rm -f psql $(OBJS) 

ifeq (depend,$(wildcard depend))
include depend
endif



