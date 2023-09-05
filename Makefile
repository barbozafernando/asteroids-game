run:
	gcc -Wall -std=c99 ./*.c -lSDL2 -o asteroids && ./asteroids

clean:
	rm asteroids
