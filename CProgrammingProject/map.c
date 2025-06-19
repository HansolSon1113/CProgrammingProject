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
#include "loader.h"
#include "entities.h"

Map *LoadMap(void)
{
    Map *map = (Map *)malloc(sizeof(Map));
    map -> pixels = malloc(sizeof(Pixel) * size.x * size.y);
    FILE *fp;
    fp = fopen("map.txt", "r");
    if (map == NULL || map -> pixels == NULL || fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to load Map!\n");
        exit(EXIT_FAILURE);
    }
    
    LoadContent content;
    content.map = map;
    LoadFromFile(fp, size, content, TERRAIN);
    
    return map;
}
