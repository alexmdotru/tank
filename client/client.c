#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "graphics.h"

void loadGame(client_t *client);

int main(int argc, char **argv) {
  // Check command line
  if(argc != 3) {
    fprintf(stderr, "usage:\t%s host port", argv[0]);
    exit(EXIT_FAILURE);
  }

  initFramework();
  client_t client;
  loadGame(&client);

  SDL_Delay(10000);
  quitFramework();
  return 0;
}

void loadGame(client_t *client) {
  // Prepare renderer
  client->window = SDL_CreateWindow("TAHK-2015", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  client->renderer = SDL_CreateRenderer(client->window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  loadTextures(client);

  // Set client flags
  client->isPlaying = 0;
  client->isInMainMenu = 1;
}
