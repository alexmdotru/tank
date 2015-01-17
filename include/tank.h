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
  int posX;
  int posY;
  uint8_t explodes;
  uint8_t explosionAnim;
} shot_t;

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
  shot_t shot;
  uint8_t null;
} tank_t;

void updateEnemyTank(tank_t *tank, map_t *map);
void moveTank(tank_t *tank);
int checkCollision(tank_t *tank, map_t *map);
