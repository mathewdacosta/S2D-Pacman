#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif


// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;


// Screen width and height
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Number of Pacman animation frames
#define CHARACTER_FRAMES 2

// Possible movement directions
enum class MoveDirection { Up, Right, Down, Left };

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Data for menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	bool _started;
	bool _spaceKeyDown;
	bool _paused;
	bool _pKeyDown;

	Texture2D* _menuTextTexture;
	Rect* _menuPausedSourceRect;
	Rect* _menuPausedDestRect;
	Rect* _menuLogoSourceRect;
	Rect* _menuLogoDestRect;
	Vector2* _menuHelpPosition;

	// Data to represent Pacman
	const float _cPacmanSpeed; // movement speed

	Vector2* _pacmanPosition; // position on screen
	Rect* _pacmanSourceRect; // current location in tilemap
	Texture2D* _pacmanTexture; // tilemap
	int _pacmanAnimFrame; // current frame in animation
	MoveDirection _pacmanDirection; // current movement direction

	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;

	// Position for String
	Vector2* _stringPosition;

	void virtual DoWallWrap();

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};