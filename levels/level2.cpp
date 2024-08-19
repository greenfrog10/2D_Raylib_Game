#include "raylib.h"
#include "level2.h"
#include "../window/window.h"
#include "../player/player.h"
#include "../floating_object/floating_object.h"
#include "../object/object.h"
#include "../change_mouse_to_target/change_mouse_to_target.h"
void run_level2() {
    bool run = true;
    Window window;
    window.width = 1600;
    window.height = 900;
    InitWindow(window.width, window.height, "level 2");
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
    
    Object pickable_gun;
    pickable_gun.pos.x = 0;
    pickable_gun.pos.y = 0;
    pickable_gun.rect_width = 80;
    pickable_gun.rect_height = 80;
    pickable_gun.sprite = LoadTexture("sprites/gun_right.png");
    
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
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            player.right = true;
            player.left = false;
        } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            player.left = true;
            player.right = false;
        } else {
            player.left = false;
            player.right = false;
        }

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            if (!player.block_jump) {
                if (player.is_on_floor) player.is_jumping = true;
                player.block_jump = true;
                player.is_on_floor = false;
                player.y_before_jump = player.pos.y;
            }
        }

        if (WindowShouldClose()) run = false;

        // Move player 
        if (player.right) player.pos.x += player.speed;
        if (player.left) player.pos.x -= player.speed;

        // Update Rectangles
        player.Update_Rect();
        bullet.Update_Rect(50, 50);
        Rectangle clickRect = {(float)click_x - 20, (float)click_y - 20, 20, 20};

        // Check Borders for player
        if (CheckCollisionRecs(player.Rect, ground)) player.is_on_floor = true;
        else player.is_on_floor = false;

        if (player.pos.x <= 10) player.pos.x = 10;
        if (player.pos.x >= window.width - 100) run = false;

        if (!player.is_on_floor && !player.is_jumping) player.fall = true;
        if (player.is_jumping) player.pos.y -= 25;
        if (player.y_before_jump - player.pos.y >= player.max_jump_height) player.is_jumping = false;

        if (player.is_on_floor) {
            player.fall = false;
            player.block_jump = false;
        } else {
            player.fall = true;
            player.block_jump = true;
        }

        if (player.fall) player.pos.y += 10;

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
        DrawText("Pickup the gun", 0, 0, 80, WHITE);
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
                bullet_x_destination = (click_x - bullet.pos.x) / 20;
                bullet_y_destination = (click_y - bullet.pos.y) / 20;
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

    CloseWindow();
}
