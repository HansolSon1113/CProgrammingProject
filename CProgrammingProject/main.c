#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <player.h>
#include <pixel.h>
#include <map.h>
#include <bool.h>
#include <score.h>

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

//Main Menu Value
typedef enum
{
    START,
    SETTINGS,
    QUIT
} MainMenu;

bool Lobby(void);
bool LobbyExit(MainMenu selection);
EnemyArray MakeEnemies(void);
void InGame(bool playing);
char GetInput(void);
void MoveEnemy(EnemyArray *enemies);
void Battle(Player *player, Enemy *enemy);
Frame GenerateFrame(const Pixel **pixels, const Player *player, const EnemyArray *enemies);
void UpdateScreen(const Frame *frame);

//Size of the screen
const Position size = {10, 10};

double deltaTime;
const double fixedDeltaTime = 1000.0 / 60.0;

//Lobby
bool Lobby(void)
{
    MainMenu selection = START;
    char input;
    Pixel *screen = (Pixel *)malloc(sizeof(Pixel) * (size.x + 1) * size.y);
    if(screen == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for screen!\n");
        exit(1);
    }
    
    Score *score = ReadScores();
    
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
    enemyArray.enemy = (EnemyArray *)malloc(sizeof(Enemy));
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
Frame GenerateFrame(const Pixel **pixels, const Player *player, const EnemyArray *enemies)
{
    char **screen = (char **)malloc(sizeof(char) * size.x * size.y);
    if(screen == NULL)
    {
        fprintf(stderr, "ERR: Failed to allocate memory for screen!\n");
        exit(1);
    }
    
    for(int y = 0; y < size.y; y++)
    {
        static Position beforePlayerPos = { -1, -1 };
        static int playerAnimIndex = 0;
        
        for(int x = 0; x < size.x; x++)
        {
            screen[y][x] = pixels[y][x].c;
            
            if(player -> position.x - player -> size.x / 2 > x &&
               x < player -> position.x + player -> size.x
               && player -> position.y - player -> size.y / 2 < y &&
               y < player -> position.y + player -> size.y / 2)
            {
                Position playerAnimPos = {
                    x - player -> position.x,
                    y - player -> position.y
                };
                
                if(beforePlayerPos.x != -1 && beforePlayerPos.y != -1 && beforePlayerPos.x != player -> position.x && beforePlayerPos.y != player -> position.y)
                {
                    screen[y][x] = player -> anim.anim[playerAnimIndex][playerAnimPos.y][playerAnimPos.x];
                    playerAnimIndex = (playerAnimIndex + 1) % 3;
                } else {
                    screen[y][x] = player -> anim.idle[playerAnimPos.y][playerAnimPos.x];
                }
            }
        }
    }
    
    char text[] = "";
    
    Frame frame = { screen, NULL };
    
    return frame;
}

// Print Frame
void UpdateScreen(const Frame *frame)
{
    system("cls");
    
    for(int y = 0; y < size.y; y++)
    {
        for(int x = 0; x < size.x; x++)
        {
            printf("%c", frame -> screen[y][x]);
        }
        printf("\n");
    }
    
    printf("%s", frame->text);
}



