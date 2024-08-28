#include "raylib.h"
#include "level1.h"
#include "../window/window.h"
#include "../player/player.h"
#include "../floating_object/floating_object.h"
#include "../object/object.h"
#include "../change_mouse_to_target/change_mouse_to_target.h"
bool run_level1() 
{
    bool run = true;
    Window window;
    window.width = 1600;
    window.height = 900;
    InitWindow(window.width, window.height, "level 1");
    Vector2 center{window.width / 2, window.height / 2};

    Player player;
    player.speed = 15;
    SetTargetFPS(60);
    player.pos.x = 0;
    player.pos.y = 500;
    player.sprite = LoadTexture("sprites/player.png");

    Floating_Object target;
    target.pos = player.pos;
    target.sprite = LoadTexture("sprites/target.png");

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
    bool gun_disabled = true;
    Rectangle ground = {0, 800, 1600, 100};

    // Main game loop
    while (run) {
        HideCursor();

        // Check Key inputs and move player
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) 
        {
            player.Right();
        } 
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) 
        {
            player.Left();
        } 
        else 
        {
            player.Stop();
        }

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            player.Jump();
        }

        if (WindowShouldClose()) return false;


        // Update Rectangles
        player.Update_Position(window.width,ground);
        player.Update_Rect();
        bullet.Update_Rect(50, 50);
        Rectangle clickRect = {(float)click_x - 20, (float)click_y - 20, 20, 20};

        if (player.pos.x >= window.width - 100) run = false;      
        if (gun_follow_player) {
            bullet.pos = gun.pos;
            if (gun_facing_right) {
                bullet.pos.x += 100;
            }
            if (gun_facing_left) {
                bullet.pos.x -= 20;
            }
        }

        // Draw the window
        BeginDrawing();
        ClearBackground(PURPLE);

        if (target.pos.x < center.x) {
            gun_facing_right = false;
            gun_facing_left = true;
        }
        if (target.pos.x > center.x) {
            gun_facing_right = true;
            gun_facing_left = false;
        }

        // Draw The Objects on the screen
        DrawRectangleRec(ground, PINK);
        player.Show();
        DrawText("Use Arrow Keys or ZQSD to move", 0, 0, 80, WHITE);

        if (!gun_disabled) {
            Change_mouse_to_Target(target.sprite, target.pos);

            if (gun_facing_right) {
                gun.pos.x = player.pos.x + 80;
                gun.pos.y = player.pos.y;
                gun.sprite = gun_right;
            }
            if (gun_facing_left) {
                gun.pos.x = player.pos.x - 80;
                gun.pos.y = player.pos.y;
                gun.sprite = gun_left;
            }

            DrawTextureV(gun.sprite, gun.pos, WHITE);

            // Check if left mouse button is released, shoot the bullet
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
                bullet_x_destination = (click_x - bullet.pos.x) / 15;
                bullet_y_destination = (click_y - bullet.pos.y) / 15;
                if (CheckCollisionRecs(bullet.Rect, clickRect)) {
                    shot_fired = false;
                    gun_follow_player = true;
                } else {
                    bullet.pos.x += bullet_x_destination;
                    bullet.pos.y += bullet_y_destination;
                }
            }
        }

        EndDrawing();
    }
    UnloadTexture(player.sprite);
    UnloadTexture(target.sprite);
    UnloadTexture(gun_left);
    UnloadTexture(gun_right);

    CloseWindow();
    return true;
}
