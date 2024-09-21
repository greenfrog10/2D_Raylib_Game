#include "raylib.h"
#include "level3.h"
#include "../window/window.h"
#include "../player/player.h"
#include "../object/object.h"

bool run_level3() 
{
    bool run = true;
    while (run)
    {
        bool make_objects_fall = true;
        bool attack_1 = true;
        bool attack_2 = false;
        bool reload_level = false;
        bool autorise_exit = false;
        int times_box_bounced = 0;
        
        reload_level = false;
        Window window;
        window.width = 1600;
        window.height = 900;
        InitWindow(window.width, window.height, "level 3");
        Vector2 center{window.width / 2, window.height / 2};

        Player player;
        player.speed = 15;
        SetTargetFPS(60);
        player.pos.x = center.x;
        player.pos.y = 500;
        player.default_sprite = LoadTexture("sprites/player.png");
        player.sprite_left = LoadTexture("sprites/player_left.png");
        player.sprite_right = LoadTexture("sprites/player_right.png");

        Object box;
        box.speed = 10;
        box.pos.x = 0;
        box.pos.y = 720;
        box.sprite = LoadTexture("sprites/dmg_box.png");
        box.rect_width = 121;
        box.rect_height = 115;
        box.Move_right();

        Object box2;
        box2.speed = box.speed;
        box2.pos.x = 1479;
        box2.pos.y = 720;
        box2.sprite = box.sprite;
        box2.rect_width = box.rect_width;
        box2.rect_height = box.rect_height;
        box2.Move_left();
        

        Rectangle ground = {0, 800, 3000, 100};
        // Main game loop
        while (!reload_level)
        {
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

            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) 
            {
                player.Jump();
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
            if(box.pos.x > 1479 && attack_1)
            {
                box.Stop(box.move_right);
                times_box_bounced += 1;
            }
            if(box2.pos.x < 0  && attack_1)
            {
                box2.Stop(box2.move_left); 
                times_box_bounced += 1;
            }
            if (times_box_bounced == 2)
            {
                attack_1 = false;
                attack_2 = true;
                times_box_bounced = 0;
            }
            if (attack_2)
            {
                make_objects_fall = false;
                box2.pos.y -= 10;
                box.pos.y -= 10;
                if(box.pos.y < -box.rect_height)
                {
                    box.pos.y = -box.rect_height;
                }
                if (box2.pos.y < -box2.rect_height)
                {
                    box2.pos.y = -box2.rect_height;
                }
            }
            if (make_objects_fall)
            {
                box.Fall(ground);
                box2.Fall(ground);
            }
            box.Update_Position();
            box.Update_Rect();
            
            box2.Update_Position();
            box2.Update_Rect();
            
            player.Update_Rect();
            
            DrawRectangleRec(ground,PINK);
            box.Show();
            box2.Show();
            player.Show();
            // check if box touched player 
            if (box.Check_collision_with_rect(player.Rect) || box2.Check_collision_with_rect(player.Rect)) player.alive = false;

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
            
            if (!player.alive)
            {
                player.show = false;
                DrawText("Press R to retry",0,0,80,WHITE);
                box.Stop(box.move_right);
                box2.Stop(box2.move_left);
            }


            EndDrawing();
        }

        UnloadTexture(player.default_sprite);
        UnloadTexture(player.sprite_left);
        UnloadTexture(player.sprite_right);
        UnloadTexture(box.sprite);

        CloseWindow();
    }

    return true;
}
