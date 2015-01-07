#pragma once

#include "client.h"

#define WINDOW_WIDTH  512
#define WINDOW_HEIGHT 480

void loadFrame(client_t *client);
void loadResources(client_t *client);
void renderMenu(client_t *client);
void render(client_t *client);
