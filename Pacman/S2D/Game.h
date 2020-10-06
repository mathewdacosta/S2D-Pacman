// file:	Game.h
//
// summary:	Declares the game class
//
// author: Paul Boocock, Staffordshire University

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#else
#define S2D_API __declspec(dllimport)
#endif

#include "S2D.h"

namespace S2D
{
	/// <summary>
	/// Abstract Class for a Game. A game should implement the methods below which will be
	/// automatically called by the Graphics Class once initialised.
	/// </summary>
	class S2D_API Game
	{
	public:

		/// <summary>	Game Constructor. </summary>
		///
		/// <param name="argc">	Application Argument Count. </param>
		/// <param name="argv">	[in,out] Application Arguments. </param>
		Game(int argc, char* argv[]);

		/// <summary>	Game Destructor. </summary>
		virtual ~Game();

		/// <summary>	Virtual Method - Must be implemented by Game. </summary>
		/// <para>Should be used to load the content for the scene</para>
		virtual void LoadContent() = 0;

		/// <summary>
		/// Virtual Method - Must be implemented by Game
		/// <para>Should be used to update the scene</para>
		/// </summary>
		///
		/// <param name="elapsedTime">	Time (ms) of last frame. </param>
		virtual void Update(int elapsedTime) = 0;

		/// <summary>
		/// Virtual Method - Must be implemented by Game
		/// <para>Should be used to draw the scene</para>
		/// </summary>
		///
		/// <param name="elapsedTime">	Time (ms) of last frame. </param>
		virtual void Draw(int elapsedTime) = 0;
	};
}