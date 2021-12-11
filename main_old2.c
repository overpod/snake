#include "raylib.h"

#define SPRITE_EDGE_SIZE 64
#define SNAKE_SIZE 40
#define GAME_SPRITES_PATH "resources/snake-parts.png"

typedef enum Actions
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    RESTART,
    PAUSE,
    NONE
} Actions;

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

enum Actions GetAction(void);

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    int action = NONE;


    InitWindow(screenWidth, screenHeight, "snake");
    SetTargetFPS(60);


    Texture2D texture = LoadTexture("resources/snake-parts.png");
    Vector2 position = { screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2 };
    Rectangle crop = {0 * SPRITE_EDGE_SIZE, 2 * SPRITE_EDGE_SIZE, SPRITE_EDGE_SIZE, SPRITE_EDGE_SIZE};


    // Array of textures of snake parts
    Texture2D gameTextures[TALE_UP + 1];

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        action = GetAction();

        DrawText(TextFormat("action: %08i", action), 0, 0, 20, LIGHTGRAY);
        //DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
        //DrawTexturePro(texture, crop, crop, position, 0, WHITE);
        //DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);

        DrawTextureRec(texture, crop, position, WHITE);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

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