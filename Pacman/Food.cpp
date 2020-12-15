#include "Food.h"

Food::Food(int width, int height) : SimpleAnimatedEntity(
    new Rect(0, 0, width, height),
    new Rect(-200, 0, width, height),
    2,
    width,
    (rand() % 170) + 300,
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

