#include "constants.h"
#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct GameObject {
  f32 x;
  f32 y;
  f32 width;
  f32 height;
  f32 vel_x;
  f32 vel_y;
  SDL_Texture *texture;
} player, target;
typedef struct GameObject GameObject;

int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
GameObject player = {0};
GameObject target = {0};
u8 score = 0;

SDL_Texture *load_texture(const char *filename) {
  SDL_Texture *texture;

  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loading %s", filename);

  texture = IMG_LoadTexture(renderer, filename);

  return texture;
}

void blit(SDL_Texture *texture, int x, int y) {
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;

  SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

  SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void draw_scoreboard(void) {
  const char *FONT_PATH = "./assets/Roboto-Regular.ttf";
  SDL_Color text_color = {255, 255, 255};

  TTF_Font *font = TTF_OpenFont(FONT_PATH, 36);

  if (font == NULL) {
    fprintf(stderr, "Font was not loaded: %s", TTF_GetError());
  }

  // Create a text buffer for the score
  char score_text[255];

  // Convert the score integer to a string
  snprintf(score_text, sizeof(score_text), "SCORE: %d", score);

  SDL_Surface *text_surface =
      TTF_RenderText_Solid(font, score_text, text_color);
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(renderer, text_surface);

  SDL_Rect scoreboard_rect = {0, 0, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT};

  SDL_RenderCopy(renderer, text_texture, NULL, &scoreboard_rect);

  SDL_RenderPresent(renderer);

  SDL_FreeSurface(text_surface);
  SDL_DestroyTexture(text_texture);

  TTF_CloseFont(font);
}

int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  if (TTF_Init() == -1) {
    fprintf(stderr, "Error initializing TTF.\n");
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

void spawn_target(void) {
  target.x = (u16)rand() % (WINDOW_WIDTH - TARGET_SIZE + 1);
  target.y = (u16)rand() % (WINDOW_HEIGHT - TARGET_SIZE + 1);
}

void setup(void) {
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);

  // Initialize the player object
  player.x = (WINDOW_WIDTH >> 1);
  player.y = (WINDOW_HEIGHT >> 1);
  player.width = PLAYER_SIZE;
  player.height = PLAYER_SIZE;
  player.vel_x = PLAYER_SPEED;
  player.vel_y = PLAYER_SPEED;
  player.texture = load_texture("assets/player_texture.png");

  srand(time(NULL));

  // Initialize the target object
  target.x = rand() % (WINDOW_WIDTH + 1);
  target.y = rand() % (WINDOW_HEIGHT + 1);
  target.width = TARGET_SIZE;
  target.height = TARGET_SIZE;
  target.vel_x = 0;
  target.vel_y = 0;
}

void update() {
  if (player.x < 0) {
    player.x = 0;
    player.vel_x = 0;
  }
  if (player.x + player.width > WINDOW_WIDTH) {
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

  if ((player.x + player.width >= target.x) &&
      (target.x + target.width >= player.x) &&
      (player.y + player.height >= target.y) &&
      (target.y + target.height >= player.y)) {

    score++;
    spawn_target();
  }
}

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
}

void destroy_window(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void process_input() {
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
        if (player.vel_x == 0) {
          player.vel_x = PLAYER_SPEED;
        }
        player.x += player.vel_x * -.3;
      }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        if (player.vel_x == 0) {
          player.vel_x = PLAYER_SPEED;
        }
        player.x += player.vel_x * .3;
      }
      if (event.key.keysym.sym == SDLK_UP) {
        if (player.vel_y == 0) {
          player.vel_y = PLAYER_SPEED;
        }
        player.y += player.vel_y * -.3;
      }
      if (event.key.keysym.sym == SDLK_DOWN) {
        if (player.vel_y == 0) {
          player.vel_y = PLAYER_SPEED;
        }
        player.y += player.vel_y * .3;
      }
      break;
    }
  }
}

int main(int argc, char **args) {
  game_is_running = initialize_window();

  setup();

  while (game_is_running) {
    process_input();
    update();
    render();
    blit(player.texture, player.x, player.y);
    draw_scoreboard();
  }

  TTF_Quit();

  destroy_window();

  return 0;
}
