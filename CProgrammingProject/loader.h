//
//  Loader.h
//  CProgrammingProject
//
//  Created by 한솔 on 6/18/25.
//

#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "entities.h"
#include "pixel.h"

typedef union {
    Map *map;
    Player *player;
} LoadContent;

void LoadFromFile(FILE *fp, Position size, LoadContent content, Entity type);
void ToMap(char c, Map *map, Position loc);
void ToPlayer(char c, Player *player, Position loc);
//void LoadEnemy(size_t size);

#endif
