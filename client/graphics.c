#include <stdio.h>
#include "framework.h"
#include "client.h"

// String constants
const char  *LOGO_TEXT = "TAHK\0";
const int    LOGO_SIZE = 64;

// Colors
SDL_Color SOLOR_BLACK = {   0,   0,   0 };
SDL_Color COLOR_RED   = { 255,   0,   0 };
SDL_Color COLOR_GREEN = {   0, 255,   0 };
SDL_Color COLOR_BLUE  = {   0,   0, 255 };
SDL_Color COLOR_WHITE = { 255, 255, 255 };

SDL_Texture *loadTextureFromText(const char *text, int size, SDL_Color color, SDL_Renderer *renderer) {
  TTF_Font *font = NULL;
  SDL_Surface *surface = NULL;
  SDL_Texture *texture = NULL;

  font = TTF_OpenFont("../../resources/font.ttf", size);
  if(font == NULL) {
    fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
  }

  surface = TTF_RenderText_Solid(font, text, color);
  if(surface == NULL) {
    fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
  }

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if(texture == NULL) {
    fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
  }

  SDL_FreeSurface(surface);
  TTF_CloseFont(font);
  return texture;
}

SDL_Texture *loadTextureFromPNG(char *file, SDL_Renderer *renderer) {
  SDL_Surface *surface = NULL;
  SDL_Texture *texture = NULL;

  surface = IMG_Load(file);
  if(surface == NULL) {
    fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
  }

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if(texture == NULL) {
    fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
  }

  SDL_FreeSurface(surface);
  return texture;
}

void loadTextures(client_t *client) {
  // Load logo texture
  client->logo = loadTextureFromText(LOGO_TEXT, LOGO_SIZE, COLOR_RED, client->renderer);
  // Load textures
  client->textures = loadTextureFromPNG("../../resources/textures.png", client->renderer);
}
