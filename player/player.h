#pragma once
#include "raylib.h"
class Player {
public:
    Vector2 pos;
    int speed;
    bool show = true;
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
    void Update_Rect();
    void Respawn(Vector2 position);
    void Show();
};