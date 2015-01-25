#pragma once

#include "framework.h"

#define WINDOW_WIDTH  512
#define WINDOW_HEIGHT 480
#define BASE_X 192
#define BASE_Y 384

typedef struct {
  SDL_Texture *tex;
  int width;
  int height;
} texture_t;

typedef struct {
  // Main frame
  SDL_Window *window;
  SDL_Renderer *renderer;
  // Texture bank
  SDL_Surface *textures;
  // Labels
  texture_t *logo1;
  texture_t *logo2;
  texture_t *lcing;
  texture_t *lcout;
  texture_t *lw2nd;
  texture_t *lcned;
  texture_t *victory;
  texture_t *defeat;
  texture_t *au1;
  texture_t *au2;
  texture_t *killed1;
  texture_t *killed2;
  texture_t *killed3;
  texture_t *killed4;
  texture_t *killed5;
  // Brick
  texture_t *brick;
  // Steel
  texture_t *steel;
  // Water
  texture_t *water[3];
  int waterAnim;
  // Tanks
  texture_t *tank[3][2];
  uint8_t tankAnim[6];
  // HUD
  texture_t *enemy;
  // Fire
  texture_t *fire;
  // Explosion
  texture_t *explosion[5];
  uint8_t baseAnim;
  uint32_t baseDelay;
  // Base
  texture_t *base[2];
  // Map viewport
  SDL_Rect mapView;
  // HUD viewport
  SDL_Rect hudView;
} graphics_t;

void loadFrame(graphics_t *graphics);
void freeFrame(graphics_t *graphics);
void freeResources(graphics_t *graphics);
