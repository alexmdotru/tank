#include <stdio.h>
#include "framework.h"
#include "client.h"
#include "map.h"

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

  SDLNet_TCP_Send(socket, &tank->driver, sizeof(driver_t));
  SDLNet_TCP_Send(socket, &tank->direction, sizeof(direction_t));
  SDLNet_TCP_Send(socket, &tank->posX, 4);
  SDLNet_TCP_Send(socket, &tank->posY, 4);
  SDLNet_TCP_Send(socket, &tank->isMoving, 1);
  SDLNet_TCP_Send(socket, &tank->isOnTheWay, 1);
  SDLNet_TCP_Send(socket, &tank->moveDelay, 4);
  SDLNet_TCP_Send(socket, &tank->isFiring, 1);
  SDLNet_TCP_Send(socket, &tank->null, 1);
  SDLNet_TCP_Send(socket, &tank->destrBlock, 2);
  SDLNet_TCP_Send(socket, &tank->id, 2);
  SDLNet_TCP_Send(socket, &tank->winsTheGame, 1);
  SDLNet_TCP_Send(socket, &tank->fire.posX, 4);
  SDLNet_TCP_Send(socket, &tank->fire.posY, 4);
  SDLNet_TCP_Send(socket, &tank->fire.explodes, 1);
  SDLNet_TCP_Send(socket, &tank->fire.direction, sizeof(direction_t));
  SDLNet_TCP_Send(socket, &tank->block[0], 1);
  SDLNet_TCP_Send(socket, &tank->block[1], 1);
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
  SDLNet_TCP_Recv(socket, &tank->driver, sizeof(driver_t));
  SDLNet_TCP_Recv(socket, &tank->direction, sizeof(direction_t));
  SDLNet_TCP_Recv(socket, &tank->posX, 4);
  SDLNet_TCP_Recv(socket, &tank->posY, 4);
  SDLNet_TCP_Recv(socket, &tank->isMoving, 1);
  SDLNet_TCP_Recv(socket, &tank->isOnTheWay, 1);
  SDLNet_TCP_Recv(socket, &tank->moveDelay, 4);
  SDLNet_TCP_Recv(socket, &tank->isFiring, 1);
  SDLNet_TCP_Recv(socket, &tank->null, 1);
  SDLNet_TCP_Recv(socket, &tank->destrBlock, 2);
  SDLNet_TCP_Recv(socket, &tank->id, 2);
  SDLNet_TCP_Recv(socket, &tank->winsTheGame, 1);
  SDLNet_TCP_Recv(socket, &tank->fire.posX, 4);
  SDLNet_TCP_Recv(socket, &tank->fire.posY, 4);
  SDLNet_TCP_Recv(socket, &tank->fire.explodes, 1);
  SDLNet_TCP_Recv(socket, &tank->fire.direction, sizeof(direction_t));
  SDLNet_TCP_Recv(socket, &tank->block[0], 1);
  SDLNet_TCP_Recv(socket, &tank->block[1], 1);
}
