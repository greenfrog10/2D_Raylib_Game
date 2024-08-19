#include "object.h"
void Object::Fall(Rectangle groundRect)
{
    if (CheckCollisionRecs(Rect, groundRect)) fall = false;
    else 
    {
    fall = true;
    }
    if (fall) pos.y += 9;
}    
void Object::CheckBulletCollision(Rectangle bulletRect, bool &shot_fired, bool &gun_follow_player) 
{
    if (show && CheckCollisionRecs(bulletRect, Rect)) 
    {
        shot_fired = false;
        show = false;
        gun_follow_player = true;
    }
}
void Object::Show() 
{
    if (show) DrawTextureV(sprite, pos, WHITE);
}

void Object::Update_Rect() 
{
    Rect = {pos.x, pos.y, rect_width, rect_height};
}
bool Object::Check_collision_with_player(Rectangle player_rect) 
{
    return CheckCollisionRecs(Rect, player_rect);
}