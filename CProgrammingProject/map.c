//
//  map.c
//  CProgrammingProject
//
//  Created by 한솔 on 6/2/25.
//

#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include "pixel.h"

Map *LoadMap(void)
{
    Map *map = (Map *)malloc(sizeof(Map));
    map -> pixels = malloc(sizeof(Pixel) * (size.x + 1) * size.y + 1);
    FILE *fp;
    fp = fopen("map.txt", "r");
    if (map == NULL || map -> pixels == NULL || fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to load Map!\n");
        exit(EXIT_FAILURE);
    }
    
    size_t total = (size.x + 1) * size.y;
    for (size_t i = 0; i < total; i++) {
        if (fread(&map -> pixels[i].c, sizeof(char), 1, fp) != 1) {
            break;
        }
    }
    fclose(fp);
    map -> pixels[total].c = '\0';
    
    return map;
}
