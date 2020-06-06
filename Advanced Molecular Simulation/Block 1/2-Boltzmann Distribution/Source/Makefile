CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC        = boltzmann.c
OBJS       = boltzmann.o
EXECS      = boltzmann

default: boltzmann

all: $(EXECS)

boltzmann:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o boltzmann $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
