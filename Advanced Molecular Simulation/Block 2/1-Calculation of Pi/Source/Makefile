CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC     = ran_uniform.c pi.c
OBJS    = ran_uniform.o pi.o
EXECS   = pi

default: pi

all: $(EXECS)

pi:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o pi $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
