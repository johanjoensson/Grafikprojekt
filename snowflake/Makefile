CC = gcc

WARNINGS = -Wall -Wextra -W -pedantic

CFLAGS = -g -DGL_GLEXT_PROTOTYPES $(WARNINGS) `sdl-config --cflags` -std=c99

LIB = GL m assimp

LDFLAGS = $(addprefix -l, $(LIB)) `sdl-config --libs`

OBJ = 	snowflake.o\
	SDL_util.o\
	GL_tools.o\

EXE = snowflake

.PHONY:	all clean

all:	$(EXE)

%.o:	%.c
	$(CC) $(CFLAGS) -c $? -o $@

snowflake:	$(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

object:	object.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm $(OBJ)

cleanall:
	rm $(OBJ) $(EXE)
