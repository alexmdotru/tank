#pragma once

#include "framework.h"

const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 480;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} client_t;

void launchGame(client_t client);
