#include "raylib.h"
#include "level1.h"
#include "../window/window.h"
#include "../player/player.h"
#include "../object/object.h"
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

        if (player.pos.x >= window.width - 100) run = false;      

        // Draw the window
        BeginDrawing();
        ClearBackground(PURPLE);


        // Draw The Objects on the screen
        DrawRectangleRec(ground, PINK);
        player.Show();
        DrawText("Use Arrow Keys or ZQSD to move", 0, 0, 80, WHITE);


        EndDrawing();
    }
    UnloadTexture(player.sprite);

    CloseWindow();
    return true;
}
