#include "raylib.h"
#include <stdio.h> 
#include <math.h>
class Window{
    public:
        int width;
        int height;
};

class Player
{
    public:
        Vector2 pos;
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
        Vector2 pos;
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
    player.pos = center;
    player.sprite = LoadTexture("sprites/player.png");
    char playerXStr[10];
    char playerYStr[10];
    char targetXStr[10];
    char targetYStr[10];
    Object target;
    target.pos = player.pos;
    target.sprite = LoadTexture("sprites/target.png");
    Object bullet;
    bool shot_fired = false;
    int click_x = GetMouseX();
    int click_y = GetMouseY();
    // Main game loop
    while (!WindowShouldClose())
    {
        // check Key inputs
        if (IsKeyDown(KEY_RIGHT) || (IsKeyDown(KEY_D))) player.pos.x += player.speed;
        if (IsKeyDown(KEY_LEFT) || (IsKeyDown(KEY_A))) player.pos.x -= player.speed;
        if (IsKeyDown(KEY_UP) || (IsKeyDown(KEY_W)))
        {
            if (player.block_jump != true)
            {
                if (player.is_on_floor) player.is_jumping = true;
                player.block_jump = true;
                player.is_on_floor = false;
                player.y_before_jump = player.pos.y;
            }
        }
        //Moving The player
        if(player.pos.x >= 1100) player.pos.x = 1100;
        if(player.pos.x <= 10) player.pos.x = 10;
        if(player.fall) player.pos.y += 10;
        if (player.is_on_floor == false && player.is_jumping == false) player.fall = true;
        if (player.is_jumping) player.pos.y -= 15;
        if (player.y_before_jump - player.pos.y >= player.max_jump_height) player.is_jumping = false;
        if (player.is_on_floor == true)
        {
            player.fall = false;
            player.block_jump = false;
        }
        if (player.pos.y >= 820)
        {            
            player.is_on_floor = true;
            player.pos.y = 820;
        }
        else
        {
            player.is_on_floor = false;
        }
        Rectangle bulletRect = {bullet.pos.x - 20 , bullet.pos.y, 50, 50 };
        Rectangle clickRect = { (float)click_x -  20, (float)click_y - 20, 50, 50 };
        if (shot_fired == false)
        {
            bullet.pos.x = player.pos.x;
            bullet.pos.y = player.pos.y;
        }
        // Convert player.pos.x and player.pos.y to string to show on screen
        int round_player_x = int(roundf(player.pos.x));
        int round_player_y = int(roundf(player.pos.y));
        sprintf(playerXStr, "%d", round_player_x);
        sprintf(playerYStr, "%d", round_player_y);
        //draw the window
        BeginDrawing();
        ClearBackground(GREEN);
        target.pos = GetMousePosition();
        int round_target_x = int(roundf(target.pos.x));
        int round_target_y = int(roundf(target.pos.y));
        sprintf(targetXStr, "%d", round_target_x);
        sprintf(targetYStr, "%d", round_target_y);
        //hide mouse and change it to target
        HideCursor();
        DrawTexture(player.sprite, player.pos.x, player.pos.y, WHITE);
        DrawTexture(target.sprite, target.pos.x, target.pos.y, WHITE);
        DrawText("playerX:", 10, 10, 80, WHITE);;
        DrawText(playerXStr, 400, 10, 80, WHITE);
        DrawText("playerY:", 10, 90, 80, WHITE);;
        DrawText(playerYStr, 400, 90, 80, WHITE);
        DrawText("TargetX:", 0, 160, 80, WHITE);
        DrawText(targetXStr, 400, 160, 80, WHITE);
        DrawText("TargetY:", 0, 240 , 80, WHITE);
        DrawText(targetYStr, 400, 240 , 80, WHITE);
        DrawCircle(bullet.pos.x,bullet.pos.y,20,YELLOW);
        DrawLine(bullet.pos.x,bullet.pos.y,target.pos.x,target.pos.y,WHITE);
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
        if (shot_fired) DrawCircle(bullet.pos.x,bullet.pos.y,20,YELLOW);
        if (bullet.pos.x < click_x) bullet.pos.x += 10;
        if (bullet.pos.x > click_x) bullet.pos.x -= 10;
        if (bullet.pos.y > click_y) bullet.pos.y -= 10;
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}