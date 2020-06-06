CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC     = mc_npt.c system.c writepdb.c ran_uniform.c
OBJS    = mc_npt.o system.o writepdb.o ran_uniform.o
EXECS   = mc_npt

default: mc_npt

all: $(EXECS)

mc_npt:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o mc_npt $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

mc-npt.o: system.h
system.o: system.h 
writepdb.o: system.h
