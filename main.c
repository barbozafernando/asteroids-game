#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_game_running = false;
int last_frame_time = 0;
SDL_Point vertices[4] = { {30,50}, {50,30}, {60,50}, {50,30}};

void move_ship(int x, int y) {
  // Get a delta time factor converted to seconds to be used to update my objects later
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  last_frame_time = SDL_GetTicks();
  
  for (int i = 0; i < 4; i++) {
    vertices[i].x += x * delta_time;
    vertices[i].y += y * delta_time;
  }
}

void handle_input_event(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type) {
    case SDL_QUIT:
      is_game_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_game_running = false;
        break;
      }
      if (event.key.keysym.sym == SDLK_UP) {
        move_ship(10,10);
        break;
      }
      break;
    default:
      break;
  }
}

void setup(void) {};

void update(void) {
  // Get a delta time factor converted to seconds to be used to update my objects later
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();
  
  for (int i = 0; i < 4; i++) {
    vertices[i].x += 30 * delta_time;
    vertices[i].y += 30 * delta_time;
  }
};

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  int vertices_number = sizeof(vertices)/sizeof(vertices[0]);
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
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
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
    handle_input_event();
    update();
    render();
  }

  destroy_window();
  
  return EXIT_SUCCESS;
}
