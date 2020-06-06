CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC     = energy.c pt.c sample_energy.c sample_xcoord.c system.c ran_uniform.c
OBJS    = energy.o pt.o sample_energy.o sample_xcoord.o system.o ran_uniform.o

default: pt

all: $(EXECS)

pt:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o pt $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

energy.o: system.h
pt.o: system.h
sample_energy.o: system.h
sample_xcoord.o: system.h
system.o: system.h
