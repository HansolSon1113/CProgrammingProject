//
//  entities.h
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum
{
    PLAYER, ENEMY, TERRAIN, NONE
} Entity;

typedef struct
{
    char **idle;
    char **anim[];
} Animation;

#endif
