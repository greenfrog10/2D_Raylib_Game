#ifndef OBJECT_H
#define OBJECT_H
#include "raylib.h"
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
    void Fall(Rectangle groundRect);
    void CheckBulletCollision(Rectangle bulletRect, bool &shot_fired, bool &gun_follow_player);
    void Show();
    void Update_Rect();
    bool Check_collision_with_player(Rectangle player_rect);
};
#endif
