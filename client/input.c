#include "framework.h"
#include "client.h"

void processInput(client_t *client) {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
      client->gameRunning = 0;
      break;

      case SDL_KEYDOWN:
      if(!event.key.repeat) {
        switch(event.key.keysym.sym) {
          case SDLK_UP:
          client->keyPressed[KEY_UP] = 1;
          break;

          case SDLK_DOWN:
          client->keyPressed[KEY_DOWN] = 1;
          break;

          case SDLK_LEFT:
          client->keyPressed[KEY_LEFT] = 1;
          break;

          case SDLK_RIGHT:
          client->keyPressed[KEY_RIGHT] = 1;
          break;

          case SDLK_SPACE:
          client->keyPressed[KEY_SPACE] = 1;
          break;
        }
      }
      break;

      case SDL_KEYUP:
      switch(event.key.keysym.sym) {
        case SDLK_UP:
        client->keyPressed[KEY_UP] = 0;
        break;

        case SDLK_DOWN:
        client->keyPressed[KEY_DOWN] = 0;
        break;

        case SDLK_LEFT:
        client->keyPressed[KEY_LEFT] = 0;
        break;

        case SDLK_RIGHT:
        client->keyPressed[KEY_RIGHT] = 0;
        break;

        case SDLK_SPACE:
        client->keyPressed[KEY_SPACE] = 0;
        break;
      }
      break;
    }
  }
}
