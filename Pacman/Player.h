#pragma once

#define PLAYER_FRAMES 8

#include "MoveDirection.h"
#include "S2D/S2D.h"
using namespace S2D;

class Player
{
private:
    const float _cBaseSpeed = 0.1f; // movement speed
    const float _cSprintMultiplier = 2.1f; // speed multiplier while sprinting
    const int _cSprintDuration = 2000; // length of sprint
    const int _cSprintCooldown = 5000; // length of cooldown between sprints
    const int _cFrameTime = 60; // interval between animation frames
    
    bool sprintKeyDown = false;
    
    int animCurrentTime = 0; // current time since last update
    int animFrame = 0; // current frame in animation
    
    int sprintTime = 0; // time left on current sprint
    int sprintCooldown = 0; // time left on current sprint

public:
    Vector2* position; // position on screen
    Rect* sourceRect; // current location in tilemap
    Texture2D* texture; // tilemap

    MoveDirection direction; // current movement direction

    bool dead = false; // whether or not pacman is dead

    /// <summary> Perform movement inputs </summary>
    void HandleMovementInput(Input::KeyboardState* keyboardState);

    void UpdateAnimation(int elapsedTime);

    void UpdatePosition(int elapsedTime);
};
