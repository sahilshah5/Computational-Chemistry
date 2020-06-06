CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC    = random1d.c ran_uniform.c sample.c
OBJS   = random1d.o ran_uniform.o sample.o
EXECS  = random1d

default: random1d

all: $(EXECS)

random1d:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o random1d $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
