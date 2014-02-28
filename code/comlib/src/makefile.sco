#
# makefile common part for application preconsole's program
#
#
.SUFFIXES:.c

HOMEDIR=/u/zyx/code
COMDIR=$(HOMEDIR)/comlib
INCDIRS=-I$(COMDIR)/include 
LIBDIRS=-L$(COMDIR)/lib 

LIB=  -lcom -lsocket

CC=cc
CFLAGS=-O -DUNIX -DDEBUG -DTRACE_FILE='"./trace"' -D__PATH__='"$(PWD)/"'

EDF=

.c:
	$(CC) -o $@ $< $(CFLAGS) $(INCDIRS) $(LIBDIRS) $(LIB)
	mv $@ $(COMDIR)/bin
	if [ -f $*.o ]; then rm $*.o; fi



