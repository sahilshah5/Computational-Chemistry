CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC     = barrier1.c integrate_and.c integrate_nve.c mdloop.c sample_diff.c system.c force.c integrate_mc.c integrate_res.c integrate_ber.c readdat.c sample_prof.c ran_uniform.c
OBJS    = barrier1.o integrate_and.o integrate_nve.o mdloop.o sample_diff.o system.o force.o integrate_mc.o integrate_res.o integrate_ber.o readdat.o sample_prof.o ran_uniform.o
EXECS   = barrier1

default: barrier1

all: $(EXECS)

barrier1:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o barrier1 $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

barrier1.o: system.h 
integrate_and.o: system.h 
integrate_nve.o: system.h 
integrate_ber.o: system.h
mdloop.o: system.h 
sample_diff.o: system.h 
system.o: system.h 
force.o: system.h 
integrate_mc.o: system.h 
integrate_res.o: system.h 
readdat.o: system.h 
sample_prof.o: system.h
