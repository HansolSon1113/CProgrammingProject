#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "loader.h"
#include "map.h"
#include "keys.h"

#define JUMP_FRAMES 10

typedef struct Map Map;
typedef enum bool bool;

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

typedef enum
{
    LEFT = -1,
    RIGHT = 1
} Direction;

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
    Direction dir;
} Player;

Player* MakePlayer(void);
bool MovePlayer(Player *player, Map *map, Keys input);
void Jump(Player *player);
void Jumping(Player *player, Map *map);

#endif
