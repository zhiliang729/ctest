.SUFFIXES:.c
HOMEDIR=/home/zyx/code
COMDIR=$(HOMEDIR)/comlib
INCDIRS=-I$(COMDIR)/include 
LIBDIRS=-L$(COMDIR)/lib 
LIB=  -lcom 
CC=cc
CFLAGS=-O -DUNIX -DDEBUG -DTRACE_FILE='"./trace"' -D__PATH__='"$(PWD)/"'
EDF=
.c:
	$(CC) -o $@ $< $(CFLAGS) $(INCDIRS) $(LIBDIRS) $(LIB)



