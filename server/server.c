#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "network.h"
#include "server.h"
#include "tank.h"
#include "map.h"

void acceptConnection(server_t *server);
void serverLoop(server_t *server);
void spawnEnemy(server_t *server);
void updateMap(map_t *map, tank_t *tank);
int explosionThread(void *data);

int main(int argc, char **argv) {
  // Check command line
  if(argc != 3) {
    fprintf(stderr, "usage:\t%s port level\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Init SDL2 frameworks
  initFramework();

  server_t server;

  // Set server host, port and level
  server.host  = NULL;
  server.port  = atoi(argv[1]);
  server.level = atoi(argv[2]);

  // Do the work
  serverLoop(&server);

  // Quit SDL2 frameworks
  quitFramework();
  return 0;
}

void acceptConnection(server_t *server) {
  // Client socket
  TCPsocket cSocket;

  // Accept connection
  cSocket = SDLNet_TCP_Accept(server->sSocket);

  if(cSocket) {
    SDLNet_TCP_Send(cSocket, &server->playerID, 4);
    server->cSocket[server->playerID] = cSocket;
    fprintf(stderr, "Accepted from player %d.\n", server->playerID++);
  }

  if(server->playerID > 1) {
    // Send level info
    SDLNet_TCP_Send(server->cSocket[0], &server->level, 4);
    SDLNet_TCP_Send(server->cSocket[1], &server->level, 4);
    SDL_Delay(3000);
  }
}

void serverLoop(server_t *server) {
  uint8_t i, j;

  // Server is running
  server->serverRunning = 1;

  // No players
  server->playerID = 0;
  for(i = 2; i < TANKS; i++) {
    server->tank[i].null = 1;
  }
  server->enemies = 0;
  server->enemiesDelay = 0;

  // Load map
  server->map = loadMap(server->level);

  // Open server socket
  server->sSocket = openTCPSocket(server->host, server->port);
  fprintf(stderr, "Ready to accept connections!\n");

  while(server->serverRunning) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
        server->serverRunning = 0;
        break;
      }
    }

    if(server->playerID > 1) {
      spawnEnemy(server);
      // Update enemies
      for(i = 2; i < TANKS; i++) {
        if(!server->tank[i].null) {
          updateEnemyTank(&server->tank[i], server->map);
          moveTank(&server->tank[i], server->map);
          updateFire(&server->tank[i], server->map);
          if(server->tank[i].fire.explodes) {
            SDL_Thread *explosionT;
            explosionT = SDL_CreateThread(explosionThread, "explosionT", (void*)&server->tank[i]);
          }
        }
        sendTankStruct(&server->tank[i], server->cSocket[0]);
        sendTankStruct(&server->tank[i], server->cSocket[1]);
        if(server->tank[i].winsTheGame) {
          server->tank[i].winsTheGame = 0;
        }
      }

      // Update players
      for(i = 0; i < 2; i++) {
        recvTankStruct(&server->tank[i], server->cSocket[i]);
        updateTanksOnMap(&server->tank[i], server->map);
      }
      for(i = 0, j = 1; i < 2; i++, j--) {
        sendTankStruct(&server->tank[i], server->cSocket[j]);
      }

      // Destroy map blocks
      for(i = 0; i < TANKS; i++) {
        updateMap(server->map, &server->tank[i]);
      }
    }
    else {
      acceptConnection(server);
    }
  }
}

int explosionThread(void *data) {
  tank_t *tank = (tank_t*)data;

  // Animation speed
  SDL_Delay(150);

  tank->fire.explodes = 0;

  return 0;
}
