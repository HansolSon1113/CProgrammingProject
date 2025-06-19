//
//  player.h
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "loader.h"
#include "map.h"

typedef struct Map Map;

// Item
typedef struct Item
{
    int barrier;
    float damageMultiplier;
} Item;

//Array holding items of player
typedef struct
{
    Item *item;
    size_t size;
} ItemArray;

// Player
typedef struct Player
{
    Position position;
    Position size;
    Animation anim;
    int health;
    int damage;
    ItemArray items;
    int jumpIndex;
} Player;

const int JUMP_FRAMES = 8;
const int jumpOffsets[JUMP_FRAMES] = {0, 3, 2, 1, 0, -1, -2, -3};

Player* MakePlayer(void);
void MovePlayer(Player *player, Map *map, char input);
void Jump(Player *player);
void Jumping(Player *player, Map *map);

#endif
