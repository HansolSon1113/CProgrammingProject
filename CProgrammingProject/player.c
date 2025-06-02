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

Player MakePlayer(void)
{
    Player player = {{0, 0}, 100, 10, NULL, 0};
    return player;
}

// Apply input to position
void MovePlayer(Player *player, Map *map, char input)
{
    if(map -> pixels[size.x * (player -> position.y + 1) + player -> position.x].entity == NONE)
    {
        player -> position.y--;
    }
    int newPos = player -> position.x;
    switch (input)
    {
        case 'a':
            newPos -= 2; //아래까지 실행되며 newPos는 최종적으로 -1이 가해진 값
        case 'd':
            newPos += 1;
            if(map -> pixels[(size.x * player -> position.y) + newPos].entity == NONE)
            {
                map -> pixels[(size.x * player -> position.y) + player -> position.x].entity = NONE; //플레이어 크기만큼 변경으로 바꿀것
                player -> position.x = newPos;
                map -> pixels[(size.x * player -> position.y) + newPos].entity = PLAYER; //여기도
            }
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
        map -> pixels[(size.x * player -> position.y) + player -> position.y].entity = NONE; //여기도 플레이어 범위만큼으로
        player -> position.y += jumpOffsets[player -> jumpIndex];
        player->jumpIndex++;
        map -> pixels[(size.x * player -> position.y) + player -> position.y].entity = PLAYER; //여기도
        
        if (player -> jumpIndex >= JUMP_FRAMES) {
            player -> jumpIndex = 0;
        }
    }
    if(map -> pixels[(player -> position.y - 1) * size.x + player -> position.x].entity == NONE) {
        player -> position.y--;
    }
}
