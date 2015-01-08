#include <stdlib.h>
#include <stdio.h>
#include "map.h"

map_t *loadMap(char *filename) {
  map_t *map;
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

  return map;
}
