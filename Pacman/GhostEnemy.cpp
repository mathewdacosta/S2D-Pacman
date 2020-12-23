#include "GhostEnemy.h"


GhostEnemy::GhostEnemy(GhostType type, int initialX, int initialY, MoveDirection initialDirection)
{
    this->_type = type;
    this->_position = new Vector2(initialX, initialY);
    this->_direction = initialDirection;
    this->_sourceRect = new Rect(0, 0, 20, 20);
    // Derive speed and frequency of AI decisions from the type of ghost
    this->_speed = 0.04f + (static_cast<int>(type) * 0.012f);
    this->_decisionTime = 500 + (static_cast<int>(type) * 300);
}

GhostEnemy::~GhostEnemy()
{
    delete _position;
    delete _sourceRect;
}

float GhostEnemy::GetPositionX()
{
    return _position->X;
}

float GhostEnemy::GetPositionY()
{
    return _position->Y;
}

int GhostEnemy::GetWidth()
{
    return _sourceRect->Width;
}

int GhostEnemy::GetHeight()
{
    return _sourceRect->Height;
}

void GhostEnemy::SetPosition(float x, float y)
{
    _position->X = x;
    _position->Y = y;
}

void GhostEnemy::SetTexture(Texture2D* texture)
{
    this->_texture = texture;
}

void GhostEnemy::ChangeDirection(int elapsedTime)
{
    _direction = static_cast<MoveDirection>(rand() % 4);
}

void GhostEnemy::Draw()
{
    SpriteBatch::Draw(_texture, _position, _sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}

void GhostEnemy::Update(int elapsedTime)
{
    _lastDecisionTime += elapsedTime;
    if (_lastDecisionTime > _decisionTime)
    {
        _lastDecisionTime = 0;
        ChangeDirection(elapsedTime);
    }

    UpdatePosition(elapsedTime);
    UpdateAnimation(elapsedTime);
}

void GhostEnemy::UpdateAnimation(int elapsedTime) const
{
    int column = static_cast<int>(_type);
    int row = static_cast<int>(_direction);

    _sourceRect->X = column * _sourceRect->Width;
    _sourceRect->Y = row * _sourceRect->Height;
}

void GhostEnemy::UpdatePosition(const int elapsedTime)
{
    const float movementAmount = _speed * elapsedTime;
    switch (_direction)
    {
    case MoveDirection::Right:
        _position->X += movementAmount; // Moves +x
        break;
    case MoveDirection::Left:
        _position->X -= movementAmount; // Moves -x
        break;
    case MoveDirection::Down:
        _position->Y += movementAmount; // Moves +y
        break;
    case MoveDirection::Up:
        _position->Y -= movementAmount; // Moves -y
        break;
    }
}
