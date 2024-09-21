#include "player.h"
void Player::Update_Rect()
{
    Rect = {pos.x, pos.y, 80, 80};
}
void Player::Show()
{
    if (show)
    {
        DrawTextureV(sprite, pos, WHITE);
    }
}
void Player::Left()
{
    left = true;
    right = false;
    sprite = sprite_left;
}
void Player::Right()
{
    right = true;
    left = false;
    sprite = sprite_right;
}
void Player::Stop()
{
    right = false;
    left = false;
    sprite = default_sprite;
}
void Player::Update_Position(int width, Rectangle ground)
{
    if (right) pos.x += speed;
    if (left) pos.x -= speed;   
    if (pos.x >= width - 100) pos.x = width - 100;
    if(CheckCollisionRecs(ground,Rect)) is_on_floor = true;
    else
    {
    is_on_floor = false;
    }
    if (!is_on_floor && !is_jumping) fall = true;
    if (is_jumping) pos.y -= 30;
    if (y_before_jump - pos.y >= max_jump_height) is_jumping = false;
    if (is_on_floor) 
    {
        fall = false;
        block_jump = false;
     } 
     else 
     {
        fall = true;
        block_jump = true;
     }
     if (fall) pos.y += 10;
}
void Player::Jump()
{
    if (!block_jump) 
    {
        if (is_on_floor) is_jumping = true;
        block_jump = true;
        is_on_floor = false;
        y_before_jump = pos.y;
    }
}
