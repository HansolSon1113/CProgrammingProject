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
    player->health = 20;
    player->damage = 1;
    player->alreadyDamaged = false;

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
        player->dir = LEFT;
    }
    else if (input.right)
    {
        newPos.x += 1;
        player->dir = RIGHT;
    }

    bool withEnemy = false;
    for (int y = newPos.y - player->size.y / 2; y <= newPos.y + player->size.y / 2 && player->alreadyDamaged == true && withEnemy == false; y++)
    {
        for (int x = newPos.x - player->size.x / 2; x <= newPos.x + player->size.x / 2; x++)
        {
            if (map->pixels[y][x].entity == ENEMY || map->pixels[y][x].entity == DAMAGE_AREA)
            {
                withEnemy = true;
            }
        }
    }
    player->alreadyDamaged = withEnemy;

    for (int y = newPos.y - player->size.y / 2; y <= newPos.y + player->size.y / 2 && player->alreadyDamaged == false; y++)
    {
        for (int x = newPos.x - player->size.x / 2; x <= newPos.x + player->size.x / 2; x++)
        {
            if ((map->pixels[y][x].entity == ENEMY || map->pixels[y][x].entity == DAMAGE_AREA) && player->alreadyDamaged == false)
            {
                player->health -= 1;
                player->alreadyDamaged = true;
                break;
            }
        }
    }

    Interaction(&player->position, newPos, player->size, PLAYER, map);

    if (input.jump)
    {
        Jump(player);
    }

    if (map->pixels[newPos.y][newPos.x + player->size.x / 2].entity == CLEAR || map->pixels[newPos.y][newPos.x - player->size.x / 2].entity == CLEAR || map->pixels[newPos.y + player->size.y / 2][newPos.x].entity == CLEAR || map->pixels[newPos.y - player->size.y / 2][newPos.x].entity == CLEAR || player->health <= 0)
    {
        ShowResult(player);
        return false; // 게임 종료
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
    Position newPos = {player->position.x, player->position.y + 1};
    Interaction(&player->position, newPos, player->size, PLAYER, map);

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

EnemyArray *MakeEnemies(int enemyCount, Map *map)
{
    FILE *fp = fopen("enemy.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to open enemy file!\n");
        exit(1);
    }

    EnemyArray *enemyArray = (EnemyArray *)malloc(sizeof(EnemyArray));
    if (enemyArray == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for enemy array!\n");
        exit(1);
    }
    enemyArray->size = enemyCount;
    enemyArray->enemies = (Enemy *)malloc(sizeof(Enemy) * enemyCount);
    if (enemyArray->enemies == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for enemy array!\n");
        exit(1);
    }
    enemyArray->sprite = (char **)malloc(sizeof(char *) * 5);
    if (enemyArray->sprite == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for enemy array!\n");
        exit(1);
    }
    for (int y = 0; y < 5; y++)
    {
        enemyArray->sprite[y] = (char *)malloc(sizeof(char) * 7);
        if (enemyArray->sprite[y] == NULL)
        {
            fprintf(stderr, "ERR: Failed to allocate memory for enemy array!\n");
            exit(1);
        }
    }

    LoadContent content;
    content.enemyArray = enemyArray;

    LoadFromFile(fp, content, ENEMY);

    for (int i = 0; i < enemyCount; i++)
    {
        int X, Y;
        bool b = false;
        do
        {
            b = false;
            X = rand() % (map->size.x - 6) + 3, Y = rand() % (map->size.y - 4) + 2;

            for (int y = Y - 2; y <= Y + 2 && !b; y++)
            {
                for (int x = X - 3; x <= X + 3; x++)
                {
                    if (map->pixels[y][x].entity != NONE)
                    {
                        b = true;
                        break;
                    }
                }
            }
        } while (b);

        enemyArray->enemies[i].position.x = X;
        enemyArray->enemies[i].position.y = Y;

        enemyArray->enemies[i].size.x = 7;
        enemyArray->enemies[i].size.y = 5;
    }

    return enemyArray;
}

// Apply random movement to enemy position
void MoveEnemy(EnemyArray *enemies, Map *map)
{
    for (int i = 0; i < enemies->size; i++)
    {
        int x = rand() % 3 - 1;
        Position newPos = {enemies->enemies[i].position.x + x, enemies->enemies[i].position.y};
        Interaction(&enemies->enemies[i].position, newPos, enemies->enemies[i].size, ENEMY, map);

        Position goDown = {enemies->enemies[i].position.x, enemies->enemies[i].position.y + 1};
        Interaction(&enemies->enemies[i].position, goDown, enemies->enemies[i].size, ENEMY, map);
    }
}
