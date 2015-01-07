#pragma once

#include "framework.h"

typedef struct {
  Uint8 playerID;
  TCPsocket socket;
  TCPsocket pSocket[2];
  char *shost;
  int sport;
  int serverRunning;
  int level;
} server_t;
