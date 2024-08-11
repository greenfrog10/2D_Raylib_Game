#include "raylib.h"
#include <stdio.h> 

class Window{
    public:
        int width;
        int height;
};

class Player
{
    public:
        int x;
        int y;
        int size;
        int speed;
        int y_velocity = 0;
        bool jumping;
        bool is_on_floor = false;
        bool fall = false;
        bool is_jumping = false;
        bool block_jump = false;
        int y_before_jump = 0;
        int max_jump_height = 250;
        Texture2D sprite;
};
class Object
{
    public:
        int x;
        int y;  
        Texture2D sprite;
};
int main()
{
    Window window;
    window.width = 1200;
    window.height = 900;
    InitWindow(window.width, window.height,"My game");

    Player player;
    player.x = window.width / 2;
    player.y = window.height / 2;
    player.size = 60;
    player.speed = 10;
    SetTargetFPS(60);   
    player.sprite = LoadTexture("sprites/player.png");
    char playerXStr[10];
    char playerYStr[10];
    Texture2D target_sprite;
    target_sprite = LoadTexture("sprites/target.png");
    // Main game loop
    while (!WindowShouldClose())
    {
        // check Key inputs
        if (IsKeyDown(KEY_RIGHT) || (IsKeyDown(KEY_D))) player.x += player.speed;
        if (IsKeyDown(KEY_LEFT) || (IsKeyDown(KEY_A))) player.x -= player.speed;
        if (IsKeyDown(KEY_UP) || (IsKeyDown(KEY_W)))
        {
            if (player.block_jump != true)
            {
                if (player.is_on_floor) player.is_jumping = true;
                player.block_jump = true;
                player.is_on_floor = false;
                player.y_before_jump = player.y;
            }
        }
        //Moving The player
        if(player.x >= 1100) player.x = 1100;
        if(player.x <= 10) player.x = 10;
        if(player.fall) player.y += 10;
        if (player.is_on_floor == false && player.is_jumping == false) player.fall = true;
        if (player.is_jumping) player.y -= 15;
        if (player.y_before_jump - player.y >= player.max_jump_height) player.is_jumping = false;
        if (player.is_on_floor == true)
        {
            player.fall = false;
            player.block_jump = false;
        }
        if (player.y >= 820)
        {            
            player.is_on_floor = true;
        }
        // Convert player.x and player.y to string to show on screen
        sprintf(playerXStr, "%d", player.x);
        sprintf(playerYStr, "%d", player.y);
        //draw the window
        BeginDrawing();
        ClearBackground(GREEN);
         //hide mouse and change it to target
        int target_x = GetMouseX();
        int target_y = GetMouseY();
        HideCursor();
        DrawTexture(player.sprite, player.x, player.y, WHITE);
        DrawTexture(target_sprite, target_x, target_y, WHITE);
        DrawText(playerXStr, 10, 10, 80, WHITE);
        DrawText(playerYStr, 10, 80, 80, WHITE);
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) DrawText("MOUSE CLICK DETECTED",10,180,80,WHITE);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
