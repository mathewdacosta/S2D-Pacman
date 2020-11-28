#include "Player.h"


void Player::HandleMovementInput(Input::KeyboardState* keyboardState)
{
    // Check WASD for directional inputs
    if (keyboardState->IsKeyDown(Input::Keys::D))
    {
        direction = MoveDirection::Right;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::A))
    {
        direction = MoveDirection::Left;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::S))
    {
        direction = MoveDirection::Down;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::W))
    {
        direction = MoveDirection::Up;
    }

    // Sprint
    if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT) && !sprintKeyDown && sprintCooldown <= 0)
    {
        sprintTime = _cSprintDuration;
        sprintCooldown = _cSprintCooldown;
        sprintKeyDown = true;
    }

    if (keyboardState->IsKeyUp(Input::Keys::LEFTSHIFT))
    {
        sprintKeyDown = false;
    }
}


void Player::UpdateAnimation(int elapsedTime)
{
    // Increment Pacman animation frame
    animCurrentTime += elapsedTime;
    if (animCurrentTime > _cFrameTime)
    {
        animCurrentTime -= _cFrameTime;
        animFrame = (animFrame + 1) % PLAYER_FRAMES;
        sourceRect->X = sourceRect->Width * animFrame;
    }

    // Update Pacman direction
    sourceRect->Y = sourceRect->Height * static_cast<int>(direction);
}


void Player::UpdatePosition(int elapsedTime)
{
    float movementAmount = _cBaseSpeed * elapsedTime;

    // Handle sprint modifier
    // TODO: turn this into a meter that depletes on hold only + recharges
    if (sprintTime > 0)
    {
        movementAmount *= _cSprintMultiplier;
        sprintTime -= elapsedTime;
    }
    else
    {
        sprintTime = 0;
        if (sprintCooldown > 0)
        {
            sprintCooldown -= elapsedTime;
        }
        else
        {
            sprintCooldown = 0;
        }
    }

    // Move in current facing direction by current move amount
    switch (direction)
    {
    case MoveDirection::Right:
        position->X += movementAmount; // Moves Pacman +x
        break;
    case MoveDirection::Left:
        position->X -= movementAmount; // Moves Pacman -x
        break;
    case MoveDirection::Down:
        position->Y += movementAmount; // Moves Pacman +y
        break;
    case MoveDirection::Up:
        position->Y -= movementAmount; // Moves Pacman -y
        break;
    }
}
