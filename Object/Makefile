CC = gcc

WARNINGS = -Wall -Wextra -W -pedantic

CFLAGS = -g -DGL_GLEXT_PROTOTYPES $(WARNINGS) `sdl-config --cflags` -std=c99

LIB = GL m assimp

LDFLAGS = $(addprefix -l, $(LIB)) `sdl-config --libs`

OBJ = 	object.o\
	SDL_util.o\
	GL_tools.o\
	vector_lib.o\
	adjacency.o\

EXE = object

.PHONY:	all clean

all:	$(EXE)

%.o:	%.c
	$(CC) $(CFLAGS) -c $? -o $@

object:	$(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm $(OBJ)

cleanall:
	rm $(OBJ) $(EXE)
