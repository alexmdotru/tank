#pragma once

#include "framework.h"
#include "tank.h"
#include "map.h"

#define ENEMIES 4

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
  // Enemies
  uint8_t enemies;
  uint32_t enemiesDelay;
  // Tanks
  tank_t tank[6];
  // Map
  map_t *map;
} server_t;
