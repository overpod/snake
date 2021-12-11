#include "raylib.h"

#define SPRITE_EDGE_SIZE 64
#define SNAKE_SIZE 40
#define GAME_SPRITES_PATH "resources/snake-parts.png"

#define GAME_WIDTH 800
#define GAME_HEIGHT 400

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

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

//
Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max);
enum Actions GetAction(void);

int main(void)
{
    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "snake");
    SetWindowMinSize(GAME_WIDTH / 2, GAME_HEIGHT / 2);

    int gameScreenWidth = GAME_WIDTH;
    int gameScreenHeight = GAME_HEIGHT;

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    // Texture scale filter to use
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int action = NONE;

    Texture2D texture = LoadTexture("resources/snake-parts.png");
    Vector2 position = {GAME_WIDTH / 2 - texture.width / 2, GAME_HEIGHT / 2 - texture.height / 2};
    Rectangle crop = {0 * SPRITE_EDGE_SIZE, 2 * SPRITE_EDGE_SIZE, SPRITE_EDGE_SIZE, SPRITE_EDGE_SIZE};

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Compute required framebuffer scaling
        float scale = min((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);

        // Draw
        //----------------------------------------------------------------------------------
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        action = GetAction();
        DrawText(TextFormat("action: %08i", action), 0, 0, 20, LIGHTGRAY);

        DrawTextureRec(texture, crop, position, WHITE);

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw render texture to screen, properly scaled
        DrawTexturePro(target.texture, (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                       (Rectangle){(GetScreenWidth() - ((float)gameScreenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)gameScreenHeight * scale)) * 0.5f,
                                   (float)gameScreenWidth * scale, (float)gameScreenHeight * scale},
                       (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target); // Unload render texture

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Clamp Vector2 value with min and max and return a new vector2
// NOTE: Required for virtual mouse, to clamp inside virtual game size
Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max)
{
    Vector2 result = value;
    result.x = (result.x > max.x) ? max.x : result.x;
    result.x = (result.x < min.x) ? min.x : result.x;
    result.y = (result.y > max.y) ? max.y : result.y;
    result.y = (result.y < min.y) ? min.y : result.y;
    return result;
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