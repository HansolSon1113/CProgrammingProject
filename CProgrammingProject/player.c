//
//  player.c
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

#include "player.h"

#include <stdio.h>
#include "pixel.h"
#include "map.h"
#include "loader.h"

Player* MakePlayer(void)
{
    Player *player = (Player *)malloc(sizeof(Player));
    Position pos = {0, 0};
    Position size = {5, 5};
    
    FILE *fp = fopen("player.txt", "r");
    
    if(player == NULL || fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for player!\n");
        exit(1);
    }
    
    player -> position = pos;
    player -> size = size;
    player -> jumpIndex = 0;
    
    LoadContent loadContent;
    loadContent.player = player;
    LoadFromFile(fp, size, loadContent, PLAYER);
    
    return player;
}

// Apply input to position
void MovePlayer(Player *player, Map *map, char input)
{
    if(map -> pixels[player -> position.y - player -> size.y - 1][player -> position.x].entity == NONE)
    {
        player -> position.y--;
    }
    
    Position newPos = { player -> position.x, player -> position.y };
    switch (input)
    {
        case 'a':
            newPos.x -= 2; //아래까지 실행되며 newPos는 최종적으로 -1이 가해진 값
        case 'd':
            newPos.x += 1;
            Interaction(player -> position, newPos, player -> size,  PLAYER, map);
            break;
        case ' ':
            Jump(player);
            break;
    }
}

// Apply input to position
void Jump(Player *player) {
    if(!player -> jumpIndex) {
        player -> jumpIndex = 1;
    }
}

void Jumping(Player *player, Map *map) {
    if(player -> jumpIndex) {
        Position newPos = { player -> position.x, player -> position.y };
        newPos.y += jumpOffsets[player -> jumpIndex];
        player->jumpIndex++;
        
        Interaction(player -> position, newPos, PLAYER, map);
        
        if (player -> jumpIndex >= JUMP_FRAMES - 1) {
            player -> jumpIndex = 0;
        }
    }
}
