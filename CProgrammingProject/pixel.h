//
//  pixel.h
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

#ifndef PIXEL_H
#define PIXEL_H

#include "player.h"
#include "entities.h"
#include "map.h"

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
    Item item;
} Pixel;

const Position size = {10, 10};

void Interaction(Position *orgPos, Position newPos, Position size, Entity entity, Map *map); //이동할때 빈자리 검증하는거 여기로 옮기기

#endif
