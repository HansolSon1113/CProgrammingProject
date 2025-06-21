#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "entities.h"
#include "pixel.h"

typedef struct Map Map;
typedef struct Player Player;
typedef struct EnemyArray EnemyArray;

typedef union {
    Map *map;
    Player *player;
    EnemyArray *enemyArray;
} LoadContent;

void LoadFromFile(FILE *fp, LoadContent content, Entity type);
void ToMap(char c, Map *map, Position loc);
void ToPlayer(char c, Player *player, Position loc);
void ToEnemy(char c, EnemyArray *enemyArray, Position loc);

#endif
