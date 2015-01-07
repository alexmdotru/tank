#pragma once

#include "framework.h"

typedef struct {
  SDLNet_SocketSet socketSet;
  Uint8 pCount;
  TCPsocket socket;
  TCPsocket pSocket[2];
  char *shost;
  int sport;
} server_t;
