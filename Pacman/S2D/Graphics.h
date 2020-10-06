// file:	Graphics.h
//
// summary:	Declares the Graphics
//
// author: Paul Boocock, Staffordshire University
//
// remarks: The reason this is in namespaces, rather than a static class pattern: http://stackoverflow.com/questions/9321/how-do-you-create-a-static-class-in-c/112451#112451

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#else
#define S2D_API __declspec(dllimport)
#endif

#include "S2D.h"
#include "Game.h"

namespace S2D
{
	//Forward declaration of Game class
	//This enables the Graphics Class to know about the Game class before it is compiled
	class Game;
	class Color;

	/// <summary> Handles all the required Graphical set up including the Game Loop </summary>
	namespace Graphics
	{
		/// <summary>
		/// Initialises Graphics Device and Creates a Window with the Default Title ("S2D Engine") and with
		/// no Preferred Frames per Second (FPS)
		/// </summary>
		///
		/// <param name="argc">				Application Argument Count. </param>
		/// <param name="argv">				[in,out] Application Arguments. </param>
		/// <param name="game">				[in,out] The Game Abstract Class. </param>
		/// <param name="viewportWidth"> 	The Desired Width of the Window. </param>
		/// <param name="viewportHeight">	The Desired Height of the Window. </param>
		/// <param name="fullScreen">		Sets application as full screen. </param>
		/// <param name="windowX"> 			The starting X position of the Window. </param>
		/// <param name="windowY">			The starting Y position of the Window. </param>
		S2D_API void Initialise(int argc, char* argv[], Game* game, int viewportWidth, int viewportHeight, bool fullScreen, int windowX, int windowY);

		/// <summary>
		/// Initialises Graphics Device and Creates a Window with a Custom Title and with no Preferred Frames
		/// per Second (FPS)
		/// </summary>
		///
		/// <param name="argc">				Application Argument Count. </param>
		/// <param name="argv">				[in,out] Application Arguments. </param>
		/// <param name="game">				[in,out] The Game Abstract Class. </param>
		/// <param name="viewportWidth"> 	The Desired Width of the Window. </param>
		/// <param name="viewportHeight">	The Desired Height of the Window. </param>
		/// <param name="fullScreen">		Sets application as full screen. </param>
		/// <param name="windowX"> 			The starting X position of the Window. </param>
		/// <param name="windowY">			The starting Y position of the Window. </param>
		/// <param name="windowTitle">   	The Title for the Window. </param>
		S2D_API void Initialise(int argc, char* argv[], Game* game, int viewportWidth, int viewportHeight, bool fullScreen, int windowX, int windowY, char* windowTitle);

		/// <summary>
		/// Initialises Graphics Device and Creates a Window with a Preferred Frames per Second (FPS) and with
		/// the Default Title ("Open2D Engine")
		/// </summary>
		///
		/// <param name="argc">				Application Argument Count. </param>
		/// <param name="argv">				[in,out] Application Arguments. </param>
		/// <param name="game">				[in,out] The Game Abstract Class. </param>
		/// <param name="viewportWidth"> 	The Desired Width of the Window. </param>
		/// <param name="viewportHeight">	The Desired Height of the Window. </param>
		/// <param name="fullScreen">		Sets application as full screen. </param>
		/// <param name="windowX"> 			The starting X position of the Window. </param>
		/// <param name="windowY">			The starting Y position of the Window. </param>
		/// <param name="preferredFPS">  	Sets the preferred FPS and attempts to run the game at this rate. </param>
		S2D_API void Initialise(int argc, char* argv[], Game* game, int viewportWidth, int viewportHeight, bool fullScreen, int windowX, int windowY, int preferredFPS);

		/// <summary>
		/// Initialises Graphics Device and Creates a Window with a Custom Title and with a Preferred Frames per
		/// Second (FPS)
		/// </summary>
		///
		/// <param name="argc">				Application Argument Count. </param>
		/// <param name="argv">				[in,out] Application Arguments. </param>
		/// <param name="game">				[in,out] The Game Abstract Class. </param>
		/// <param name="viewportWidth"> 	The Desired Width of the Window. </param>
		/// <param name="viewportHeight">	The Desired Height of the Window. </param>
		/// <param name="fullScreen">		Sets application as full screen. </param>
		/// <param name="windowX"> 			The starting X position of the Window. </param>
		/// <param name="windowY">			The starting Y position of the Window. </param>
		/// <param name="windowTitle">   	The Title for the Window. </param>
		/// <param name="preferredFPS">  	Sets the preferred FPS and attempts to run the game at this rate. </param>
		S2D_API void Initialise(int argc, char* argv[], Game* game, int viewportWidth, int viewportHeight, bool fullScreen, int windowX, int windowY, char* windowTitle, int preferredFPS);


		/// <summary>	Cleans up the Graphics Manager.  </summary>
		S2D_API void Destroy();

		/// <summary>	Begins the game loop. Ensure all initialise is done before calling this. </summary>
		S2D_API void StartGameLoop();

		/// <summary>	Returns the Viewport Width. </summary>
		///
		/// <returns>	The viewport width. </returns>
		S2D_API int GetViewportWidth();

		/// <summary>	Returns the Viewport Height. </summary>
		///
		/// <returns>	The viewport height. </returns>
		S2D_API int GetViewportHeight();

		/// <summary>	Returns is the Graphics are refreshing at a Fixed Time step. </summary>
		///
		/// <returns>	true if fixed time step, false if not. </returns>
		S2D_API bool IsFixedTimeStep();

		/// <summary>	Returns the Preferred FPS. </summary>
		///
		/// <returns>	The preferred FPS. </returns>
		S2D_API int GetPreferredFPS();

		/// <summary>	Sets the current clear color </summary>
		///
		///  <param name="color"> The clear color </param>
		S2D_API void SetClearColor(const Color* color);

		/// <summary>	Sets the current clear color </summary>
		///
		/// <param name="r"> The red component of the clear color </param>
		/// <param name="g"> The green component of clear color </param>
		/// <param name="b"> The blue component of clear color </param>
		 S2D_API void SetClearColor(float r, float g, float b);
	}
}