#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Position on screen
typedef struct
{
    int x;
    int y;
} Position;

// Image to print on screen.
typedef struct
{
    char *screen;
    Position size;
    char *text;
} Frame;

// Item
typedef struct
{
    int barrier;
    float damageMultiplier;
} Item;

//Array holding items of player
typedef struct
{
    Item *item;
    size_t size;
} ItemArray;

// Player
typedef struct
{
    Position position;
    int health;
    int damage;
    ItemArray items;
    int jumpIndex;
} Player;

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

// Map and items on the map
typedef struct {
    char *map;
    ItemArray items;
} Map;

//Main Menu Value
typedef enum
{
    start,
    settings,
    quit
} MainMenu;

//BOOLEAN
typedef enum
{
    false,
    true
} bool;

MainMenu Lobby(void);
bool LobbyExit(MainMenu selection);
Map *LoadMap(void);
Player MakePlayer(void);
EnemyArray MakeEnemies(void);
void InGame(bool playing);
char GetInput(void);
void MovePlayer(Player *player, char input);
void Jump(Player *player);
void Jumping(Player *player, Map *map);
void MoveEnemy(EnemyArray *enemies);
void Interaction(Player *player);
void Battle(Player *player, Enemy *enemy);
Frame GenerateFrame(const char *map, const Player *player, const EnemyArray *enemies);
void UpdateScreen(const Frame *frame);

//Size of the screen
const Position size = {10, 10};
const int JUMP_FRAMES = 8;
const int jumpOffsets[JUMP_FRAMES] = {0, 3, 2, 1, 0, -1, -2, -3};
double deltaTime;

//Lobby
MainMenu Lobby(void)
{
    MainMenu selection = start;
    char input;
    char *screen = malloc(sizeof(char) * (size.x + 1) * size.y);
    
    while (true)
    {
        Frame frame = GenerateFrame(screen, NULL, NULL);
        UpdateScreen(&frame);
        
        input = GetInput();
        
        switch (input)
        {
            case 'a':
                if(selection > start) {
                    selection--;
                }
                break;
            case 'd':
                if(selection < quit) {
                    selection++;
                }
                break;
            case '\n':
                free(screen);
                return LobbyExit(selection);
        }
    }
}

//Return bool based on Lobby's value
bool LobbyExit(MainMenu selection)
{
    switch(selection) {
        case start:
            return true;
        case settings:
            return true;
        case quit:
            return false;
    }
}

Map *LoadMap(void)
{
    Map *map = malloc(sizeof(Map));
    map -> map = malloc(sizeof(char) * (size.x + 1) * size.y + 1);
    FILE *fp;
    
    fp = fopen("map.txt", "r");
    if (map || map -> map || fp == NULL)
    {
        fprintf(stderr, "ERR: Failed to load Map!\n");
        exit(EXIT_FAILURE);
    }
    
    size_t bytes = fread(map -> map, sizeof(char), (size.x + 1) * size.y, fp);
    fclose(fp);
    map -> map[bytes] = '\0';
    
    return map;
}

Player MakePlayer(void)
{
    Player player = {{0, 0}, 100, 10, NULL, 0};
    return player;
}

EnemyArray MakeEnemies(void)
{
    EnemyArray enemyArray;
    enemyArray.enemy = malloc(sizeof(Enemy));
    if (enemyArray.enemy == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for enemy array!\n");
        exit(EXIT_FAILURE);
    }
    
    enemyArray.size = 0;
    return enemyArray;
}

int main(void)
{
    InGame(Lobby());
    
    return 0;
}

// In Game Loop
void InGame(bool playing)
{
    if (playing == false)
    {
        return;
    }
    
    srand((unsigned)time(NULL));
    
    LARGE_INTEGER frequency, frameStart, frameEnd;
    const double frameDuration = 1000.0 / 60.0; //60프레임 제한
    QueryPerformanceFrequency(&frequency);
    
    Map *map = LoadMap();
    Player player = MakePlayer();
    EnemyArray enemies = MakeEnemies();
    
    while (playing)
    {
        QueryPerformanceCounter(&frameStart); //측정 시작
        
        Frame frame = GenerateFrame(map -> map, &player, &enemies);
        UpdateScreen(&frame);
        
        char input = GetInput();
        if (char != '\0')
        {
            MovePlayer(&player, input);
        }
        Jumping(&player, map);
        MoveEnemy(&enemies);
        
        QueryPerformanceCounter(&frameEnd); //측정 끝
        
        deltaTime = (double)(frameEnd.QuadPart - frameStart.QuadPart) * 1000.0 / frequency.QuadPart;
        if (deltaTime < frameDuration) //60프레임으로 맞춤
        {
            Sleep((DWORD)(frameDuration - deltaTime));
        }
    }
}

//Returns input when keyboard clicked
char GetInput(void)
{
    if (_kbhit())
    {
        return _getch();
    }
    
    return '\0';
}

// Apply input to position
void MovePlayer(Player *player, char input)
{
    switch (input)
    {
        case 'a':
            player -> position.x--;
            break;
        case 'd':
            player -> position.x++;
            break;
        case ' ':
            Jump(player);
            break;
    }
}

void Jump(Player *player) {
    if(!player -> jumpIndex) {
        player -> jumpIndex = 1;
    }
}

void Jumping(Player *player, Map *map) {
    if(player -> jumpIndex) {
        player -> position.y += jumpOffsets[player -> jumpIndex];
        player->jumpIndex++;
        
        if (player -> jumpIndex >= JUMP_FRAMES) {
            player -> jumpIndex = 0;
        }
    }
    if(map -> map[(player -> position.y - 1) * size.x + player -> position.x] == ' ') {
        player -> position.y--;
    }
}

// Apply random movement to enemy position
void MoveEnemy(EnemyArray *enemies)
{
    for (int i = 0; i < enemies -> size; i++)
    {
        int _x = rand() % 2 - 1, _y = rand() % 2 - 1;
        
        enemies -> enemy[i].position.x += _x;
        enemies -> enemy[i].position.y += _y;
    }
}

// Combine all entities, map, screen border and return
Frame GenerateFrame(const char *map, const Player *player, const EnemyArray *enemies)
{
    Frame frame = { NULL, size, NULL };
    frame.screen = map;
    
    return frame;
}

// Print Frame
void UpdateScreen(const Frame *frame)
{
    system("cls");
    printf("%s", frame->screen);
}
