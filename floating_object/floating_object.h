#pragma once
#include "raylib.h"
class Floating_Object {
public:
    Vector2 pos;
    Texture2D sprite;
    Rectangle Rect;
    void Update_Rect(int width, int height);
};
