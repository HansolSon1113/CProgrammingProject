#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "entities.h"
#include "pixel.h"

typedef struct Map Map;
typedef struct Player Player;

typedef union {
    Map *map;
    Player *player;
} LoadContent;

void LoadFromFile(FILE *fp, LoadContent content, Entity type);
void ToMap(char c, Map *map, Position loc);
void ToPlayer(char c, Player *player, Position loc);
//void LoadEnemy(size_t size);

#endif
