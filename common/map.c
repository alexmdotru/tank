#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"
#include "tank.h"

map_t *loadMap(int level) {
  map_t *map;

  // Process filename
  char filename[256];
  char levelStr[256];
  sprintf(levelStr, "%d", level);
  strcpy(filename, "resources/maps/level");
  strcat(filename, levelStr);
  strcat(filename, ".map");

  map = malloc(sizeof(map_t));

  // Open map file
  FILE *file;
  file = fopen(filename, "r");

  if(file) {
    int i, j;
    char row[MAP_SIZE];

    for(i = 0; i < MAP_SIZE; i++) {
      // Read row
      fread(row, 1, MAP_SIZE, file);

      // Process blocks
      for(j = 0; j < MAP_SIZE; j++) {
        switch(row[j]) {
          case 't':
          map->block[i][j].material = TERRA; break;

          case 'b':
          map->block[i][j].material = BRICK; break;

          case 's':
          map->block[i][j].material = STEEL; break;

          case 'g':
          map->block[i][j].material = GRASS; break;

          case 'w':
          map->block[i][j].material = WATER; break;
        }
      }
      fgetc(file);
    }
  }
  else {
    fprintf(stderr, "Error reading file %s\n", filename);
  }

  map->block[24][12].material = BASE;
  map->block[24][13].material = BASE;
  map->block[25][12].material = BASE;
  map->block[25][13].material = BASE;

  return map;
}

void updateMap(map_t *map, tank_t *tank) {
  if(!tank->null && tank->destrBlock >= 0) {
    uint32_t i, j;
    i = tank->destrBlock / 26;
    j = tank->destrBlock % 26;
    map->block[i][j].material = TERRA;

    tank->destrBlock = -1;
  }
}
