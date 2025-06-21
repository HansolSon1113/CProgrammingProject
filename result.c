#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "result.h"
#include "score.h"
#include "player.h"
#include "map.h"
#include "keys.h"
#include "sounds.h"
#include "bool.h"

extern double playTime;

void SaveScore(const Player *player) {
    Score *head = ReadScores();
    head = AddScore(head, "PLAYER", playTime / 1000.0, player->health);  // 👈 여기에서 AddScore 방식에 맞춰 이름 사용
    SaveScoresToFile(head);
}

int ShowResultMenu(const Player *player) {
    const char *options[] = {
        "Return to Lobby",
        "View Rankings",
        "Exit Game"
    };
    int selection = 0;
    char input;

    while (1) {
        system("cls");
        printf("===== GAME OVER =====\n\n");
        printf("Your Time: %.2f sec\n", playTime / 1000.0);
        printf("Remaining HP: %d\n\n", player->health);

        for (int i = 0; i < 3; i++) {
            if (i == selection)
                printf("> %s\n", options[i]);
            else
                printf("  %s\n", options[i]);
        }

        printf("\nW/S: Move   Enter: Select\n");

        input = _getch();
        if ((input == 'w' || input == 'W') && selection > 0)
            selection--;
        else if ((input == 's' || input == 'S') && selection < 2)
            selection++;
        else if (input == '\r')
            return selection;
    }
}

void ShowResult(Player *player) {
    SaveScore(player);
    int choice = ShowResultMenu(player);

    // 🔥 키보드 버퍼 클리어
    while (_kbhit()) _getch();

    printf("\nPress any key to continue...");
    _getch();

    switch (choice) {
        case 0: Lobby(); break;
        case 1: ShowRanking(); Lobby(); break;
        case 2: exit(0);
    }
}