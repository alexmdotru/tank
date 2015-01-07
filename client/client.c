#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "graphics.h"
#include "network.h"

void gameLoop(client_t *client);
void updateMenu(client_t *client);
void update(client_t *client);
int connectToServer(void *data);

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
  client->waterAnim = 0;

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
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
      client->gameRunning = 0; break;
    }
  }
}

int connectToServer(void *data) {
  client_t *client = (client_t*)data;

  // Set connection status
  client->cstatus = CONNECTING;

  // Connection timeout
  Uint32 timeout = SDL_GetTicks() + 30000;
  fprintf(stderr, "Connecting to %s:%d\n", client->shost, client->sport);

  while(SDL_GetTicks() < timeout) {
    // Open socket
    client->socket = openTCPSocket(client->shost, client->sport);

    if(client->socket) {
      SDLNet_TCP_Recv(client->socket, &client->id, 1);
      fprintf(stderr, "Connection successful! Player id %d.\n", client->id);
      client->cstatus = WAITING2ND;
      SDLNet_TCP_Recv(client->socket, &client->level, 4);
      client->cstatus = CONNECTED;
      SDL_Delay(3000);
      client->isInMainMenu = 0;
      client->isPlaying = 1;
      break;
    }
  }

  if(!client->socket) {
    client->cstatus = CTIMEOUT;
    fprintf(stderr, "Server didn't respond.\n");
    return 1;
  }

  return 0;
}
