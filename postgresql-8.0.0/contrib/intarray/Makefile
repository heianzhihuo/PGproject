# $PostgreSQL: pgsql/contrib/intarray/Makefile,v 1.11 2004/08/20 20:13:04 momjian Exp $

MODULE_big = _int
OBJS = _int_bool.o _int_gist.o _int_op.o _int_tool.o _intbig_gist.o 
DATA_built = _int.sql
DOCS = README.intarray
REGRESS = _int

ifdef USE_PGXS
PGXS = $(shell pg_config --pgxs)
include $(PGXS)
else
subdir = contrib/intarray
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
