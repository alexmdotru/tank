#pragma once

#include "framework.h"
#include "map.h"
#include "graphics.h"
#include "tank.h"

#define KEYS 5

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
  KEY_RIGHT,
  KEY_SPACE
} keys_t;

typedef struct {
  int keyPressed[5];
  // Tanks
  tank_t tank[6];
  uint32_t tanks;
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
