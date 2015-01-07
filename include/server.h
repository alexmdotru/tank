#pragma once

#include "framework.h"

typedef struct {
  SDLNet_SocketSet socketSet;
  int pCount;
  TCPsocket socket;
  TCPsocket pSocket[2];
  char *shost;
  int sport;
} server_t;
