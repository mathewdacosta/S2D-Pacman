#pragma once

#define PLAYER_FRAMES 8

#include "Entity.h"
#include "MoveDirection.h"
#include "S2D/S2D.h"
using namespace S2D;

class Player : public Entity
{
private:
    const float _cBaseSpeed = 0.1f;             // movement speed
    const float _cSprintSpeedMultiplier = 2.1f; // speed multiplier while sprinting
    const float _cSprintDepleteRate = 1.0f;     // multiplier for depletion of sprint meter
    const float _cSprintRecoveryRate = 0.4f;    // multiplier for recovery of sprint meter 
    const float _cSprintMaximum = 2000;         // maximum amount of sprint meter
    const int _cAnimFrameDuration = 60;         // interval between animation frames
    
    int _animCurrentTime = 0;                   // current time since last update
    int _animFrame = 0;                         // current frame in animation
    
    float _sprintMeter = 0;                     // time left on current sprint
    bool _sprintKeyDown = false;                // state of sprint key

    bool _dead = false;                         // whether or not pacman is dead
    
    Vector2* _position;                         // position on screen
    MoveDirection _direction;                   // current movement direction
    Rect* _sourceRect;                          // current location in tilemap
    Texture2D* _texture;                        // tilemap

    void CheckViewportCollision();
    void UpdateAnimation(int elapsedTime);
    void UpdatePosition(int elapsedTime);

public:
    Player();
    ~Player();

    void LoadTexture();
    
    bool IsDead() const;
    void SetDead(bool dead);

    Vector2* GetPosition();
    int GetWidth();
    int GetHeight();
    
    /// <summary> Perform movement inputs </summary>
    void HandleMovementInput(Input::KeyboardState* keyboardState);

    void Draw() override;
    void Update(int elapsedTime);
};
