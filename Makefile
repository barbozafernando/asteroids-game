CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -g -lSDL2 -lSDL2_ttf -lSDL2_image 
CFLAGS_WIN=sdl2-config --cflags --libs
SRC=main.c
TARGET=asteroids

run:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) 

run-win:
	x86_64-w64-mingw32-gcc -o $(TARGET).exe $(SRC) `$(CFLAGS_WIN)` -I/C:/SDL2/SDL2-2.28.5/x86_64-w64-mingw32/include -LC:/SDL2/SDL2-2.28.5/x86_64-w64-mingw32/lib 

clean:
	rm $(TARGET)
