#include "raylib.h"
#include "level2.h"
#include "../window/window.h"
#include "../player/player.h"
#include "../floating_object/floating_object.h"
#include "../object/object.h"
#include "../change_mouse_to_target/change_mouse_to_target.h"

bool run_level2() 
{
    bool run = true;
    while (run)
    {
        bool reload_level = false;
        bool autorise_exit = true;
        char* text = "Quick jump these boxes can kill you !";
        reload_level = false;
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

        Object box;
        box.speed = 10;
        box.pos.x = center.x;
        box.pos.y = 720;
        box.sprite = LoadTexture("sprites/dmg_box.png");
        box.rect_width = 121;
        box.rect_height = 115;
        box.Move_left();

        Object box2;
        box2.pos.x = box.pos.x + 500;
        box2.pos.y = 720;
        box2.sprite = box.sprite;
        box2.rect_width = box.rect_width;
        box2.rect_height = box.rect_height;
        box2.Move_left();
        
        Object box3;
        box3.pos.x = box2.pos.x + 500;
        box3.pos.y = 720;
        box3.sprite = box.sprite;
        box3.rect_width = box.rect_width;
        box3.rect_height = box.rect_height;
        box3.Move_left();
        
        Object box4;
        box4.pos.x = box3.pos.x + 500;
        box4.pos.y = 720;
        box4.sprite = box.sprite;
        box4.rect_width = box.rect_width;
        box4.rect_height = box.rect_height;
        box4.Move_left();
  
        Rectangle ground = {0, 800, 3000, 100};
        // Main game loop
        while (!reload_level)
        {
            HideCursor();
            if (player.alive)
            {
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

                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) 
                {
                    player.Jump();
                }
            }
            if (IsKeyDown(KEY_R))
            {
                reload_level = true;
            }

            if (WindowShouldClose())
            {
                CloseWindow();
                break;
                return false;
            }

            player.Update_Position(window.width,ground);
            // Update Rectangles
            box2.speed = box.speed;
            box3.speed = box.speed;
            box4.speed = box.speed;
            box.Fall(ground);
            box.Update_Position();
            box.Update_Rect();
            
            box2.Fall(ground);
            box2.Update_Position();
            box2.Update_Rect();
            
            box3.Fall(ground);
            box3.Update_Position();
            box3.Update_Rect();
            
            box4.Fall(ground);
            box4.Update_Position();
            box4.Update_Rect();
            
            player.Update_Rect();
            
            
            // check if box touched player 
            if (box.Check_collision_with_rect(player.Rect) || box2.Check_collision_with_rect(player.Rect)) player.alive = false;
            if (box3.Check_collision_with_rect(player.Rect)) player.alive = false;

            // Check Borders and Ground for player
            if (CheckCollisionRecs(player.Rect, ground)) player.is_on_floor = true;
            else player.is_on_floor = false;

            if (player.pos.x >= window.width - 100)
            {
                if (autorise_exit)
                {
                    run = false;
                    break;
                }              
            }
            // Draw the window
            BeginDrawing();
            ClearBackground(PURPLE);

            // Draw The Objects on the screen
            DrawRectangleRec(ground, PINK);
            player.Show();
            box.Show();
            box2.Show();
            box3.Show();
            box4.Show();
            DrawText(text,0,0,80,WHITE);
            if (!player.alive)
            {
                player.show = false;
                box.Stop(box.move_left);
                box2.Stop(box2.move_left);
                box3.Stop(box3.move_left);
                box4.Stop(box4.move_left);
                text = "Press R to retry";
            }


            EndDrawing();
        }

        UnloadTexture(player.sprite);
        UnloadTexture(box.sprite);

        CloseWindow();
    }

    return true;
}
