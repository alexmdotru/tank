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

void sendTankStruct(tank_t *tank, TCPsocket socket) {
  // Block until request
  uint8_t request = 1;
  SDLNet_TCP_Recv(socket, &request, 1);

  int8_t null = 0;
  if(tank->null) {
    null = 1;
  }
  SDLNet_TCP_Send(socket, &null, 1);
  if(null) return;

  // SDLNet_TCP_Send(&socket, &tank->driver, sizeof(driver_t));
  SDLNet_TCP_Send(socket, &tank->direction, sizeof(direction_t));
  SDLNet_TCP_Send(socket, &tank->posX, 4);
  SDLNet_TCP_Send(socket, &tank->posY, 4);
  // SDLNet_TCP_Send(&socket, &tank->velocity, 1);
  SDLNet_TCP_Send(socket, &tank->isMoving, 1);
  // SDLNet_TCP_Send(&socket, &tank->isOnTheWay, 1);
  // SDLNet_TCP_Send(&socket, &tank->moveDelay, 4);
}

void recvTankStruct(tank_t *tank, TCPsocket socket) {
  uint8_t request = 1;
  SDLNet_TCP_Send(socket, &request, 1);

  int8_t null;
  SDLNet_TCP_Recv(socket, &null, 1);
  if(null) {
    tank->null = 1;
    return;
  }

  tank->null = 0;
  // SDLNet_TCP_Send(&socket, &tank->driver, sizeof(driver_t));
  SDLNet_TCP_Recv(socket, &tank->direction, sizeof(direction_t));
  SDLNet_TCP_Recv(socket, &tank->posX, 4);
  SDLNet_TCP_Recv(socket, &tank->posY, 4);
  // SDLNet_TCP_Send(&socket, &tank->velocity, 1);
  SDLNet_TCP_Recv(socket, &tank->isMoving, 1);
  // SDLNet_TCP_Send(&socket, &tank->isOnTheWay, 1);
  // SDLNet_TCP_Send(&socket, &tank->moveDelay, 4);
}
