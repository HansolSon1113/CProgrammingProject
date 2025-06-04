#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <player.h>
#include <pixel.h>
#include <map.h>

// Image to print on screen.
typedef struct
{
    char *screen;
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

//Main Menu Value
typedef enum
{
    START,
    SETTINGS,
    QUIT
} MainMenu;

//BOOLEAN
typedef enum
{
    false,
    true
} bool;

bool Lobby(void);
bool LobbyExit(MainMenu selection);
EnemyArray MakeEnemies(void);
void InGame(bool playing);
char GetInput(void);
void MoveEnemy(EnemyArray *enemies);
void Battle(Player *player, Enemy *enemy);
Frame GenerateFrame(const Pixel *pixels, const Player *player, const EnemyArray *enemies);
void UpdateScreen(const Frame *frame);

//Size of the screen
extern const Position size;

double deltaTime;
const double fixedDeltaTime = 1000.0 / 60.0;

//Lobby
bool Lobby(void)
{
    MainMenu selection = START;
    char input;
    Pixel *screen = malloc(sizeof(Pixel) * (size.x + 1) * size.y);
    
    while (true)
    {
        Frame frame = GenerateFrame(screen, NULL, NULL);
        UpdateScreen(&frame);
        
        input = GetInput();
        
        switch (input)
        {
            case 'a':
                if(selection > START) {
                    selection--;
                }
                break;
            case 'd':
                if(selection < QUIT) {
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
        case START:
            return true;
        case SETTINGS:
            return true;
        case QUIT:
            return false;
    }
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
        if (input != '\0')
        {
            MovePlayer(&player, map, input);
        }
        Jumping(&player, map);
        MoveEnemy(&enemies);
        
        QueryPerformanceCounter(&frameEnd); //측정 끝
        
        deltaTime = (double)(frameEnd.QuadPart - frameStart.QuadPart) * 1000.0 / frequency.QuadPart;
        if (deltaTime < fixedDeltaTime) //60프레임으로 맞춤
        {
            Sleep((DWORD)(fixedDeltaTime - deltaTime));
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

// Apply random movement to enemy position
void MoveEnemy(EnemyArray *enemies)
{
    for (int i = 0; i < enemies -> size; i++)
    {
        int x = rand() % 2 - 1, y = rand() % 2 - 1;
        
        enemies -> enemy[i].position.x += x;
        enemies -> enemy[i].position.y += y;
    }
}

// Combine all entities, map, screen border and return
Frame GenerateFrame(const Pixel *pixels, const Player *player, const EnemyArray *enemies)
{
    char screen[(size.x + 1) * size.y];
    for(int i = 0; i < (size.x + 1) * size.y; i++)
    {
        screen[i] = pixels[i].c;
    }
    
    Frame frame = { screen, NULL };
    
    return frame;
}

// Print Frame
void UpdateScreen(const Frame *frame)
{
    system("cls");
    printf("%s", frame->screen);
    printf("%s", frame->text);
}
