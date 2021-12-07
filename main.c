#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

#define SPRITE_EDGE_SIZE 64
#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 400
#define SNAKE_SIZE 40
#define SNAKE_SPEED 0.3f

#define FEILD_WIDTH CANVAS_WIDTH / SNAKE_SIZE
#define FEILD_HEIGHT CANVAS_HEIGHT / SNAKE_SIZE

const Vector2 ZERO_VEC = {0, 0};

// Global declaration
//====================================================================================

int foodX;
int foodY;
int key;
bool collision = 0;
bool pause = 0;
bool death = 0;
int scoreCount = 0;

typedef struct Snake
{
    char directionX;      // Direction on x scale
    char directionY;      // Direction on y scale
    int headX;            // Head coordination on x scale (in array blocks)
    int headY;            // Head coordination on y scale (in array blocks)
    char length;          // Length of snake
    char lifeTime;        // lifeTime of each part of snake - from head to tale
    float speed;          // In seconds till next movement
    float lastStepUpdate; // Time in seconds to set snake moves
    char nextDirectionX;  // Next direction on x scale
    char nextDirectionY;  // Next direction on y scale
    char prevDirectionX;  // Previos direction on x scale
    char prevDirectionY;  // Previos direction on y scale
    char headDirectionY;
    char headDirectionX;
    bool hasEaten; // Return true if food has eaten
} Snake;

typedef enum SnakeParts
{
    TURN_UP_TO_RIGHT,
    BODY_HORIZONTAL,
    TURN_UP_TO_LEFT,
    HEAD_UP,
    HEAD_RIGHT,
    TURN_DOWN_TO_RIGHT,
    BODY_VERTICAL,
    HEAD_LEFT,
    HEAD_DOWN,
    TURN_DOWN_TO_LEFT,
    TALE_DOWN,
    TALE_LEFT,
    APPLE,
    TALE_RIGHT,
    TALE_UP
} SnakeParts;

Texture2D textureSnakeParts[TALE_UP + 1]; // Array of textures of snake parts
Sound eatApple;
Sound wallCollision;

// Presentation canvas window as array blocks
Snake gameFeild[FEILD_WIDTH][FEILD_HEIGHT] = {0};

// Snake parametrs
Snake snake = {
    .directionX = 1,
    .directionY = 0,
    .nextDirectionX = 1,
    .nextDirectionY = 0,
    .headX = FEILD_WIDTH / 2,
    .headY = FEILD_HEIGHT / 2 - 1,
    .length = 2,
    .speed = SNAKE_SPEED,
    .lastStepUpdate = 0,
    .hasEaten = 0,
    .prevDirectionX = 1,
    .prevDirectionY = 0,
    .headDirectionX = 1,
    .headDirectionY = 0,
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

void UploadSnakeParts(void);

//====================================================================================

void Draw(void)
{
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
    Rectangle canvasField = {0, 0, canvas.texture.width, -canvas.texture.height};
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

    UploadSnakeParts();

    //-----------------------------------------------------------------------------------

    SetupSnake();
    SetTargetFPS(60);
    // Main loop
    //====================================================================================
    while (!WindowShouldClose())
    {
        // Restart
        if (IsKeyPressed(KEY_R))
        {
            death = 0;
            collision = 0;
            key = 0;
            SetupSnake();
        }
        // Pause
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
        DrawTexturePro(canvas.texture, canvasField, GetCanvasTarget(), ZERO_VEC, 0, WHITE);

        EndDrawing();
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

void UploadSnakeParts(void)
{
    // Upload snake parts from image to texture array
    Image snakeParts[TALE_UP + 1];
    Image snakePartsImage = LoadImage("resources/snake-parts.png");

    for (int textureIndex = TURN_UP_TO_RIGHT; textureIndex < TALE_UP + 1; textureIndex++)
    {
        int x = 0;
        int y = 0;
        switch (textureIndex)
        {
        case TURN_UP_TO_RIGHT:
            x = 0;
            y = 0;
            break;
        case BODY_HORIZONTAL:
            x = 1;
            y = 0;
            break;
        case TURN_UP_TO_LEFT:
            x = 2;
            y = 0;
            break;
        case HEAD_UP:
            x = 3;
            y = 0;
            break;
        case HEAD_RIGHT:
            x = 4;
            y = 0;
            break;
        case TURN_DOWN_TO_RIGHT:
            x = 0;
            y = 1;
            break;
        case BODY_VERTICAL:
            x = 2;
            y = 1;
            break;
        case HEAD_LEFT:
            x = 3;
            y = 1;
            break;
        case HEAD_DOWN:
            x = 4;
            y = 1;
            break;
        case TURN_DOWN_TO_LEFT:
            x = 2;
            y = 2;
            break;
        case TALE_DOWN:
            x = 3;
            y = 2;
            break;
        case TALE_LEFT:
            x = 4;
            y = 2;
            break;
        case APPLE:
            x = 0;
            y = 3;
            break;
        case TALE_RIGHT:
            x = 3;
            y = 3;
            break;
        case TALE_UP:
            x = 4;
            y = 3;
            break;
        default:
            break;
        }

        Rectangle crop = {x * SPRITE_EDGE_SIZE, y * SPRITE_EDGE_SIZE, SPRITE_EDGE_SIZE, SPRITE_EDGE_SIZE};
        Image partImage = ImageFromImage(snakePartsImage, crop);
        ImageResize(&partImage, SNAKE_SIZE, SNAKE_SIZE);
        textureSnakeParts[textureIndex] = LoadTextureFromImage(partImage);
    }
}

void SetupSnake(void)
{
    for (int width = 0; width < FEILD_WIDTH; width++)
    {
        for (int height = 0; height < FEILD_WIDTH; height++)
        {
            gameFeild[width][height].lifeTime = 0;
        }
    }
    CreateNewSnake();
    SpawnFood();
}

void CreateNewSnake(void)
{
    snake.directionX = 1;
    snake.directionY = 0;
    snake.prevDirectionX = 2;
    snake.prevDirectionY = 2;
    snake.nextDirectionX = 1;
    snake.nextDirectionY = 0;
    snake.headX = FEILD_WIDTH / 2;
    snake.headY = FEILD_HEIGHT / 2 - 1;
    snake.length = 2;
    snake.lastStepUpdate = 0;
    snake.hasEaten = 0;

    for (int i = 0; i < snake.length; i++)
    {
        int x = snake.headX - snake.directionX * i;
        int y = snake.headY - snake.directionY * i;
        gameFeild[x][y].lifeTime = snake.length - i;

        gameFeild[x][y].directionX = snake.directionX;
        gameFeild[x][y].directionY = snake.directionY;

        gameFeild[snake.headX][snake.headY].headDirectionX = 1;
        gameFeild[snake.headX][snake.headY].headDirectionY = 0;
    }
}

// Not used function. Need to draw cell in game feild, it helps in bug fixing
void DrawFeild(void)
{
    for (int x = 0; x < FEILD_WIDTH; x++)
    {
        for (int y = 0; y < FEILD_WIDTH; y++)
        {
            DrawRectangle(x * SNAKE_SIZE, y * SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE, RED);
            DrawRectangle(x * SNAKE_SIZE + 1, y * SNAKE_SIZE + 1, SNAKE_SIZE - 2, SNAKE_SIZE - 2, GREEN);
        }
    }
}

void DrawSnakeBody(int x, int y)
{
    int prevDirX = gameFeild[x][y].prevDirectionX;
    int prevDirY = gameFeild[x][y].prevDirectionY;
    int dirX = gameFeild[x][y].directionX;
    int dirY = gameFeild[x][y].directionY;

    Texture2D temporaryTexture = {0};

    if (prevDirX == 0 && dirX == 0)
        temporaryTexture = textureSnakeParts[BODY_VERTICAL];
    else if (prevDirY == 0 && dirY == 0)
        temporaryTexture = textureSnakeParts[BODY_HORIZONTAL];
    else if ((prevDirX == 0 && prevDirY == -1 && dirX == -1 && dirY == 0) ||
             (prevDirX == 1 && prevDirY == 0 && dirX == 0 && dirY == 1))
        temporaryTexture = textureSnakeParts[TURN_UP_TO_LEFT];
    else if ((prevDirX == 0 && prevDirY == -1 && dirX == 1 && dirY == 0) ||
             (prevDirX == -1 && prevDirY == 0 && dirX == 0 && dirY == 1))
        temporaryTexture = textureSnakeParts[TURN_UP_TO_RIGHT];
    else if ((prevDirX == 0 && prevDirY == 1 && dirX == 1 && dirY == 0) ||
             (prevDirX == -1 && prevDirY == 0 && dirX == 0 && dirY == -1))
        temporaryTexture = textureSnakeParts[TURN_DOWN_TO_RIGHT];
    else if ((prevDirX == 0 && prevDirY == 1 && dirX == -1 && dirY == 0) ||
             (prevDirX == 1 && prevDirY == 0 && dirX == 0 && dirY == -1))
        temporaryTexture = textureSnakeParts[TURN_DOWN_TO_LEFT];

    DrawTexture(temporaryTexture, x * SNAKE_SIZE, y * SNAKE_SIZE, WHITE);
}

void DrawSnakeHead(void)
{
    if (collision)
    {
        if (((gameFeild[snake.headX + 1][snake.headY].lifeTime == 2 ||
              gameFeild[snake.headX - 1][snake.headY].lifeTime == 2 ||
              gameFeild[snake.headX][snake.headY + 1].lifeTime == 2 ||
              gameFeild[snake.headX][snake.headY - 1].lifeTime == 2) &&
             (gameFeild[snake.headX + 1][snake.headY].lifeTime != 4 &&
              gameFeild[snake.headX - 1][snake.headY].lifeTime != 4 &&
              gameFeild[snake.headX][snake.headY + 1].lifeTime != 4 &&
              gameFeild[snake.headX][snake.headY - 1].lifeTime != 4)) ||
            (snake.length == 5 && gameFeild[snake.headX][snake.headY].lifeTime > 0))
            DrawSnakeTale(snake.headX, snake.headY);
        else
            DrawSnakeBody(snake.headX, snake.headY);
    }

    Texture2D temporaryTexture = {0};

    if (gameFeild[snake.headX][snake.headY].headDirectionX == 1 &&
        gameFeild[snake.headX][snake.headY].headDirectionY == 0)
    {
        temporaryTexture = textureSnakeParts[HEAD_RIGHT];
    }
    else if (gameFeild[snake.headX][snake.headY].headDirectionX == -1 &&
             gameFeild[snake.headX][snake.headY].headDirectionY == 0)
    {
        temporaryTexture = textureSnakeParts[HEAD_LEFT];
    }
    else if (gameFeild[snake.headX][snake.headY].headDirectionX == 0 &&
             gameFeild[snake.headX][snake.headY].headDirectionY == -1)
    {
        temporaryTexture = textureSnakeParts[HEAD_UP];
    }
    else if (gameFeild[snake.headX][snake.headY].headDirectionX == 0 &&
             gameFeild[snake.headX][snake.headY].headDirectionY == 1)
    {
        temporaryTexture = textureSnakeParts[HEAD_DOWN];
    }

    DrawTexture(temporaryTexture, snake.headX * SNAKE_SIZE, snake.headY * SNAKE_SIZE, WHITE);
}

void DrawSnakeTale(int x, int y)
{

    Texture2D temporaryTexture = {0};

    if (gameFeild[x][y].directionX == -1 && gameFeild[x][y].directionY == 0)
    {
        temporaryTexture = textureSnakeParts[TALE_RIGHT];
    }
    if (gameFeild[x][y].directionX == 1 && gameFeild[x][y].directionY == 0)
    {
        temporaryTexture = textureSnakeParts[TALE_LEFT];
    }
    if (gameFeild[x][y].directionX == 0 && gameFeild[x][y].directionY == 1)
    {
        temporaryTexture = textureSnakeParts[TALE_UP];
    }
    if (gameFeild[x][y].directionX == 0 && gameFeild[x][y].directionY == -1)
    {
        temporaryTexture = textureSnakeParts[TALE_DOWN];
    }
    DrawTexture(temporaryTexture, x * SNAKE_SIZE, y * SNAKE_SIZE, WHITE);
}

void DrawSnake(void)
{
    for (int x = 0; x < FEILD_WIDTH; x++)
    {
        for (int y = 0; y < FEILD_HEIGHT; y++)
        {
            if (gameFeild[x][y].lifeTime > 1 && gameFeild[x][y].lifeTime < snake.length)
                DrawSnakeBody(x, y);
            else if (gameFeild[x][y].lifeTime == 1)
                DrawSnakeTale(x, y);
            else if (gameFeild[x][y].lifeTime == 0)
            {
                gameFeild[x][y].prevDirectionX = gameFeild[x][y].prevDirectionY = gameFeild[x][y].directionX = gameFeild[x][y].directionY = gameFeild[x][y].headDirectionX = gameFeild[x][y].headDirectionY = 2;
            }
        }
    }
    DrawSnakeHead();
}

void CheckFoodIsEaten(void)
{
    if (foodX == snake.headX && foodY == snake.headY)
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
        for (int x = 0; x < FEILD_WIDTH; x++)
        {
            for (int y = 0; y < FEILD_HEIGHT; y++)
            {
                if (gameFeild[x][y].lifeTime > 0)
                {
                    gameFeild[x][y].lifeTime--;
                }
            }
        }
    }

    gameFeild[snake.headX][snake.headY].prevDirectionX = snake.directionX;
    gameFeild[snake.headX][snake.headY].prevDirectionY = snake.directionY;

    snake.directionX = snake.nextDirectionX;
    snake.directionY = snake.nextDirectionY;

    gameFeild[snake.headX][snake.headY].directionX = snake.directionX;
    gameFeild[snake.headX][snake.headY].directionY = snake.directionY;

    snake.headX += snake.directionX;
    snake.headY += snake.directionY;

    CheckRecicling();

    gameFeild[snake.headX][snake.headY].headDirectionX = snake.directionX;
    gameFeild[snake.headX][snake.headY].headDirectionY = snake.directionY;

    CheckCollision();

    gameFeild[snake.headX][snake.headY].lifeTime = snake.length;
}

void CheckRecicling(void)
{
    if (snake.headX > FEILD_WIDTH - 1)
        snake.headX = 0;
    if (snake.headX < 0)
        snake.headX = FEILD_WIDTH - 1;
    if (snake.headY > FEILD_HEIGHT - 1)
        snake.headY = 0;
    if (snake.headY < 0)
        snake.headY = FEILD_HEIGHT - 1;
}

void CheckDirection(void)
{
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && snake.directionX != 1)
    {
        snake.nextDirectionX = -1;
        snake.nextDirectionY = 0;
        key = KEY_LEFT;
    }
    if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && snake.directionX != -1)
    {
        snake.nextDirectionX = 1;
        snake.nextDirectionY = 0;
        key = KEY_RIGHT;
    }
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && snake.directionY != 1)
    {
        snake.nextDirectionX = 0;
        snake.nextDirectionY = -1;
        key = KEY_UP;
    }
    if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && snake.directionY != -1)
    {
        snake.nextDirectionX = 0;
        snake.nextDirectionY = 1;
        key = KEY_DOWN;
    }
}

void SpawnFood(void)
{
    do
    {
        foodX = rand() % FEILD_WIDTH;
        foodY = rand() % FEILD_HEIGHT;
    } while (gameFeild[foodX][foodY].lifeTime > 0);
}

void DrawFood(void)
{
    DrawTexture(textureSnakeParts[APPLE], foodX * SNAKE_SIZE, foodY * SNAKE_SIZE, WHITE);
}

void CheckCollision(void)
{
    if (gameFeild[snake.headX][snake.headY].lifeTime > 0)
    {
        collision = 1;
        death = 1;
        PlaySound(wallCollision);
    }
}