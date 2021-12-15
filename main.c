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

typedef struct AnimationСounters
{
    int framesCounter;
    int currentFrame;
} AnimationСounters;

//
enum Actions GetAction(void);
void updateAnimateFrameCount(AnimationСounters *animationСounters, int framesSpeed, int maxFrameCount);

int main(void)
{
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "game");

    Texture2D appleSprites = LoadTexture("resources/apple.png");
    int appleEdgeLength = 32;
    int appleSpritesFramesCount = appleSprites.width / appleEdgeLength;

    Vector2 position = {0, GAME_HEIGHT / 2};
    Rectangle frameRec = {0, 0, appleEdgeLength, appleEdgeLength};
    AnimationСounters animationСounters = {0, 0};

    SetTargetFPS(60);

    int action = NONE;

    // for snake apple animation
    Texture2D snakePartsImage = LoadTexture("resources/snake-parts.png");

    Rectangle snakeAppleSource = {0, 2 * 64, 64, 64};
    Vector2 snakeApplePosition = {GAME_WIDTH / 2, GAME_HEIGHT / 2};

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        updateAnimateFrameCount(&animationСounters, 16, appleSpritesFramesCount);
        frameRec.x = animationСounters.currentFrame * appleEdgeLength;

        // Draw render texture to screen, properly scaled
        BeginDrawing();
        ClearBackground(RAYWHITE);

        action = GetAction();
        DrawText(TextFormat("action: %08i", action), 0, 0, 20, LIGHTGRAY);
        DrawText(TextFormat("FPS: %08i", GetFPS()), 0, 100, 20, BLACK);
        DrawText(TextFormat("currentFrame: %08i", animationСounters.currentFrame), 0, 200, 20, BLACK);

        DrawTextureRec(appleSprites, frameRec, position, WHITE);

        DrawTextureRec(snakePartsImage, snakeAppleSource, snakeApplePosition, WHITE);
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

void updateAnimateFrameCount(
    AnimationСounters *animationСounters,
    const int framesSpeed,
    const int maxFrameCount)
{

    animationСounters->framesCounter++;
    if (animationСounters->framesCounter >= (GetFPS() / framesSpeed))
    {
        animationСounters->framesCounter = 0;
        animationСounters->currentFrame++;

        if (animationСounters->currentFrame > maxFrameCount)
            animationСounters->currentFrame = 0;
    }
}