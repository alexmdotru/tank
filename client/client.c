#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "graphics.h"
#include "network.h"

void gameLoop(client_t *client);
void updateMenu(client_t *client);
void update(client_t *client);

int main(int argc, char **argv) {
  // Check command line
  if(argc != 3) {
    fprintf(stderr, "usage:\t%s host port", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Init SDL2 frameworks
  initFramework();

  client_t client;

  // Set server host and port
  client.shost = argv[1];
  client.sport = atoi(argv[2]);

  // Prepare renderer
  loadFrame(&client);

  // Load labels and textures
  loadResources(&client);

  // Start game
  gameLoop(&client);

  // Quit SDL2 frameworks
  quitFramework();
  return 0;
}

void gameLoop(client_t *client) {
  client->gameRunning = 1;
  client->isInMainMenu = 1;

  SDL_Thread *cToServer;
  cToServer = SDL_CreateThread(connectToServer, "cToServer", (void*)client);

  while(client->gameRunning) {
    if(client->isPlaying) {
      update(client);
      render(client);
    }
    else if(client->isInMainMenu) {
      updateMenu(client);
      renderMenu(client);
    }
  }
}

void updateMenu(client_t *client) {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
      client->gameRunning = 0; break;
    }
  }
}

void update(client_t *client) {

}
