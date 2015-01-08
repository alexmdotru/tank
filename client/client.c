#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "graphics.h"
#include "network.h"
#include "map.h"

void gameLoop(client_t *client);
int connectToServerThread(void *data);
int loadMapThread(void *data);
void updateMenu(client_t *client);
void renderMenu(client_t *client);
void update(client_t *client);
void render(client_t *client);

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
  client.host = argv[1];
  client.port = atoi(argv[2]);

  // Prepare renderer
  client.graphics = malloc(sizeof(graphics_t));
  loadFrame(client.graphics);

  // Load labels and textures
  loadResources(client.graphics);

  // Start game
  gameLoop(&client);

  SDL_Delay(2000);

  // Free renderer
  freeFrame(client.graphics);

  // Quit SDL2 frameworks
  quitFramework();
  return 0;
}

void gameLoop(client_t *client) {
  client->gameRunning = 1;
  client->isInMainMenu = 1;

  SDL_Thread *cToServerT;
  cToServerT = SDL_CreateThread(connectToServerThread, "cToServerT", (void*)client);

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

int connectToServerThread(void *data) {
  client_t *client = (client_t*)data;

  // Set connection status
  client->cstatus = CONNECTING;

  // Connection timeout
  int timeout = SDL_GetTicks() + 30000;
  fprintf(stderr, "Connecting to %s:%d\n", client->host, client->port);

  while(SDL_GetTicks() < timeout) {
    // Open socket
    client->socket = openTCPSocket(client->host, client->port);

    if(client->socket) {
      SDLNet_TCP_Recv(client->socket, &client->playerID, 4);
      fprintf(stderr, "Connection successful! Player ID %d.\n", client->playerID);
      client->cstatus = WAITING2ND;
      SDLNet_TCP_Recv(client->socket, &client->level, 4);
      client->cstatus = CONNECTED;
      // Load map
      SDL_Thread *loadMapT;
      loadMapT = SDL_CreateThread(loadMapThread, "loadMapT", (void*)client);
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

int loadMapThread(void *data) {
  client_t *client = (client_t*)data;
  char  file[256];
  char level[256];
  sprintf(level, "%d", client->level);
  strcpy(file, "resources/maps/level");
  strcat(file, level);
  strcat(file, ".map");
  client->map = loadMap(file);
  return 0;
}
