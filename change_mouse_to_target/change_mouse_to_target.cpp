#include "change_mouse_to_target.h"

void Change_mouse_to_Target(Texture2D sprite, Vector2 &pos) {
    pos = GetMousePosition();
    DrawTextureV(sprite, pos, WHITE);
}
