#pragma once

/// <summary> Possible movement directions - the ordinals match the position of the Pacman sprites on the spritesheet </summary>
struct MovingEnemy
{
    float speed; // movement speed
    Vector2* position; // position on screen
    Rect* sourceRect; // current location in tilemap
    Texture2D* texture; // tilemap
    MoveDirection direction; // current movement direction
    int animCurrentTime; // current time since last update
    int animFrame; // current frame in animation
};