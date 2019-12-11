PROGRAM = 3D_Tetris
SOURCES	= $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
CC      = gcc
CFLAGS  = -g -Wall -Wextra 
# -I/usr/X11R6/include -I/usr/pkg/include
#LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS = -lglut -lGLU -lGL -lm

$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	-rm src/*.o $(PROGRAM)

