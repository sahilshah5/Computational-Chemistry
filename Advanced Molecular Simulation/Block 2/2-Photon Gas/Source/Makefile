CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC        = photon.c ran_uniform.c
OBJS       = photon.o ran_uniform.o
EXECS      = photon

default: photon

all: $(EXECS)

photon:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o photon $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
