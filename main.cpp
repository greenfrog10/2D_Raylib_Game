#include "raylib.h"
#include "levels/level1.h" 
#include "levels/level2.h"
#include "levels/level3.h"
#include <iostream>
int main() 
{
    if(run_level1())
    {
        if(run_level2())
        {
            run_level3();
        }
    }
}