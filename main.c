#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 400
#define SNAKE_SIZE 40
#define SNAKE_SPEED 0.3f

#define FEILD_WIDTH CANVAS_WIDTH / SNAKE_SIZE
#define FEILD_HEIGHT CANVAS_HEIGHT / SNAKE_SIZE

const Vector2 ZERO_VEC = {0, 0};

// Global declaration
//====================================================================================

int food_x;
int food_y;
int key;
bool collision = 0;
bool pause = 0;
bool death = 0;
int scoreCount = 0;

typedef struct Snake
{
    char direction_x;     // Direction on x scale
    char direction_y;     // Direction on y scale
    int head_x;           // Head coordination on x scale (in array blocks)
    int head_y;           // Head coordination on y scale (in array blocks)
    char length;          // Length of snake
    char lifetime;        // Lifetime of each part of snake - from head to tale
    float speed;          // In seconds till next movement
    float lastStepUpdate; // Time in seconds to set snake moves
    char nextDirection_x; // Next direction on x scale
    char nextDirection_y; // Next direction on y scale
    char prevDirection_x; // Previos direction on x scale
    char prevDirection_y; // Previos direction on y scale
    char headDirection_y;
    char headDirection_x;
    bool hasEaten; // Return true if food has eaten
} Snake;

typedef enum SnakeParts
{
    TURN_UP_TO_RIGHT = 0,
    BODY_HORIZONTAL,
    TURN_UP_TO_LEFT,
    HEAD_UP,
    HEAD_RIGHT,
    TURN_DOWN_TO_RIGHT,
    NOTHING_1,
    BODY_VERTICAL,
    HEAD_LEFT,
    HEAD_DOWN,
    NOTHING_2,
    NOTHING_3,
    TURN_DOWN_TO_LEFT,
    TALE_DOWN,
    TALE_LEFT,
    APPLE,
    NOTHING_4,
    NOTHING_5,
    TALE_RIGHT,
    TALE_UP,
    NUMBER_OF_PARTS,
} SnakeParts;

Texture2D textureSnakeParts[NUMBER_OF_PARTS]; // Array of textures of snake parts
Sound eatApple;
Sound wallCollision;

// Presentation canvas window as array blocks
Snake gameFeild[FEILD_WIDTH][FEILD_HEIGHT] = {0};

// Snake parametrs
Snake snake = {
    .direction_x = 1,
    .direction_y = 0,
    .nextDirection_x = 1,
    .nextDirection_y = 0,
    .head_x = FEILD_WIDTH / 2,
    .head_y = FEILD_HEIGHT / 2 - 1,
    .length = 2,
    .speed = SNAKE_SPEED,
    .lastStepUpdate = 0,
    .hasEaten = 0,
    .prevDirection_x = 1,
    .prevDirection_y = 0,
    .headDirection_x = 1,
    .headDirection_y = 0,
};
//====================================================================================

// Function declaration
//====================================================================================
void SetupSnake(void);

void CreateNewSnake(void);

void DrawSnakeBody(int, int);

void DrawSnakeHead(void);

void DrawSnakeTale(int, int);

void DrawSnake(void);

void DrawFeild(void);

void DrawFood(void);

void MoveSnake(void);

void CheckDirection(void);

void PrintDirection(int);

void CheckRecicling(void);

void SpawnFood(void);

void CheckFoodIsEaten(void);

void CheckCollision(void);

//====================================================================================

void Draw(void)
{

    // DrawFeild ();
    DrawFood();
    DrawSnake();
}

void Update()
{

    CheckDirection();
    MoveSnake();
    CheckFoodIsEaten();
}

// Function to scale canvas
Rectangle GetCanvasTarget()
{
    float screenHeight = (float)GetScreenHeight();
    float screenWidth = (float)GetScreenWidth();
    float scale = fminf(screenHeight / CANVAS_HEIGHT, screenWidth / CANVAS_WIDTH);
    Rectangle rec = {0, 0, CANVAS_WIDTH * scale, CANVAS_HEIGHT * scale};
    return rec;
}

int main(void)
{
    // Resizable window
    //-----------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "Snake");

    // Canvas setup
    RenderTexture2D canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
    Rectangle canvas_field = {0, 0, (float)canvas.texture.width, -(float)canvas.texture.height};
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);
    //-----------------------------------------------------------------------------------

    // Initialize audio device
    InitAudioDevice();

    // Load WAV audio file
    eatApple = LoadSound("resources/eat_apple.mp3");
    wallCollision = LoadSound("resources/wall_collision.mp3");

    // Declarations
    //-----------------------------------------------------------------------------------
    srand(time(NULL));
    int framesCounter = 0;

    // Upload snake parts from image to texture array
    Image snakeParts[NUMBER_OF_PARTS];
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            snakeParts[j * 5 + i] = LoadImage("resources/snake-parts.png");
            Rectangle crop = {snakeParts[j * 5 + i].width / 5 * i,
                              snakeParts[j * 5 + i].height / 4 * j,
                              snakeParts[j * 5 + i].width / 5,
                              snakeParts[j * 5 + i].height / 4};
            ImageCrop(&snakeParts[j * 5 + i], crop);
            ImageResize(&snakeParts[j * 5 + i], SNAKE_SIZE, SNAKE_SIZE);
            textureSnakeParts[j * 5 + i] = LoadTextureFromImage(snakeParts[j * 5 + i]);
            UnloadImage(snakeParts[j * 5 + i]);
        }
    }

    //-----------------------------------------------------------------------------------

    SetupSnake();
    SetTargetFPS(60);
    // Main loop
    //====================================================================================
    while (!WindowShouldClose())
    {
        // Update
        // -------------------------------------------------------------------------------
        // Restart
        if (IsKeyPressed(KEY_R))
        {
            death = 0;
            collision = 0;
            key = 0;
            SetupSnake();
        }

        if (IsKeyPressed(KEY_SPACE) && !death)
            pause = !pause;

        // Pause and death
        if (!pause && !death)
        {
            Update();
        }
        else
            framesCounter++;

        // -------------------------------------------------------------------------------

        // Draw
        // -------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        // CANVAS
        //----------------------------------------------------------------------
        BeginTextureMode(canvas);
        ClearBackground(GREEN);

        Draw();

        // On pause, we draw a blinking message
        if (pause && ((framesCounter / 30) % 2))
            DrawText("PAUSED", CANVAS_WIDTH / 2 - 60, CANVAS_HEIGHT / 2 - 20, 30, GRAY);

        if (death)
        {
            DrawText(TextFormat("SCORE: %i", scoreCount), 280, 50, 40, MAROON);
            DrawText("      YOU ARE DEAD !!!  =(\n Press R to restart the game", CANVAS_WIDTH / 2 - 220, CANVAS_HEIGHT / 2 - 60, 30, BLACK);
        }

        EndTextureMode();
        DrawTexturePro(canvas.texture, canvas_field, GetCanvasTarget(), ZERO_VEC, (float)0, WHITE);
        //----------------------------------------------------------------------------

        EndDrawing();
        // -------------------------------------------------------------------------------
    }
    //====================================================================================
    // Unload sound data
    UnloadSound(eatApple);
    UnloadSound(wallCollision);

    // Close audio device
    CloseAudioDevice();

    CloseWindow();

    return 0;
}

// FUNCTIONS
//====================================================================================

void SetupSnake(void)
{
    for (int i = 0; i < FEILD_WIDTH; i++)
    {
        for (int j = 0; j < FEILD_WIDTH; j++)
        {
            gameFeild[i][j].lifetime = 0;
        }
    }
    CreateNewSnake();
    SpawnFood();
}

void CreateNewSnake(void)
{

    snake.direction_x = 1;
    snake.direction_y = 0;
    snake.prevDirection_x = 2;
    snake.prevDirection_y = 2;
    snake.nextDirection_x = 1;
    snake.nextDirection_y = 0;
    snake.head_x = FEILD_WIDTH / 2;
    snake.head_y = FEILD_HEIGHT / 2 - 1;
    snake.length = 2;
    snake.lastStepUpdate = 0;
    snake.hasEaten = 0;

    for (int i = 0; i < snake.length; i++)
    {
        int x = snake.head_x - snake.direction_x * i;
        int y = snake.head_y - snake.direction_y * i;
        gameFeild[x][y].lifetime = snake.length - i;

        gameFeild[x][y].direction_x = snake.direction_x;
        gameFeild[x][y].direction_y = snake.direction_y;

        gameFeild[snake.head_x][snake.head_y].headDirection_x = 1;
        gameFeild[snake.head_x][snake.head_y].headDirection_y = 0;
    }
}

// Not used function. Need to draw cell in game feild, it helps in bug fixing
void DrawFeild(void)
{
    for (int i = 0; i < FEILD_WIDTH; i++)
    {
        for (int j = 0; j < FEILD_WIDTH; j++)
        {
            DrawRectangle(i * SNAKE_SIZE, j * SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE, RED);
            DrawRectangle(i * SNAKE_SIZE + 1, j * SNAKE_SIZE + 1, SNAKE_SIZE - 2, SNAKE_SIZE - 2, GREEN);
        }
    }
}

void DrawSnakeBody(int i, int j)
{
    int prev_dir_x = gameFeild[i][j].prevDirection_x;
    int prev_dir_y = gameFeild[i][j].prevDirection_y;
    int dir_x = gameFeild[i][j].direction_x;
    int dir_y = gameFeild[i][j].direction_y;

    Texture2D temporaryTexture = {0};

    if (prev_dir_x == 0 && dir_x == 0)
        temporaryTexture = textureSnakeParts[BODY_VERTICAL];
    else if (prev_dir_y == 0 && dir_y == 0)
        temporaryTexture = textureSnakeParts[BODY_HORIZONTAL];
    else if ((prev_dir_x == 0 && prev_dir_y == -1 && dir_x == -1 && dir_y == 0) ||
             (prev_dir_x == 1 && prev_dir_y == 0 && dir_x == 0 && dir_y == 1))
        temporaryTexture = textureSnakeParts[TURN_UP_TO_LEFT];
    else if ((prev_dir_x == 0 && prev_dir_y == -1 && dir_x == 1 && dir_y == 0) ||
             (prev_dir_x == -1 && prev_dir_y == 0 && dir_x == 0 && dir_y == 1))
        temporaryTexture = textureSnakeParts[TURN_UP_TO_RIGHT];
    else if ((prev_dir_x == 0 && prev_dir_y == 1 && dir_x == 1 && dir_y == 0) ||
             (prev_dir_x == -1 && prev_dir_y == 0 && dir_x == 0 && dir_y == -1))
        temporaryTexture = textureSnakeParts[TURN_DOWN_TO_RIGHT];
    else if ((prev_dir_x == 0 && prev_dir_y == 1 && dir_x == -1 && dir_y == 0) ||
             (prev_dir_x == 1 && prev_dir_y == 0 && dir_x == 0 && dir_y == -1))
        temporaryTexture = textureSnakeParts[TURN_DOWN_TO_LEFT];

    DrawTexture(temporaryTexture, i * SNAKE_SIZE, j * SNAKE_SIZE, WHITE);
}

void DrawSnakeHead(void)
{
    if (collision)
    {
        if (((gameFeild[snake.head_x + 1][snake.head_y].lifetime == 2 ||
              gameFeild[snake.head_x - 1][snake.head_y].lifetime == 2 ||
              gameFeild[snake.head_x][snake.head_y + 1].lifetime == 2 ||
              gameFeild[snake.head_x][snake.head_y - 1].lifetime == 2) &&
             (gameFeild[snake.head_x + 1][snake.head_y].lifetime != 4 &&
              gameFeild[snake.head_x - 1][snake.head_y].lifetime != 4 &&
              gameFeild[snake.head_x][snake.head_y + 1].lifetime != 4 &&
              gameFeild[snake.head_x][snake.head_y - 1].lifetime != 4)) ||
            (snake.length == 5 && gameFeild[snake.head_x][snake.head_y].lifetime > 0))
            DrawSnakeTale(snake.head_x, snake.head_y);
        else
            DrawSnakeBody(snake.head_x, snake.head_y);
    }

    Texture2D temporaryTexture = {0};

    if (gameFeild[snake.head_x][snake.head_y].headDirection_x == 1 &&
        gameFeild[snake.head_x][snake.head_y].headDirection_y == 0)
    {
        temporaryTexture = textureSnakeParts[HEAD_RIGHT];
    }
    else if (gameFeild[snake.head_x][snake.head_y].headDirection_x == -1 &&
             gameFeild[snake.head_x][snake.head_y].headDirection_y == 0)
    {
        temporaryTexture = textureSnakeParts[HEAD_LEFT];
    }
    else if (gameFeild[snake.head_x][snake.head_y].headDirection_x == 0 &&
             gameFeild[snake.head_x][snake.head_y].headDirection_y == -1)
    {
        temporaryTexture = textureSnakeParts[HEAD_UP];
    }
    else if (gameFeild[snake.head_x][snake.head_y].headDirection_x == 0 &&
             gameFeild[snake.head_x][snake.head_y].headDirection_y == 1)
    {
        temporaryTexture = textureSnakeParts[HEAD_DOWN];
    }

    DrawTexture(temporaryTexture, snake.head_x * SNAKE_SIZE, snake.head_y * SNAKE_SIZE, WHITE);
}

void DrawSnakeTale(int i, int j)
{

    Texture2D temporaryTexture = {0};

    if (gameFeild[i][j].direction_x == -1 && gameFeild[i][j].direction_y == 0)
    {
        temporaryTexture = textureSnakeParts[TALE_RIGHT];
    }
    if (gameFeild[i][j].direction_x == 1 && gameFeild[i][j].direction_y == 0)
    {
        temporaryTexture = textureSnakeParts[TALE_LEFT];
    }
    if (gameFeild[i][j].direction_x == 0 && gameFeild[i][j].direction_y == 1)
    {
        temporaryTexture = textureSnakeParts[TALE_UP];
    }
    if (gameFeild[i][j].direction_x == 0 && gameFeild[i][j].direction_y == -1)
    {
        temporaryTexture = textureSnakeParts[TALE_DOWN];
    }
    DrawTexture(temporaryTexture, i * SNAKE_SIZE, j * SNAKE_SIZE, WHITE);
}

void DrawSnake(void)
{
    for (int i = 0; i < FEILD_WIDTH; i++)
    {
        for (int j = 0; j < FEILD_HEIGHT; j++)
        {
            if (gameFeild[i][j].lifetime > 1 && gameFeild[i][j].lifetime < snake.length)
                DrawSnakeBody(i, j);
            else if (gameFeild[i][j].lifetime == 1)
                DrawSnakeTale(i, j);
            else if (gameFeild[i][j].lifetime == 0)
            {
                gameFeild[i][j].prevDirection_x = gameFeild[i][j].prevDirection_y = gameFeild[i][j].direction_x = gameFeild[i][j].direction_y = gameFeild[i][j].headDirection_x = gameFeild[i][j].headDirection_y = 2;
            }
        }
    }
    DrawSnakeHead();
}

void CheckFoodIsEaten(void)
{
    if (food_x == snake.head_x && food_y == snake.head_y)
    {
        snake.length++;
        scoreCount += 10;
        snake.hasEaten = 1;
        PlaySound(eatApple);
        SpawnFood();
    }
}

void MoveSnake(void)
{

    snake.lastStepUpdate += GetFrameTime();
    if (snake.lastStepUpdate >= snake.speed)
        snake.lastStepUpdate -= snake.speed;
    else
        return;

    if (snake.hasEaten)
    {
        snake.hasEaten = 0;
    }
    else
    {
        for (int i = 0; i < FEILD_WIDTH; i++)
        {
            for (int j = 0; j < FEILD_HEIGHT; j++)
            {
                if (gameFeild[i][j].lifetime > 0)
                {
                    gameFeild[i][j].lifetime--;
                }
            }
        }
    }

    gameFeild[snake.head_x][snake.head_y].prevDirection_x = snake.direction_x;
    gameFeild[snake.head_x][snake.head_y].prevDirection_y = snake.direction_y;

    snake.direction_x = snake.nextDirection_x;
    snake.direction_y = snake.nextDirection_y;

    gameFeild[snake.head_x][snake.head_y].direction_x = snake.direction_x;
    gameFeild[snake.head_x][snake.head_y].direction_y = snake.direction_y;

    snake.head_x += snake.direction_x;
    snake.head_y += snake.direction_y;

    CheckRecicling();

    gameFeild[snake.head_x][snake.head_y].headDirection_x = snake.direction_x;
    gameFeild[snake.head_x][snake.head_y].headDirection_y = snake.direction_y;

    CheckCollision();

    gameFeild[snake.head_x][snake.head_y].lifetime = snake.length;
}

void CheckRecicling(void)
{
    if (snake.head_x > FEILD_WIDTH - 1)
        snake.head_x = 0;
    if (snake.head_x < 0)
        snake.head_x = FEILD_WIDTH - 1;
    if (snake.head_y > FEILD_HEIGHT - 1)
        snake.head_y = 0;
    if (snake.head_y < 0)
        snake.head_y = FEILD_HEIGHT - 1;
}

void CheckDirection(void)
{
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && snake.direction_x != 1)
    {
        snake.nextDirection_x = -1;
        snake.nextDirection_y = 0;
        key = KEY_LEFT;
    }
    if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && snake.direction_x != -1)
    {
        snake.nextDirection_x = 1;
        snake.nextDirection_y = 0;
        key = KEY_RIGHT;
    }
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && snake.direction_y != 1)
    {
        snake.nextDirection_x = 0;
        snake.nextDirection_y = -1;
        key = KEY_UP;
    }
    if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && snake.direction_y != -1)
    {
        snake.nextDirection_x = 0;
        snake.nextDirection_y = 1;
        key = KEY_DOWN;
    }
}

void SpawnFood(void)
{
    do
    {
        food_x = rand() % FEILD_WIDTH;
        food_y = rand() % FEILD_HEIGHT;
    } while (gameFeild[food_x][food_y].lifetime > 0);
}

void DrawFood(void)
{
    DrawTexture(textureSnakeParts[APPLE], food_x * SNAKE_SIZE, food_y * SNAKE_SIZE, WHITE);
}

void CheckCollision(void)
{
    if (gameFeild[snake.head_x][snake.head_y].lifetime > 0)
    {
        collision = 1;
        death = 1;
        PlaySound(wallCollision);
    }
}