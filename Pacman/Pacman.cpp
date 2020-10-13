#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
    _frameCount = 0;

    //Initialise important Game aspects
    Graphics::Initialise(argc, argv, this, SCREEN_WIDTH, SCREEN_HEIGHT, false, 25, 25, "Pacman", 60);
    Input::Initialise();

    // Start the Game Loop - This calls Update and Draw in game loop
    Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
    delete _pacmanTexture;
    delete _pacmanSourceRect;
    delete _munchieBlueTexture;
    delete _munchieInvertedTexture;
    delete _munchieRect;
}

void Pacman::LoadContent()
{
    // Load Pacman
    _pacmanTexture = new Texture2D();
    _pacmanTexture->Load("Textures/Pacman.tga", false);
    _pacmanPosition = new Vector2(350.0f, 350.0f);
    _pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);
    _pacmanAnimFrame = 0;

    // Load Munchie
    _munchieBlueTexture = new Texture2D();
    _munchieBlueTexture->Load("Textures/Munchie.tga", true);
    _munchieInvertedTexture = new Texture2D();
    _munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
    _munchieRect = new Rect(100.0f, 450.0f, 12, 12);

    // Set string position
    _stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
    /* ====== INPUT ====== */

    // Gets the current state of the keyboard
    Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

    if (keyboardState->IsKeyDown(Input::Keys::D)) // Checks if D key is pressed
    {
        _pacmanPosition->X += 0.1f * elapsedTime; // Moves Pacman +x
        _pacmanSourceRect->Y = 0;
    }

    if (keyboardState->IsKeyDown(Input::Keys::A)) // Checks if A key is pressed
    {
        _pacmanPosition->X -= 0.1f * elapsedTime; // Moves Pacman -x
        _pacmanSourceRect->Y = 64;
    }
    
    if (keyboardState->IsKeyDown(Input::Keys::S)) // Checks if S key is pressed
    {
        _pacmanPosition->Y += 0.1f * elapsedTime; // Moves Pacman +y
        _pacmanSourceRect->Y = 32;
    }
    
    if (keyboardState->IsKeyDown(Input::Keys::W)) // Checks if W key is pressed
    {
        _pacmanPosition->Y -= 0.1f * elapsedTime; // Moves Pacman -y
        _pacmanSourceRect->Y = 96;
    }
    
    /* ====== WALL COLLISION ====== */
    DoWallWrap();
}

void Pacman::Draw(int elapsedTime)
{
    // Allows us to easily create a string
    std::stringstream stream;
    stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

    SpriteBatch::BeginDraw(); // Starts Drawing

    // Increment animation frame every 12 frames
    // TODO: stop sharing this with munchie
    if (_frameCount % 12 == 0)
    {
        _pacmanAnimFrame = (_pacmanAnimFrame + 1) % CHARACTER_FRAMES;
        _pacmanSourceRect->X = _pacmanAnimFrame * 32;        
    }
    SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

    if (_frameCount < 30)
    {
        // Draws Red Munchie
        SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White,
                          SpriteEffect::NONE);

        _frameCount++;
    }
    else
    {
        // Draw Blue Munchie
        SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White,
                          SpriteEffect::NONE);

        _frameCount++;

        if (_frameCount >= 60)
            _frameCount = 0;
    }

    // Draws String
    SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
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
