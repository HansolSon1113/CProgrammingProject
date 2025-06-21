#include "pixel.h"

#include "entities.h"
#include "map.h"
#include "bool.h"

void Interaction(Position *orgPos, Position targetPos, Position entitySize, Entity entity, Map *map)
{
    Position diff = {0, 0};

    while (orgPos->x != targetPos.x || orgPos->y != targetPos.y)
    {
        diff.x = orgPos->x < targetPos.x ? 1 : (orgPos->x > targetPos.x ? -1 : 0);
        diff.y = orgPos->y < targetPos.y ? 1 : (orgPos->y > targetPos.y ? -1 : 0);

        Position newPos = {orgPos->x + diff.x, orgPos->y + diff.y};

        if (CheckScreenBoundary(newPos, entitySize, map->size))
            return;

        for (int y = newPos.y - entitySize.y / 2; y <= newPos.y + entitySize.y / 2; y++)
        {
            for (int x = newPos.x - entitySize.x / 2; x <= newPos.x + entitySize.x / 2; x++)
            {
                if (map->pixels[y][x].entity != NONE && map->pixels[y][x].entity != entity)
                {
                    return;
                }
            }
        }

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