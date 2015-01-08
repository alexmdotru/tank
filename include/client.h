#pragma once

#include "framework.h"
#include "map.h"
#include "graphics.h"

typedef enum {
  CONNECTING,
  CTIMEOUT,
  WAITING2ND,
  CONNECTED
} cstatus_t;

typedef struct {
  // Server host and port
  char *host;
  int port;
  // Server socket
  TCPsocket socket;
  // Client status variables
  int gameRunning;
  int isInMainMenu;
  int isPlaying;
  cstatus_t cstatus;
  // Player ID
  int playerID;
  // Level map
  int level;
  map_t *map;
  // Graphics
  graphics_t *graphics;
} client_t;
