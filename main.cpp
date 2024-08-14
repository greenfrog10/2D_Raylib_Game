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
    Rectangle Rect;
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
    void Fall(Rectangle groundRect,bool &is_on_floor, bool &fall, Vector2 &pos) 
    {
        if (is_on_floor == false) fall = true;
        if (is_on_floor == true) fall = false;
        if (fall) pos.y += 9;
        if (CheckCollisionRecs(Rect,groundRect)) is_on_floor = true;
        else 
        {
            fall = true;
        }
    }
    void CheckBulletCollision(Rectangle bulletRect, bool &shot_fired, bool &gun_follow_player) 
    {
        if (show && CheckCollisionRecs(bulletRect, Rect)) 
        {
            shot_fired = false;
            show = false;
            gun_follow_player = true;
        }
    }
};

int main() {
    
    Window window;
    window.width = 1480;
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
    
    Object box;  
    box.pos.x = 1200;
    box.pos.y = 0;
    
    Object box2; 
    box2.pos.x = 1300;
    box2.pos.y = 0;
    box.sprite = LoadTexture("sprites/box.png");
    box2.sprite = box.sprite;
    
    Object gun;
    gun.pos = player.pos;
    Texture2D gun_left = LoadTexture("sprites/gun_left.png");
    Texture2D gun_right = LoadTexture("sprites/gun_right.png");
    gun.sprite = gun_right;
    bool gun_facing_left = false;
    bool gun_facing_right = true;
    
    Floating_Object bullet;
    int click_x = GetMouseX();
    int click_y = GetMouseY();
    int bullet_x_destination = 0;
    int bullet_y_destination = 0;
    bool gun_follow_player = true;
    Rectangle bulletRect = {bullet.pos.x - 20, bullet.pos.y - 20, 60, 60};
    bool shot_fired = false;
    
    Object ground;
    ground.pos.x = 0;
    ground.pos.y = 780;
    // Main game loop
    while (!WindowShouldClose()) {
        // Check Key inputs and move player
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
        
        // create rectangles 
        player.Rect = {player.pos.x,player.pos.y,60,60};
        Rectangle bulletRect = {bullet.pos.x - 20, bullet.pos.y - 20, 50,50};
        Rectangle clickRect = {(float)click_x - 20, (float)click_y - 20, 20, 20};
        box.Rect = {box.pos.x, box.pos.y, 60, 60};   
        box2.Rect = {box2.pos.x, box2.pos.y, 60, 60};
        ground.Rect = {ground.pos.x,ground.pos.y,1450,100};
        // Check Borders for player
        if (CheckCollisionRecs(player.Rect, ground.Rect)) player.is_on_floor = true;
        if (player.pos.x <= 10) player.pos.x = 10;
        if (player.pos.x >= window.width - 100) player.pos.x = window.width - 100;
        if (!player.is_on_floor && !player.is_jumping) player.fall = true;
        if (player.is_jumping) player.pos.y -= 20;
        if (player.y_before_jump - player.pos.y >= player.max_jump_height) player.is_jumping = false;
        if (player.is_on_floor) {
            player.fall = false;
            player.block_jump = false;
        }
        else 
        {
            player.fall = true;
            player.block_jump = true;
        }
        if (player.fall) player.pos.y += 10;
        box.Fall(ground.Rect,box.is_on_floor, box.fall, box.pos);
        box2.Fall(ground.Rect,box2.is_on_floor, box2.fall, box2.pos);
        
        if (gun_follow_player) 
        {
            bullet.pos = gun.pos;
            if(gun_facing_right)
            {
                bullet.pos.x += 100;
            }
            if(gun_facing_left)
            {
                bullet.pos.x -= 20;
            }
        }
        // Draw the window
        BeginDrawing();
        ClearBackground(PURPLE);
        target.pos = GetMousePosition();
        if (target.pos.x < center.x)
        {
            gun_facing_right = false;
            gun_facing_left = true;
        }
        if (target.pos.x > center.x)
        {
            gun_facing_right = true;
            gun_facing_left = false;    
        }
        // Hide mouse and change it to target
        HideCursor();
        
        // Draw The Objects on the screeen
        DrawRectangleV(ground.pos,Vector2{1500,500},WHITE);
        DrawTextureV(player.sprite, player.pos, WHITE);
        DrawTextureV(target.sprite, target.pos, WHITE);
        if (box.show) DrawTextureV(box.sprite, box.pos, WHITE);
        if (box2.show) DrawTextureV(box2.sprite, box2.pos, WHITE);
        if (gun_facing_right)
        {
            gun.pos.x = player.pos.x + 80;
            gun.pos.y = player.pos.y;
            gun.sprite = gun_right;
        }
        if (gun_facing_left)
        {
            gun.pos.x = player.pos.x - 80;
            gun.pos.y = player.pos.y;
            gun.sprite = gun_left;
        }
        DrawTextureV(gun.sprite, gun.pos,WHITE);
        
        // check if click left is release,if click left is released shoot the bullet
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (!shot_fired) {
                click_x = GetMouseX();
                click_y = GetMouseY();
                shot_fired = true;
            }
        }
        
        if (shot_fired) {
            gun_follow_player = false;
            DrawCircle(bullet.pos.x, bullet.pos.y, 20, YELLOW);
            bullet_x_destination = (click_x - bullet.pos.x) / 20;
            bullet_y_destination = (click_y - bullet.pos.y) / 20;
            if (CheckCollisionRecs(bulletRect, clickRect)) {
                shot_fired = false;
                gun_follow_player = true;
            } else {
                bullet.pos.x += bullet_x_destination;
                bullet.pos.y += bullet_y_destination;
            }
            // Use the new method for handling collisions
            box.CheckBulletCollision(bulletRect, shot_fired, gun_follow_player);
            box2.CheckBulletCollision(bulletRect, shot_fired, gun_follow_player);
        }
        
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
