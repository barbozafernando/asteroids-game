run:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o asteroids && ./asteroids

clean:
	rm asteroids
