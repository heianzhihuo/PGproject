#-------------------------------------------------------------------------
#
# Makefile.inc--
#    Makefile for storage/ipc
#
# Copyright (c) 1994, Regents of the University of California
#
#
# IDENTIFICATION
#    $Header: /usr/local/cvsroot/postgres95/src/backend/storage/ipc/Makefile.inc,v 1.1.1.1 1996/07/09 06:21:54 scrappy Exp $
#
#-------------------------------------------------------------------------

SUBSRCS+= ipc.c ipci.c s_lock.c shmem.c shmqueue.c sinval.c \
	sinvaladt.c spin.c
