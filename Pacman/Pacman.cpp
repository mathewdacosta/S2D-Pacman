#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanSprintMultiplier(2.1f), _cPacmanSprintDuration(2000), _cPacmanSprintCooldown(5000), _cPacmanFrameTime(200), _cMunchieFrameTime(450)
{
    _started = false;
    _paused = false;
    _pKeyDown = false;
    _sprintKeyDown = false;

    //Initialise important Game aspects
    Graphics::Initialise(argc, argv, this, SCREEN_WIDTH, SCREEN_HEIGHT, false, 25, 25, "Logo here: The Game", 60);
    Input::Initialise();

    // Start the Game Loop - This calls Update and Draw in game loop
    Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
    delete _menuBackground;
    delete _menuTextTexture;
    delete _pacmanTexture;
    delete _pacmanSourceRect;
    delete _munchieTexture;
    delete _munchieRect;
}

void Pacman::LoadContent()
{
    // Set up menu
    _menuBackground = new Texture2D();
    _menuBackground->Load("Textures/Transparency.png", false);
    _menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

    _menuTextTexture = new Texture2D();
    _menuTextTexture->Load("Textures/MenuText.png", false);
    _menuPausedSourceRect = new Rect(0.0f, 0.0f, 200, 40);
    _menuPausedDestRect = new Rect(
        (Graphics::GetViewportWidth() - _menuPausedSourceRect->Width) / 2.0f,
        (Graphics::GetViewportHeight() - _menuPausedSourceRect->Height) / 2.0f,
        _menuPausedSourceRect->Width,
        _menuPausedSourceRect->Height);
    _menuLogoSourceRect = new Rect(0.0f, 40.0f, 215, 47);
    _menuLogoDestRect = new Rect(
        (Graphics::GetViewportWidth() - _menuLogoSourceRect->Width) / 2.0f,
        (Graphics::GetViewportHeight() / 3.0f) - (_menuLogoSourceRect->Height / 2.0f),
        _menuLogoSourceRect->Width,
        _menuLogoSourceRect->Height);
    _menuHelpPosition = new Vector2(10, 740);
    
    // Load Pacman
    _pacmanTexture = new Texture2D();
    _pacmanTexture->Load("Textures/Pacman.tga", false);
    _pacmanPosition = new Vector2(350.0f, 350.0f);
    _pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);
    _pacmanAnimCurrentTime = 0;
    _pacmanAnimFrame = 0;
    _pacmanDirection = MoveDirection::Right;
    _pacmanSprintTime = 0;
    _pacmanSprintCooldown = 0;

    // Load Munchie
    _munchieTexture = new Texture2D();
    _munchieTexture->Load("Textures/Cherry.png", true);
    _munchieRect = new Rect(100.0f, 450.0f, 32, 32);
    _munchieSourceRect = new Rect(0, 0, 32, 32);
    _munchieAnimCurrentTime = 0;
    _munchieAnimFrame = 0;

    // Set string position
    _stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
    // Gets the current state of the keyboard
    Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

    /* ====== START ====== */
    if (!_started && keyboardState->IsKeyDown(Input::Keys::SPACE))
    {
        _started = true;
    }

    if (!_started) return;

    /* ====== PAUSE ====== */
    if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
    {
        _paused = !_paused;
        _pKeyDown = true;
    }

    if (keyboardState->IsKeyUp(Input::Keys::P))
    {
        _pKeyDown = false;
    }

    if (_paused) return; 
    
    /* ====== INPUT ====== */

    if (keyboardState->IsKeyDown(Input::Keys::D)) // Checks if D key is pressed
    {
        _pacmanDirection = MoveDirection::Right;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::A)) // Checks if A key is pressed
    {
        _pacmanDirection = MoveDirection::Left;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::S)) // Checks if S key is pressed
    {
        _pacmanDirection = MoveDirection::Down;
    }
    else if (keyboardState->IsKeyDown(Input::Keys::W)) // Checks if W key is pressed
    {
        _pacmanDirection = MoveDirection::Up;
    }

    // Sprint
    if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT) && !_sprintKeyDown && _pacmanSprintCooldown <= 0)
    {
        _pacmanSprintTime = _cPacmanSprintDuration;
        _pacmanSprintCooldown = _cPacmanSprintCooldown;
        _sprintKeyDown = true;
    }

    if (keyboardState->IsKeyUp(Input::Keys::LEFTSHIFT))
    {
        _sprintKeyDown = false;
    }
    
    /* ====== MOVEMENT ====== */

    // Handle sprint modifier
    // TODO: turn this into a meter that depletes on hold only + recharges
    float movementAmount = _cPacmanSpeed * elapsedTime;
    if (_pacmanSprintTime > 0)
    {
        movementAmount *= _cPacmanSprintMultiplier;
        _pacmanSprintTime -= elapsedTime;
    }
    else if (_pacmanSprintCooldown > 0)
    {
        _pacmanSprintCooldown -= elapsedTime;
    }
    
    switch (_pacmanDirection)
    {
        case MoveDirection::Right:
            _pacmanPosition->X += movementAmount; // Moves Pacman +x
            break;
        case MoveDirection::Left:
            _pacmanPosition->X -= movementAmount; // Moves Pacman -x
            break;
        case MoveDirection::Down:
            _pacmanPosition->Y += movementAmount; // Moves Pacman +y
            break;
        case MoveDirection::Up:
            _pacmanPosition->Y -= movementAmount; // Moves Pacman -y
            break;
    }
    
    /* ====== WALL COLLISION ====== */
    DoWallWrap();

    /* ====== ANIMATION UPDATES ====== */

    // Increment Pacman animation frame
    _pacmanAnimCurrentTime += elapsedTime;
    if (_pacmanAnimCurrentTime > _cPacmanFrameTime)
    {
        _pacmanAnimCurrentTime = 0;
        _pacmanAnimFrame = (_pacmanAnimFrame + 1) % CHARACTER_FRAMES;
        _pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanAnimFrame;
    }

    // Increment munchie animation frame
    _munchieAnimCurrentTime += elapsedTime;
    if (_munchieAnimCurrentTime > _cMunchieFrameTime)
    {
        _munchieAnimCurrentTime = 0;
        _munchieAnimFrame = (_munchieAnimFrame + 1) % MUNCHIE_FRAMES;  
        _munchieSourceRect->X = _munchieSourceRect->Width * _munchieAnimFrame;   
    }

    // Update Pacman direction
    _pacmanSourceRect->Y = _pacmanSourceRect->Height * static_cast<int>(_pacmanDirection);
}

void Pacman::Draw(int elapsedTime)
{

    SpriteBatch::BeginDraw(); // Starts Drawing

    if (_started)
    {
        // Allows us to easily create a string
        std::stringstream stream;
        stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;
        
        SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman
        SpriteBatch::Draw(_munchieTexture, _munchieRect, _munchieSourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White,
                              SpriteEffect::NONE); // Draws munchie

        // Draws String
        SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

        // Draw pause menu
        if (_paused)
        {
            SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
            SpriteBatch::Draw(_menuTextTexture, _menuPausedDestRect, _menuPausedSourceRect);
        }
    }
    else
    {
        SpriteBatch::Draw(_menuTextTexture, _menuLogoDestRect, _menuLogoSourceRect);
        stringstream help;
        help << "Press SPACE to start, WASD to change direction, P to pause";
        SpriteBatch::DrawString(help.str().c_str(), _menuHelpPosition, Color::Green);
    }
    
    SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::DoWallWrap()
{
	float centreX = _pacmanPosition->X + (_pacmanSourceRect->Width / 2);
	float centreY = _pacmanPosition->Y + (_pacmanSourceRect->Height / 2);
	
	if (centreX > Graphics::GetViewportWidth()) // if centre X coord > screen width
	{
		_pacmanPosition->X -= Graphics::GetViewportWidth();
	}

	if (centreX < 0) // if centre X coord < 0
	{
		_pacmanPosition->X += Graphics::GetViewportWidth();
	}
	
    if (centreY > Graphics::GetViewportHeight()) // if centre Y coord > screen height
	{
        _pacmanPosition->Y -= Graphics::GetViewportHeight();
	}

	if (centreY < 0) // if centre Y coord < 0
	{
		_pacmanPosition->Y += Graphics::GetViewportHeight();
	}
}
