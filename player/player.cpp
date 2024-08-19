#include "player.h"
void Player::Update_Rect()
{
    Rect = {pos.x, pos.y, 80, 80};
}
void Player::Respawn(Vector2 position) 
{
    pos = position;
}
void Player::Show()
{
    DrawTextureV(sprite, pos, WHITE);
}