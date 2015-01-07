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

  // Set server host and port
  server.shost = NULL;
  server.sport = atoi(argv[1]);
  server.level = atoi(argv[2]);

  // Do the work
  serverLoop(&server);

  // Quit SDL2 frameworks
  quitFramework();
  return 0;
}

void acceptConnection(server_t *server) {
  // Client socket
  TCPsocket socket;

  // Accept connection
  socket = SDLNet_TCP_Accept(server->socket);

  if(socket) {
    SDLNet_TCP_Send(socket, &server->playerID, 1);
    server->pSocket[server->playerID] = socket;
    fprintf(stderr, "Accepted from player %d.\n", server->playerID++);
  }

  if(server->playerID > 1) {
    // Send level info
    SDLNet_TCP_Send(server->pSocket[0], &server->level, 4);
    SDLNet_TCP_Send(server->pSocket[1], &server->level, 4);
  }
}

void serverLoop(server_t *server) {
  // Server is running
  server->serverRunning = 1;

  // No players
  server->playerID = 0;

  // Open server socket
  server->socket = openTCPSocket(server->shost, server->sport);
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
