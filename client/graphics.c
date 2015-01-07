#include "stdlib.h"
#include "framework.h"
#include "client.h"
#include "graphics.h"

// Colors
SDL_Color SOLOR_BLACK = {   0,   0,   0 };
SDL_Color COLOR_RED   = { 255,   0,   0 };
SDL_Color COLOR_GREEN = {   0, 255,   0 };
SDL_Color COLOR_BLUE  = {   0,   0, 255 };
SDL_Color COLOR_WHITE = { 255, 255, 255 };

texture_t *loadLabel(char *text, int size, SDL_Color color, SDL_Renderer *renderer);
void renderTexture(texture_t *texture, int x, int y, int sx, int sy, SDL_Renderer *renderer);

void loadFrame(client_t *client) {
  client->window = SDL_CreateWindow("TAHK-2015", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  client->renderer = SDL_CreateRenderer(client->window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void loadResources(client_t *client) {
  // Create and load labels
  client->logo1 = loadLabel("TAHK", 64, COLOR_RED, client->renderer);
  client->logo2 = loadLabel("2015", 64, COLOR_RED, client->renderer);
  // Connection labels
  client->lcing = loadLabel("соединяю...", 16, COLOR_WHITE, client->renderer);
  client->lcout = loadLabel("нет ответа", 16, COLOR_WHITE, client->renderer);
  client->lw2nd = loadLabel("жду второго игрока...", 16, COLOR_WHITE, client->renderer);
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
  // Clear render
  SDL_RenderClear(client->renderer);

  // Render logo
  renderTexture(client->logo1, 128, 64, 1, 1, client->renderer);
  renderTexture(client->logo2, 128, 128, 1, 1, client->renderer);

  // Render connection status
  switch(client->cstatus) {
    case CONNECTING:
    renderTexture(client->lcing, 168, 256, 1, 1, client->renderer); break;
    case CTIMEOUT:
    renderTexture(client->lcout, 176, 256, 1, 1, client->renderer); break;
  }

  // Update render
  SDL_RenderPresent(client->renderer);
}

void render(client_t *client) {

}

void renderTexture(texture_t *texture, int x, int y, int sx, int sy, SDL_Renderer *renderer) {
  SDL_Rect dstrect = { x, y, sx * texture->width, sy * texture->height };
  SDL_RenderCopy(renderer, texture->tex, NULL, &dstrect);
}
