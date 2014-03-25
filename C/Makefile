SRC=MD_access.c control.c util.c 
OBJ=$(SRC:.c=.o)
CC=pgcc
CFLAGS= -O3 -fast -Mipa=fast,inline  -Msafeptr -Mvect=altcode,assoc,fuse,gather,idiom,prefetch,sse,tile,uniform #-Minfo=all -Mneginfo=all

#CFLAGS = -O0 


all: MD

MD: $(OBJ)
	$(CC) $(CFLAGS)  -o $@  $(OBJ) -lm


output.dat: MD input.dat
	./MD


clean:
	rm -f MD $(OBJ) 

$(OBJ) : coord.h Makefile


