.SUFFIXES:.c
HOMEDIR=/u/zyx/code
COMDIR=$(HOMEDIR)/comlib
INCDIRS=-I$(COMDIR)/include 
LIBDIRS=-L$(COMDIR)/lib 
LIB=  -lcom -lsocket
CC=cc
CFLAGS=-O -KPIC -G -DUNIX -DDEBUG -DTRACE_FILE='"./trace"' -D__PATH__='"$(PWD)/"'
EDF=
.c:
	$(CC) -o $@.so $< $(CFLAGS) $(INCDIRS) $(LIBDIRS) $(LIB)



