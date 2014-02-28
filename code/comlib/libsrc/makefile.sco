#                                                                   
# makefile for general foundation extension library
#
#
.SUFFIXES:.c

HOMEDIR=/u/zyx/code
COMDIR=$(HOMEDIR)/comlib
INCDIR=-I$(COMDIR)/include 
    
CC=cc
CFLAG=-O -DUNIX -DLIB -DDEBUG -DTRACE_FILE='"./trace"' 
EDF=

help: 
	@echo Please input filename that will be made 
	@echo 'syntax: make <help|all|"filename">'

all: config debug file msg pid sem shm str tcp time udp

.c:
	if [ -f $(COMDIR)/lib/libcom.a ]; then ar -x $(COMDIR)/lib/libcom.a; fi
	$(CC) -c $< $(CFLAG) $(INCDIR)
	ar -rv libcom.a *.o
	rm *.o
	mv libcom.a $(COMDIR)/lib
