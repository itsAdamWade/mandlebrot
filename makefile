CC = gcc

CFLAGS = \
	-ggdb \
	-std=c89 \
	-pedantic \
	-pedantic-errors \ 
	-Werror \
	-Wextra \
	-Wmissing-prototypes \
	-Wall \
	-Wold-style-definition \
	-Wdeclaration-after-statement \
	-Wundef \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wfloat-equal \
	-Wno-missing-braces \
	-Wincompatible-pointer-types
	
LDFLAGS=
	
.PHONY: all clean fractals
	
all: fractals
	
fractals:
	$(CC) mandel.c $(CFLAGS) $(LDFLAGS) -o bin\mandel.c

	
clean:
	$(RM) *.o
	$(RM) bin\fractals