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
bool Object::Check_collision_with_rect(Rectangle rect) 
{
    return CheckCollisionRecs(Rect,rect);
}
void Object::Move_right()
{
    move_left = false;
    move_right = true;
}
void Object::Move_left()
{
    move_right = false;
    move_left = true;
}
void Object::Move_Up()
{
    move_up = true;
}
void Object::Stop(bool &move)
{
    move = false;
}
void Object::Update_Position()
{
    if (move_right)
    {
        pos.x += speed;
    }
    if(move_left)
    {
        pos.x -= speed;
    }
}