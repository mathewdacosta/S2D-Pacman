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
    _munchies = new Munchie[MUNCHIE_COUNT];
    _cherries = new Cherry[CHERRY_COUNT];
    _walls = new Wall[WALL_COUNT];
    _ghosts = new GhostEnemy[GHOST_COUNT];

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
    delete[] _munchies;
    delete[] _cherries;
    delete[] _walls;
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
    _menu->helpPosition = new Vector2(10, Graphics::GetViewportHeight() - 28);

    // Load Pacman
    _player->LoadTexture();

    // Load and set textures
    Texture2D* munchieTexture = new Texture2D();
    munchieTexture->Load("Textures/Munchie.png", false);
    
    Texture2D* cherryTexture = new Texture2D();
    munchieTexture->Load("Textures/Cherry.png", false);
    
    Texture2D* wallTexture = new Texture2D();
    wallTexture->Load("Textures/WallTemp.png", false);
    
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i].SetTexture(munchieTexture);
    }

    for (int i = 0; i < CHERRY_COUNT; i++)
    {
        _cherries[i].SetTexture(cherryTexture);
    }

    for (int i = 0; i < WALL_COUNT; i++)
    {
        _walls[i].SetTexture(wallTexture);
    }

    // Load enemy
    Texture2D* ghostTexture = new Texture2D();
    ghostTexture->Load("Textures/Ghosts.png", false);
    for (int i = 0; i < GHOST_COUNT; i++)
    {
        _ghosts[i].texture = ghostTexture;
        _ghosts[i].type = static_cast<GhostType>(i % 4);
        _ghosts[i].direction = static_cast<MoveDirection>(rand() % 4);
        _ghosts[i].sourceRect = new Rect(0, 0, 20, 20);
        _ghosts[i].position = new Vector2(rand() % (SCREEN_WIDTH - 20), rand() % (SCREEN_HEIGHT - 20));
        _ghosts[i].speed = 0.04f + (static_cast<int>(_ghosts[i].type) * 0.012f);
        _ghosts[i].decisionTime = 500 + (static_cast<int>(_ghosts[i].type) * 300);
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

    _player->Update(elapsedTime);
    
    for (int i = 0; i < MUNCHIE_COUNT; i++)
    {
        _munchies[i].Update(elapsedTime);
    }
    
    for (int i = 0; i < CHERRY_COUNT; i++)
    {
        _cherries[i].Update(elapsedTime);
    }
    
    for (int i = 0; i < WALL_COUNT; i++)
    {
        _walls[i].Update(elapsedTime);
    }

    UpdateGhosts(elapsedTime);
}

void Pacman::Draw(int elapsedTime)
{
    SpriteBatch::BeginDraw(); // Starts Drawing

    if (_menu->started)
    {
        // Draws player info string
        std::stringstream stream;
        stream << "Collisions: " << _collisionCount << "\nX: " << _player->GetPosition()->X << "\nY: " << _player->GetPosition()->Y;
        SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

        _player->Draw();

        // Draw munchies, cherries, walls and ghosts
        for (int i = 0; i < MUNCHIE_COUNT; i++)
            _munchies[i].Draw();

        for (int i = 0; i < CHERRY_COUNT; i++)
            _cherries[i].Draw();

        for (int i = 0; i < WALL_COUNT; i++)
            _walls[i].Draw();

        for (int i = 0; i < GHOST_COUNT; i++)
            _ghosts[i].Draw();

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
    Vector2* playerPosition = _player->GetPosition();
    _collisionCount = 0;
    for (int i = 0; i < MUNCHIE_COUNT; ++i)
    {
        if (BoxCollisions::CheckBoxCollision(_munchies[i].GetBoundingBox(),
                                             playerPosition->X, playerPosition->X + _player->GetWidth(),
                                             playerPosition->Y, playerPosition->Y + _player->GetHeight()))
        {
            _collisionCount++;
        }
    }
}

bool Pacman::CheckWallCollisions(int x, int y, int width, int height)
{
    for (int i = 0; i < WALL_COUNT; ++i)
    {
        if (BoxCollisions::CheckBoxCollision(_walls[i].GetBoundingBox(), x, x + width, y, y + height))
        {
            return true;
        }
    }

    return false;
}

void Pacman::UpdateGhosts(int elapsedTime)
{
    for (int i = 0; i < GHOST_COUNT; i++)
    {
        GhostEnemy currentGhost = _ghosts[i];
        const float ghostX = currentGhost.position->X;
        const float ghostY = currentGhost.position->Y;
        const int ghostWidth = currentGhost.sourceRect->Width;
        const int ghostHeight = currentGhost.sourceRect->Height;

        const Vector2 lastPosition = Vector2{
            ghostX,
            ghostY
        };

        currentGhost.Update(elapsedTime);

        if (CheckWallCollisions(ghostX, ghostY, ghostX + ghostWidth, ghostY + ghostHeight) ||
            !BoxCollisions::CheckBoxCollision(
                ghostX, ghostX + ghostWidth,
                ghostY, ghostY + ghostHeight,
                0 + ghostWidth, SCREEN_WIDTH - ghostWidth,
                0 + ghostHeight, SCREEN_HEIGHT - ghostHeight
            ))
        {
            currentGhost.position->X = lastPosition.X;
            currentGhost.position->Y = lastPosition.Y;
            currentGhost.ChangeDirection(elapsedTime);
        }

        if (BoxCollisions::CheckBoxCollision(
            _player->GetPosition()->X, _player->GetPosition()->X + _player->GetWidth(),
            _player->GetPosition()->Y, _player->GetPosition()->Y + _player->GetHeight(),
            ghostX, ghostX + ghostWidth,
            ghostY, ghostY + ghostHeight))
        {
            _collisionCount += 100;
            _player->SetDead(true);
        }
    }
}

