UNAME := $(shell uname -s)
CC = g++
OBJ_COMMON = kuma.o main.o map.o
OBJ = game.o $(OBJ_COMMON)
OBJ_HELL = game_hell.o $(OBJ_COMMON)
LDFLAGS = -lGL -lGLU -lglut

# Mac OS
ifeq ($(UNAME), Darwin)
	LDFLAGS = -framework OpenGL -framework glut
endif

all : teddy_block teddy_block_hell

teddy_block : $(OBJ)
	$(CC) -o teddy_block $(OBJ) $(LDFLAGS)

teddy_block_hell : $(OBJ_HELL)
	$(CC) -o teddy_block_hell $(OBJ_HELL) $(LDFLAGS)

game.o : game.cpp game.h map.h kuma.h
	$(CC) -c -g game.cpp

game_hell.o : game.cpp game.h map.h kuma.h
	$(CC) -o game_hell.o -c game.cpp -DSTART_FROM_HELL

kuma.o : kuma.cpp kuma.h game.h map.h
	$(CC) -c -g kuma.cpp

main.o : main.cpp game.h map.h kuma.h
	$(CC) -c -g main.cpp

map.o : map.cpp map.h
	$(CC) -c -g map.cpp

clean :
	rm $(OBJ) game_hell.o
