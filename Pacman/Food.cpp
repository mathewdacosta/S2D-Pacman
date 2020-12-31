#include "Food.h"

Food::Food(int width, int height, int frameCount, int frameSpeed) : SimpleAnimatedEntity(
    new Rect(0, 0, width, height),
    new Rect(-200, 0, width, height),
    frameCount,
    width,
    frameSpeed,
    0,
    rand() % 1
)
{
}

Food::~Food()
{
    delete _sourceRect;
    delete _destRect;
}

