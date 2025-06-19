//
//  pixel.c
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

#include "pixel.h"

#include "player.h"
#include "entities.h"
#include "map.h"

void Interaction(Position *orgPos, Position newPos, Position entitySize, Entity entity, Map *map)
{
    if(CheckScreenBoundary(newPos, entitySize, size)) return;
    
    if(map -> pixels[newPos.y][newPos.x].entity == NONE)
    {
        for(int y = orgPos -> y - entitySize.y/2; y <= orgPos -> y + entitySize.y / 2; y++)
        {
            for(int x = orgPos -> x - entitySize.x/2; x <= orgPos -> x + entitySize.x / 2; x++)
            {
                map -> pixels[y][x].entity = NONE;
            }
        }
        
        *orgPos = newPos;
        for(int y = newPos.y - entitySize.y/2; y <= newPos.y + entitySize.y / 2; y++)
        {
            for(int x = newPos.x - entitySize.x/2; x <= newPos.x + entitySize.x / 2; x++)
            {
                map -> pixels[y][x].entity = entity;
            }
        }
    }
}
