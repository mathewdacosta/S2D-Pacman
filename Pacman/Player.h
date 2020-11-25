#pragma once
#include "MoveDirection.h"

struct Player
{
    const float _cBaseSpeed; // movement speed
    const float _cSprintMultiplier; // speed multiplier while sprinting
    const int _cSprintDuration; // length of sprint
    const int _cSprintCooldown; // length of cooldown between sprints
    const int _cFrameTime; // interval between animation frames

    Vector2* position; // position on screenw
    Rect* sourceRect; // current location in tilemap
    Texture2D* texture; // tilemap
    MoveDirection direction; // current movement direction
    int animCurrentTime; // current time since last update
    int animFrame; // current frame in animation
    
    int sprintTime; // time left on current sprint
    int sprintCooldown; // time left on current sprint

    bool dead; // whether or not pacman is dead
};
