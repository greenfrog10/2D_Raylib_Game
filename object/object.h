#pragma once
#include "raylib.h"
class Object {
public:
    Vector2 pos;
    Texture2D sprite;
    double speed = 0;
    double max_speed = 0;
    bool fall = true;
    bool show = true;
    bool move_right = false;
    bool move_left = false;
    int rect_width;
    int rect_height;
    Rectangle Rect = {0, 0, rect_width, rect_height};   
    void Fall(Rectangle groundRect);
    void CheckBulletCollision(Rectangle bulletRect, bool &shot_fired, bool &gun_follow_player);
    void Show();
    void Update_Rect();
    bool Check_collision_with_rect(Rectangle rect);
    void Move_right();
    void Move_left();
    void Update_Position();
};
