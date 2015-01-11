#pragma once

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
  driver_t driver;
  direction_t direction;
  int posX;
  int posY;
  int velocity;
  int isMoving;
  int isOnTheWay;
} tank_t;
