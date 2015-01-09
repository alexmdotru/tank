#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "graphics.h"
#include "network.h"
#include "map.h"
#include "tank.h"

void gameLoop(client_t *client);
int connectToServerThread(void *data);
void updateMenu(client_t *client);
void renderMenu(client_t *client);
void update(client_t *client);
void render(client_t *client);
void startGame(client_t *client);
int prepareClientThread(void *data);
void loadMapFromFile(client_t *client);
void loadResources(client_t *client);
void updateCoordinates(client_t *client);

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
      client->gameRunning = 0; break;
    }
  }
}

void update(client_t *client) {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
      client->isPlaying = 0;
      client->gameRunning = 0;
      break;

      case SDL_KEYDOWN:
      if(!event.key.repeat) {
        switch(event.key.keysym.sym) {
          case SDLK_UP:
          client->keyPressed[KEY_UP] = 1;
          break;

          case SDLK_DOWN:
          client->keyPressed[KEY_DOWN] = 1;
          break;

          case SDLK_LEFT:
          client->keyPressed[KEY_LEFT] = 1;
          break;

          case SDLK_RIGHT:
          client->keyPressed[KEY_RIGHT] = 1;
          break;
        }
      }
      break;

      case SDL_KEYUP:
      switch(event.key.keysym.sym) {
        case SDLK_UP:
        client->keyPressed[KEY_UP] = 0;
        break;

        case SDLK_DOWN:
        client->keyPressed[KEY_DOWN] = 0;
        break;

        case SDLK_LEFT:
        client->keyPressed[KEY_LEFT] = 0;
        break;

        case SDLK_RIGHT:
        client->keyPressed[KEY_RIGHT] = 0;
        break;
      }
      break;
    }
  }

  updateCoordinates(client);
}

void updateCoordinates(client_t *client) {
  tank_t *tank = &client->tank[client->myPlayerID];

  // Move up and turn
  if(client->keyPressed[KEY_UP]) {
    tank->isMoving = 1;

    if(client->keyPressed[KEY_DOWN]) {
      tank->isMoving = 0;
    }
    else if(client->keyPressed[KEY_LEFT]) {
      tank->direction = LEFT;
    }
    else if(client->keyPressed[KEY_RIGHT]) {
      tank->direction = RIGHT;
    }
    else tank->direction = UP;
  }

  // Move down and turn
  else if(client->keyPressed[KEY_DOWN]) {
    tank->isMoving = 1;

    if(client->keyPressed[KEY_LEFT]) {
      tank->direction = LEFT;
    }
    else if(client->keyPressed[KEY_RIGHT]) {
      tank->direction = RIGHT;
    }
    else tank->direction = DOWN;
  }

  // Turn left
  else if(client->keyPressed[KEY_LEFT]) {
    tank->isMoving = 1;

    if(client->keyPressed[KEY_RIGHT]) {
      tank->isMoving = 0;
    }
    else tank->direction = LEFT;
  }

  // Turn right
  else if(client->keyPressed[KEY_RIGHT]) {
    tank->isMoving = 1;
    tank->direction = RIGHT;
  }

  // Stop
  else tank->isMoving = 0;

  if(tank->isMoving) {
    switch(tank->direction) {
      case UP:
      tank->posY -= tank->velocity;
      break;

      case DOWN:
      tank->posY += tank->velocity;
      break;

      case LEFT:
      tank->posX -= tank->velocity;
      break;

      case RIGHT:
      tank->posX += tank->velocity;
      break;
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

int prepareClientThread(void *data) {
  client_t *client = (client_t*)data;

  // Load level
  loadMapFromFile(client);

  // No input yet
  int i;
  client->keyPresses = 0;
  for(i = 0; i < 4; i++) {
    client->keyPressed[i] = 0;
  }

  // Set up player IDs
  if(client->myPlayerID == 0) client->hisPlayerID = 1;
  else client->hisPlayerID = 0;

  // Prepare tanks
  client->tank[0] = (tank_t) { PLAYER, UP, 128, 384, 1, 0 };
  client->tank[1] = (tank_t) { PLAYER, UP, 256, 384, 1, 0 };

  return 0;
}

void loadMapFromFile(client_t *client) {
  char  file[256];
  char level[256];
  sprintf(level, "%d", client->level);
  strcpy(file, "resources/maps/level");
  strcat(file, level);
  strcat(file, ".map");
  client->map = loadMap(file);
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
