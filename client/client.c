#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "graphics.h"
#include "network.h"
#include "map.h"
#include "tank.h"
#include "input.h"

void gameLoop(client_t *client);
int connectToServerThread(void *data);
void updateMenu(client_t *client);
void renderMenu(client_t *client);
void updateOver(client_t *client);
void renderOver(client_t *client);
void update(client_t *client);
void render(client_t *client);
void startGame(client_t *client);
int prepareClientThread(void *data);
void loadResources(client_t *client);
void updatePlayerTank(client_t *client);
void updateMap(map_t *map, tank_t *tank);
void recvMapUpdates(client_t *client);
int gameOverThread(void *data);

int main(int argc, char **argv) {
  // Check command line
  if(argc != 3) {
    fprintf(stderr, "usage:\ttank host port\n");
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
  loadResources(&client);

  // Start game
  gameLoop(&client);

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
    else if(client->isGameOver) {
      updateOver(client);
      renderOver(client);
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
      client->gameRunning = 0;
      break;
    }
  }
}

void updateOver(client_t *client) {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
      client->gameRunning = 0;
      break;
    }
  }
}

void update(client_t *client) {
  uint8_t i;
  // Update enemies
  for(i = 2; i < TANKS; i++) {
    recvTankStruct(&client->tank[i], client->socket);
    updateTanksOnMap(&client->tank[i], client->map);
    if(!client->tank[i].null) {
      if(client->tank[i].destrTank >= 0) {
        client->tank[client->tank[i].destrTank].explodes = 1;
      }
    }
  }

  // Update me
  sendTankStruct(&client->tank[client->myPlayerID], client->socket);

  // Update friend
  recvTankStruct(&client->tank[client->hisPlayerID], client->socket);
  updateTanksOnMap(&client->tank[client->hisPlayerID], client->map);

  // Tank updates
  for(i = 0; i < TANKS; i++) {
    if(!client->tank[i].null) {
      updateMap(client->map, &client->tank[i]);
      if(client->tank[i].destroyedBase) {
        client->result = BASE_DESTROYED;
        client->baseExplodes = 1;
        client->tank[i].destroyedBase = 0;
        SDL_Thread *gameOverT;
        gameOverT = SDL_CreateThread(gameOverThread, "gameOverT", (void*)client);
      }
    }
    else {
      freeMapFromTank(i, client->map);
    }
  }

  // Process player input
  processInput(client);
  // Update coordinates and fire
  updatePlayerTank(client);
  moveTank(&client->tank[client->myPlayerID], client->map);
  updateFire(&client->tank[client->myPlayerID], client->map);

  // Check if tank explodes
  if(client->tank[client->myPlayerID].destrTank >= 2) {
    client->tank[client->tank[client->myPlayerID].destrTank].explodes = 1;
  }

  // Send destroy update
  sendDestrUpdate(&client->tank[client->myPlayerID].destrTank, client->socket);
  client->tank[client->myPlayerID].destrTank = -1;

  // Recv killed enemies
  recvEnemiesKilled(&client->enemiesKilled, client->socket);
  if(client->enemiesKilled == ENEMIES) {
    client->result = ENEMIES_KILLED;
    SDL_Thread *gameOverT;
    gameOverT = SDL_CreateThread(gameOverThread, "gameOverT", (void*)client);
  }

  // Check if the game is over for us
  if(client->tank[client->myPlayerID].null && client->tank[client->hisPlayerID].null) {
    client->result = PLAYERS_KILLED;
    SDL_Thread *gameOverT;
    gameOverT = SDL_CreateThread(gameOverThread, "gameOverT", (void*)client);
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
      SDLNet_TCP_Recv(client->socket, &client->myPlayerID, 4);
      fprintf(stderr, "Connection successful! Player ID %d.\n", client->myPlayerID);
      client->cstatus = WAITING2ND;
      SDLNet_TCP_Recv(client->socket, &client->level, 4);
      client->cstatus = CONNECTED;
      startGame(client);
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

void startGame(client_t *client) {
  // Prepare client
  SDL_Thread *prepareClientT;
  prepareClientT = SDL_CreateThread(prepareClientThread, "prepareClientT", (void*)client);
  SDL_Delay(3000);

  // Start game
  client->isInMainMenu = 0;
  client->isPlaying = 1;
}

int prepareClientThread(void *data) {
  client_t *client = (client_t*)data;

  // Load level
  client->map = loadMap(client->level);

  // No input yet
  uint8_t i;
  for(i = 0; i < KEYS; i++) {
    client->keyPressed[i] = 0;
  }

  // Base is healthy
  client->base = 0;
  client->winner = -1;

  // Init isGameOver
  client->isGameOver = 0;

  // Set up player IDs
  if(client->myPlayerID == 0) client->hisPlayerID = 1;
  else client->hisPlayerID = 0;

  client->tanks = 2;
  client->tank[0] = (tank_t) { PLAYER, UP, 128, 384, 1, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0 };
  client->tank[1] = (tank_t) { PLAYER, UP, 256, 384, 1, 0, 0, 0, 0, 0, -1, -1, 1, 0, 0, 0, 0 };
  for(i = 0; i < 2; i++) {
    updateTankOnMap(&client->tank[i], client->map);
    updateTanksOnMap(&client->tank[i], client->map);
  }

  for(i = 2; i < TANKS; i++) {
    client->tank[i].null = 1;
    client->tank[i].destrBlock = -1;
    client->tank[i].explodes  = 0;
    client->tank[i].explAnim  = 0;
    client->tank[i].explDelay = 0;
  }

  return 0;
}

int gameOverThread(void *data) {
  client_t *client = (client_t*)data;

  SDL_Delay(1500);
  client->isPlaying = 0;
  client->isGameOver = 1;

  return 0;
}
