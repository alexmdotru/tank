#include <stdio.h>
#include "framework.h"
#include "client.h"

TCPsocket openTCPSocket(char *host, int port) {
  IPaddress ip;
  TCPsocket socket;

  if(SDLNet_ResolveHost(&ip, host, port) != 0) {
    fprintf(stderr, "SDLNet_ResolveHost Error: %s\n", SDLNet_GetError());
  }

  socket = SDLNet_TCP_Open(&ip);

  if(!socket) {
    fprintf(stderr, "SDLNet_TCP_Open Error: %s\n", SDLNet_GetError());
  }

  return socket;
}

int connectToServer(void *data) {
  int i;
  client_t *client = (client_t*)data;

  client->cstatus = CONNECTING;

  for(i = 0; i < 5; i++) {
    // Open socket
    fprintf(stderr, "Connecting to %s:%d\n", client->shost, client->sport);
    client->socket = openTCPSocket(client->shost, client->sport);

    if(client->socket) {
      client->cstatus = WAITING2ND;
      fprintf(stderr, "Connection successful!\n");
      break;
    }

    fprintf(stderr, "Server didn't respond. Retrying...\n");
    SDL_Delay(3000);
  }

  if(!client->socket) {
    client->cstatus = CTIMEOUT;
    return -1;
  }

  return 0;
}
