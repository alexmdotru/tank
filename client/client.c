#include <stdlib.h>
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2_net/SDL_net.h"

int main(int argc, char **argv) {
  // Check command line
  if(argc != 3) {
    fprintf(stderr, "usage:\t%s host port", argv[0]);
    exit(1);
  }

  // Init framework
  if(!SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    exit(2);
  }

  if(!SDLNet_Init()) {
    fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
    exit(3);
  }

  SDLNet_Quit();
  SDL_Quit();
  return 0;
}
