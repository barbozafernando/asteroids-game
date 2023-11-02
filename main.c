#include "constants.h"
#include "types.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

///////////////////////////////////////////////////////////////////////////////
// Declare two game objects for the player and the target
///////////////////////////////////////////////////////////////////////////////
struct GameObject {
  f32 x;
  f32 y;
  f32 width;
  f32 height;
  f32 vel_x;
  f32 vel_y;
} player, target;
typedef struct GameObject GameObject;

///////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }
  window =
      SDL_CreateWindow("Square Eater", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function that runs once at the beginning of our program
///////////////////////////////////////////////////////////////////////////////
GameObject setup(void) {
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);

  // Initialize the player object
  player.x = (WINDOW_WIDTH >> 1);
  player.y = (WINDOW_HEIGHT >> 1);
  player.width = 20;
  player.height = 20;
  player.vel_x = 80;
  player.vel_y = 80;

  srand(time(NULL));

  // Initialize the target object
  target.x = rand() % (WINDOW_WIDTH + 1);
  target.y = rand() % (WINDOW_HEIGHT + 1);
  target.width = 10;
  target.height = 10;
  target.vel_x = 0;
  target.vel_y = 0;

  return player;
}

///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
  // Get delta_time factor converted to seconds to be used to update objects
  // f32 delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  // Store the milliseconds of the current frame to be used in the next one
  // last_frame_time = SDL_GetTicks();

  // Move player as a function of delta time
  // player.x += player.vel_x * delta_time;
  // player.y += player.vel_y * delta_time;

  // Check for player collision with the window borders
  if (player.x < 0) {
    player.x = 0;
    player.vel_x = 0;
  }
  if (player.x + player.height > WINDOW_WIDTH) {
    player.x = WINDOW_WIDTH - player.width;
    player.vel_x = 0;
  }
  if (player.y < 0) {
    player.y = 0;
    player.vel_y = 0;
  }
  if (player.y + player.height > WINDOW_HEIGHT) {
    player.y = WINDOW_HEIGHT - player.height;
    player.vel_y = 0;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw a rectangle for the player object
  SDL_Rect player_rect = {(u16)player.x, (u16)player.y, (u16)player.width,
                        (u16)player.height};

  SDL_Rect target_rect = {(u16)target.x, (u16)target.y, (u16)target.width,
                          (u16)target.height};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &player_rect);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &target_rect);

  SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Function to destroy SDL window and renderer
///////////////////////////////////////////////////////////////////////////////
void destroy_window(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// Function to poll SDL events and process keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(GameObject *game_obj) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        game_is_running = false;
      }
      if (event.key.keysym.sym == SDLK_LEFT) {
        if (player.vel_x == 0){ 
          player.vel_x = 80;
        }
        player.x += player.vel_x * -PLAYER_SPEED;
      }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        if (player.vel_x == 0){ 
          player.vel_x = 80;
        }
        player.x += player.vel_x * PLAYER_SPEED;
      }
      if (event.key.keysym.sym == SDLK_UP) {
        if (player.vel_y == 0){ 
          player.vel_y = 80;
        }
        player.y += player.vel_y * -PLAYER_SPEED;
      }
      if (event.key.keysym.sym == SDLK_DOWN) {
        if (player.vel_y == 0){ 
          player.vel_y = 80;
        }
        player.y += player.vel_y * PLAYER_SPEED;
      }
      break;
    }
  }
}

int main(int argc, char **args) {
  game_is_running = initialize_window();

  GameObject game_obj = setup();

  while (game_is_running) {
    process_input(&game_obj);
    update();
    render();
  }

  destroy_window();

  return 0;
}
