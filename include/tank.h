#pragma once

#include "map.h"

#define TANKS 6

typedef enum {
  PLAYER,
  ENEMY
} driver_t;

typedef enum {
  UP = 0,
  DOWN = 180,
  LEFT = 270,
  RIGHT = 90
} direction_t;

typedef struct {
  uint32_t posX;
  uint32_t posY;
  uint8_t explodes;
  uint8_t explosionAnim;
  direction_t direction;
} fire_t;

typedef struct {
  driver_t driver;
  direction_t direction;
  uint32_t posX;
  uint32_t posY;
  uint8_t velocity;
  uint8_t isMoving;
  uint8_t isOnTheWay;
  uint32_t moveDelay;
  uint8_t isFiring;
  uint8_t null;
  int16_t destrBlock;
  int8_t destrTank;
  uint8_t id;
  uint8_t destroyedBase;
  uint8_t explodes;
  uint32_t explDelay;
  uint8_t explAnim;
  fire_t fire;
  uint8_t block[2];
} tank_t;

void updateEnemyTank(tank_t *tank, map_t *map);
void moveTank(tank_t *tank, map_t *map);
int checkCollision(tank_t *tank, map_t *map);
void updateFire(tank_t *tank, map_t *map);
void updateTankOnMap(tank_t *tank, map_t *map);
void updateTanksOnMap(tank_t *tank, map_t *map);
int checkBlock(uint32_t i, uint32_t j, tank_t *tank, map_t *map);
void fire(tank_t *tank);
void freeMapFromTank(uint8_t id, map_t *map);
