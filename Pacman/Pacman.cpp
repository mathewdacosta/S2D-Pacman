#include "Pacman.h"
#include "Player.h"
#include "Food.h"
#include "GhostEnemy.h"

#include <sstream>
#include <time.h>

#include "BoxCollisions.h"

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
    srand(time(nullptr));

    _menu = new MenuState{
        .started = false,
        .spaceKeyDown = false,
        .paused = false,
        .pKeyDown = false
    };

    _player = new Player();

    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i] = new Food();
        _walls[i] = new Food();
    }

    for (int i = 0; i < GHOST_COUNT; i++)
    {
        _ghosts[i] = new GhostEnemy();
    }

    //Initialise important Game aspects
    Audio::Initialise();
    Graphics::Initialise(argc, argv, this, SCREEN_WIDTH, SCREEN_HEIGHT, false, 25, 25, "Pacman", 60);
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
    Texture2D* wallTexture = new Texture2D();
    wallTexture->Load("Textures/WallTemp.png", false);
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i]->texture = munchieTexture;
        _munchies[i]->animCurrentTime = 0;
        _munchies[i]->animFrame = rand() % 1;
        _munchies[i]->animFrameTime = (rand() % 170) + 300;
        _munchies[i]->sourceRect = new Rect(0, 0, 21, 30);
        _munchies[i]->destRect = new Rect(rand() % (SCREEN_WIDTH - 21), rand() % (SCREEN_HEIGHT - 30), 21, 30);

        _walls[i]->texture = wallTexture;
        _walls[i]->animCurrentTime = 0;
        _walls[i]->animFrame = rand() % 1;
        _walls[i]->animFrameTime = 300;
        _walls[i]->sourceRect = new Rect(0, 0, 32, 32);
        _walls[i]->destRect = new Rect(rand() % (SCREEN_WIDTH - 32), rand() % (SCREEN_HEIGHT - 32), 32, 32);
    }

    // Load enemy
    Texture2D* ghostTexture = new Texture2D();
    ghostTexture->Load("Textures/Ghosts.png", false);
    for (int i = 0; i < GHOST_COUNT; i++)
    {
        _ghosts[i]->texture = ghostTexture;
        _ghosts[i]->type = static_cast<GhostType>(i % 4);
        _ghosts[i]->direction = static_cast<MoveDirection>(rand() % 4);
        _ghosts[i]->sourceRect = new Rect(0, 0, 20, 20);
        _ghosts[i]->position = new Vector2(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 20));
        _ghosts[i]->speed = 0.04f + (static_cast<int>(_ghosts[i]->type) * 0.012f);
        _ghosts[i]->decisionTime = 500 + (static_cast<int>(_ghosts[i]->type) * 300);
    }

    // Set string position
    _stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
    // Gets the current state of the keyboard
    Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

    // Check for start input
    CheckStart(keyboardState, Input::Keys::SPACE);
    if (!_menu->started) return;

    // Check for pause input/toggle
    CheckPaused(keyboardState, Input::Keys::P);
    if (_menu->paused) return;

    // Handle movement inputs
    _player->HandleMovementInput(keyboardState);

    // Store last position in case of collision
    Vector2 lastPosition = Vector2{
        _player->position->X,
        _player->position->Y
    };

    // Update movement according to input
    _player->UpdatePosition(elapsedTime);

    if (CheckWallCollisions(_player->position))
    {
        _player->position->X = lastPosition.X;
        _player->position->Y = lastPosition.Y;
    }

    // Check whether Pacman is off viewport boundaries
    CheckViewportCollision();

    // Check collisions with munchies
    CheckMunchieCollisions();

    // Update animation frames
    _player->UpdateAnimation(elapsedTime);
    
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        UpdateMunchieFrame(_munchies[i], elapsedTime);
        UpdateMunchieFrame(_walls[i], elapsedTime);
    }

    UpdateGhosts(elapsedTime);
}

void Pacman::Draw(int elapsedTime)
{
    SpriteBatch::BeginDraw(); // Starts Drawing

    if (_menu->started)
    {
        // Allows us to easily create a string
        std::stringstream stream;
        stream << "Collisions: " << _collisionCount << "\nX: " << _player->position->X << "\nY: " << _player->position->
            Y;

        // Draw player if not dead
        if (!_player->dead)
        {
            SpriteBatch::Draw(_player->texture, _player->position, _player->sourceRect);
        }

        // Draw munchies
        for (int i = 0; i < MUNCHIE_COUNT; i++)
        {
            SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->destRect, _munchies[i]->sourceRect, Vector2::Zero,
                              1.0f, 0.0f, Color::White,
                              SpriteEffect::NONE);
            SpriteBatch::Draw(_walls[i]->texture, _walls[i]->destRect, _walls[i]->sourceRect, Vector2::Zero, 1.0f, 0.0f,
                              Color::White,
                              SpriteEffect::NONE);
        }

        // Draw ghosts
        for (int i = 0; i < GHOST_COUNT; i++)
        {
            SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect, Vector2::Zero, 1.0f,
                              0.0f, Color::White,
                              SpriteEffect::NONE);
        }

        // Draws String
        SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

        // TODO: sprint meter display        
        // SpriteBatch::DrawRectangle(12, 760, _player->sprintTime / 2, 4, Color::Blue);
        // SpriteBatch::DrawRectangle(12, 764, _player->sprintCooldown / 5, 4, Color::Cyan);

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

void Pacman::CheckStart(Input::KeyboardState* keyboardState, Input::Keys startKey)
{
    if (!_menu->started && keyboardState->IsKeyDown(startKey))
        _menu->started = true;
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


void Pacman::CheckMunchieCollisions()
{
    Vector2* playerPosition = _player->position;
    _collisionCount = 0;
    for (int i = 0; i < MUNCHIE_COUNT; ++i)
    {
        if (CheckBoxCollision(_munchies[i]->destRect,
                              playerPosition->X, playerPosition->X + _player->sourceRect->Width,
                              playerPosition->Y, playerPosition->Y + _player->sourceRect->Height))
        {
            _collisionCount++;
        }
    }
}

bool Pacman::CheckWallCollisions(Vector2* position)
{
    for (int i = 0; i < MUNCHIE_COUNT; ++i)
    {
        if (CheckBoxCollision(_walls[i]->destRect, position->X, position->X + _player->sourceRect->Width,
                              position->Y, position->Y + _player->sourceRect->Height))
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


void Pacman::UpdateGhosts(int elapsedTime)
{
    for (int i = 0; i < GHOST_COUNT; i++)
    {
        GhostEnemy* currentGhost = _ghosts[i];

        const Vector2 lastPosition = Vector2{
            currentGhost->position->X,
            currentGhost->position->Y
        };

        currentGhost->Update(elapsedTime);

        if (CheckWallCollisions(_ghosts[i]->position) ||
            !CheckBoxCollision(
                currentGhost->position->X, currentGhost->position->X + currentGhost->sourceRect->Width,
                currentGhost->position->Y, currentGhost->position->Y + currentGhost->sourceRect->Height,
                0 + currentGhost->sourceRect->Width, SCREEN_WIDTH - currentGhost->sourceRect->Width,
                0 + currentGhost->sourceRect->Height, SCREEN_HEIGHT - currentGhost->sourceRect->Height
            ))
        {
            currentGhost->position->X = lastPosition.X;
            currentGhost->position->Y = lastPosition.Y;
            currentGhost->ChangeDirection(elapsedTime);
        }

        if (CheckBoxCollision(
            _player->position->X, _player->position->X + _player->sourceRect->Width,
            _player->position->Y, _player->position->Y + _player->sourceRect->Height,
            currentGhost->position->X, currentGhost->position->X + currentGhost->sourceRect->Width,
            currentGhost->position->Y, currentGhost->position->Y + currentGhost->sourceRect->Height))
        {
            _collisionCount += 100; // todo death
        }
    }
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
