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
    if(loc.y > size.y)
    {
        fprintf(stderr, "ERR: Saved map format is not valid!\n");
        exit(2);
    }
    map -> pixels[loc.x][loc.y].c = c;
}

void ToPlayerIdle(char c, Player *player, Position loc)
{
    if(loc.y > player -> size.y * 3)
    {
        fprintf(stderr, "ERR: Saved player format is not valid!\n");
        exit(2);
    }
    
    switch(loc.y / player -> size.y)
    {
        case 0:
            player -> anim.idle[loc.x][loc.y] = c;
            break;
        case 1:
            loc.y -= player -> size.y;
            player -> anim.anim[0][loc.x][loc.y] = c;
            break;
        case 2:
            loc.y -= player -> size.y * 2;
            player -> anim.anim[1][loc.x][loc.y] = c;
            break;
    }
}

//void LoadEnemy(size_t size);
