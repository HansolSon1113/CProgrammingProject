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

void Interaction(Player *player); //이동할때 빈자리 검증하는거 여기로 옮기기

#endif
