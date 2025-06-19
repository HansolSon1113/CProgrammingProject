//
//  loader.c
//  CProgrammingProject
//
//  Created by 한솔 on 6/18/25.
//

#include "loader.h"

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "entities.h"
#include "pixel.h"

void LoadFromFile(FILE *fp, Position size, LoadContent content, Entity type)
{
    char c;
    for (int y = 0; (c = fgetc(fp)) != EOF; y++) {
        for (int x = 0; x < size.x; x++)
        {
            if(c == '\n') {
                continue;
            } else {
                Position location = { x, y };
                switch(type)
                {
                    case TERRAIN:
                        ToMap(c, content.map, location);
                        break;
                    case PLAYER:
                        ToPlayer(c, content.player, location);
                        break;
                }
            }
        }
    }
    fclose(fp);
}

void ToMap(char c, Map *map, Position loc)
{
    switch(loc.y / size.y)
    {
        case 0:
            map -> pixels[loc.y][loc.x].c = c;
            break;
        case 1:
            if(c != ' ')
            {
                map -> pixels[loc.y][loc.x].entity = TERRAIN;
            }
            break;
        default:
            fprintf(stderr, "ERR: Saved map format is not valid!\n");
            exit(2);
    }
}

void ToPlayer(char c, Player *player, Position loc)
{
    int animIndex = loc.y / player -> size.y;
    
    switch(animIndex)
    {
        case 0:
            player -> anim.idle[loc.y][loc.x] = c;
            break;
        case 1:
            loc.y += player -> size.y;
        case 2:
            loc.y += player -> size.y;
        case 3:
            loc.y -= player -> size.y * 3;
            player -> anim.anim[animIndex][loc.y][loc.x] = c;
        default:
            fprintf(stderr, "ERR: Saved player format is not valid!\n");
            exit(2);
    }
}

//void LoadEnemy(size_t size);
