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
  fire_t fire;
} tank_t;

void updateEnemyTank(tank_t *tank, map_t *map);
void moveTank(tank_t *tank);
int checkCollision(tank_t *tank, map_t *map);
void updateFire(tank_t *tank, map_t *map);
void destroyBlock(tank_t *tank, uint8_t i, uint8_t j);
