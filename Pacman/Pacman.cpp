#include "Pacman.h"

#include <iomanip>

#include "BoxCollisions.h"
#include "Munchie.h"
#include "Cherry.h"
#include "Player.h"
#include "GhostEnemy.h"

#include <sstream>
#include <time.h>

#include "MenuState.h"

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
    srand(time(nullptr));

    _menu = {
        GameState::MAIN_MENU,
        false,
        false,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
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

    // Randomise positions of game objects
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
    Audio::Destroy();
    Input::Destroy();
    
    delete _menu.overlayBackground;
    delete _menu.overlayRect;
    delete _menu.textTexture;
    delete _menu.pausedSourceRect;
    delete _menu.pausedDestRect;

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

    delete _munchieTexture;
    delete _cherryTexture;
    delete _wallTexture;
    delete _ghostTexture;

    delete _gameScorePosition;
    delete _gameOverScorePosition;
}

void Pacman::LoadContent()
{
    // Set up menus
    _menu.gameOverBackground = new Texture2D();
    _menu.gameOverBackground->Load("Textures/GameOver.png", false);
    
    _menu.mainMenuBackground = new Texture2D();
    _menu.mainMenuBackground->Load("Textures/Menu.png", false);

    _menu.overlayBackground = new Texture2D();
    _menu.overlayBackground->Load("Textures/Transparency.png", false);
    _menu.overlayRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

    _menu.textTexture = new Texture2D();
    _menu.textTexture->Load("Textures/MenuText.png", false);

    _menu.pausedSourceRect = new Rect(0.0f, 0.0f, 200, 40);
    _menu.pausedDestRect = new Rect(
        (Graphics::GetViewportWidth() - _menu.pausedSourceRect->Width) / 2.0f,
        (Graphics::GetViewportHeight() - _menu.pausedSourceRect->Height) / 2.0f,
        _menu.pausedSourceRect->Width,
        _menu.pausedSourceRect->Height);

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
    _gameScorePosition = new Vector2(10.0f, 25.0f);
    _gameOverScorePosition = new Vector2(GAME_OVER_SCORE_X, GAME_OVER_SCORE_Y);
}

void Pacman::Update(int elapsedTime)
{
    // Gets the current state of the keyboard
    Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

    // Check for quit but only if not playing
    if (_menu.state != GameState::PLAYING)
    {
        CheckQuit(keyboardState);
    }

    // Check for start game input
    if (_menu.state == GameState::MAIN_MENU)
    {
        CheckStart(keyboardState, Input::Keys::SPACE);
        return;
    }

    // Check for pause input/toggle
    CheckPaused(keyboardState, Input::Keys::P);

    // Don't run game loop when paused/dead
    if (_menu.state == GameState::PAUSED || _menu.state == GameState::GAME_OVER)
        return;

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
        _player->SetPosition(lastPosition.X, lastPosition.Y);

    CheckPlayerCollisions(elapsedTime);

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

    if (_player->IsDeathAnimationComplete())
    {
        _menu.state = GameState::GAME_OVER;
    }
}

void Pacman::Draw(int elapsedTime)
{
    SpriteBatch::BeginDraw(); // Starts Drawing

    if (_menu.state == GameState::PLAYING || _menu.state == GameState::PAUSED)
    {
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

        // Draws player info string
        std::stringstream stream;
        stream << "Score: " << _collisionCount << "\nX: " << _player->GetPosition()->X << "\nY: " << _player->GetPosition()->Y;
        SpriteBatch::DrawString(stream.str().c_str(), _gameScorePosition, Color::Green);

        // Draw pause menu
        if (_menu.state == GameState::PAUSED)
        {
            SpriteBatch::Draw(_menu.overlayBackground, _menu.overlayRect, nullptr);
            SpriteBatch::Draw(_menu.textTexture, _menu.pausedDestRect, _menu.pausedSourceRect);
        }
    }
    else if (_menu.state == GameState::MAIN_MENU)
    {
        SpriteBatch::Draw(_menu.mainMenuBackground, _menu.overlayRect);
    }
    else if (_menu.state == GameState::GAME_OVER)
    {
        std::stringstream stream;
        stream << std::setw(5) << std::setfill('0') << _collisionCount;
        SpriteBatch::Draw(_menu.gameOverBackground, _menu.overlayRect);
        SpriteBatch::DrawString(stream.str().c_str(), _gameOverScorePosition, Color::White);
    }

    SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::CheckStart(Input::KeyboardState* keyboardState, Input::Keys startKey)
{
    if (keyboardState->IsKeyDown(startKey))
        _menu.state = GameState::PLAYING;
}

void Pacman::CheckPaused(Input::KeyboardState* keyboardState, Input::Keys pauseKey)
{
    if (keyboardState->IsKeyDown(pauseKey) && !_menu.pKeyDown)
    {
        if (_menu.state == GameState::PLAYING)
            _menu.state = GameState::PAUSED;
        else if (_menu.state == GameState::PAUSED)
            _menu.state = GameState::PLAYING;

        // debounce P key
        _menu.pKeyDown = true;
    }

    if (keyboardState->IsKeyUp(pauseKey))
    {
        _menu.pKeyDown = false;
    }
}

void Pacman::CheckQuit(Input::KeyboardState* keyboardState)
{
    if (keyboardState->IsKeyDown(Input::Keys::Q))
    {
        Graphics::Destroy();
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
            _player->Kill();
        }
    }
}
