#ifndef KEYS_H
#define KEYS_H

#include <windows.h>

typedef struct
{
    short left, right, jump;
} Keys;

Keys GetKeys(void);

#endif