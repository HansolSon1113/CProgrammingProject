#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum
{
    PLAYER, ENEMY, TERRAIN, DAMAGE_AREA, NONE, CLEAR
} Entity;

typedef struct
{
    char **idle;
    char ****anim;
} Animation;

#endif
