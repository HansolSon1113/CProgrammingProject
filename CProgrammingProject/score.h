//
//  score.h
//  CProgrammingProject
//
//  Created by 한솔 on 6/19/25.
//

#ifndef SCORE_H
#define SCORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _score {
    char name[100];
    double clearTime;
    int clearHealth;
    struct _score *next;
};

typedef struct _score Score;

void *AddScore(Score *head, char *name, double clearTime, int clearHealth);
Score *FindScore(Score *head, char *name);
Score *ReadScores(void);
void SaveScoresToFile(Score *head);

#endif
