#pragma once

#define MAP_SIZE 26

typedef enum {
  TERRA,
  BRICK,
  STEEL,
  GRASS,
  WATER
} material_t;

typedef struct {
  material_t material;
} block_t;

typedef struct {
  block_t block[26][26];
} map_t;

map_t *loadMap(char *filename);
