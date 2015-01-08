#pragma once

#include "framework.h"

typedef struct {
  // Server host and port
  char *host;
  int port;

  // Level to play
  int level;

  // Server and client sockets
  TCPsocket sSocket;
  TCPsocket cSocket[2];

  int serverRunning;

  // Current player id
  int playerID;
} server_t;
