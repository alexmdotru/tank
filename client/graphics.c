#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "graphics.h"
#include "map.h"
#include "animation.h"

// Colors
SDL_Color SOLOR_BLACK = {   0,   0,   0 };
SDL_Color COLOR_RED   = { 255,   0,   0 };
SDL_Color COLOR_GREEN = {   0, 255,   0 };
SDL_Color COLOR_BLUE  = {   0,   0, 255 };
SDL_Color COLOR_WHITE = { 255, 255, 255 };

texture_t *loadLabel(char *text, int size, SDL_Color color, SDL_Renderer *renderer);
void renderTexture(texture_t *texture, int x, int y, int sx, int sy, double rotation, SDL_Renderer *renderer);
SDL_Surface *pngToSurface(char *file);
texture_t *loadTexture(SDL_Surface *src, int x, int y, int szx, int szy, SDL_Renderer *renderer);

void loadFrame(graphics_t *graphics) {
  graphics->window = SDL_CreateWindow("TAHK-2015", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  graphics->renderer = SDL_CreateRenderer(graphics->window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void freeFrame(graphics_t *graphics) {
  SDL_DestroyRenderer(graphics->renderer);
  SDL_DestroyWindow(graphics->window);
}

void loadResources(client_t *client) {
  graphics_t *graphics = client->graphics;

  // Logo labels
  graphics->logo1 = loadLabel("TAHK", 64, COLOR_RED, graphics->renderer);
  graphics->logo2 = loadLabel("2015", 64, COLOR_RED, graphics->renderer);

  // Connection labels
  graphics->lcing = loadLabel("СОЕДИНЯЮ...", 16, COLOR_WHITE, graphics->renderer);
  graphics->lcout = loadLabel("НЕТ ОТВЕТА", 16, COLOR_WHITE, graphics->renderer);
  graphics->lw2nd = loadLabel("ЖДУ ВТОРОГО ИГРОКА...", 16, COLOR_WHITE, graphics->renderer);
  graphics->lcned = loadLabel("ИГРА НАЧИНАЕТСЯ...", 16, COLOR_WHITE, graphics->renderer);

  // Author labels
  graphics->au1 = loadLabel("(c) 2015 ВЛАДИСЛАВ Т", 16, COLOR_WHITE, graphics->renderer);
  graphics->au2 = loadLabel("АЛЛ РИГХТС РЕСЕРВЕД", 16, COLOR_WHITE, graphics->renderer);

  // Game over labels
  graphics->victory = loadLabel("ПОБЕДА!", 64, COLOR_RED, graphics->renderer);
  graphics->defeat = loadLabel("ПОРАЖЕНИЕ", 52, COLOR_RED, graphics->renderer);

  // Game result labels
  graphics->killed1 = loadLabel("ВСЕ ВРАЖЕСКИЕ ТАНКИ", 16, COLOR_WHITE, graphics->renderer);
  graphics->killed2 = loadLabel("У Н И Ч Т О Ж Е Н Ы", 16, COLOR_WHITE, graphics->renderer);

  // Load textures bank
  graphics->textures = pngToSurface("../../resources/textures.png");

  // Load brick texture
  graphics->brick = loadTexture(graphics->textures, 256, 64, 8, 8, graphics->renderer);

  // Load steel texture
  graphics->steel = loadTexture(graphics->textures, 256, 72, 8, 8, graphics->renderer);

  // Load water textures
  graphics->water[0] = loadTexture(graphics->textures, 256, 80, 8, 8, graphics->renderer);
  graphics->water[1] = loadTexture(graphics->textures, 264, 80, 8, 8, graphics->renderer);
  graphics->water[2] = loadTexture(graphics->textures, 272, 80, 8, 8, graphics->renderer);

  // Load tank textures
  // Player 1
  graphics->tank[0][0] = loadTexture(graphics->textures,   0,   0, 15, 16, graphics->renderer);
  graphics->tank[0][1] = loadTexture(graphics->textures,  16,   0, 15, 16, graphics->renderer);
  // Player 2
  graphics->tank[1][0] = loadTexture(graphics->textures,   0, 128, 15, 16, graphics->renderer);
  graphics->tank[1][1] = loadTexture(graphics->textures,  16, 128, 15, 16, graphics->renderer);
  // Enemy
  graphics->tank[2][0] = loadTexture(graphics->textures, 128,   0, 15, 16, graphics->renderer);
  graphics->tank[2][1] = loadTexture(graphics->textures, 144,   0, 15, 16, graphics->renderer);

  // Load fire texture
  graphics->fire = loadTexture(graphics->textures, 320, 100, 8, 8, graphics->renderer);

  // Load explosion textures
  graphics->explosion[0] = loadTexture(graphics->textures, 256, 128, 16, 16, graphics->renderer);
  graphics->explosion[1] = loadTexture(graphics->textures, 272, 128, 16, 16, graphics->renderer);
  graphics->explosion[2] = loadTexture(graphics->textures, 288, 128, 16, 16, graphics->renderer);
  graphics->explosion[3] = loadTexture(graphics->textures, 304, 128, 32, 32, graphics->renderer);
  graphics->explosion[4] = loadTexture(graphics->textures, 336, 128, 32, 32, graphics->renderer);

  // Load base texture
  graphics->base[0] = loadTexture(graphics->textures, 304,  32, 16, 16, graphics->renderer);
  graphics->base[1] = loadTexture(graphics->textures, 320,  32, 16, 16, graphics->renderer);

  // HUD
  graphics->enemy = loadTexture(graphics->textures, 320, 192, 8, 8, graphics->renderer);

  // Animation threads
  SDL_Thread *waterAnimT;
  waterAnimT = SDL_CreateThread(waterAnimThread, "waterAnimT", (void*)graphics);

  SDL_Thread *tankAnimT;
  tankAnimT = SDL_CreateThread(tankAnimThread, "tankAnimT", (void*)client);

  SDL_Thread *explosionAnimT;
  explosionAnimT = SDL_CreateThread(explosionAnimThread, "explosionAnimT", (void*)client);

  graphics->baseAnim  = 0;
  graphics->baseDelay = 0;
  client->baseExplodes = 0;
}

void freeResources(graphics_t *graphics) {
  SDL_FreeSurface(graphics->textures);
}

SDL_Surface *pngToSurface(char *file) {
  SDL_Surface *surface;

  surface = IMG_Load(file);
  if(surface == NULL) {
    fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
  }

  return surface;
}

texture_t *loadTexture(SDL_Surface *src, int x, int y, int szx, int szy, SDL_Renderer *renderer) {
  texture_t *texture;
  texture = malloc(sizeof(texture_t));
  texture->tex = NULL;

  Uint32 rmask, gmask, bmask, amask;
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;

  SDL_Surface *dst = SDL_CreateRGBSurface(0, szx, szy, 32, rmask, gmask, bmask, amask);

  SDL_Rect rect = { x, y, szx, szy };
  if(SDL_BlitSurface(src, &rect, dst, NULL) != 0) {
    fprintf(stderr, "SDL_BlitSurface Error: %s\n", SDL_GetError());
  }

  texture->tex = SDL_CreateTextureFromSurface(renderer, dst);
  if(texture->tex == NULL) {
    fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
  }

  SDL_QueryTexture(texture->tex, NULL, NULL, &texture->width, &texture->height);

  SDL_FreeSurface(dst);
  return texture;
}

texture_t *loadLabel(char *text, int size, SDL_Color color, SDL_Renderer *renderer) {
  texture_t *texture;
  TTF_Font *font = NULL;
  SDL_Surface *surface = NULL;
  texture = malloc(sizeof(texture_t));
  texture->tex = NULL;

  font = TTF_OpenFont("../../resources/font.ttf", size);
  if(font == NULL) {
    fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
  }

  surface = TTF_RenderUTF8_Solid(font, text, color);
  if(surface == NULL) {
    fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
  }

  texture->tex = SDL_CreateTextureFromSurface(renderer, surface);
  if(texture->tex == NULL) {
    fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
  }

  SDL_QueryTexture(texture->tex, NULL, NULL, &texture->width, &texture->height);

  SDL_FreeSurface(surface);
  TTF_CloseFont(font);
  return texture;
}

void renderMenu(client_t *client) {
  graphics_t *graphics = client->graphics;

  // Clear render
  SDL_RenderClear(graphics->renderer);

  // Render logo
  renderTexture(graphics->logo1, 128, 64, 1, 1, 0.0, graphics->renderer);
  renderTexture(graphics->logo2, 128, 128, 1, 1, 0.0, graphics->renderer);

  // Render connection status
  switch(client->cstatus) {
    case CONNECTING:
    renderTexture(graphics->lcing, 168, 256, 1, 1, 0.0, graphics->renderer); break;
    case CTIMEOUT:
    renderTexture(graphics->lcout, 176, 256, 1, 1, 0.0, graphics->renderer); break;
    case WAITING2ND:
    renderTexture(graphics->lw2nd,  88, 256, 1, 1, 0.0, graphics->renderer); break;
    case CONNECTED:
    renderTexture(graphics->lcned, 112, 256, 1, 1, 0.0, graphics->renderer); break;
  }

  // Render author labels
  renderTexture(graphics->au1,  96, 400, 1, 1, 0.0, graphics->renderer);
  renderTexture(graphics->au2, 104, 432, 1, 1, 0.0, graphics->renderer);

  // Update render
  SDL_RenderPresent(graphics->renderer);
}

void renderOver(client_t *client) {
  graphics_t *graphics = client->graphics;

  // Clear render
  SDL_RenderClear(graphics->renderer);

  // Render result
  if(client->result == BASE_DESTROYED)
    renderTexture(graphics->defeat, 26, 128, 1, 1, 0.0, graphics->renderer);
  else
    renderTexture(graphics->victory, 40, 140, 1, 1, 0.0, graphics->renderer);

  if(client->result == ENEMIES_KILLED) {
    renderTexture(graphics->killed1, 104, 280, 1, 1, 0.0, graphics->renderer);
    renderTexture(graphics->killed2, 104, 312, 1, 1, 0.0, graphics->renderer);
  }

  // Update render
  SDL_RenderPresent(graphics->renderer);
}

void render(client_t *client) {
  graphics_t *graphics = client->graphics;

  // Clear render
  SDL_RenderClear(graphics->renderer);

  // Fill background
  SDL_Rect background = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
  SDL_SetRenderDrawColor(graphics->renderer,  99,  99,  99, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(graphics->renderer, &background);
  SDL_SetRenderDrawColor(graphics->renderer,   0,   0,   0, SDL_ALPHA_OPAQUE);

  // Set viewport to map
  graphics->mapView = (SDL_Rect) { 32, 32, 416, 416 };
  SDL_RenderSetViewport(graphics->renderer, &graphics->mapView);

  // Render map
  uint32_t i, j, k;
  int x = 0, y = 0;
  for(i = 0; i < MAP_SIZE; i++, y += 16, x = 0) {
    for(j = 0; j < MAP_SIZE; j++, x += 16) {
      SDL_Rect rect = { x, y, 16, 16 };

      switch(client->map->block[i][j].material) {
        case TERRA:
        SDL_RenderFillRect(graphics->renderer, &rect);
        break;

        case BRICK:
        renderTexture(graphics->brick, x, y, 2, 2, 0.0, graphics->renderer);
        break;

        case STEEL:
        renderTexture(graphics->steel, x, y, 2, 2, 0.0, graphics->renderer);
        break;

        case GRASS:
        break;

        case WATER:
        renderTexture(graphics->water[graphics->waterAnim], x, y, 2, 2, 0.0, graphics->renderer);
        break;

        default:
        SDL_RenderFillRect(graphics->renderer, &rect);
        break;
      }
    }
  }

  // Render tanks
  for(k = 0; k < 2; k++) {
    renderTexture(graphics->tank[k][graphics->tankAnim[k]], client->tank[k].posX+1,
    client->tank[k].posY, 2, 2, client->tank[k].direction, graphics->renderer);
  }

  for(k = 2; k < TANKS; k++) {
    if(!client->tank[k].null) {
      if(client->tank[k].explodes) {
        if(client->tank[k].explAnim < 3)
          renderTexture(graphics->explosion[client->tank[k].explAnim], client->tank[k].posX,
          client->tank[k].posY, 2, 2, 0, graphics->renderer);
        else
          renderTexture(graphics->explosion[client->tank[k].explAnim], client->tank[k].posX - 16,
          client->tank[k].posY - 16, 2, 2, 0, graphics->renderer);

        if(SDL_GetTicks() > client->tank[k].explDelay) {
          if(++client->tank[k].explAnim == 5) {
            client->tank[k].explAnim = 0;
            client->tank[k].explodes = 0;
          }
          client->tank[k].explDelay = SDL_GetTicks() + 60;
        }
      }
      else
        renderTexture(graphics->tank[2][graphics->tankAnim[k]], client->tank[k].posX+1,
        client->tank[k].posY, 2, 2, client->tank[k].direction, graphics->renderer);
    }
  }

  // Render fire
  for(k = 0; k < TANKS; k++) {
    if(!client->tank[k].null) {
      if(client->tank[k].isFiring) {
        renderTexture(graphics->fire, client->tank[k].fire.posX + 7, client->tank[k].fire.posY,
        2, 2, client->tank[k].fire.direction, graphics->renderer);
      }
      else if(client->tank[k].fire.explodes) {
        uint32_t x, y, direction;
        x = client->tank[k].fire.posX;
        y = client->tank[k].fire.posY;
        direction = client->tank[k].fire.direction;
        switch(direction) {
          case UP:
          y -= 16;
          break;

          case DOWN:
          y += 16;
          break;

          case LEFT:
          x -= 16;
          break;

          case RIGHT:
          x += 16;
          break;
        }
        renderTexture(graphics->explosion[client->tank[k].fire.explosionAnim],
        x, y, 2, 2, direction, graphics->renderer);
      }
    }
  }

  // Render base
  if(client->baseExplodes) {
    if(graphics->baseAnim < 3)
      renderTexture(graphics->explosion[graphics->baseAnim], BASE_X, BASE_Y, 2, 2, 0, graphics->renderer);
    else
      renderTexture(graphics->explosion[graphics->baseAnim], BASE_X - 16, BASE_Y - 16, 2, 2, 0, graphics->renderer);

    if(SDL_GetTicks() > graphics->baseDelay) {
      if(++graphics->baseAnim == 5) {
        graphics->baseAnim = 0;
        client->baseExplodes = 0;
        client->base = 1;
      }
      graphics->baseDelay = SDL_GetTicks() + 50;
    }
  }
  else renderTexture(graphics->base[client->base], BASE_X, BASE_Y, 2, 2, 0, graphics->renderer);

  // Render HUD
  // Set viewport to HUD
  graphics->hudView = (SDL_Rect) { 464, 48, 32, 320 };
  SDL_RenderSetViewport(graphics->renderer, &graphics->hudView);

  uint8_t enemiesRendered = 0;
  for(y = 0; y < 320; y += 16) {
    for(x = 0; x < 32; x += 16) {
      if(enemiesRendered < ENEMIES - client->enemiesKilled) {
        renderTexture(graphics->enemy, x, y, 2, 2, 0, graphics->renderer);
        enemiesRendered++;
      }
    }
  }

  // Restore viewport
  SDL_RenderSetViewport(graphics->renderer, NULL);

  // Update render
  SDL_RenderPresent(graphics->renderer);
}

void renderTexture(texture_t *texture, int x, int y, int sx, int sy, double rotation, SDL_Renderer *renderer) {
  SDL_Rect dstrect = { x, y, sx * texture->width, sy * texture->height };
  SDL_RenderCopyEx(renderer, texture->tex, NULL, &dstrect, rotation, NULL, SDL_FLIP_NONE);
}
