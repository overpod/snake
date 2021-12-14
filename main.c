#include "raylib.h"

#define GAME_WIDTH 800
#define GAME_HEIGHT 450

enum Actions
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    RESTART,
    PAUSE,
    NONE
};

//
enum Actions GetAction(void);
void UpdateAnimateFrame(int maxFPS, int *framesCounter, int *currentFrame, int framesSpeed, int maxFrameCount);

int main(void)
{
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "game");

    Texture2D appleSprites = LoadTexture("resources/apple.png");
    int appleEdgeLength = 32;
    int appleSpritesFramesCount = appleSprites.width / appleEdgeLength;

    Vector2 position = {0, GAME_HEIGHT / 2};
    Rectangle frameRec = {0, 0, appleEdgeLength, appleEdgeLength};
    int currentFrame = 0;

    int framesCounter = 0;
    int maxFPS = 60;

    SetTargetFPS(maxFPS);

    int action = NONE;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateAnimateFrame(maxFPS, &framesCounter, &currentFrame, 16, appleSpritesFramesCount);
        frameRec.x = currentFrame * appleEdgeLength;

        // Draw render texture to screen, properly scaled
        BeginDrawing();
        ClearBackground(RAYWHITE);

        action = GetAction();
        DrawText(TextFormat("action: %08i", action), 0, 0, 20, LIGHTGRAY);
        DrawText(TextFormat("currentFrame: %08i", currentFrame), 0, 100, 20, BLACK);

        DrawTextureRec(appleSprites, frameRec, position, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

enum Actions GetAction(void)
{
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        return LEFT;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        return RIGHT;

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        return UP;

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        return DOWN;

    if (IsKeyDown(KEY_R))
        return RESTART;

    if (IsKeyDown(KEY_SPACE))
        return PAUSE;

    return NONE;
}

void UpdateAnimateFrame(const int maxFPS, int *framesCounter, int *currentFrame, const int framesSpeed, const int maxFrameCount)
{
    (*framesCounter)++;
    if ((*framesCounter) >= (maxFPS / framesSpeed))
    {
        (*framesCounter) = 0;
        (*currentFrame)++;

        if ((*currentFrame) > maxFrameCount)
            (*currentFrame) = 0;
    }
}