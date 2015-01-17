#include "client.h"
#include "tank.h"
#include "server.h"

void randomizeSpawn(uint32_t *x, uint32_t *y);

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
        tank->moveDelay = 0;
      }
    }
  }
}

void updateEnemyTank(tank_t *tank, map_t *map) {
  uint8_t i, change;

  for(i = 2; i < 3; i++) {
    if(!tank->isOnTheWay) {
      // Do we want to change? 10%.
      change = rand() % 10;

      if(!change) {
        // Choose new direction
        change = rand() % 4;

        switch(change) {
          case 0:
          tank->direction = UP;
          break;

          case 1:
          tank->direction = DOWN;
          break;

          case 2:
          tank->direction = LEFT;
          break;

          case 3:
          tank->direction = RIGHT;
          break;
        }
      }
      tank->isMoving = 1;
      if(!checkCollision(tank, map)) {
        tank->isOnTheWay = 1;
      }
    }
  }
}

int checkCollision(tank_t *tank, map_t *map) {
  // Calculate tank position
  uint32_t x, y;
  x = tank->posX / 16;
  y = tank->posY / 16;

  switch(tank->direction) {
    case UP:
    if(tank->posY == 0) return 1;
    if(map->block[y-1][x].material   != TERRA) return 1;
    if(map->block[y-1][x+1].material != TERRA) return 1;
    break;

    case DOWN:
    if(tank->posY == 384) return 1;
    if(map->block[y+2][x].material   != TERRA) return 1;
    if(map->block[y+2][x+1].material != TERRA) return 1;
    break;

    case LEFT:
    if(tank->posX == 0) return 1;
    if(map->block[y][x-1].material   != TERRA) return 1;
    if(map->block[y+1][x-1].material != TERRA) return 1;
    break;

    case RIGHT:
    if(tank->posX == 384) return 1;
    if(map->block[y][x+2].material   != TERRA) return 1;
    if(map->block[y+1][x+2].material != TERRA) return 1;
    break;
  }

  return 0;
}

void spawnEnemy(server_t *server) {
  uint8_t i;
  uint32_t x, y;

  if(server->enemies < ENEMIES && SDL_GetTicks() > server->enemiesDelay) {
    randomizeSpawn(&x, &y);

    for(i = 2; i < TANKS; i++) {
      if(server->tank[i].null) {
        server->tank[i] = (tank_t) { ENEMY, DOWN, x, y, 1, 0, 0, 0, 0 };
        server->enemies++;
        break;
      }
    }

    server->enemiesDelay = SDL_GetTicks() + 3000;
    fprintf(stderr, "Spawned enemy id %d at x: %d y: %d.\n", i, x, y);
  }
}

void randomizeSpawn(uint32_t *x, uint32_t *y) {
  uint8_t spawn;
  spawn = rand() % 3;

  switch(spawn) {
    case 0:
    *x = 0;   *y = 0;
    break;

    case 1:
    *x = 192; *y = 0;
    break;

    case 2:
    *x = 384; *y = 0;
    break;
  }
}
