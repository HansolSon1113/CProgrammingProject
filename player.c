#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include "loader.h"

const int jumpOffsets[JUMP_FRAMES] = {0, -3, -2, -1, 0, 1, 2, 3};

Player *MakePlayer(void)
{
    Player *player = (Player *)malloc(sizeof(Player));
    player->dir = LEFT;

    Position pos = {4, 4};
    Position size = {5, 5};

    player->anim.idle = (char **)malloc(sizeof(char *) * size.y);
    if (player->anim.idle == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
        exit(1);
    }
    for (int y = 0; y < size.y; y++)
    {
        player->anim.idle[y] = (char *)malloc(sizeof(char) * size.x);
        if (player->anim.idle[y] == NULL)
        {
            fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
            exit(1);
        }
    }

    player->anim.anim = (char ***)malloc(sizeof(char **) * PLAYER_ANIM_FRAME);
    if (player->anim.anim == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
        exit(1);
    }
    for (int i = 0; i < PLAYER_ANIM_FRAME; i++)
    {
        player->anim.anim[i] = (char **)malloc(sizeof(char *) * size.y);
        if (player->anim.anim[i] == NULL)
        {
            fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
            exit(1);
        }
        for (int y = 0; y < size.y; y++)
        {
            player->anim.anim[i][y] = (char *)malloc(sizeof(char) * size.x);
            if (player->anim.anim[i][y] == NULL)
            {
                fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
                exit(1);
            }
        }
    }

    FILE *fp = fopen("player.txt", "r");

    if (player == NULL || fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to load player!\n");
        exit(1);
    }

    player->position = pos;
    player->size = size;
    player->jumpIndex = 0;

    LoadContent loadContent;
    loadContent.player = player;
    LoadFromFile(fp, loadContent, PLAYER);

    return player;
}

// Apply input to position
void MovePlayer(Player *player, Map *map, char input)
{
    if (map->pixels[player->position.y - player->size.y + 1][player->position.x].entity == NONE)
    {
        player->position.y--;
    }

    Position newPos = {player->position.x, player->position.y};
    switch (input)
    {
    case 'a':
        newPos.x -= 1;
        Interaction(&player->position, newPos, player->size, PLAYER, map);
        player->dir = LEFT;
    case 'd':
        newPos.x += 1;
        player->dir = RIGHT;
        Interaction(&player->position, newPos, player->size, PLAYER, map);
        break;
    case ' ':
        Jump(player);
        break;
    }
}

// Apply input to position
void Jump(Player *player)
{
    if (!player->jumpIndex)
    {
        player->jumpIndex = 1;
    }
}

void Jumping(Player *player, Map *map)
{
    if (player->jumpIndex)
    {
        Position newPos = {player->position.x, player->position.y};
        newPos.y += jumpOffsets[player->jumpIndex];
        player->jumpIndex++;

        Interaction(&player->position, newPos, player->size, PLAYER, map);

        if (player->jumpIndex >= JUMP_FRAMES)
        {
            player->jumpIndex = 0;
        }
    }
}
