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
      updateTankOnMap(tank, map);
    }
  }

  if(client->keyPressed[KEY_SPACE]) {
    fire(tank);
    client->keyPressed[KEY_SPACE] = 0;
  }
}

void moveTank(tank_t *tank, map_t *map) {
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

void fire(tank_t *tank) {
  if(!tank->isFiring && !tank->fire.explodes) {
    tank->isFiring = 1;
    tank->fire.posX = (tank->posX / 16) * 16;
    tank->fire.posY = (tank->posY / 16) * 16;
    tank->fire.direction = tank->direction;
  }
}

void updateEnemyTank(tank_t *tank, map_t *map) {
  // 0 — don't know
  // 1 — change
  // 2 - don't change
  uint8_t change = 0, random;

  if(!tank->isOnTheWay) {
    // Go DOWN to the BASE
    if(tank->direction == DOWN) {
      if(checkCollision(tank, map) == BRICK || !checkCollision(tank, map)) {
        change = 2;
      }
    }

    // Change if the collision is detected (not a BRICK)
    if(checkCollision(tank, map) && checkCollision(tank, map) != BRICK) {
      change = 1;
    }

    // Decide if we need to change
    random = rand() % 100;

    if(change == 1 || (change == 0 && !random)) {
      // Choose new direction
      random = rand() % 4;

      switch(random) {
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

    if(!checkCollision(tank, map)) {
      tank->isOnTheWay = 1;
      updateTankOnMap(tank, map);
    }
  }

  // Do we want to shoot?
  random = rand() % 25;
  if(!random) fire(tank);
}

void updateFire(tank_t *tank, map_t *map) {
  if(tank->isFiring) {
    // Calculate fire position
    uint32_t blockX, blockY;
    blockX = tank->fire.posX / 16;
    blockY = tank->fire.posY / 16;

    switch(tank->fire.direction) {
      case UP:
      if(tank->fire.posY < 16) {
        tank->fire.posY = 0;
        tank->isFiring = 0;
        tank->fire.explodes = 1;
      }
      else if(tank->fire.posY % 16 == 0) {
        if(!checkBlock(blockY-1, blockX+1, tank, map))
          checkBlock(blockY-1, blockX, tank, map);
      }
      tank->fire.posY -= 4 * tank->velocity;
      break;

      case DOWN:
      if(tank->fire.posY >= 384) {
        tank->isFiring = 0;
        tank->fire.explodes = 1;
      }
      else if(tank->fire.posY % 16 == 0) {
        if(!checkBlock(blockY+2, blockX, tank, map))
          checkBlock(blockY+2, blockX+1, tank, map);
      }
      tank->fire.posY += 4 * tank->velocity;
      break;

      case LEFT:
      if(tank->fire.posX < 16) {
        tank->fire.posX = 0;
        tank->isFiring = 0;
        tank->fire.explodes = 1;
      }
      else if(tank->fire.posX % 16 == 0) {
        if(!checkBlock(blockY, blockX-1, tank, map))
          checkBlock(blockY+1, blockX-1, tank, map);
      }
      tank->fire.posX -= 4 * tank->velocity;
      break;

      case RIGHT:
      if(tank->fire.posX >= 384) {
        tank->isFiring = 0;
        tank->fire.explodes = 1;
      }
      else if(tank->fire.posX % 16 == 0) {
        if(!checkBlock(blockY+1, blockX+2, tank, map))
          checkBlock(blockY, blockX+2, tank, map);
      }
      tank->fire.posX += 4 * tank->velocity;
      break;
    }
  }
}

int checkBlock(uint32_t i, uint32_t j, tank_t *tank, map_t *map) {
  if(map->block[i][j].material == BRICK) {
    tank->destrBlock = (i * 26) + j;
    tank->isFiring = 0;
    tank->fire.explodes = 1;
    return 1;
  }

  if(map->block[i][j].material == STEEL || map->block[i][j].material == STEEL) {
    tank->isFiring = 0;
    tank->fire.explodes = 1;
    return 1;
  }

  if(map->block[i][j].material == BASE) {
    map->block[24][12].material = STEEL;
    map->block[24][13].material = STEEL;
    map->block[25][12].material = STEEL;
    map->block[25][13].material = STEEL;
    tank->isFiring = 0;
    tank->destroyedBase = 1;
    fprintf(stderr, "Game over!\n");
    return 1;
  }

  if(tank->driver == PLAYER && map->block[i][j].material >= TANK2) {
    tank->destrTank = map->block[i][j].material - 10;
    tank->isFiring = 0;
    fprintf(stderr, "Enemy %d destroyed!\n", tank->destrTank);
    return 1;
  }

  if(tank->driver == ENEMY && (map->block[i][j].material == TANK0 || map->block[i][j].material == TANK1)) {
    tank->destrTank = map->block[i][j].material - 10;
    tank->isFiring = 0;
    fprintf(stderr, "Player %d destroyed!\n", tank->destrTank + 1);
    return 1;
  }

  return 0;
}

int checkCollision(tank_t *tank, map_t *map) {
  // Calculate tank position
  uint32_t x, y;
  x = tank->posX / 16;
  y = tank->posY / 16;

  switch(tank->direction) {
    case UP:
    if(tank->posY == 0) return 1;
    if(map->block[y-1][x].material   == BRICK) return BRICK;
    if(map->block[y-1][x+1].material == BRICK) return BRICK;
    if(map->block[y-1][x].material   != TERRA) return 1;
    if(map->block[y-1][x+1].material != TERRA) return 1;
    break;

    case DOWN:
    if(tank->posY == 384) return 1;
    if(map->block[y+2][x].material   == BRICK) return BRICK;
    if(map->block[y+2][x+1].material == BRICK) return BRICK;
    if(map->block[y+2][x].material   != TERRA) return 1;
    if(map->block[y+2][x+1].material != TERRA) return 1;
    break;

    case LEFT:
    if(tank->posX == 0) return 1;
    if(map->block[y][x-1].material   == BRICK) return BRICK;
    if(map->block[y+1][x-1].material == BRICK) return BRICK;
    if(map->block[y][x-1].material   != TERRA) return 1;
    if(map->block[y+1][x-1].material != TERRA) return 1;
    break;

    case RIGHT:
    if(tank->posX == 384) return 1;
    if(map->block[y][x+2].material   != TERRA) return BRICK;
    if(map->block[y+1][x+2].material != TERRA) return BRICK;
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
        server->tank[i] = (tank_t) { ENEMY, DOWN, x, y, 1, 1, 1, 0, 0, 0, -1, -1, i, 0, 0 };
        fprintf(stderr, "Spawned Enemy %d at X %d Y %d\n", ++server->enemies, x, y);
        break;
      }
    }
    server->enemiesDelay = SDL_GetTicks() + 3000;
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

void updateTankOnMap(tank_t *tank, map_t *map) {
  uint8_t i, j;

  for(i = 0; i < MAP_SIZE; i++) {
    for(j = 0; j < MAP_SIZE; j++) {
      if(map->block[i][j].material == tank->id + 10) {
        map->block[i][j].material = TERRA;
      }
    }
  }

  // Calculate tank position
  uint32_t x, y;
  x = tank->posX / 16;
  y = tank->posY / 16;

  if(tank->isOnTheWay) {
    switch(tank->direction) {
      case UP:    y -= 1; break;
      case DOWN:  y += 1; break;
      case LEFT:  x -= 1; break;
      case RIGHT: x += 1; break;
    }
  }
  tank->block[0] = x;
  tank->block[1] = y;

  map->block[y][x].material = tank->id + 10;
  map->block[y][x+1].material = tank->id + 10;
  map->block[y+1][x].material = tank->id + 10;
  map->block[y+1][x+1].material = tank->id + 10;
}

void updateTanksOnMap(tank_t *tank, map_t *map) {
  uint8_t i, j;

  for(i = 0; i < MAP_SIZE; i++) {
    for(j = 0; j < MAP_SIZE; j++) {
      if(map->block[i][j].material == tank->id + 10) {
        map->block[i][j].material = TERRA;
      }
    }
  }

  uint32_t x, y;
  x = tank->block[0];
  y = tank->block[1];

  map->block[y][x].material = tank->id + 10;
  map->block[y][x+1].material = tank->id + 10;
  map->block[y+1][x].material = tank->id + 10;
  map->block[y+1][x+1].material = tank->id + 10;
}

void freeMapFromTank(uint8_t id, map_t *map) {
  uint8_t i, j;

  for(i = 0; i < MAP_SIZE; i++) {
    for(j = 0; j < MAP_SIZE; j++) {
      if(map->block[i][j].material == id + 10) {
        map->block[i][j].material = TERRA;
      }
    }
  }
}
