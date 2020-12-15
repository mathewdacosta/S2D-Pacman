#include "Wall.h"

Wall::Wall() : SimpleAnimatedEntity(
    new Rect(0, 0, 32, 32),
    new Rect(-200, 0, 32, 32),
    1,
    32,
    1000000,
    0,
    rand() % 1
)
{
}


Wall::~Wall()
{
    delete _sourceRect;
    delete _destRect;
}
