#include "Pacman.h"
#include "Player.h"
#include "Food.h"
#include "MovingEnemy.h"

#include <sstream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
    srand(time(nullptr));

    _menu = new MenuState {
        .started = false,
        .spaceKeyDown = false,
        .paused = false,
        .pKeyDown = false
    };

    _player = new Player {
        ._cBaseSpeed = 0.1f,
        ._cSprintMultiplier = 2.1f,
        ._cSprintDuration = 2000,
        ._cSprintCooldown = 5000,
        ._cFrameTime = 60,
        .animCurrentTime = 0,
        .animFrame = 0,
        .sprintTime = 0,
        .sprintCooldown = 0,
        .dead = false
    };
    _sprintKeyDown = false;

    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i] = new Food();
        _walls[i] = new Food();
    }

    for (int i = 0; i < GHOST_COUNT; i++)
    {
        _ghosts[i] = new MovingEnemy();
    }

    //Initialise important Game aspects
    Graphics::Initialise(argc, argv, this, SCREEN_WIDTH, SCREEN_HEIGHT, false, 25, 25, "Logo here: The Game", 60);
    Input::Initialise();

    // Start the Game Loop - This calls Update and Draw in game loop
    Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
    delete _menu->background;
    delete _menu->rectangle;
    delete _menu->textTexture;
    delete _menu->pausedSourceRect;
    delete _menu->pausedDestRect;
    delete _menu->logoSourceRect;
    delete _menu->logoDestRect;
    delete _menu->helpPosition;
    delete _menu;
    
    delete _player->texture;
    delete _player->sourceRect;
    delete _player->position;
    delete _player;

    delete _munchies[0]->texture;
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        delete _munchies[i]->sourceRect;
        delete _munchies[i]->destRect;
        delete _munchies[i];
        delete _walls[i]->sourceRect;
        delete _walls[i]->destRect;
        delete _walls[i];
    }
    delete[] _munchies;
    delete[] _walls;

    delete _ghosts[0]->texture;
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        delete _ghosts[i]->sourceRect;
        delete _ghosts[i]->position;
        delete _ghosts[i];
    }
    delete[] _ghosts;
}

void Pacman::LoadContent()
{
    // Set up menu
    _menu->background = new Texture2D();
    _menu->background->Load("Textures/Transparency.png", false);
    _menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

    _menu->textTexture = new Texture2D();
    _menu->textTexture->Load("Textures/MenuText.png", false);
    _menu->pausedSourceRect = new Rect(0.0f, 0.0f, 200, 40);
    _menu->pausedDestRect = new Rect(
        (Graphics::GetViewportWidth() - _menu->pausedSourceRect->Width) / 2.0f,
        (Graphics::GetViewportHeight() - _menu->pausedSourceRect->Height) / 2.0f,
        _menu->pausedSourceRect->Width,
        _menu->pausedSourceRect->Height);
    _menu->logoSourceRect = new Rect(0.0f, 40.0f, 215, 47);
    _menu->logoDestRect = new Rect(
        (Graphics::GetViewportWidth() - _menu->logoSourceRect->Width) / 2.0f,
        (Graphics::GetViewportHeight() / 3.0f) - (_menu->logoSourceRect->Height / 2.0f),
        _menu->logoSourceRect->Width,
        _menu->logoSourceRect->Height);
    _menu->helpPosition = new Vector2(10, Graphics::GetViewportHeight() - 28);

    // Load Pacman
    _player->texture = new Texture2D();
    _player->texture->Load("Textures/Pacman.png", false);
    _player->position = new Vector2(350.0f, 350.0f);
    _player->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

    // Load Munchie
    Texture2D* munchieTexture = new Texture2D();
    munchieTexture->Load("Textures/Cherry.png", false);
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i]->texture = munchieTexture;
        _munchies[i]->animCurrentTime = 0;
        _munchies[i]->animFrame = rand() % 1;
        _munchies[i]->animFrameTime = (rand() % 170) + 300;
        _munchies[i]->sourceRect = new Rect(0, 0, 21, 30);
        _munchies[i]->destRect = new Rect(rand() % (SCREEN_WIDTH - 21), rand() % (SCREEN_HEIGHT - 30), 21, 30);
        
        _walls[i]->texture = munchieTexture;
        _walls[i]->animCurrentTime = 0;
        _walls[i]->animFrame = rand() % 1;
        _walls[i]->animFrameTime = 0;
        _walls[i]->sourceRect = new Rect(0, 0, 21, 30);
        _walls[i]->destRect = new Rect(rand() % (SCREEN_WIDTH - 21), rand() % (SCREEN_HEIGHT - 30), 21, 30);
    }

    // Load enemy
    Texture2D* ghostTexture = new Texture2D();
    ghostTexture->Load("Textures/GhostBlue.png", false);
    for (int i = 0; i < GHOST_COUNT; i++)
    {
        _ghosts[i]->texture = ghostTexture;
        _ghosts[i]->animCurrentTime = 0;
        _ghosts[i]->sourceRect = new Rect(0, 0, 20, 20);
        _ghosts[i]->position = new Vector2(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 20));
    }

    // Set string position
    _stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
    // Gets the current state of the keyboard
    Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

    // Check for start input
    if (!_menu->started && keyboardState->IsKeyDown(Input::Keys::SPACE))
        _menu->started = true;

    if (!_menu->started) return;

    // Check for pause input/toggle
    CheckPaused(keyboardState, Input::Keys::P);
    if (_menu->paused) return;

    // Handle gameplay inputs
    Input(keyboardState);

    // Store last position in case of collision
    Vector2 lastPosition = Vector2 {
        _player->position->X,
        _player->position->Y
    };

    // Update movement according to input
    UpdatePacmanMovement(elapsedTime);

    if (CheckWallCollisions())
    {
        _player->position->X = lastPosition.X;
        _player->position->Y = lastPosition.Y;
    }

    // Check viewport boundaries
    CheckViewportCollision();

    // Check collisions with munchies
    CheckMunchieCollisions();

    // Update animation frames
    UpdatePacmanFrame(elapsedTime);
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        UpdateMunchieFrame(_munchies[i], elapsedTime);
        UpdateMunchieFrame(_walls[i], elapsedTime);
    }

    // TODO: update ghost position, do AI and collisions
}

void Pacman::Draw(int elapsedTime)
{
    SpriteBatch::BeginDraw(); // Starts Drawing

    if (_menu->started)
    {
        // Allows us to easily create a string
        std::stringstream stream;
        stream << "Collisions: " << _collisionCount << "\nX: " << _player->position->X << "\nY: " << _player->position->Y;

        // Draw player if not dead
        if (!_player->dead)
        {
            SpriteBatch::Draw(_player->texture, _player->position, _player->sourceRect);
        }

        // Draw munchies
        for (int i = 0; i < MUNCHIE_COUNT; i++)
        {
            SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->destRect, _munchies[i]->sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White,
                          SpriteEffect::NONE);
            SpriteBatch::Draw(_walls[i]->texture, _walls[i]->destRect, _walls[i]->sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White,
                          SpriteEffect::NONE);
        }

        // TODO: draw ghosts

        // Draws String
        SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

        SpriteBatch::DrawRectangle(12, 760, _player->sprintTime / 2, 4, Color::Blue);
        SpriteBatch::DrawRectangle(12, 764, _player->sprintCooldown / 5, 4, Color::Cyan);

        // Draw pause menu
        if (_menu->paused)
        {
            SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
            SpriteBatch::Draw(_menu->textTexture, _menu->pausedDestRect, _menu->pausedSourceRect);
        }
    }
    else
    {
        SpriteBatch::Draw(_menu->textTexture, _menu->logoDestRect, _menu->logoSourceRect);
        stringstream help;
        help << "Press SPACE to start, WASD to change direction, P to pause";
        SpriteBatch::DrawString(help.str().c_str(), _menu->helpPosition, Color::Green);
    }

    SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Input(Input::KeyboardState* keyboardState)
{
    // Check WASD for directional inputs
    if (keyboardState->IsKeyDown(Input::Keys::D))
    {
        _player->direction = MoveDirection::Right;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::A))
    {
        _player->direction = MoveDirection::Left;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::S))
    {
        _player->direction = MoveDirection::Down;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::W))
    {
        _player->direction = MoveDirection::Up;
    }

    // Sprint
    if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT) && !_sprintKeyDown && _player->sprintCooldown <= 0)
    {
        _player->sprintTime = _player->_cSprintDuration;
        _player->sprintCooldown = _player->_cSprintCooldown;
        _sprintKeyDown = true;
    }

    if (keyboardState->IsKeyUp(Input::Keys::LEFTSHIFT))
    {
        _sprintKeyDown = false;
    }
}

void Pacman::CheckPaused(Input::KeyboardState* keyboardState, Input::Keys pauseKey)
{
    if (keyboardState->IsKeyDown(pauseKey) && !_menu->pKeyDown)
    {
        _menu->paused = !_menu->paused;
        _menu->pKeyDown = true; // debounce P key
    }

    if (keyboardState->IsKeyUp(pauseKey))
    {
        _menu->pKeyDown = false;
    }
}


bool Pacman::CheckBoxCollision(Rect* rect1, Rect* rect2)
{
    return CheckBoxCollision(
        rect1->X, rect1->X + rect1->Width, rect1->Y, rect1->Y + rect1->Height,
        rect2->X, rect2->X + rect2->Width, rect2->Y, rect2->Y + rect2->Height);
}


bool Pacman::CheckBoxCollision(Rect* rect1, float left2, float right2, float top2, float bottom2)
{
    return CheckBoxCollision(
        rect1->X, rect1->X + rect1->Width, rect1->Y, rect1->Y + rect1->Height,
        left2, right2, top2, bottom2);
}

bool Pacman::CheckBoxCollision(float left1, float right1, float top1, float bottom1, float left2, float right2, float top2, float bottom2)
{
    if (left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2)
        return false;

    return true;
}


void Pacman::CheckMunchieCollisions()
{
    Vector2* playerPosition = _player->position;
    _collisionCount = 0;
    for (int i = 0; i < MUNCHIE_COUNT; ++i)
    {
        if (CheckBoxCollision(_munchies[i]->destRect, playerPosition->X, playerPosition->X + _player->sourceRect->Width, playerPosition->Y, playerPosition->Y + _player->sourceRect->Height))
        {
            _collisionCount++;
        }
    }
}

bool Pacman::CheckWallCollisions()
{
    Vector2* playerPosition = _player->position;

    for (int i = 0; i < MUNCHIE_COUNT; ++i)
    {
        if (CheckBoxCollision(_walls[i]->destRect, playerPosition->X, playerPosition->X + _player->sourceRect->Width, playerPosition->Y, playerPosition->Y + _player->sourceRect->Height))
        {
            return true;
        }
    }

    return false;
}


void Pacman::CheckViewportCollision()
{
    float centreX = _player->position->X + (_player->sourceRect->Width / 2);
    float centreY = _player->position->Y + (_player->sourceRect->Height / 2);

    // Check if Pacman centre is off to RIGHT of screen
    if (centreX > Graphics::GetViewportWidth())
    {
        _player->position->X -= Graphics::GetViewportWidth();
    }

    // Check if Pacman centre is off to LEFT of screen
    if (centreX < 0) // if centre X coord < 0
    {
        _player->position->X += Graphics::GetViewportWidth();
    }

    // Check if Pacman centre is off BOTTOM of screen
    if (centreY > Graphics::GetViewportHeight())
    {
        _player->position->Y -= Graphics::GetViewportHeight();
    }

    // Check if Pacman centre is off TOP of screen
    if (centreY < 0)
    {
        _player->position->Y += Graphics::GetViewportHeight();
    }
}

void Pacman::UpdatePacmanMovement(int elapsedTime)
{
    float movementAmount = _player->_cBaseSpeed * elapsedTime;
    
    // Handle sprint modifier
    // TODO: turn this into a meter that depletes on hold only + recharges
    if (_player->sprintTime > 0)
    {
        movementAmount *= _player->_cSprintMultiplier;
        _player->sprintTime -= elapsedTime;
    }
    else
    {
        _player->sprintTime = 0;
        if (_player->sprintCooldown > 0)
        {
            _player->sprintCooldown -= elapsedTime;
        }
        else
        {
            _player->sprintCooldown = 0;
        }
    }

    // Move in current facing direction by current move amount
    switch (_player->direction)
    {
    case MoveDirection::Right:
        _player->position->X += movementAmount; // Moves Pacman +x
        break;
    case MoveDirection::Left:
        _player->position->X -= movementAmount; // Moves Pacman -x
        break;
    case MoveDirection::Down:
        _player->position->Y += movementAmount; // Moves Pacman +y
        break;
    case MoveDirection::Up:
        _player->position->Y -= movementAmount; // Moves Pacman -y
        break;
    }
}

void Pacman::UpdatePacmanFrame(int elapsedTime)
{
    // Increment Pacman animation frame
    _player->animCurrentTime += elapsedTime;
    if (_player->animCurrentTime > _player->_cFrameTime)
    {
        // _player->animCurrentTime = 0;
        _player->animCurrentTime -= _player->_cFrameTime;
        _player->animFrame = (_player->animFrame + 1) % PLAYER_FRAMES;
        _player->sourceRect->X = _player->sourceRect->Width * _player->animFrame;
    }

    // Update Pacman direction
    _player->sourceRect->Y = _player->sourceRect->Height * static_cast<int>(_player->direction);
}

void Pacman::UpdateMunchieFrame(Food* munchie, int elapsedTime)
{
    // Increment munchie animation frame
    munchie->animCurrentTime += elapsedTime;
    if (munchie->animCurrentTime > munchie->animFrameTime)
    {
        munchie->animCurrentTime = 0;
        munchie->animFrame = (munchie->animFrame + 1) % MUNCHIE_FRAMES;
        munchie->sourceRect->X = munchie->sourceRect->Width * munchie->animFrame;
    }
}
