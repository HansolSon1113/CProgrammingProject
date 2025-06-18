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
    if(map -> pixels[(player -> position.y - 1) * size.x + player -> position.x].entity == NONE) {
        player -> position.y--;
    }
}
