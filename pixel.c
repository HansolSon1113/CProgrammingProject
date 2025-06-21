#include "pixel.h"

#include "entities.h"
#include "map.h"
#include "bool.h"

void Interaction(Position *orgPos, Position newPos, Position entitySize, Entity entity, Map *map)
{
    printf("Interaction Called\n");
    if (CheckScreenBoundary(newPos, entitySize, map->size))
        return;

    bool condition = true;
    if (orgPos->x < newPos.x)
    {
        for (int y = newPos.y - size.y / 2; y <= newPos.y + size.y / 2; ++y)
        {
            if (map->pixels[y][newPos.x + size.x / 2].entity != NONE)
            {
                condition = false;
                break;
            }
        }
    }
    else if (orgPos->x > newPos.x)
    {
        for (int y = newPos.y - size.y / 2; y <= newPos.y + size.y / 2; ++y)
        {
            if (map->pixels[y][newPos.x - size.x / 2].entity != NONE)
            {
                condition = false;
                break;
            }
        }
    }
    else if (orgPos->y < newPos.y)
    {
        for (int x = newPos.x - entitySize.x / 2; x <= newPos.x + entitySize.x / 2; ++x)
        {
            if (map->pixels[newPos.y + entitySize.y / 2][x].entity != NONE)
            {
                condition = false;
                break;
            }
        }
    }
    else if (orgPos->y > newPos.y)
    {
        for (int x = newPos.x - entitySize.x / 2; x <= newPos.x + entitySize.x / 2; ++x)
        {
            if (map->pixels[newPos.y - entitySize.y / 2][x].entity != NONE)
            {
                condition = false;
                break;
            }
        }
    }
    else
    {
        return;
    }

    if (condition)
    {
        printf("Moving\n");
        for (int y = orgPos->y - entitySize.y / 2; y <= orgPos->y + entitySize.y / 2; y++)
        {
            for (int x = orgPos->x - entitySize.x / 2; x <= orgPos->x + entitySize.x / 2; x++)
            {
                map->pixels[y][x].entity = NONE;
            }
        }

        *orgPos = newPos;
        for (int y = newPos.y - entitySize.y / 2; y <= newPos.y + entitySize.y / 2; y++)
        {
            for (int x = newPos.x - entitySize.x / 2; x <= newPos.x + entitySize.x / 2; x++)
            {
                map->pixels[y][x].entity = entity;
            }
        }
    }
}
