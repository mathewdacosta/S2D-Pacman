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
#define CHARACTER_FRAMES 8

// Number of munchie animation frames
#define MUNCHIE_FRAMES 2

/// <summary> Possible movement directions - the ordinals match the position of the Pacman sprites on the spritesheet </summary>
enum class MoveDirection { Right = 0, Down, Left, Up };

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
	bool _sprintKeyDown;

	Texture2D* _menuTextTexture;
	Rect* _menuPausedSourceRect;
	Rect* _menuPausedDestRect;
	Rect* _menuLogoSourceRect;
	Rect* _menuLogoDestRect;
	Vector2* _menuHelpPosition;

	// Data to represent Pacman
	const float _cPacmanSpeed; // movement speed
	const float _cPacmanSprintMultiplier; // speed multiplier while sprinting
	const int _cPacmanSprintDuration; // length of sprint
	const int _cPacmanSprintCooldown; // length of cooldown between sprints
	const int _cPacmanFrameTime; // interval between animation frames

	Vector2* _pacmanPosition; // position on screen
	Rect* _pacmanSourceRect; // current location in tilemap
	Texture2D* _pacmanTexture; // tilemap
	int _pacmanAnimCurrentTime; // current time since last update
	int _pacmanAnimFrame; // current frame in animation
	MoveDirection _pacmanDirection; // current movement direction
	int _pacmanSprintTime; // time left on current sprint
	int _pacmanSprintCooldown; // time left on current sprint

	// Data to represent Munchie
	const int _cMunchieFrameTime; // interval between animation frames	
	
	Rect* _munchieRect;
	Rect* _munchieSourceRect;
	Texture2D* _munchieTexture;
	int _munchieAnimCurrentTime; // current time since last update
	int _munchieAnimFrame; // current frame in animation

	// Position for String
	Vector2* _stringPosition;

	/// <summary> Perform movement inputs </summary>
	void Input(Input::KeyboardState* keyboardState);

	/// <summary> Check the pause menu input </summary>
	void CheckPaused(Input::KeyboardState* keyboardState, Input::Keys pauseKey);
	
	/// <summary> Checks whether Pacman has collided with the wall. </summary>
	void CheckViewportCollision();

	/// <summary> Update position of Pacman </summary>
	void UpdatePacmanMovement(int elapsedTime);
	
	/// <summary> Update animation of Pacman </summary>
	void UpdatePacmanFrame(int elapsedTime);
	
	/// <summary> Update animation of munchies </summary>
	void UpdateMunchieFrame(int elapsedTime);

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