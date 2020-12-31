#pragma once
#include "SimpleAnimatedEntity.h"

class Food : public SimpleAnimatedEntity
{
protected:
    Food(int width, int height, int frameCount, int frameSpeed);
    ~Food();
};
