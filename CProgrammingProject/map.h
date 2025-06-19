//
//  map.h
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

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
} Map;

Map *LoadMap(void);

#endif
