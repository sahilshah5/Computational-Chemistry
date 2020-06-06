CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC        = harmonic.c ran_uniform.c
OBJS       = harmonic.o ran_uniform.o
EXECS      = harmonic

default: harmonic

all: $(EXECS)

harmonic:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o harmonic $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
