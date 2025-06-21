#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "player.h"
#include "pixel.h"
#include "map.h"
#include "score.h"
#include "keys.h"
#include "bool.h"
#include "sounds.h"

// Image to print on screen.
typedef struct
{
    char **screen;
    char *text;
} Frame;

// Enemy
typedef struct
{
    Position position;
    int health;
    int damage;
} Enemy;

// Enemies and size of array with enemies
typedef struct
{
    Enemy *enemy;
    size_t size;
} EnemyArray;

// Main Menu Value
typedef enum
{
    START,
    SETTINGS,
    QUIT
} MainMenu;

bool Lobby(void);
bool LobbyExit(MainMenu selection);
EnemyArray *MakeEnemies(void);
void InGame(bool playing);
char GetInput(void);
void MoveEnemy(EnemyArray *enemies);
void Battle(Player *player, Enemy *enemy);
Frame GenerateFrame(const Map *map, const Player *player, const EnemyArray *enemies);
void UpdateScreen(const Frame *frame);

// Size of the screen
const Position size = {80, 15};

double deltaTime;
const double fixedDeltaTime = 1000.0 / 60.0;

// Lobby
bool Lobby(void)
{
    SetVolume(500);
    InitAudio();
    StartBgm("sounds/lobby.wav");
#ifdef DEBUG
    printf("Entering Lobby...\n");
    fflush(stdout);
#endif
    MainMenu selection = START;
    char input;
    Map map;
    map.size.x = 80;
    map.size.y = 15;
    map.pixels = (Pixel **)malloc(sizeof(Pixel *) * size.y);
    if (map.pixels == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for screen!\n");
        exit(1);
    }
    for (int y = 0; y < size.y; y++)
    {
        map.pixels[y] = malloc(sizeof(Pixel) * size.x);
        if (map.pixels[y] == NULL)
        {
            fprintf(stderr, "ERR: Failed to allocate memory for screen!\n");
            exit(1);
        }
    }

#ifdef DEBUG
    printf("Reading saved scores...\n");
    fflush(stdout);
#endif
    Score *score = ReadScores();

#ifdef DEBUG
    printf("Starting Lobby Loop...\n");
    fflush(stdout);
#endif
    while (true)
    {
        input = GetInput();
        for (int y = 0; y < size.y; y++)
        {
            for (int x = 0; x < size.x; x++)
            {
                map.pixels[y][x].c = input;
            }
        }

        switch (input)
        {
        case 'a':
            PlaySelection();
            if (selection > START)
            {
                selection--;
            }
            break;
        case 'd':
            PlaySelection();
            if (selection < QUIT)
            {
                selection++;
            }
            break;
        case '\r':
            for (int y = 0; y < size.y; y++)
            {
                free(map.pixels[y]);
            }
            free(map.pixels);
            StopBgm();
            return LobbyExit(selection);
        }

        Frame frame = GenerateFrame(&map, NULL, NULL);
        UpdateScreen(&frame);
        Sleep((DWORD)100);
    }
}

// Return bool based on Lobby's value
bool LobbyExit(MainMenu selection)
{
    switch (selection)
    {
    case START:
        return true;
    case SETTINGS:
        return true;
    case QUIT:
        return false;
    default:
        fprintf(stderr, "ERR: Wrong input!\n");
        exit(3);
    }
}

EnemyArray *MakeEnemies(void)
{
    EnemyArray *enemyArray = (EnemyArray *)malloc(sizeof(EnemyArray));
    if (enemyArray == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for enemy array!\n");
        exit(1);
    }

    enemyArray->size = 0;
    return enemyArray;
}

int main(void)
{
    CONSOLE_CURSOR_INFO cursorInfo = {
        0,
    };
    cursorInfo.bVisible = 0;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    InGame(Lobby());

    return 0;
}

// In Game Loop
void InGame(bool playing)
{
#ifdef DEBUG
    printf("InGame Start: playing = %d\n", playing);
    fflush(stdout);
#endif

    if (!playing)
    {
        return;
    }

    srand((unsigned)time(NULL));

    LARGE_INTEGER frequency, frameStart, frameEnd;
    QueryPerformanceFrequency(&frequency);

#ifdef DEBUG
    printf("Starting Map Load...\n");
    fflush(stdout);
#endif
    Map *map = LoadMap();
#ifdef DEBUG
    printf("Stating Player Load...\n");
    fflush(stdout);
#endif
    Player *player = MakePlayer();
    EnemyArray *enemies = MakeEnemies();

    StartBgm("sounds/nightmare.wav");

#ifdef DEBUG
    printf("Entering Loop...\n");
    fflush(stdout);
#endif

    while (playing)
    {
#ifdef DEBUG
        printf("Start of Frame\n");
        fflush(stdout);
#endif
        QueryPerformanceCounter(&frameStart);

#ifdef DEBUG
        printf("Generating Frame...\n");
        fflush(stdout);
#endif
        Frame frame = GenerateFrame(map, player, enemies);
#ifdef DEBUG
        printf("Updating Screen...\n");
        fflush(stdout);
#endif
        UpdateScreen(&frame);

#ifdef DEBUG
        printf("Getting Input...\n");
        fflush(stdout);
#endif
        Keys input = GetKeys();
        if (input.left || input.right || input.jump)
        {
#ifdef DEBUG
            printf("Moving Player...\n");
            fflush(stdout);
#endif
            playing = MovePlayer(player, map, input);
        }
#ifdef DEBUG
        printf("Jump Sequence...\n");
        fflush(stdout);
#endif
        Jumping(player, map);
#ifdef DEBUG
        printf("Moving Enemies...\n");
        fflush(stdout);
#endif
        MoveEnemy(enemies);

#ifdef DEBUG
        printf("End of Frame...\n");
        fflush(stdout);
#endif
        QueryPerformanceCounter(&frameEnd);

#ifdef DEBUG
        printf("Matching 60fps...\n");
        fflush(stdout);
#endif
        deltaTime = (double)(frameEnd.QuadPart - frameStart.QuadPart) * 1000.0 / frequency.QuadPart;
        if (deltaTime < fixedDeltaTime)
        {
            Sleep((DWORD)(fixedDeltaTime - deltaTime));
        }
    }
}

// Returns input when keyboard clicked
char GetInput(void)
{
    if (_kbhit())
    {
        return _getch();
    }

    return '\0';
}

// Apply random movement to enemy position
void MoveEnemy(EnemyArray *enemies)
{
    for (int i = 0; i < enemies->size; i++)
    {
        int x = rand() % 2 - 1, y = rand() % 2 - 1;

        enemies->enemy[i].position.x += x;
        enemies->enemy[i].position.y += y;
    }
}

// Combine all entities, map, screen border and return
Frame GenerateFrame(const Map *map, const Player *player, const EnemyArray *enemies)
{
    const Pixel **pixels = map->pixels;

    char **screen = (char **)malloc(sizeof(char *) * size.y);
    if (screen == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for screen!\n");
        exit(1);
    }
    for (int y = 0; y < size.y; y++)
    {
        screen[y] = malloc(sizeof(char) * size.x);
        if (screen[y] == NULL)
        {
            fprintf(stderr, "ERR: Failed to allocate memory for screen!\n");
            exit(1);
        }
    }

    static Position beforePlayerPos = {-1, -1};
    static int playerAnimIndex = 0, countBeforeAnimChange = 0;

    Position half;
    Position printPos;
    Position center;

    if (player != NULL)
    {
        half.x = player->size.x / 2;
        half.y = player->size.y / 2;
        int tempX = player->position.x - size.x / 2, tempY = player->position.y - size.y / 2;
        printPos.x = tempX < map->size.x - size.x ? (tempX > 0 ? tempX : 0) : map->size.x - size.x;
        printPos.y = tempY < map->size.y - size.y ? (tempY > 0 ? tempY : 0) : map->size.y - size.y;
        center.x = player->position.x - printPos.x;
        center.y = player->position.y - printPos.y;

        if (printPos.x < 0)
        {
            printPos.x = 0;
        }
        if (printPos.y < 0)
        {
            printPos.y = 0;
        }
        if (printPos.x >= map->size.x)
        {
            printPos.x = map->size.x - size.x;
        }
        if (printPos.x >= map->size.x || printPos.y >= map->size.y)
        {
            printPos.y = map->size.y - size.y;
        }
    }

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            if (player != NULL)
            {
                screen[y][x] = pixels[printPos.y + y][printPos.x + x].c;
            }
            else
            {
                screen[y][x] = pixels[y][x].c;
            }
        }
    }

    if (player != NULL)
    {
        for (int y = -half.y; y <= half.y; y++)
        {
            for (int x = -half.x; x <= half.x; x++)
            {
                Position playerToScreenPos = {center.x + x, center.y + y};
                if (playerToScreenPos.x < 0 || playerToScreenPos.x >= size.x || playerToScreenPos.y < 0 || playerToScreenPos.y > size.y)
                    continue;

                Position playerAnimPos = {x + half.x, y + half.y};

                if (beforePlayerPos.x != -1 && beforePlayerPos.y != -1 && beforePlayerPos.x != player->position.x || beforePlayerPos.y != player->position.y)
                {
                    if (player->dir == LEFT)
                    {
                        screen[playerToScreenPos.y][playerToScreenPos.x] = player->anim.anim[0][playerAnimIndex][playerAnimPos.y][playerAnimPos.x];
                    }
                    else
                    {
                        screen[playerToScreenPos.y][playerToScreenPos.x] = player->anim.anim[1][playerAnimIndex][playerAnimPos.y][playerAnimPos.x];
                    }
                    countBeforeAnimChange++;
                    if (countBeforeAnimChange > 60)
                    {
                        playerAnimIndex = (playerAnimIndex + 1) % 3;
                        countBeforeAnimChange = 0;
                        PlayWalk();
                    }
                }
                else
                {
                    screen[playerToScreenPos.y][playerToScreenPos.x] = player->anim.idle[playerAnimPos.y][playerAnimPos.x];
                }
            }
        }
        beforePlayerPos.x = player->position.x;
        beforePlayerPos.y = player->position.y;
    }

    char text[] = "";

    Frame frame = {screen, text};

    return frame;
}

// Print Frame
void UpdateScreen(const Frame *frame)
{
    system("cls");

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            printf("%c", frame->screen[y][x]);
        }
        printf("\n");
    }

    printf("%s", frame->text);

    for (int y = 0; y < size.y; y++)
    {
        free(frame->screen[y]);
    }
    free(frame->screen);
}
