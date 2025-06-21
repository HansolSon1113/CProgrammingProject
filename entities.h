#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum
{
    PLAYER, ENEMY, TERRAIN, NONE, CLEAR
} Entity;

typedef struct
{
    char **idle;
    char ****anim;
} Animation;

#endif
