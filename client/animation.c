#include "graphics.h"
#include "client.h"

int waterAnimThread(void *data) {
  graphics_t *graphics = (graphics_t*)data;
  graphics->waterAnim = 0;

  while(1) {
    if(++graphics->waterAnim == 3) {
      graphics->waterAnim = 0;
    }

    // Animation speed
    SDL_Delay(333);
  }
}

int tankAnimThread(void *data) {
  client_t *client = (client_t*)data;

  size_t i;
  for(i = 0; i < 2; i++) {
    client->graphics->tankAnim[i] = 0;
  }

  while(1) {
    for(i = 0; i < 2; i++) {
      if(client->tank[i].isMoving) {
        if(++client->graphics->tankAnim[i] == 2) {
          client->graphics->tankAnim[i] = 0;
        }
      }
    }

    // Animation speed
    SDL_Delay(100);
  }
}

int explosionAnimThread(void *data) {
  client_t *client = (client_t*)data;

  size_t i;
  for(i = 0; i < client->tanks; i++) {
    client->tank[i].shot.explosionAnim = 0;
  }

  while(1) {
    for(i = 0; i < client->tanks; i++) {
      if(client->tank[i].shot.explodes) {
        if(++client->tank[i].shot.explosionAnim == 3) {
          client->tank[i].shot.explosionAnim = 0;
          client->tank[i].shot.explodes = 0;
        }
      }
    }

    // Animation speed
    SDL_Delay(50);
  }
}
