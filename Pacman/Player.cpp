#include "Player.h"

#include "Food.h"


Player::Player()
{
    _position = new Vector2(350.0f, 350.0f);
    _direction = MoveDirection::Right;
    _sourceRect = new Rect(0.0f, 0.0f, 27, 27);
    _texture = nullptr;
}

Player::~Player()
{
    delete _position;
    delete _sourceRect;
    delete _texture;
}

void Player::LoadTexture()
{
    _texture = new Texture2D();
    _texture->Load("Textures/Pacman.png", false);
}


void Player::Update(int elapsedTime)
{
    if (_dead)
        // TODO update death animation
        return;
    
    // Update movement according to input
    UpdatePosition(elapsedTime);

    // Check whether Pacman is off viewport boundaries
    CheckViewportCollision();

    // Update animation frames
    UpdateAnimation(elapsedTime);
}

void Player::Draw()
{
    // Draw player if not dead
    if (!_dead)
    {
        SpriteBatch::Draw(_texture, _position, _sourceRect);
    }
}

bool Player::IsDead() const
{
    return _dead;
}

void Player::SetDead(const bool dead)
{
    this->_dead = dead;
}

Vector2* Player::GetPosition()
{
    return _position;
}


void Player::SetPosition(int x, int y)
{
    _position->X = x;
    _position->Y = y;
}

int Player::GetWidth()
{
    return _sourceRect->Width;
}

int Player::GetHeight()
{
    return _sourceRect->Height;
}

void Player::HandleMovementInput(Input::KeyboardState* keyboardState)
{
    // Check WASD for directional inputs
    if (keyboardState->IsKeyDown(Input::Keys::D))
    {
        _direction = MoveDirection::Right;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::A))
    {
        _direction = MoveDirection::Left;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::S))
    {
        _direction = MoveDirection::Down;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::W))
    {
        _direction = MoveDirection::Up;
    }

    // Sprint
    _sprintKeyDown = keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT);
}


void Player::CheckViewportCollision()
{
    float centreX = _position->X + (_sourceRect->Width / 2);
    float centreY = _position->Y + (_sourceRect->Height / 2);

    // Check if Pacman centre is off to RIGHT of screen
    if (centreX > Graphics::GetViewportWidth())
    {
        _position->X -= Graphics::GetViewportWidth();
    }

    // Check if Pacman centre is off to LEFT of screen
    if (centreX < 0)
        {
        _position->X += Graphics::GetViewportWidth();
        }

    // Check if Pacman centre is off BOTTOM of screen
    if (centreY > Graphics::GetViewportHeight())
    {
        _position->Y -= Graphics::GetViewportHeight();
    }

    // Check if Pacman centre is off TOP of screen
    if (centreY < 0)
    {
        _position->Y += Graphics::GetViewportHeight();
    }
}


void Player::UpdateAnimation(int elapsedTime)
{
    // Increment Pacman animation frame
    _animCurrentTime += elapsedTime;
    if (_animCurrentTime > _cAnimFrameDuration)
    {
        _animCurrentTime -= _cAnimFrameDuration;
        _animFrame = (_animFrame + 1) % PLAYER_FRAMES;
        _sourceRect->X = _sourceRect->Width * _animFrame;
    }

    // Update Pacman direction
    _sourceRect->Y = _sourceRect->Height * static_cast<int>(_direction);
}


void Player::UpdatePosition(int elapsedTime)
{
    float movementAmount = _cBaseSpeed * elapsedTime;

    if (_sprintKeyDown)
    {
        movementAmount *= _cSprintSpeedMultiplier;
    }

    // Move in current facing direction by current move amount
    switch (_direction)
    {
    case MoveDirection::Right:
        _position->X += movementAmount; // Moves Pacman +x
        break;
    case MoveDirection::Left:
        _position->X -= movementAmount; // Moves Pacman -x
        break;
    case MoveDirection::Down:
        _position->Y += movementAmount; // Moves Pacman +y
        break;
    case MoveDirection::Up:
        _position->Y -= movementAmount; // Moves Pacman -y
        break;
    }
}


