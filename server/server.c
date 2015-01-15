#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "network.h"
#include "server.h"

void acceptConnection(server_t *server);
void serverLoop(server_t *server);

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
  }
}

void serverLoop(server_t *server) {
  // Server is running
  server->serverRunning = 1;

  // No players
  server->playerID = 0;

  // Open server socket
  server->sSocket = openTCPSocket(server->host, server->port);
  fprintf(stderr, "Ready to accept connections!\n");

  while(server->serverRunning) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
        server->serverRunning = 0; break;
      }
    }

    if(server->playerID > 1) {

    }
    else {
      acceptConnection(server);
    }
  }
}
