#include "framework.h"

void initFramework() {
  // Init framework
  if(!SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    exit(2);
  }

  if(!SDLNet_Init()) {
    fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
    exit(3);
  }
}
