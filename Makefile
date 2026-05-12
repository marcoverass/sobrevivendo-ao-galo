CC = gcc
TARGET = sobrevivendo_ao_galo
SRC = src/main.c src/game.c src/menu.c src/player.c src/npc_list.c src/ranking.c
RAYLIB_PATH = C:/raylib/w64devkit
INCLUDES = -Iinclude -I"$(RAYLIB_PATH)/include"
LIBS = -L"$(RAYLIB_PATH)/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

all:
	$(CC) $(SRC) -o $(TARGET) $(INCLUDES) $(LIBS)

run: all
	./$(TARGET)

clean:
	del /Q $(TARGET).exe 2>nul || exit 0
