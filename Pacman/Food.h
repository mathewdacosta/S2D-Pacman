#pragma once

#include "S2D/S2D.h"
using namespace S2D;

struct Food
{
    Rect* sourceRect;
    Rect* destRect;
    Texture2D* texture;
    int animCurrentTime; // current time since last update
    int animFrame; // current frame in animation
    int animFrameTime; // interval between animation frames
};
