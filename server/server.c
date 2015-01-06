#include <stdlib.h>
#include <stdio.h>
#include "framework.h"

int main(int argc, char **argv) {
  // Check command line
  if(argc != 2) {
    fprintf(stderr, "usage:\t%s port", argv[0]);
    exit(1);
  }

  initFramework();

  SDLNet_Quit();
  SDL_Quit();
  return 0;
}
