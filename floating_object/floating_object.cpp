#include "floating_object.h"
void Floating_Object::Update_Rect(int width, int height) 
{
    Rect = {pos.x - 20, pos.y - 20, width, height};
}