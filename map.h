#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include "pixel.h"
#include "loader.h"

// Map and items on the map
typedef struct Map
{
    Pixel **pixels;
    Position size;
} Map;

Map *LoadMap(void);

#endif
