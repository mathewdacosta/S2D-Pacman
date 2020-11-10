#pragma once

/// <summary> Possible movement directions - the ordinals match the position of the Pacman sprites on the spritesheet </summary>
enum class MoveDirection
{
    Right = 0,
    Down,
    Left,
    Up
};

struct Player
{
    const float _cBaseSpeed; // movement speed
    const float _cSprintMultiplier; // speed multiplier while sprinting
    const int _cSprintDuration; // length of sprint
    const int _cSprintCooldown; // length of cooldown between sprints
    const int _cFrameTime; // interval between animation frames

    Vector2* position; // position on screen
    Rect* sourceRect; // current location in tilemap
    Texture2D* texture; // tilemap
    MoveDirection direction; // current movement direction
    int animCurrentTime; // current time since last update
    int animFrame; // current frame in animation
    
    int sprintTime; // time left on current sprint
    int sprintCooldown; // time left on current sprint
};