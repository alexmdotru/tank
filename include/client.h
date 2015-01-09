#pragma once

#include "framework.h"
#include "map.h"
#include "graphics.h"
#include "tank.h"

typedef enum {
  CONNECTING,
  CTIMEOUT,
  WAITING2ND,
  CONNECTED
} cstatus_t;

typedef enum {
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT
} keys_t;

typedef struct {
  int keyPressed[4];
  // Tanks
  tank_t tank[2];
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
  int myPlayerID;
  int hisPlayerID;
  // Level map
  int level;
  map_t *map;
  // Graphics
  graphics_t *graphics;
} client_t;
