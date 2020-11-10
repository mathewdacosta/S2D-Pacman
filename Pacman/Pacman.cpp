#include "Pacman.h"
#include "Player.h"

#include <sstream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
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
        .sprintCooldown = 0
    };
    _sprintKeyDown = false;

    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i] = new Food();
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
        delete _munchies[0]->sourceRect;
        delete _munchies[0]->destRect;
        delete _munchies[0];
    }
    delete[] _munchies;
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
    _menu->helpPosition = new Vector2(10, 740);

    // Load Pacman
    _player->texture = new Texture2D();
    _player->texture->Load("Textures/Pacman.png", false);
    _player->position = new Vector2(350.0f, 350.0f);
    _player->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

    // Load Munchie
    Texture2D* munchieTexture = new Texture2D();
    munchieTexture->Load("Textures/Cherry.png", true);
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i]->texture = munchieTexture;
        _munchies[i]->animCurrentTime = 0;
        _munchies[i]->animFrame = rand() % 1;
        _munchies[i]->animFrameTime = (rand() % 170) + 300;
        _munchies[i]->sourceRect = new Rect(0, 0, 32, 32);
        _munchies[i]->destRect = new Rect(rand() % (SCREEN_WIDTH - 32), rand() % (SCREEN_HEIGHT - 32), 32, 32);
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

    // Update movement according to input
    UpdatePacmanMovement(elapsedTime);

    // Check viewport boundaries and 
    CheckViewportCollision();

    // Update animation frames
    UpdatePacmanFrame(elapsedTime);
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        UpdateMunchieFrame(_munchies[i], elapsedTime);
    }
}

void Pacman::Draw(int elapsedTime)
{
    SpriteBatch::BeginDraw(); // Starts Drawing

    if (_menu->started)
    {
        // Allows us to easily create a string
        std::stringstream stream;
        stream << "Pacman X: " << _player->position->X << " Y: " << _player->position->Y;

        // Draw player
        SpriteBatch::Draw(_player->texture, _player->position, _player->sourceRect);

        // Draw munchies
        for (int i = 0; i < MUNCHIE_COUNT; i++)
        {
            SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->destRect, _munchies[i]->sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White,
                          SpriteEffect::NONE);
        }

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
