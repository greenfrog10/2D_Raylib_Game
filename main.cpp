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
    bool shot_fired = false;
    Window window;
    window.width = 1400;
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
    int click_x = GetMouseX();
    int click_y = GetMouseY();
    int x_destination = 0;
    int y_destination = 0;
    int pos_division = 100;
    bool bullet_follow_player = true;
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
        if(player.pos.x >= 1300) player.pos.x = 1300;
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
        if (bullet_follow_player) bullet.pos = player.pos;
        Rectangle bulletRect = {bullet.pos.x - 20 , bullet.pos.y - 20, 60, 60 };
        Rectangle clickRect = { (float)click_x -  20, (float)click_y - 20, 60, 60 };
        //draw the window
        BeginDrawing();
        ClearBackground(GREEN);
        target.pos = GetMousePosition();
        //hide mouse and change it to target
        HideCursor();
        DrawTextureV(player.sprite, player.pos, WHITE);
        DrawTextureV(target.sprite, target.pos, WHITE);
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
                if (shot_fired != true)
                {
                    click_x = GetMouseX();
                    click_y = GetMouseY();
                    shot_fired = true;
                }
        }
        if (shot_fired)
        {
            bullet_follow_player = false;
            DrawCircle(bullet.pos.x,bullet.pos.y,20,PURPLE);
            x_destination = (click_x - bullet.pos.x) / 20;
            y_destination = (click_y - bullet.pos.y) / 20;
        }
        if (CheckCollisionRecs(bulletRect,clickRect) == false)
        {
        bullet.pos.x += x_destination;
        bullet.pos.y += y_destination;
        }
        else
        {
                shot_fired = false;
                bullet_follow_player = true;
        }
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}