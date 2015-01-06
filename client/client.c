#include <stdlib.h>
#include <stdio.h>
#include "client.h"
#include "framework.h"

int main(int argc, char **argv) {
  client_t client;

  // Check command line
  if(argc != 3) {
    fprintf(stderr, "usage:\t%s host port", argv[0]);
    exit(1);
  }

  initFramework();
  launchGame(client);

  SDL_Delay(10000);
  SDLNet_Quit();
  SDL_Quit();
  return 0;
}

void launchGame(client_t client) {
  client.window = SDL_CreateWindow("TAHK-2015", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  client.renderer = SDL_CreateRenderer(client.window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}
