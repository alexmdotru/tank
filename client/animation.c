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
  for(i = 0; i < TANKS; i++) {
    client->graphics->tankAnim[i] = 0;
  }

  while(1) {
    for(i = 0; i < TANKS; i++) {
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
  for(i = 0; i < TANKS; i++) {
    client->tank[i].fire.explosionAnim = 0;
  }

  while(1) {
    for(i = 0; i < TANKS; i++) {
      if(client->tank[i].fire.explodes) {
        if(++client->tank[i].fire.explosionAnim == 3) {
          client->tank[i].fire.explosionAnim = 0;
          client->tank[i].fire.explodes = 0;
        }
      }
    }

    // Animation speed
    SDL_Delay(50);
  }
}
