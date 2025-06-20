#include "score.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *AddScore(Score *head, char *name, double clearTime, int clearHealth)
{
    Score *node = (Score *)malloc(sizeof(Score));
    strcpy(node -> name, name);
    node -> clearTime = clearTime;
    node -> clearHealth = clearHealth;
    node -> next = NULL;
    
    if(head == NULL)
    {
        head = node;
    } else {
        Score *tail = head;
        
        while(tail -> next != NULL)
        {
            tail = tail -> next;
        }
        
        tail -> next = node;
    }
    
    return head;
}

Score *FindScore(Score *head, char *name)
{
    Score *tail = head;
    while(tail != NULL)
    {
        if(!strcmp(name, tail -> name))
        {
            return tail;
        }
        
        tail = tail -> next;
    }

    return NULL;
}

Score *ReadScores(void)
{
    FILE *fp = fopen("scores.bin", "ab");

    fp = fopen("scores.bin", "rb");
    if(fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to open saved scores!\n");
        exit(1);
    }
    
    Score *head = NULL, *tail = NULL;
    
    while(1)
    {
        char name[100];
        if(fread(name, sizeof(char), 100, fp) != 100)
        {
            break;
        }
        
        Score *node = (Score *)malloc(sizeof(Score));
        if(node == NULL)
        {
            fprintf(stderr, "ERR: Failed to allocate memory for scores!\n");
            exit(1);
        }
        
        strcpy(node -> name, name);
        fread(&node -> clearTime, sizeof(double), 1, fp);
        fread(&node -> clearHealth, sizeof(int), 1, fp);
        node -> next = NULL;
        
        if(head == NULL)
        {
            head = tail = node;
        } else {
            tail -> next = node;
            tail = node;
        }
    }
    
    fclose(fp);
    return head;
}

void SaveScoresToFile(Score *head)
{
    FILE *fp = fopen("scores.bin", "wb");
    if(fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to save scores to file!");
        exit(3);
    }
    
    Score *tail = head;
    while(tail != NULL)
    {
        fwrite(tail -> name, sizeof(char), 100, fp);
        fwrite(&tail -> clearTime, sizeof(double), 1, fp);
        fwrite(&tail -> clearHealth, sizeof(int), 1, fp);
        
        tail = tail -> next;
    }
    
    fclose(fp);
}
