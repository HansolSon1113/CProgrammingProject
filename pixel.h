#ifndef PIXEL_H
#define PIXEL_H

#include "entities.h"

typedef struct Map Map;
typedef struct Item Item;

#define CheckScreenBoundary(pos, entitySize, screenSize) ( \
    ( (pos).x < (entitySize).x / 2 + 1 ) || \
    ( (pos).x > (screenSize).x - ((entitySize).x / 2 + 1) ) || \
    ( (pos).y < (entitySize).y / 2 + 1 ) || \
    ( (pos).y > (screenSize).y - ((entitySize).y / 2 + 1) ) \
) ? 0 : 1

// Position on screen
typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    char c;
    Entity entity;
    Item *item;
} Pixel;

extern const Position size;


void Interaction(Position *orgPos, Position newPos, Position size, Entity entity, Map *map);

#endif
