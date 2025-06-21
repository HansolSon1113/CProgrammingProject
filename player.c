#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include "loader.h"
#include "bool.h"
#include "keys.h"

const int jumpOffsets[JUMP_FRAMES] = {0, -5, -3, -1, 0, 0, 0, 1, 3, 5};
const int PLAYER_ANIM_FRAME = 3;

Player *MakePlayer(void)
{
    Player *player = (Player *)malloc(sizeof(Player));

    Position pos = {40, 8};
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

    player->anim.anim = (char ****)malloc(sizeof(char ***) * 2);
    if (player->anim.anim == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
        exit(1);
    }
    for (int t = 0; t < 2; t++)
    {
        player->anim.anim[t] = (char ***)malloc(sizeof(char **) * PLAYER_ANIM_FRAME);
        for (int i = 0; i < PLAYER_ANIM_FRAME; i++)
        {
            player->anim.anim[t][i] = (char **)malloc(sizeof(char *) * size.y);
            if (player->anim.anim[t][i] == NULL)
            {
                fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
                exit(1);
            }
            for (int y = 0; y < size.y; y++)
            {
                player->anim.anim[t][i][y] = (char *)malloc(sizeof(char) * size.x);
                if (player->anim.anim[t][i][y] == NULL)
                {
                    fprintf(stderr, "ERR: Failed to allocate memory for player animatation!\n");
                    exit(1);
                }
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
    player->dir = LEFT;

    LoadContent loadContent;
    loadContent.player = player;
    LoadFromFile(fp, loadContent, PLAYER);

    return player;
}

// Apply input to position
bool MovePlayer(Player *player, Map *map, Keys input)
{
    Position newPos = {player->position.x, player->position.y};
    if (input.left)
    {
        newPos.x -= 1;
        Interaction(&player->position, newPos, player->size, PLAYER, map);
        player->dir = LEFT;
    }
    else if (input.right)
    {
        newPos.x += 1;
        player->dir = RIGHT;
        Interaction(&player->position, newPos, player->size, PLAYER, map);
    }

    if (input.jump)
    {
        Jump(player);
    }

    if (map->pixels[newPos.y][newPos.x + player->size.x / 2].entity == CLEAR || map->pixels[newPos.y][newPos.x - player->size.x / 2].entity == CLEAR || map->pixels[newPos.y + player->size.y / 2][newPos.x].entity == CLEAR || map->pixels[newPos.y - player->size.y / 2][newPos.x].entity == CLEAR)
    {
        ShowResult(player);
        return false;  // 게임 종료
    }

    return true;
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
    Position goDown = {player->position.x, player->position.y + 1};
    Interaction(&player->position, goDown, player->size, PLAYER, map);

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
