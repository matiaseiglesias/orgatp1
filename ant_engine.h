#ifndef __ANT_ENGINE_H__
#define __ANT_ENGINE_H__

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "ant_constants.h"

typedef enum colour {RED = CR, GREEN = CG, BLUE = CB, YELLOW = CY, BLACK = CN, WHITE = CW} colour_t;
typedef enum orientation {NORTH = ON, SOUTH = OS, EAST = OE, WEST = OW} orientation_t;
typedef enum rotation {LEFT = RL, RIGHT = RR} rotation_t;

void* paint(void *ant, void *grid, void *palette, void *rules,  uint32_t iterations);

#define panic(s)       \
  do {                 \
    fprintf(stderr, "%s: %s\n", __FUNCTION__, s);\
    exit(1);            \
  } while(0);


#endif /* __ANT_ENGINE_H__ */

