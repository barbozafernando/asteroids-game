#include "constants.h"
#include "types.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

///////////////////////////////////////////////////////////////////////////////
// Declare two game objects for the ball and the paddle
///////////////////////////////////////////////////////////////////////////////
struct GameObject {
  f32 x;
  f32 y;
  f32 width;
  f32 height;
  f32 vel_x;
  f32 vel_y;
} ball, paddle;
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
      SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
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

  // Initialize the ball object moving down at a constant velocity
  ball.x = (WINDOW_WIDTH / 2.0);
  ball.y = (WINDOW_HEIGHT / 2.0);
  ball.width = 20;
  ball.height = 20;
  ball.vel_x = 80;
  ball.vel_y = 80;

  return ball;
}

///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
  // Get delta_time factor converted to seconds to be used to update objects
  f32 delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  // Store the milliseconds of the current frame to be used in the next one
  last_frame_time = SDL_GetTicks();

  // Move ball as a function of delta time
  // ball.x += ball.vel_x * delta_time;
  // ball.y += ball.vel_y * delta_time;

  // Check for ball collision with the window borders
  if (ball.x < 0) {
    ball.x = 0;
    ball.vel_x = -ball.vel_x;
  }
  if (ball.x + ball.height > WINDOW_WIDTH) {
    ball.x = WINDOW_WIDTH - ball.width;
    ball.vel_x = -ball.vel_x;
  }
  if (ball.y < 0) {
    ball.y = 0;
    ball.vel_y = -ball.vel_y;
  }
  if (ball.y + ball.height > WINDOW_HEIGHT) {
    ball.y = WINDOW_HEIGHT - ball.height;
    ball.vel_y = -ball.vel_y;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw a rectangle for the ball object
  SDL_Rect ball_rect = {(int)ball.x, (int)ball.y, (int)ball.width,
                        (int)ball.height};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);

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
void process_input(GameObject *go) {
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
        ball.x += ball.vel_x * -.3;
      }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        ball.x += ball.vel_x * .3;
      }
      if (event.key.keysym.sym == SDLK_UP) {
        ball.y += ball.vel_y * -.3;
      }
      if (event.key.keysym.sym == SDLK_DOWN) {
        ball.y += ball.vel_y * .3;
      }
      break;
    }
  }
  render();
}

int main(int argc, char **args) {
  game_is_running = initialize_window();

  GameObject go = setup();

  while (game_is_running) {
    process_input(&go);
    update();
    render();
  }

  destroy_window();

  return 0;
}
