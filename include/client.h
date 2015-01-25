#pragma once

#include "framework.h"
#include "map.h"
#include "graphics.h"
#include "tank.h"

#define KEYS 5
#define ENEMIES 20

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

typedef enum {
  BASE_DESTROYED,
  ENEMIES_KILLED,
  PLAYERS_KILLED
} result_t;

typedef struct {
  tank_t tank[6];
  uint8_t keyPressed[5];
  result_t result;
  uint8_t baseExplodes;
  // Tanks
  uint32_t tanks;
  // Server host and port
  char *host;
  int port;
  // Server socket
  TCPsocket socket;
  // Client status variables
  uint8_t gameRunning;
  uint8_t isInMainMenu;
  uint8_t isPlaying;
  uint8_t isGameOver;
  cstatus_t cstatus;
  // Player ID
  uint8_t myPlayerID;
  uint8_t hisPlayerID;
  // Level map
  uint8_t level;
  uint8_t base;
  int8_t winner;
  uint8_t enemiesKilled;
  map_t *map;
  // Graphics
  graphics_t *graphics;
} client_t;
