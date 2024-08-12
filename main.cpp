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
    Vector2 center{window.width / 2,window.height / 2};
    Player player;
    player.size = 60;
    player.speed = 10;
    SetTargetFPS(60);
    player.x = window.width / 2;
    player.y = window.height / 2;
    player.sprite = LoadTexture("sprites/player.png");
    char playerXStr[10];
    char playerYStr[10];
    char targetXStr[10];
    char targetYStr[10];
    Object target;
    target.x = 0;
    target.y = 0;
    target.sprite = LoadTexture("sprites/target.png");
    Object bullet;
    bool shot_fired = false;
    int click_x = GetMouseX();
    int click_y = GetMouseY();
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
            player.y = 820;
        }
        else
        {
            player.is_on_floor = false;
        }
        Rectangle bulletRect = { (float)bullet.x -  20, (float)bullet.y - 20, 50, 50 };
        Rectangle clickRect = { (float)click_x -  20, (float)click_y - 20, 50, 50 };
        if (shot_fired == false)
        {
            bullet.x = player.x;
            bullet.y = player.y;
        }
        // Convert player.x and player.y to string to show on screen
        sprintf(playerXStr, "%d", player.x);
        sprintf(playerYStr, "%d", player.y);
        //draw the window
        BeginDrawing();
        ClearBackground(GREEN);
        target.x = GetMouseX();
        target.y = GetMouseY();
        sprintf(targetXStr, "%d", target.x);
        sprintf(targetYStr, "%d", target.y);
        //hide mouse and change it to target
        HideCursor();
        DrawTexture(player.sprite, player.x, player.y, WHITE);
        DrawTexture(target.sprite, target.x, target.y, WHITE);
        DrawText("playerX:", 10, 10, 80, WHITE);;
        DrawText(playerXStr, 400, 10, 80, WHITE);
        DrawText("playerY:", 10, 90, 80, WHITE);;
        DrawText(playerYStr, 400, 90, 80, WHITE);
        DrawText("TargetX:", 0, 160, 80, WHITE);
        DrawText(targetXStr, 400, 160, 80, WHITE);
        DrawText("TargetY:", 0, 240 , 80, WHITE);
        DrawText(targetYStr, 400, 240 , 80, WHITE);
        DrawCircle(bullet.x,bullet.y,20,YELLOW);
        DrawLine(bullet.x,bullet.y,target.x,target.y,WHITE);
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) DrawText("MOUSE CLICK DETECTED",10,400,80,RED);
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (shot_fired == false)
            {
                shot_fired = true;
                click_x = GetMouseX();
                click_y = GetMouseY();
            }
        }
        if (CheckCollisionRecs(bulletRect,clickRect)) shot_fired = false;
        if (shot_fired) DrawCircle(bullet.x,bullet.y,20,YELLOW);
        if (bullet.x < click_x) bullet.x += 10;
        if (bullet.x > click_x) bullet.x -= 10;
        if (bullet.y > click_y) bullet.y -= 10;
        if (bullet.x == click_x)
        {
            if (bullet.y == click_y)
            {
                shot_fired = false;
            }
        }
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}