CC          = gcc
CLINKER     = gcc

CFLAGS      =   -Wall -O4 -g 
LIBS        = -lm
DEPEND= makedepend

SRC        = distribution.c ran_uniform.c
OBJS       = distribution.o ran_uniform.o
EXECS      = distribution

default: distribution

all: $(EXECS)

distribution:$(OBJS)
	$(CLINKER) $(OPTFLAGS) -o distribution $(OBJS) $(LIBS)

clean:
	/bin/rm -f *.o *~ $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
