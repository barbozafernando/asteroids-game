#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_game_running = false;
SDL_Point vertices[] = { {30,50}, {50,30}, {60,50}, {50,30}};

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type) {
    case SDL_QUIT:
      is_game_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        is_game_running = false;
      break;
    default:
      break;
  }
}

void setup(void) {};

void update(void) {
  for (int i = 0; i < 4; i++) {
    vertices[i].x += 1;
    vertices[i].y += 1;
  }
  SDL_Delay(1);
};

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  // SDL_RenderDrawLine(renderer, 400, 100, 200, 400);
  // SDL_RenderDrawLine(renderer, 200, 400, 600, 400);
  // SDL_RenderDrawLine(renderer, 600, 400, 400, 100);

  // SDL_RenderDrawLine(renderer, 30, 50, 50, 30);
  // SDL_RenderDrawLine(renderer, 60, 50, 50, 30);
  // SDL_RenderDrawLine(renderer, 160, 10, 50, 230);
  uint8_t vertices_number = sizeof(vertices)/sizeof(vertices[0]);
  SDL_RenderDrawLines(renderer, vertices, vertices_number);

  // Buffer swap
  SDL_RenderPresent(renderer);
};

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_GAME) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  window = SDL_CreateWindow(
    "Asteroids",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (!window) {
    fprintf(stderr, "Error creating  SDL window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating  SDL renderer.\n");
    return false;
  }

  return true;
}

void destroy_window(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main() {
  is_game_running = initialize_window();

  setup();

  while(is_game_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  
  return EXIT_SUCCESS;
}
