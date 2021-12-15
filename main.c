#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

// Global declaration
//====================================================================================

#define SPRITE_SIZE 30
#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 400
#define SNAKE_LENGTH 45 //initial snake lenght min == 45px

//game state
typedef enum State {
    Play = 1,
    Pause = 2,
    Gameover = 3//,
    //Start = 4
};

//snake head direction
typedef enum direction {
    right = 1,
    left = 2,
    top = 3,
    bottom = 4 
} direction;

//position offsets for snake HEAD
typedef struct Offset {
        int right;
        int left;
        int top;
        int bottom;
} Offset;

//Snake path node
typedef struct Snake
{
//TODO x,y coordinates modify to Raylib Vector2    
int x;
int y;
struct Snake *next;
} Snake;


//====================================================================================

// Function declaration
//====================================================================================
void offset_calc(Offset*, int, int, int, int);

Snake* create(int, int);

Snake* add_element_start(int x, int y, Snake *head);

Snake* remove_element_end(Snake*);

Snake* reverse(Snake*);

Snake* walk(Snake*, direction, int, int);

Snake* grow(Snake*, direction, int, int);

int intersectDetect(Snake*);

int intersectDetectFood(Vector2, Snake*);

Vector2 getFoodCoordinates(int max_x, int max_y);

void DrawSnake(Snake*, Texture2D*[], direction);

// FUNCTIONS
//====================================================================================
//calculate offset for snake HEAD and snake TAIL
void offset_calc(Offset *offset, int max_x, int max_y, int x, int y)
{
    //calculate left offset
    if ((x - 1) == 0) {
        offset->left = max_x;        
    } else {
        offset->left = x - 1;
    }

    //calculate right offset
    if (x == max_x) {
        offset->right = 1;        
    } else {
        offset->right = x + 1;
    }

    //calculate top offset
    if ((y-1) == 0) {
        offset->top = max_y;        
    } else {
        offset->top = y - 1;
    }

    //calculate bottom offset
    if (y == max_y) {
        offset->bottom = 1;        
    } else {
        offset->bottom = y + 1;
    }   
}

Snake *create(int x, int y)
{
// Выделение памяти под корень списка
Snake *tmp = (Snake*)malloc(sizeof(Snake));
// Присваивание значения узлу
tmp ->x = x;
tmp ->y = y;
// Присваивание указателю на следующий элемент значения NULL
tmp -> next = NULL;
return(tmp);
}

Snake *add_element_start(int x, int y, Snake *head)
{
// Выделение памяти под узел списка
Snake *tmp = (Snake*)malloc(sizeof(Snake));
// Присваивание значения узлу
tmp -> x = x;
tmp -> y = y;
// Присваивание указателю на следующий элемент значения указателя на «голову» 
// первоначального списка
tmp -> next = head;
return(tmp);
}

Snake *remove_element_end(Snake *head)
{
// Присваивание новому указателю  tmp указателя head, p - NULL
Snake *tmp = head, *p = NULL;
// Проверка списка на пустоту
if (head == NULL)
return NULL;
// Если список не пуст, поиск указателя на искомый элемент
while (tmp->next != NULL)
{
p = tmp;
tmp = tmp -> next;
}

// Присваивание новому указателю указателя tmp
p -> next = NULL;
// Освобождение памяти для указателя tmp
free(tmp);
return head;
}


Snake* reverse(Snake* root) {
  Snake* new_root = 0;
  while (root) {
    Snake* next = root->next;
    root->next = new_root;
    new_root = root;
    root = next;
  }
  return new_root;
}

// 0 - no intersection // 1 - intersect detect
int intersectDetect(Snake* head){
    Snake *tmp = head;
    tmp = tmp -> next;
    while (tmp->next != NULL)
    {
        //intersect condition
        if (tmp->x == head->x && tmp->y == head->y) 
        {
            free(tmp);
            return 1;
        };
        tmp = tmp -> next;
    }
    free(tmp);
    return 0;   
}


// 0 - no intersection // 1 - intersect detect
int intersectDetectFood(Vector2 coord, Snake* snake){
    Snake *head = snake;
    //distance between two points AB = √(xb - xa)2 + (yb - ya)2
    double distance  = sqrt(pow((coord.x-head->x), 2.0) + pow((coord.y-head->y), 2.0));    
    free(head);
    //if the distance between food and snake head center coordinates is less than  20.0 we should assume that the food is caught    
    if (distance < 20.0 ) return 1;
    return 0;
}


Snake *walk(Snake *snake, enum direction d, int max_x, int max_y)
{
//initialising offset    
Offset off = {0,0,0,0};    
Offset *off_ptr = &off;
offset_calc(off_ptr, max_x, max_y, snake->x, snake->y);
//remove TAIL
snake = remove_element_end(snake);
        
switch (d) {
    //right
    case 1:        
        snake = add_element_start(off_ptr->right, snake->y, snake);         
        break;
    //left    
    case 2:        
        snake = add_element_start(off_ptr->left, snake->y, snake);
        break;        
    //top    
    case 3:        
        snake = add_element_start( snake->x, off_ptr->top, snake);
        break;        
    //bottom    
    case 4:        
        snake = add_element_start(snake->x, off_ptr->bottom, snake);
        break;        
    }
    
    return snake;
}

Snake *grow(Snake *snake, enum direction d, int max_x, int max_y)
{
//initialising offset    
Offset off = {0,0,0,0};    
Offset *off_ptr = &off;              
int counter;

switch (d) {
    //right
    case 1:
            
            for (counter=0; counter<20; counter++){
                offset_calc(off_ptr, max_x, max_y, snake->x, snake->y);
                snake = add_element_start(off_ptr->right++, snake->y, snake);
                if (intersectDetect(snake)==1){
                    printf("intersect detection");
                };
            }        
                 
        break;
    //left    
    case 2:
         for (counter=0; counter<20; counter++){
                offset_calc(off_ptr, max_x, max_y, snake->x, snake->y);
                snake = add_element_start(off_ptr->left++, snake->y, snake);
                if (intersectDetect(snake)==1){
                    printf("intersect detection");
                };
        }              
        
        break;        
    //top    
    case 3:
        for (counter=0; counter<20; counter++){
                offset_calc(off_ptr, max_x, max_y, snake->x, snake->y);
                snake = add_element_start( snake->x, off_ptr->top++, snake);
                if (intersectDetect(snake)==1){
                    printf("intersect detection");
                };
        }              
        
        break;        
    //bottom    
    case 4:        
        for (counter=0; counter<20; counter++){
                offset_calc(off_ptr, max_x, max_y, snake->x, snake->y);
                snake = add_element_start(snake->x, off_ptr->bottom++, snake);
                if (intersectDetect(snake)==1){
                    printf("intersect detection");
                };
        }  
        
        break;        
    }   
    return snake;
}

int getSnakePathLength(Snake *snake) {
    int counter = 0;
    Snake *tmp = snake;
    while (tmp != NULL){
        counter++;
        tmp = tmp -> next;
    }
    free(tmp);
    return counter;
}

void DrawSnake(Snake *snake, Texture2D* textures[], enum direction d){
    Vector2 snakeHeadPosition = { snake->x, snake->y };
    snake = reverse(snake);
            //draw snake body
            Snake *tmp = snake;
            // we will draw snake from tail to head            
            int path_counter=1;            
            while (tmp != NULL)
            {
            //draw the snake
            if (path_counter%15 == 0) {
                snakeHeadPosition.x = tmp->x;
                snakeHeadPosition.y = tmp->y;                     
                snakeHeadPosition.x = tmp->x - textures[0]->width/4;
                snakeHeadPosition.y = tmp->y - textures[0]->height/4;
                DrawTextureEx(*textures[0], snakeHeadPosition, 0.0, 0.5, WHITE);
            }

            if (tmp->next == NULL) {
                snakeHeadPosition.x = tmp->x - textures[(int)d]->width/4;
                snakeHeadPosition.y = tmp->y - textures[(int)d]->height/4;
                DrawTextureEx(*textures[(int)d], snakeHeadPosition, 0.0, 0.5, WHITE);
            }

            path_counter++;               
    

            tmp = tmp -> next;
            }
           free(tmp);
           snake = reverse(snake);
}

//generate random coordinates for 
 Vector2 getFoodCoordinates(int max_x, int max_y)
{
    
    Vector2 rand_coord = { rand()%max_x, rand()%max_y};
    return rand_coord;
}

/////////////////////////////////////////////////////////////////////////////////////


int main(void)
{
    // Resizable window
    //-----------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "STARWARS SNAKE BATTLE");
    SetWindowMinSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    SetTargetFPS(60);

    //Load snake sprites

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)    
    Image snake_head_img = LoadImage("resources/head.png");
    Texture2D head_top =  LoadTextureFromImage(snake_head_img);
    ImageRotateCW(&snake_head_img);
    Texture2D head_right =  LoadTextureFromImage(snake_head_img);
    ImageRotateCW(&snake_head_img);
    Texture2D head_bottom =  LoadTextureFromImage(snake_head_img);
    ImageRotateCW(&snake_head_img);
    Texture2D head_left =  LoadTextureFromImage(snake_head_img);
    Texture2D snake_body_texture = LoadTexture("resources/body.png");

    Texture2D* textures[] = {
        &snake_body_texture,
        &head_right,
        &head_left,
        &head_top,
        &head_bottom         
    };

    //RenderTexture2D target = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
    //SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    //-----------------------------------------------------------------------------------

    //TODO find Starwars sounds
    // Initialize audio device
    //InitAudioDevice();

    // Load WAV audio file
    //eatApple = LoadSound("resources/eat_apple.mp3");
    //wallCollision = LoadSound("resources/wall_collision.mp3");

    // Declarations
    //-----------------------------------------------------------------------------------
    srand(time(NULL));
    

    // initialize snake path initial coordinates   
    Snake *snake = create(20, 20);
    
    //add to snake path (50 px) 50 nodes
    int counter;
    for (counter=0; counter<=50; counter++){
        snake = add_element_start(counter + 20,20, snake);              
    }
    
    enum direction direct = 1;
    enum State GameState = 1;    
    int framesCounter = 0;
    int framesSpeed = 30; 
    int timer = 0;
    Vector2 food_coordinates = getFoodCoordinates(GetScreenWidth(), GetScreenHeight());
    
    


    // Main loop
    //====================================================================================
    while (!WindowShouldClose())
    {

        /*
        float scale = min((float)GetScreenWidth() / CANVAS_WIDTH, (float)GetScreenHeight() / CANVAS_HEIGHT);
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

        ////////////////////////////////////////////////////////////////
        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            // see what display we are on right now
            int display = GetCurrentMonitor();

            if (IsWindowFullscreen())
            {
                ToggleFullscreen();
                // if we are full screen, then go back to the windowed size
                SetWindowSize(CANVAS_WIDTH, CANVAS_HEIGHT);
            }
            else
            {
                // if we are not full screen, set the window size to match the monitor we are on
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));

                // toggle the state
                ToggleFullscreen();
            }
        }
        */


        // Update cycle
        //----------------------------------------------------------------------------------

        framesCounter++;

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            int counter;
            for (counter=0; counter<4; counter++){
                snake = walk(snake, direct, GetScreenWidth(), GetScreenHeight());
                /*
                if (intersectDetect(snake)==1){
                    //TODO GAME OVER render
                    printf("Game Over");
                    printf("%f\n", GetTime());
                };
                
                
                int condition = intersectDetectFood(food_coordinates, snake);

                if (condition==1){
                    //TODO New FOOD timing correction
                    printf("Got the Food");
                    printf("%f\n", GetTime());
                    food_coordinates = getFoodCoordinates(GetScreenWidth(), GetScreenHeight());
                    snake = grow(snake, direct, GetScreenWidth(), GetScreenHeight());
                };
                */
                

            }
            
        }

        // 8 seconds for food to disappear
        int t = (int)(fmod (GetTime(),8.0));
        if (timer !=  t) {
            timer = t;
            if (timer == 0) {
                food_coordinates = getFoodCoordinates(GetScreenWidth(), GetScreenHeight());
            };
        };
       

        //Change Snake Head direction by pressing arrow keys
        if (IsKeyPressed(KEY_RIGHT)) {
            //skip opposite direction
            if (direct!=2) direct = 1;
        };
        
        if (IsKeyPressed(KEY_LEFT)) {
            //skip opposite direction
            if (direct!=1) direct = 2;
        };

        if (IsKeyPressed(KEY_UP)) {
            //skip opposite direction
            if (direct!=4) direct = 3;
        };
        
        if (IsKeyPressed(KEY_DOWN)) {
             //skip opposite direction
            if (direct!=3) direct = 4;
        };                
        

        // -------------------------------------------------------------------------------
        // Draw
        // -------------------------------------------------------------------------------
        
        //TODO Pause and death conditions
        /*
        // On pause, we draw a blinking message
        if (pause && ((framesCounter / 30) % 2))
            DrawText("PAUSED", CANVAS_WIDTH / 2 - 60, CANVAS_HEIGHT / 2 - 20, 30, GRAY);

        if (death)
        {
            DrawText(TextFormat("SCORE: %i", scoreCount), 280, 50, 40, MAROON);
            DrawText("      YOU ARE DEAD !!!  =(\n Press R to restart the game", CANVAS_WIDTH / 2 - 220, CANVAS_HEIGHT / 2 - 60, 30, BLACK);
        }

        */


        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("move snake with arrow keys", 10, 10, 20, WHITE);

            DrawSnake(snake, textures, direct);
            DrawTextureEx(*textures[0], food_coordinates, 0.0, 0.5, WHITE);            
           
        EndDrawing();     
    }
    
    /*
    TODO starwars sounds
    //====================================================================================
    // Unload sound data
    UnloadSound(eatApple);
    UnloadSound(wallCollision);

    // Close audio device
    CloseAudioDevice();
    */
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(snake_body_texture);       // Texture unloading
    UnloadTexture(head_top);
    UnloadTexture(head_bottom);
    UnloadTexture(head_right);
    UnloadTexture(head_left);    
    UnloadImage(snake_head_img);    

    CloseWindow();

    return 0;
}

