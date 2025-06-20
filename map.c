#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include "pixel.h"
#include "loader.h"

Map *LoadMap(void)
{
    Map *map = (Map *)malloc(sizeof(Map));
    map->size.x = 800;
    map->size.y = 15;
    map->pixels = malloc(sizeof(Pixel *) * map->size.y);

    FILE *fp;
    fp = fopen("map.txt", "r");
    if (map == NULL || map->pixels == NULL || fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to load Map!\n");
        exit(1);
    }

    for (int y = 0; y < map->size.y; y++)
    {
        map->pixels[y] = malloc(sizeof(Pixel) * map->size.x);
        if (map->pixels[y] == NULL)
        {
            fprintf(stderr, "ERR: Failed to allocate memory for map!\n");
            exit(1);
        }
    }

    LoadContent content;
    content.map = map;
    LoadFromFile(fp, content, TERRAIN);

    return map;
}