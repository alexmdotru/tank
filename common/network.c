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
