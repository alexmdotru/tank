#pragma once

#define WINDOW_WIDTH  512
#define WINDOW_HEIGHT 480

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
  // Brick
  texture_t *brick;
  // Steel
  texture_t *steel;
  // Water
  texture_t *water[3];
  int waterAnim;
  // Tanks
  texture_t *tank[2][2];
  int tankAnim[2];
  // Map viewport
  SDL_Rect mapView;
} graphics_t;

void loadFrame(graphics_t *graphics);
void freeFrame(graphics_t *graphics);
void freeResources(graphics_t *graphics);
