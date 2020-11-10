#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif


// Include S2D and use namespace
#include "S2D/S2D.h"
using namespace S2D;

// Include our types
#include "MenuState.h"
#include "Player.h";
#include "Food.h"

// Screen width and height
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Number of animation frames for player and munchie
#define PLAYER_FRAMES 8
#define MUNCHIE_FRAMES 2

// Number of munchies to spawn
#define MUNCHIE_COUNT 50

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Data for start and pause menus
	MenuState* _menu;

	// Data to represent Pacman
	Player* _player;
	bool _sprintKeyDown;

	// Data to represent Munchie
	Food* _munchies[MUNCHIE_COUNT];
	
	// Position for debug string
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
	void UpdateMunchieFrame(Food* munchie, int elapsedTime);

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