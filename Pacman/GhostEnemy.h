#pragma once

#include "Entity.h"
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

class GhostEnemy : public Entity
{
private:
    float _speed; // movement speed
    Vector2* _position; // position on screen
    Rect* _sourceRect; // current location in tilemap
    Texture2D* _texture; // tilemap
    MoveDirection _direction; // current movement direction
    GhostType _type; // current ghost type

    int _decisionTime; // interval between changing direction
    int _lastDecisionTime; // time since last decision
    
    void UpdateAnimation(int elapsedTime) const;
    void UpdatePosition(int elapsedTime);
    
public:
    GhostEnemy(GhostType type, int initialX, int initialY, MoveDirection initialDirection);
    ~GhostEnemy();

    // Properties
    float GetPositionX();
    float GetPositionY();
    int GetWidth();
    int GetHeight();
    void SetPosition(float x, float y);
    void SetTexture(Texture2D* texture);
    
    // Game logic    
    void ChangeDirection(int elapsedTime);

    // Game loop
    void Draw() override;
    void Update(int elapsedTime);
};