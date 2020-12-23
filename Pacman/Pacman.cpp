#include "Pacman.h"
#include "BoxCollisions.h"
#include "Munchie.h"
#include "Cherry.h"
#include "Player.h"
#include "GhostEnemy.h"

#include <sstream>
#include <time.h>

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
    _collisionCount = 0;

    for (int i = 0; i < MUNCHIE_COUNT; i++)
        _munchies[i] = new Munchie();

    for (int i = 0; i < CHERRY_COUNT; i++)
        _cherries[i] = new Cherry();

    for (int i = 0; i < WALL_COUNT; i++)
        _walls[i] = new Wall();

    for (int i = 0; i < GHOST_COUNT; i++)
    {
        GhostType type = static_cast<GhostType>(i % 4);
        MoveDirection direction = static_cast<MoveDirection>(rand() % 4);
        int x = rand() % (SCREEN_WIDTH - 20);
        int y = rand() % (SCREEN_HEIGHT - 20);
        _ghosts[i] = new GhostEnemy(type, x, y, direction);
    }

    SetRandomEntityPositions();

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

    delete _player;
    for (int i = 0; i < MUNCHIE_COUNT; i++)
        delete _munchies[i];
    delete[] _munchies;

    for (int i = 0; i < CHERRY_COUNT; i++)
        delete _cherries[i];
    delete[] _cherries;

    for (int i = 0; i < WALL_COUNT; i++)
        delete _walls[i];
    delete[] _walls;

    for (int i = 0; i < GHOST_COUNT; i++)
        delete _ghosts[i];
    delete[] _ghosts;

    delete _playerTexture;
    delete _munchieTexture;
    delete _cherryTexture;
    delete _wallTexture;
    delete _ghostTexture;
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
    _menu->helpPosition = new Vector2(10, Graphics::GetViewportHeight() - 80);

    // Load Pacman
    _player->LoadTexture();

    // Load and set textures
    Texture2D* munchieTexture = new Texture2D();
    munchieTexture->Load("Textures/Munchie.png", false);

    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i]->SetTexture(munchieTexture);
    }

    Texture2D* cherryTexture = new Texture2D();
    cherryTexture->Load("Textures/Cherry.png", false);

    for (int i = 0; i < CHERRY_COUNT; i++)
    {
        _cherries[i]->SetTexture(cherryTexture);
    }

    Texture2D* wallTexture = new Texture2D();
    wallTexture->Load("Textures/WallTemp.png", false);

    for (int i = 0; i < WALL_COUNT; i++)
    {
        _walls[i]->SetTexture(wallTexture);
    }

    // Load enemy
    Texture2D* ghostTexture = new Texture2D();
    ghostTexture->Load("Textures/Ghosts.png", false);
    for (int i = 0; i < GHOST_COUNT; i++)
    {
        _ghosts[i]->SetTexture(ghostTexture);
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

    if (!_player->IsDead())
    {
        // Handle movement inputs
        _player->HandleMovementInput(keyboardState);

        // Store last position in case of collision
        const Vector2 lastPosition = Vector2{
            *_player->GetPosition()
        };

        // Update player position
        _player->Update(elapsedTime);

        // Undo movement in case of wall collision 
        if (CheckWallCollisions(_player->GetPosition()->X, _player->GetPosition()->Y, _player->GetWidth(),
                                _player->GetHeight()))
        {
            _player->SetPosition(lastPosition.X, lastPosition.Y);
        }

        CheckPlayerCollisions(elapsedTime);
    }

    // Update all munchies, cherries and walls
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i]->Update(elapsedTime);
    }

    for (int i = 0; i < CHERRY_COUNT; i++)
    {
        _cherries[i]->Update(elapsedTime);
    }

    for (int i = 0; i < WALL_COUNT; i++)
    {
        _walls[i]->Update(elapsedTime);
    }

    // Update ghosts
    UpdateGhosts(elapsedTime);
}

void Pacman::Draw(int elapsedTime)
{
    SpriteBatch::BeginDraw(); // Starts Drawing

    if (_menu->started)
    {
        // Draws player info string
        std::stringstream stream;
        stream << "Score: " << _collisionCount << "\nX: " << _player->GetPosition()->X << "\nY: " << _player->
            GetPosition()->Y;
        SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

        _player->Draw();

        // Draw munchies, cherries, walls and ghosts
        for (int i = 0; i < MUNCHIE_COUNT; i++)
            _munchies[i]->Draw();

        for (int i = 0; i < CHERRY_COUNT; i++)
            _cherries[i]->Draw();

        for (int i = 0; i < WALL_COUNT; i++)
            _walls[i]->Draw();

        for (int i = 0; i < GHOST_COUNT; i++)
            _ghosts[i]->Draw();

        

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
        help << "Press SPACE to start,\nWASD to change direction\nP to pause";
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


void Pacman::CheckFoodCollisions()
{
    Vector2* playerPosition = _player->GetPosition();
    for (int i = 0; i < MUNCHIE_COUNT; ++i)
    {
        if (BoxCollisions::CheckBoxCollision(_munchies[i]->GetBoundingBox(),
                                             playerPosition->X, playerPosition->X + _player->GetWidth(),
                                             playerPosition->Y, playerPosition->Y + _player->GetHeight()))
        {
            _collisionCount += 10;
            _munchies[i]->SetScreenPosition(-100, -100);
        }
    }

    for (int i = 0; i < CHERRY_COUNT; ++i)
    {
        if (BoxCollisions::CheckBoxCollision(_cherries[i]->GetBoundingBox(),
                                             playerPosition->X, playerPosition->X + _player->GetWidth(),
                                             playerPosition->Y, playerPosition->Y + _player->GetHeight()))
        {
            _collisionCount += 100;
            _cherries[i]->SetScreenPosition(-100, -100);
        }
    }
}

bool Pacman::CheckWallCollisions(int x, int y, int width, int height)
{
    for (int i = 0; i < WALL_COUNT; ++i)
    {
        if (BoxCollisions::CheckBoxCollision(_walls[i]->GetBoundingBox(), x, x + width, y, y + height))
        {
            return true;
        }
    }

    return false;
}

void Pacman::SetRandomEntityPositions()
{
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        int x = rand() % (SCREEN_WIDTH - _munchies[i]->GetBoundingBox()->Width);
        int y = rand() % (SCREEN_HEIGHT - _munchies[i]->GetBoundingBox()->Height);
        _munchies[i]->SetScreenPosition(x, y);
    }

    for (int i = 0; i < CHERRY_COUNT; i++)
    {
        int x = rand() % (SCREEN_WIDTH - _cherries[i]->GetBoundingBox()->Width);
        int y = rand() % (SCREEN_HEIGHT - _cherries[i]->GetBoundingBox()->Height);
        _cherries[i]->SetScreenPosition(x, y);
    }

    for (int i = 0; i < WALL_COUNT; i++)
    {
        int x = rand() % (SCREEN_WIDTH - _walls[i]->GetBoundingBox()->Width);
        int y = rand() % (SCREEN_HEIGHT - _walls[i]->GetBoundingBox()->Height);
        _walls[i]->SetScreenPosition(x, y);
    }
}

void Pacman::UpdateGhosts(int elapsedTime)
{
    for (int i = 0; i < GHOST_COUNT; i++)
    {
        GhostEnemy* currentGhost = _ghosts[i];

        const Vector2 lastPosition = Vector2{
            currentGhost->GetPositionX(),
            currentGhost->GetPositionY()
        };

        currentGhost->Update(elapsedTime);
        
        const float ghostX = currentGhost->GetPositionX();
        const float ghostY = currentGhost->GetPositionY();
        const int ghostWidth = currentGhost->GetWidth();
        const int ghostHeight = currentGhost->GetHeight();

        if (CheckWallCollisions(ghostX, ghostY, ghostWidth, ghostHeight) ||
            !BoxCollisions::CheckBoxCollision(
                ghostX, ghostX + ghostWidth,
                ghostY, ghostY + ghostHeight,
                0 + ghostWidth, SCREEN_WIDTH - ghostWidth,
                0 + ghostHeight, SCREEN_HEIGHT - ghostHeight
            ))
        {
            currentGhost->SetPosition(lastPosition.X, lastPosition.Y);
            currentGhost->ChangeDirection(elapsedTime);
        }
    }
}

void Pacman::CheckPlayerCollisions(int elapsedTime)
{
    CheckFoodCollisions();

    for (int i = 0; i < GHOST_COUNT; i++)
    {
        if (BoxCollisions::CheckBoxCollision(
            _player->GetPosition()->X, _player->GetPosition()->X + _player->GetWidth(),
            _player->GetPosition()->Y, _player->GetPosition()->Y + _player->GetHeight(),
            _ghosts[i]->GetPositionX(), _ghosts[i]->GetPositionX() + _ghosts[i]->GetWidth(),
            _ghosts[i]->GetPositionY(), _ghosts[i]->GetPositionY() + _ghosts[i]->GetHeight()))
        {
            _player->SetDead(true);
        }
    }
}
