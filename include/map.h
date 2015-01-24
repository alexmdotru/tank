#pragma once

#define MAP_SIZE 26

typedef enum {
  TERRA,
  BRICK,
  STEEL,
  GRASS,
  WATER,
  BASE,
  TANK0 = 10,
  TANK1 = 11,
  TANK2 = 12,
  TANK3 = 13,
  TANK4 = 14,
  TANK5 = 15,
} material_t;

typedef struct {
  material_t material;
} block_t;

typedef struct {
  block_t block[26][26];
} map_t;

map_t *loadMap(int level);
