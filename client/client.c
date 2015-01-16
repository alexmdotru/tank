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
void update(client_t *client);
void render(client_t *client);
void startGame(client_t *client);
int prepareClientThread(void *data);
void loadResources(client_t *client);
void updateCoordinates(client_t *client);
void updateFire(client_t *client);
void destroyBlock(client_t *client, int i, int j);
void updatePlayerTank(client_t *client);

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

void update(client_t *client) {
  processInput(client);
  updatePlayerTank(client);
  updateEnemyTank(&client->tank[2], client->map);
  moveTank(&client->tank[client->myPlayerID]);
  moveTank(&client->tank[2]);
  // updateCoordinates(client);
  // updateFire(client);
}

void updateFire(client_t *client) {
  tank_t *tank = &client->tank[client->myPlayerID];

  if(!tank->isFiring && client->keyPressed[KEY_SPACE]) {
    tank->isFiring = 1;
    tank->shot.posX = tank->posX;
    tank->shot.posY = tank->posY;
    client->keyPressed[KEY_SPACE] = 0;
    return;
  }
  else if(tank->isFiring) {
    // Calculate shot position
    size_t blockX, blockY;
    blockX = tank->shot.posX / 16;
    blockY = tank->shot.posY / 16;

    switch(tank->direction) {
      case UP:
      if(tank->shot.posY % 16 == 0) {
        if(client->map->block[blockY-1][blockX+1].material == BRICK) {
          destroyBlock(client, blockY-1, blockX+1);
        }
        else if(client->map->block[blockY-1][blockX].material == BRICK) {
          destroyBlock(client, blockY-1, blockX);
        }
      }
      tank->shot.posY -= 4 * tank->velocity;
      break;

      case DOWN:
      if(tank->shot.posY % 16 == 0) {
        if(client->map->block[blockY+2][blockX+1].material == BRICK) {
          destroyBlock(client, blockY+2, blockX+1);
        }
        else if(client->map->block[blockY+2][blockX].material == BRICK) {
          destroyBlock(client, blockY+2, blockX);
        }
      }
      tank->shot.posY += 4 * tank->velocity;
      break;

      case LEFT:
      if(tank->shot.posX % 16 == 0) {
        if(client->map->block[blockY][blockX-1].material == BRICK) {
          destroyBlock(client, blockY, blockX-1);
        }
        else if(client->map->block[blockY+1][blockX-1].material == BRICK) {
          destroyBlock(client, blockY+1, blockX-1);
        }
      }
      tank->shot.posX -= 4 * tank->velocity;
      break;

      case RIGHT:
      tank->shot.posX += 4 * tank->velocity;
      break;
    }
  }
}

void destroyBlock(client_t *client, int i, int j) {
  client->map->block[i][j].material = TERRA;
  client->tank[client->myPlayerID].isFiring = 0;
  client->tank[client->myPlayerID].shot.explodes = 1;
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

  // Set up player IDs
  if(client->myPlayerID == 0) client->hisPlayerID = 1;
  else client->hisPlayerID = 0;

  // Prepare tanks
  client->tanks = 2;
  client->tank[0] = (tank_t) { PLAYER, UP, 128, 384, 1, 0, 0, 0, 0 };
  client->tank[1] = (tank_t) { PLAYER, UP, 256, 384, 1, 0, 0, 0, 0 };

  return 0;
}
