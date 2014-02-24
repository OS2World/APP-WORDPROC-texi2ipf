
# Makefile for Texinfo to IPF translator (texi2ipf)
# Tested with emx/gcc 0.8f

CC = gcc
CFLAGS = -Wall -Zomf -Zsys -O2

.c.obj:
        $(CC) -c $(CFLAGS) $<

OBJECTS = texi2ipf.obj table.obj translat.obj items.obj

texi2ipf.exe: $(OBJECTS)
       $(CC) $(CFLAGS) -o $@ $(OBJECTS) texi2ipf.def

texi2ipf.obj : texi2ipf.h

translat.obj : texi2ipf.h

table.obj : texi2ipf.h table.h

items.obj: texi2ipf.h