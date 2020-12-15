#include "GhostEnemy.h"


void GhostEnemy::ChangeDirection(int elapsedTime)
{
    lastDecisionTime = 0;
    direction = static_cast<MoveDirection>(rand() % 4);
}

void GhostEnemy::Draw()
{
    SpriteBatch::Draw(texture, position, sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}

void GhostEnemy::Update(int elapsedTime)
{
    lastDecisionTime += elapsedTime;
    if (lastDecisionTime > decisionTime)
    {
        ChangeDirection(elapsedTime);
    }

    UpdatePosition(elapsedTime);
    UpdateAnimation(elapsedTime);
}

void GhostEnemy::UpdateAnimation(int elapsedTime) const
{
    int column = static_cast<int>(type);
    int row = static_cast<int>(direction);

    sourceRect->X = column * sourceRect->Width;
    sourceRect->Y = row * sourceRect->Height;
}

void GhostEnemy::UpdatePosition(const int elapsedTime) const
{
    const float movementAmount = speed * elapsedTime;
    switch (direction)
    {
    case MoveDirection::Right:
        position->X += movementAmount; // Moves +x
        break;
    case MoveDirection::Left:
        position->X -= movementAmount; // Moves -x
        break;
    case MoveDirection::Down:
        position->Y += movementAmount; // Moves +y
        break;
    case MoveDirection::Up:
        position->Y -= movementAmount; // Moves -y
        break;
    }
}
