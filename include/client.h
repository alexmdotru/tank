#pragma once

#include "framework.h"

#define WINDOW_WIDTH  512
#define WINDOW_HEIGHT 480

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  TCPsocket socket;
  // Textures
  SDL_Texture *logo;
  SDL_Texture *textures;
  int isPlaying;
  int isInMainMenu;
} client_t;
