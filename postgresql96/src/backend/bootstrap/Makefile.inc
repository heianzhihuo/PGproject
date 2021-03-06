#-------------------------------------------------------------------------
#
# Makefile.inc--
#    Makefile for the bootstrap module
#
# Copyright (c) 1994, Regents of the University of California
#
#
# IDENTIFICATION
#    $Header: /usr/local/cvsroot/postgres95/src/backend/bootstrap/Makefile.inc,v 1.1.1.1 1996/07/09 06:21:14 scrappy Exp $
#
#
# Another kinda weird Makefile.inc cause we need two
#  scanner/parsers in the backend and most yaccs and lexs
#  don't have the prefix option.
#
#	sed files are HACK CITY! - redo...
#
#-------------------------------------------------------------------------

bootdir= $(CURDIR)/bootstrap
VPATH:= $(VPATH):$(bootdir)

#BOOTYACCS= bootstrap_tokens.h bootparse.c
BOOTYACCS= bootparse.c

SRCS_BOOTSTRAP= bootparse.c bootscanner.c bootstrap.c 

$(BOOTYACCS): bootparse.y
	cd $(objdir); \
	$(YACC) $(YFLAGS) $<; \
	sed -f $(bootdir)/boot.sed < y.tab.c > bootparse.c; \
	mv y.tab.h bootstrap_tokens.h; \
	rm -f y.tab.c

$(objdir)/bootparse.o: bootparse.c
	$(cc_inobjdir)


bootscanner.c: bootscanner.l
	cd $(objdir); \
	$(LEX) $<; \
	sed -f $(bootdir)/boot.sed < lex.yy.c > bootscanner.c; \
	rm -f lex.yy.c

$(objdir)/bootscanner.o: bootscanner.c
	$(cc_inobjdir)



#
# The following insures that y.tab.h gets made as bootstrap.c
# includes it
#
bootstrap.o: $(BOOTYACCS)

POSTGRES_DEPEND+= $(BOOTYACCS) bootscanner.c


CLEANFILES+= bootscanner.c $(BOOTYACCS) y.tab.h y.output

HEADERS+= bootstrap.h

