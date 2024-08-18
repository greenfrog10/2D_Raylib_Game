// level1.cpp
#include "raylib.h"
#include "level1.h"
// Include any necessary class definitions
class Window {
public:
    int width;
    int height;
};

class Player {
public:
    Vector2 pos;
    int speed;
    bool jumping;
    bool is_on_floor = false;
    bool fall = false;
    bool is_jumping = false;
    bool block_jump = false;
    bool left = false;
    bool right = false;
    int y_before_jump = 0;
    int max_jump_height = 270;
    Rectangle Rect;
    Texture2D sprite;

    void Update_Rect() {
        Rect = {pos.x, pos.y, 80, 80};
    }

    void Respawn(Vector2 position) {
        pos = position;
    }

    void Show() {
        DrawTextureV(sprite, pos, WHITE);
    }
};

class Floating_Object {
public:
    Vector2 pos;
    Texture2D sprite;
    Rectangle Rect;

    void Update_Rect(int width, int height) {
        Rect = {pos.x - 20, pos.y - 20, width, height};
    }
};

class Object {
public:
    Vector2 pos;
    Texture2D sprite;
    bool is_on_floor = false;
    bool fall = true;
    bool show = true;
    int rect_width;
    int rect_height;
    Rectangle Rect = {0, 0, rect_width, rect_height};   

    void Fall(Rectangle groundRect) {
        if (CheckCollisionRecs(Rect, groundRect)) fall = false;
        else {
            fall = true;
        }
        if (fall) pos.y += 9;
    }

    void CheckBulletCollision(Rectangle bulletRect, bool &shot_fired, bool &gun_follow_player) {
        if (show && CheckCollisionRecs(bulletRect, Rect)) {
            shot_fired = false;
            show = false;
            gun_follow_player = true;
        }
    }

    void Show() {
        if (show) DrawTextureV(sprite, pos, WHITE);
    }

    void Update_Rect() {
        Rect = {pos.x, pos.y, rect_width, rect_height};
    }

    bool Check_collision_with_player(Rectangle player_rect) {
        return CheckCollisionRecs(Rect, player_rect);
    }
};

void Change_mouse_to_Target(Texture2D sprite, Vector2 &pos) {
    pos = GetMousePosition();
    DrawTextureV(sprite, pos, WHITE);
}
void run_level1() {
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
