#pragma once

#include "framework.h"

typedef struct {
  SDL_Texture *tex;
  int width;
  int height;
} texture_t;

typedef enum {
  CONNECTING,
  CTIMEOUT,
  WAITING2ND,
  CONNECTED
} cstatus_t;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  char *shost;
  int sport;
  cstatus_t cstatus;
  TCPsocket socket;
  // Textures
  texture_t *logo1;
  texture_t *logo2;
  texture_t *lcing;
  texture_t *lcout;
  texture_t *lw2nd;
  texture_t *lcned;
  // Water
  texture_t *water[3];
  int waterAnim;
  SDL_Surface *textures;
  int gameRunning;
  int isPlaying;
  int isInMainMenu;
  Uint8 id;
  int level;
} client_t;
