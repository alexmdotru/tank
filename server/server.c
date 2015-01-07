#include <stdlib.h>
#include <stdio.h>
#include "framework.h"
#include "network.h"
#include "server.h"

SDLNet_SocketSet createSocketSet(int size);
void acceptConnections(server_t *server);

int main(int argc, char **argv) {
  // Check command line
  if(argc != 2) {
    fprintf(stderr, "usage:\t%s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Init SDL2 frameworks
  initFramework();

  server_t server;

  // Set server host and port
  server.shost = NULL;
  server.sport = atoi(argv[1]);

  // Wait for players to connect
  acceptConnections(&server);

  // Quit SDL2 frameworks
  quitFramework();
  return 0;
}

SDLNet_SocketSet createSocketSet(int size) {
  SDLNet_SocketSet socketSet;

  socketSet = SDLNet_AllocSocketSet(size);
  if(!socketSet) {
    fprintf(stderr, "SDLNet_AllocSocketSet Error: %s\n", SDLNet_GetError());
  }

  return socketSet;
}

void acceptConnections(server_t *server) {
  // No players
  server->pCount = 0;

  // Allocate socket set
  server->socketSet = createSocketSet(2);

  // Open server socket
  server->socket = openTCPSocket(server->shost, server->sport);

  fprintf(stderr, "Ready to accept connections!\n");

  while(server->pCount < 2) {
    TCPsocket pSocket;

    pSocket = SDLNet_TCP_Accept(server->socket);
    if(pSocket) {
      fprintf(stderr, "Accepted from player %d.\n", server->pCount);
      SDLNet_TCP_Send(pSocket, &server->pCount, 1);
      server->pSocket[server->pCount] = pSocket;
      SDLNet_TCP_AddSocket(server->socketSet, pSocket);
      server->pCount++;
    }
    else {
      SDL_Delay(3000);
    }
  }

  int i;
  for(i = 0; i < 2; i++) {
    SDLNet_TCP_Send(server->pSocket[i], &server->pCount, 1);
    fprintf(stderr, "Starting game...");
  }
}
