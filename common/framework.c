#include "framework.h"

void initFramework() {
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if(SDLNet_Init() != 0) {
    fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }

  if(TTF_Init() != 0) {
    fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
    exit(EXIT_FAILURE);
  }
}

void quitFramework() {
  IMG_Quit();
  TTF_Quit();
  SDLNet_Quit();
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
