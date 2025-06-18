//
//  pixel.c
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

#include "pixel.h"

#include "player.h"
#include "entities.h"

void Interaction(Position *orgPos, Position newPos, Position entitySize, Entity entity, Map *map)
{
    if(CheckScreenBoundary(newPos, entitySize, size)) return;
    
    if(map -> pixels[(size.x * newPos.y) + newPos.x].entity == NONE)
    {
        for(int y = -entitySize.y/2; y < entitySize.y / 2; y++)
        {
            for(int x = -entitySize.x/2; x < entitySize.x / 2; x++)
            {
                map -> pixels[(size.x * (orgPos -> y + y)) + (orgPos -> x + x)].entity = NONE;
            }
        }
        
        *orgPos = newPos;
        for(int y = -entitySize.y/2; y < entitySize.y / 2; y++)
        {
            for(int x = -entitySize.x/2; x < entitySize.x / 2; x++)
            {
                map -> pixels[(size.x * (newPos.y + y)) + (newPos.x + x)].entity = entity;
            }
        }
    }
}
