#include "raylib.h"
#include <stdio.h> 
#include <math.h>

class Window {
public:
    int width;
    int height;
};

class Player {
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

class Floating_Object {
public:
    Vector2 pos;
    Texture2D sprite;
};

class Object {
public:
    Vector2 pos;
    Texture2D sprite;
    bool is_on_floor = false;
    bool fall = true;
    bool show = true;
    Rectangle Rect = {0, 0, 60, 60};   
    void Fall(bool &is_on_floor, bool &fall, Vector2 &pos) 
    {
        if (is_on_floor == false) fall = true;
        if (fall) pos.y += 8;
        if (pos.y >= 820) 
        {
            is_on_floor = true;
            pos.y = 820;
        } 
        else 
        {
            fall = true;
        }
    }
    void CheckBulletCollision(Rectangle bulletRect, bool &shot_fired, bool &bullet_follow_player) 
    {
        if (show && CheckCollisionRecs(bulletRect, Rect)) 
        {
            shot_fired = false;
            show = false;
            bullet_follow_player = true;
        }
    }
};

int main() {
    bool shot_fired = false;
    Window window;
    window.width = 1500;
    window.height = 900;
    InitWindow(window.width, window.height, "My game");
    Vector2 center{window.width / 2, window.height / 2};
    Player player;
    player.size = 60;
    player.speed = 10;
    SetTargetFPS(60);
    player.pos = center;
    player.sprite = LoadTexture("sprites/player.png");
    Floating_Object target;
    target.pos = player.pos;
    target.sprite = LoadTexture("sprites/target.png");
    Floating_Object bullet;
    Object box;
    Object box2;
    box.pos.x = 1200;
    box.pos.y = 0;
    box2.pos.x = 1300;
    box2.pos.y = 0;
    box.sprite = LoadTexture("sprites/box.png");
    box2.sprite = box.sprite;
    int click_x = GetMouseX();
    int click_y = GetMouseY();
    int bullet_x_destination = 0;
    int bullet_y_destination = 0;
    bool bullet_follow_player = true;
    Rectangle bulletRect = {bullet.pos.x - 20, bullet.pos.y - 20, 60, 60};
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Check Key inputs
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.pos.x += player.speed;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.pos.x -= player.speed;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            if (!player.block_jump) {
                if (player.is_on_floor) player.is_jumping = true;
                player.block_jump = true;
                player.is_on_floor = false;
                player.y_before_jump = player.pos.y;
            }
        }
        
        // Check Borders for player
        if (player.pos.x >= window.width - 100) player.pos.x = window.width - 100;
        if (player.pos.x <= 10) player.pos.x = 10;
        if (player.fall) player.pos.y += 10;
        if (!player.is_on_floor && !player.is_jumping) player.fall = true;
        if (player.is_jumping) player.pos.y -= 15;
        if (player.y_before_jump - player.pos.y >= player.max_jump_height) player.is_jumping = false;
        if (player.is_on_floor) {
            player.fall = false;
            player.block_jump = false;
        }
        if (player.pos.y >= window.height - 80) {
            player.is_on_floor = true;
            player.pos.y = window.height - 80;
        } else {
            player.is_on_floor = false;
        }
        
        box.Fall(box.is_on_floor, box.fall, box.pos);
        box2.Fall(box2.is_on_floor, box2.fall, box2.pos);
        
        if (bullet_follow_player) bullet.pos = player.pos;
        Rectangle bulletRect = {bullet.pos.x - 20, bullet.pos.y - 20, 50,50};
        Rectangle clickRect = {(float)click_x - 20, (float)click_y - 20, 20, 20};
        box.Rect = {box.pos.x, box.pos.y, 60, 60};   
        box2.Rect = {box2.pos.x, box2.pos.y, 60, 60};
        
        // Draw the window
        BeginDrawing();
        ClearBackground(GREEN);
        target.pos = GetMousePosition();
        // Hide mouse and change it to target
        HideCursor();
        DrawTextureV(player.sprite, player.pos, WHITE);
        DrawTextureV(target.sprite, target.pos, WHITE);
        if (box.show) DrawTextureV(box.sprite, box.pos, WHITE);
        if (box2.show) DrawTextureV(box2.sprite, box2.pos, WHITE);
        
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (!shot_fired) {
                click_x = GetMouseX();
                click_y = GetMouseY();
                shot_fired = true;
            }
        }
        
        if (shot_fired) {
            bullet_follow_player = false;
            DrawCircle(bullet.pos.x, bullet.pos.y, 20, YELLOW);
            bullet_x_destination = (click_x - bullet.pos.x) / 20;
            bullet_y_destination = (click_y - bullet.pos.y) / 20;
            if (CheckCollisionRecs(bulletRect, clickRect)) {
                shot_fired = false;
                bullet_follow_player = true;
            } else {
                bullet.pos.x += bullet_x_destination;
                bullet.pos.y += bullet_y_destination;
            }
            // Use the new method for handling collisions
            box.CheckBulletCollision(bulletRect, shot_fired, bullet_follow_player);
            box2.CheckBulletCollision(bulletRect, shot_fired, bullet_follow_player);
        }
        
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
