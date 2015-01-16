#include "client.h"
#include "tank.h"

void updatePlayerTank(client_t *client) {
  tank_t *tank = &client->tank[client->myPlayerID];
  map_t *map = client->map;

  if(!tank->isOnTheWay) {
    // Move up and turn
    if(client->keyPressed[KEY_UP]) {
      tank->isMoving = 1;

      if(client->keyPressed[KEY_DOWN]) {
        tank->isMoving = 0;
      }
      else if(client->keyPressed[KEY_LEFT]) {
        tank->direction = LEFT;
      }
      else if(client->keyPressed[KEY_RIGHT]) {
        tank->direction = RIGHT;
      }
      else tank->direction = UP;
    }

    // Move down and turn
    else if(client->keyPressed[KEY_DOWN]) {
      tank->isMoving = 1;

      if(client->keyPressed[KEY_LEFT]) {
        tank->direction = LEFT;
      }
      else if(client->keyPressed[KEY_RIGHT]) {
        tank->direction = RIGHT;
      }
      else tank->direction = DOWN;
    }

    // Turn left
    else if(client->keyPressed[KEY_LEFT]) {
      tank->isMoving = 1;

      if(client->keyPressed[KEY_RIGHT]) {
        tank->isMoving = 0;
      }
      else tank->direction = LEFT;
    }

    // Turn right
    else if(client->keyPressed[KEY_RIGHT]) {
      tank->isMoving = 1;
      tank->direction = RIGHT;
    }

    // Stop
    else tank->isMoving = 0;

    if(tank->isMoving && !checkCollision(tank, map)) {
      tank->isOnTheWay = 1;
      tank->moveDelay = 0;
    }
  }
}

void moveTank(tank_t *tank) {
  if(tank->isOnTheWay) {
    if(SDL_GetTicks() > tank->moveDelay) {
      switch(tank->direction) {
        case UP:
        tank->posY -= tank->velocity;
        break;

        case DOWN:
        tank->posY += tank->velocity;
        break;

        case LEFT:
        tank->posX -= tank->velocity;
        break;

        case RIGHT:
        tank->posX += tank->velocity;
        break;
      }

      tank->moveDelay = SDL_GetTicks() + 5;
      if(++tank->isOnTheWay == 17) {
        tank->isOnTheWay = 0;
      }
    }
  }
}

// void moveEnemyTank() {
//   uint8_t i;
//   for(i = 2; i < tanks; i++) {
//   if(!tank->isOnTheWay) {
//   uint32_t whereTo = rand() % 5;
// }
//   switch(whereTo)
//   // Decide where to go
// }
//
// }

int checkCollision(tank_t *tank, map_t *map) {
  // Calculate tank position
  uint32_t x, y;
  x = tank->posX / 16;
  y = tank->posY / 16;

  switch(tank->direction) {
    case UP:
    if(map->block[y-1][x].material   != TERRA) return 1;
    if(map->block[y-1][x+1].material != TERRA) return 1;
    break;

    case DOWN:
    if(map->block[y+2][x].material   != TERRA) return 1;
    if(map->block[y+2][x+1].material != TERRA) return 1;
    break;

    case LEFT:
    if(map->block[y][x-1].material   != TERRA) return 1;
    if(map->block[y+1][x-1].material != TERRA) return 1;
    break;

    case RIGHT:
    if(map->block[y][x+2].material   != TERRA) return 1;
    if(map->block[y+1][x+2].material != TERRA) return 1;
    break;
  }

  return 0;
}
