MODULES = tablefunc
DATA_built = tablefunc.sql
DOCS = README.tablefunc
REGRESS = tablefunc

ifdef USE_PGXS
PGXS = $(shell pg_config --pgxs)
include $(PGXS)
else
subdir = contrib/tablefunc
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
