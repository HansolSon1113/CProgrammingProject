#include "loader.h"

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "player.h"

void LoadFromFile(FILE *fp, LoadContent content, Entity type)
{
    int c = 0;
    for (int y = 0; c != EOF; y++)
    {
        for (int x = 0; (c = fgetc(fp)) != '\n'; x++)
        {
#ifdef DEBUG
            printf("y %d x %d c %c\n", y, x, c);
#endif
            if (c == EOF)
            {
                break;
            }
            else
            {
                Position location = {x, y};
                switch (type)
                {
                case TERRAIN:
                    ToMap(c, content.map, location);
                    break;
                case PLAYER:
                    ToPlayer(c, content.player, location);
                    break;
                case ENEMY:
                    ToEnemy(c, content.enemyArray, location);
                    break;
                }
            }
        }
    }
    fclose(fp);
}

void ToMap(char c, Map *map, Position loc)
{
    switch (loc.y / map->size.y)
    {
    case 0:
        map->pixels[loc.y][loc.x].c = c;
        break;
    case 1:
        loc.y -= map->size.y;
        if (c == ' ')
        {
            map->pixels[loc.y][loc.x].entity = NONE;
        }
        else if (c == 'C')
        {
            map->pixels[loc.y][loc.x].entity = CLEAR;
        }
        else if (c == '^')
        {
            map->pixels[loc.y][loc.x].entity = DAMAGE_AREA;
        }
        else
        {
            map->pixels[loc.y][loc.x].entity = TERRAIN;
        }
        break;
    default:
        fprintf(stderr, "ERR: Saved map format is not valid!\n");
        exit(2);
    }
}

void ToPlayer(char c, Player *player, Position loc)
{
    int animIndex = loc.y / player->size.y;

    switch (animIndex)
    {
    case 0:
        player->anim.idle[loc.y][loc.x] = c;
        break;
    case 1:
        loc.y += player->size.y;
    case 2:
        loc.y += player->size.y;
    case 3:
        loc.y -= player->size.y * 3;
        player->anim.anim[0][animIndex - 1][loc.y][loc.x] = c;
        break;
    case 4:
        loc.y += player->size.y;
    case 5:
        loc.y += player->size.y;
    case 6:
        loc.y -= player->size.y * 6;
        player->anim.anim[1][animIndex - 4][loc.y][loc.x] = c;
        break;
    default:
        fprintf(stderr, "ERR: Saved player format is not valid!\n");
        exit(2);
    }
}

void ToEnemy(char c, EnemyArray *enemyArray, Position loc)
{
    enemyArray->sprite[loc.y][loc.x] = c;
}
