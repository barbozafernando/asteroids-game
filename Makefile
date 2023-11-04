run:
	gcc -Wall -std=c99 ./*.c -lSDL2 -lSDL2_ttf -o asteroids && ./asteroids

run-win:
	x86_64-w64-mingw32-gcc -o asteroids.exe main.c `sdl2-config --cflags --libs` -I/C:/SDL2/SDL2-2.28.5/x86_64-w64-mingw32/include -LC:/SDL2/SDL2-2.28.5/x86_64-w64-mingw32/lib 

clean:
	rm asteroids
