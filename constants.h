#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SCOREBOARD_WIDTH 100
#define SCOREBOARD_HEIGHT 50

#define PLAYER_SPEED 80

#define TARGET_SIZE 20
#define PLAYER_SIZE 20

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#define SDL_INIT_GAME                                                          \
  SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS

#endif
