// file:	Input.h
//
// summary:	Declares the Input Methods
//
// author: Paul Boocock, Staffordshire University
//
// remarks: The reason this is in namespaces, rather than a static class pattern: http://stackoverflow.com/questions/9321/how-do-you-create-a-static-class-in-c/112451#112451

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#define S2D_TEMPLATE
#else
#define S2D_API __declspec(dllimport)
#define S2D_TEMPLATE extern
#endif

#include "S2D.h"

#include <vector>
#include <array>

namespace S2D
{
	/// <summary>	Contains various classes and structures for dealing with Input. </summary>
	namespace Input
	{
		/// <summary>	Initialises Input. </summary>
		void S2D_API Initialise();

		/// <summary>	Destroys Input. </summary>
		void S2D_API Destroy();

		/// <summary>	Determines whether a button is released or pressed. </summary>
		enum class S2D_API ButtonState
		{
			RELEASED = 0,
			PRESSED
		};

		/// <summary>	Determines whether a key is released or pressed. </summary>
		enum class S2D_API KeyState
		{
			RELEASED = 0,
			PRESSED
		};

		/// <summary>	Represents all the support keys. </summary>
		enum class S2D_API Keys
		{
			A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			UP, DOWN, LEFT, RIGHT, BACK,  HOME, END, PAGEUP, PAGEDOWN, 
			INSERT, DELETEKEY, RETURN, BACKSPACE, TAB, ESCAPE,
			SPACE, PLUS, SUBTRACT, MULTIPLY, DIVIDE, DECIMAL, EQUALS,
			LEFTCONTROL, LEFTALT, LEFTSHIFT, RIGHTCONTROL, RIGHTALT, RIGHTSHIFT, 
			NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
			COUNT
		};

		/// <summary>	Represents the current state of the mouse. </summary>
		class S2D_API MouseState
		{
		public:
			/// <summary>	The left button. </summary>
			ButtonState LeftButton;
			/// <summary>	The middle button. </summary>
			ButtonState MiddleButton;
			/// <summary>	The right button. </summary>
			ButtonState RightButton;
			/// <summary>	The scroll wheel value. </summary>
			int ScrollWheelValue;
			/// <summary>	The X coordinate. </summary>
			int X;
			/// <summary>	The Y coordinate. </summary>
			int Y;

			/// <summary>	Default constructor. </summary>
			MouseState();
		};

		/// <summary>	Stores the Keys. </summary>
		class S2D_API KeyArray
		{
		private:
			std::array<KeyState, static_cast< std::size_t >(Keys::COUNT)> Items;

		public:
			/// <summary>	Default constructor. </summary>
			KeyArray();

			/// <summary>	Array indexer operator. </summary>
			///
			/// <param name="key">	The key. </param>
			///
			/// <returns>	The indexed value. </returns>
			KeyState& operator[] (Keys key);

			/// <summary>	Array indexer operator. </summary>
			///
			/// <param name="key">	The key. </param>
			///
			/// <returns>	The indexed value. </returns>
			KeyState operator[] (Keys key) const;
		};

		S2D_TEMPLATE template class S2D_API std::vector<Keys>;

		/// <summary>	Represents the current state of the Keyboard. </summary>
		class S2D_API KeyboardState
		{
		public:
			/// <summary>	The items. </summary>
			KeyArray Items;

			/// <summary>	Default constructor. </summary>
			KeyboardState();

			/// <summary>	Gets pressed keys. </summary>
			///
			/// <returns>	null if it fails, else the pressed keys. </returns>
			std::vector<Keys>& GetPressedKeys();

			/// <summary>	Query if 'key' is down. </summary>
			///
			/// <param name="key">	The key. </param>
			///
			/// <returns>	true if key down, false if not. </returns>
			bool IsKeyDown(Keys key);

			/// <summary>	Query if 'key' is  up. </summary>
			///
			/// <param name="key">	The key. </param>
			///
			/// <returns>	true if key up, false if not. </returns>
			bool IsKeyUp(Keys key);

		private:
			std::vector<Keys> PressedKeys;
		};

		/// <summary>	Returns the Mouse State. </summary>
		namespace Mouse
		{
			/// <summary>	Gets the state. </summary>
			///
			/// <returns>	null if it fails, else the state. </returns>
			S2D_API MouseState* GetState();

			/// <summary>	Sets the mouse position. </summary>
			///
			/// <param name="x">	The x coordinate. </param>
			/// <param name="y">	The y coordinate. </param>
			S2D_API void SetPosition(int x, int y);
		}

		/// <summary>	Returns the Keyboard State. </summary>
		namespace Keyboard
		{
			/// <summary>	Gets the state. </summary>
			///
			/// <returns>	null if it fails, else the state. </returns>
			S2D_API KeyboardState* GetState();
		}
	}
}