#pragma once

#include "MoveDirection.h"
#include "S2D/S2D.h"
using namespace S2D;

enum class GhostType
{
    BLUE,
    YELLOW,
    RED,
    PINK
};

class GhostEnemy
{
private:
    int lastDecisionTime = 0;
    void UpdateAnimation(int elapsedTime) const;
    void UpdatePosition(int elapsedTime) const;
    
public:
    float speed; // movement speed
    Vector2* position; // position on screen
    Rect* sourceRect; // current location in tilemap
    Texture2D* texture; // tilemap
    MoveDirection direction; // current movement direction
    GhostType type; // current ghost type
    int decisionTime; // interval between changing direction

    void ChangeDirection(int elapsedTime);
    void Update(int elapsedTime);
};