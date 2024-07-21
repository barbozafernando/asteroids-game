#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

#include "constants.h"

typedef struct {
  float x;
  float y;
  float width;
  float height;
  float vel_x;
  float vel_y;
} Player, Target;

typedef struct {
  int           game_is_running;
  int           last_frame_time;
  uint8_t       score;
  TTF_Font*     font;
  SDL_Window*   window;
  SDL_Renderer* renderer;
  Player        player;
  Player        target;
} Game;

float calculate_fps(uint32_t startTick, uint32_t endTick) {
    float elapsedTime = (endTick - startTick) / 1000.0f;
    return elapsedTime > 0 ? 1.0f / elapsedTime : 0;
}

void draw_fps(Game* game, float fps) {
  const char FONT_PATH[] = "./assets/Roboto-Regular.ttf";
  SDL_Color text_color = {255, 255, 255, 255};

  game->font = TTF_OpenFont(FONT_PATH, 16);

  if (game->font == NULL) {
    fprintf(stderr, "Font was not loaded: %s", TTF_GetError());
  }

  // Create a text buffer for the score
  char score_text[255];

  // Convert the score integer to a string
  snprintf(score_text, sizeof(score_text), "FPS: %.2f", fps);

  SDL_Surface *text_surface =
      TTF_RenderText_Blended(game->font, score_text, text_color);
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(game->renderer, text_surface);

  SDL_Rect fps_counter = {(WINDOW_WIDTH - 100), 0, FPS_COUNTER_WIDTH, FPS_COUNTER_HEIGHT};

  SDL_RenderCopy(game->renderer, text_texture, NULL, &fps_counter);

  SDL_RenderPresent(game->renderer);

  SDL_FreeSurface(text_surface);
  SDL_DestroyTexture(text_texture);

  TTF_CloseFont(game->font);
}

void draw_scoreboard(Game* game) {
  const char FONT_PATH[] = "./assets/Roboto-Regular.ttf";
  SDL_Color text_color = {255, 255, 255, 255};

  game->font = TTF_OpenFont(FONT_PATH, 36);

  if (game->font == NULL) {
    fprintf(stderr, "Font was not loaded: %s", TTF_GetError());
  }

  // Create a text buffer for the score
  char score_text[255];

  // Convert the score integer to a string
  snprintf(score_text, sizeof(score_text), "SCORE: %d", game->score);

  SDL_Surface *text_surface =
      TTF_RenderText_Solid(game->font, score_text, text_color);
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(game->renderer, text_surface);

  SDL_Rect scoreboard_rect = {0, 0, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT};

  SDL_RenderCopy(game->renderer, text_texture, NULL, &scoreboard_rect);

  SDL_RenderPresent(game->renderer);

  SDL_FreeSurface(text_surface);
  SDL_DestroyTexture(text_texture);

  TTF_CloseFont(game->font);
}

int initialize_window(Game* game) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  if (TTF_Init() == -1) {
    fprintf(stderr, "Error initializing TTF.\n");
  }

  game->window =
      SDL_CreateWindow("Square Eater", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (!game->window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return false;
  }

  game->renderer = SDL_CreateRenderer(game->window, -1, 0);

  if (!game->renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return false;
  }

  return true;
}

void spawn_target(Game* game) {
  game->target.x = (uint16_t)rand() % (WINDOW_WIDTH - TARGET_SIZE + 1);
  game->target.y = (uint16_t)rand() % (WINDOW_HEIGHT - TARGET_SIZE + 1);
}

void setup(Game* game) {
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);

  // Initialize the player object
  game->player.x = (WINDOW_WIDTH >> 1);
  game->player.y = (WINDOW_HEIGHT >> 1);
  game->player.width = PLAYER_SIZE;
  game->player.height = PLAYER_SIZE;
  game->player.vel_x = PLAYER_SPEED;
  game->player.vel_y = PLAYER_SPEED;

  srand(time(NULL));

  // Initialize the target object
  game->target.x = rand() % (WINDOW_WIDTH + 1);
  game->target.y = rand() % (WINDOW_HEIGHT + 1);
  game->target.width = TARGET_SIZE;
  game->target.height = TARGET_SIZE;
  game->target.vel_x = 0;
  game->target.vel_y = 0;
}

void update(Game* game) {
  if (game->player.x + game->player.width > WINDOW_WIDTH) {
    game->player.x = 0;
  }

  if (game->player.y + game->player.height > WINDOW_HEIGHT) {
    game->player.y = 0;
  }

  if ((game->player.x + game->player.width >= game->target.x) &&
      (game->target.x + game->target.width >= game->player.x) &&
      (game->player.y + game->player.height >= game->target.y) &&
      (game->target.y + game->target.height >= game->player.y)) {

    game->score++;
    spawn_target(game);
  }
}

void render(Game* game) {
  SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
  SDL_RenderClear(game->renderer);

  // Draw a rectangle for the player object
  SDL_Rect player_rect = {(uint16_t)game->player.x, (uint16_t)game->player.y, (uint16_t)game->player.width,
                          (uint16_t)game->player.height};

  SDL_Rect target_rect = {(uint16_t)game->target.x, (uint16_t)game->target.y, (uint16_t)game->target.width,
                          (uint16_t)game->target.height};

  SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(game->renderer, &player_rect);
  SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(game->renderer, &target_rect);

  draw_scoreboard(game);
}

void process_input(Game* game) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      game->game_is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        game->game_is_running = false;
      }
      if (event.key.keysym.sym == SDLK_LEFT) {
        if (game->player.vel_x == 0) {
          game->player.vel_x = PLAYER_SPEED;
        }
        game->player.x -= game->player.vel_x * .3f;
      }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        game->player.x += game->player.vel_x * .3f;
      }
      if (event.key.keysym.sym == SDLK_UP) {
        game->player.vel_y = PLAYER_SPEED;
        game->player.y += game->player.vel_y * -.3f;
      }
      if (event.key.keysym.sym == SDLK_DOWN) {
        game->player.y += game->player.vel_y * .3f;
      }
      break;
    }
  }
}

int main() {
  static Game   game_instance;
  static Player player;
  static Target target;

  game_instance.game_is_running = initialize_window(&game_instance);
  game_instance.last_frame_time = 0;
  game_instance.score           = 0;
  game_instance.player          = player;
  game_instance.target          = target;

  float fps;
  uint32_t start_tick, end_tick;

  setup(&game_instance);

  while (game_instance.game_is_running) {
    start_tick = SDL_GetTicks();

    process_input(&game_instance);
    update(&game_instance);
    render(&game_instance);

    end_tick = SDL_GetTicks();
    fps = calculate_fps(start_tick, end_tick);
    draw_fps(&game_instance, fps);
    SDL_Delay(16);
  }

  TTF_Quit();
  SDL_DestroyRenderer(game_instance.renderer);
  SDL_DestroyWindow(game_instance.window);
  SDL_Quit();

  return 0;
}
