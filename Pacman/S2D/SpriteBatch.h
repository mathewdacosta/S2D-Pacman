 // file:	SpriteBatch.h
//
// summary:	Declares the Sprite Batch
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

namespace S2D
{
	//Forward declarations
	class Color;
	class Vector2;
	class Texture2D;
	class Rect;

	/// <summary>	Used when drawing sprites to flip the image. </summary>
	enum class S2D_API SpriteEffect
	{
		NONE = 0,
		FLIPVERTICAL,
		FLIPHORIZONTAL,
		FLIPBOTH
	};

	/// <summary>	Handles drawing of 2D Sprites and Strings. </summary>
	namespace SpriteBatch
	{
		/// <summary>	Begins the drawing procedure. </summary>
		S2D_API void BeginDraw();

		/// <summary>	Ends the drawing procedure. </summary>
		S2D_API void EndDraw();

		/// <summary>	Draws a rectangle. </summary>
		///
		/// <param name="x">	 	The x coordinate. </param>
		/// <param name="y">	 	The y coordinate. </param>
		/// <param name="width"> 	The width. </param>
		/// <param name="height">	The height. </param>
		/// <param name="color"> 	The color. </param>
		S2D_API void DrawRectangle(float x, float y, int width, int height, const Color* color);

		/// <summary>	Draws a rectangle. </summary>
		///
		/// <param name="position">	The position. </param>
		/// <param name="width">   	The width. </param>
		/// <param name="height">  	The height. </param>
		/// <param name="color">   	The color. </param>
		S2D_API void DrawRectangle(const Vector2* position, int width, int height, const Color* color);

		/// <summary>	Draws a rectangle. </summary>
		///
		/// <param name="destRectangle">	Destination rectangle. </param>
		/// <param name="color">			The color. </param>
		S2D_API void DrawRectangle(const Rect* destRectangle, const Color* color);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture"> 	The texture. </param>
		/// <param name="position">	The position. </param>
		S2D_API void Draw(const Texture2D* texture, const Vector2* position);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture">		  	The texture. </param>
		/// <param name="position">		  	The position. </param>
		/// <param name="sourceRectangle">	Source rectangle. </param>
		S2D_API void Draw(const Texture2D* texture, const Vector2* position, const Rect* sourceRectangle);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture"> 	The texture. </param>
		/// <param name="position">	The position. </param>
		/// <param name="scale">   	The scale. </param>
		/// <param name="rotation">	The rotation. </param>
		S2D_API void Draw(const Texture2D* texture, const Vector2* position, float scale, float rotation);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture">		  	The texture. </param>
		/// <param name="position">		  	The position. </param>
		/// <param name="sourceRectangle">	Source rectangle. </param>
		/// <param name="origin">		  	The origin. </param>
		/// <param name="scale">		  	The scale. </param>
		/// <param name="rotation">		  	The rotation. </param>
		/// <param name="color">		  	The color. </param>
		/// <param name="spriteEffect">   	The sprite effect. </param>
		S2D_API void Draw(const Texture2D* texture, const Vector2* position, const Rect* sourceRectangle, const Vector2* origin, float scale, float rotation, const Color* color, SpriteEffect spriteEffect);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture">			The texture. </param>
		/// <param name="destRectangle">	Destination rectangle. </param>
		S2D_API void Draw(const Texture2D* texture, const Rect* destRectangle);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture">		  	The texture. </param>
		/// <param name="destRectangle">  	Destination rectangle. </param>
		/// <param name="sourceRectangle">	Source rectangle. </param>
		S2D_API void Draw(const Texture2D* texture, const Rect* destRectangle, const Rect* sourceRectangle);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture">			The texture. </param>
		/// <param name="destRectangle">	Destination rectangle. </param>
		/// <param name="scale">			The scale. </param>
		/// <param name="rotation">			The rotation. </param>
		S2D_API void Draw(const Texture2D* texture, const Rect* destRectangle, float scale, float rotation);

		/// <summary>	Draws a sprite at a given position. </summary>
		///
		/// <param name="texture">		  	The texture. </param>
		/// <param name="destRectangle">  	Destination rectangle. </param>
		/// <param name="sourceRectangle">	Source rectangle. </param>
		/// <param name="origin">		  	The origin. </param>
		/// <param name="scale">		  	The scale. </param>
		/// <param name="rotation">		  	The rotation. </param>
		/// <param name="color">		  	The color. </param>
		/// <param name="spriteEffect">   	The sprite effect. </param>
		S2D_API void Draw(const Texture2D* texture, const Rect* destRectangle, const Rect* sourceRectangle, const Vector2* origin, float scale, float rotation, const Color* color, SpriteEffect spriteEffect);

		/// <summary>	Draws a string at a given position. </summary>
		///
		/// <param name="text">	   	The text. </param>
		/// <param name="position">	The position. </param>
		/// <param name="color">   	The color. </param>
		S2D_API void DrawString(const char* text, const Vector2* position, const Color* color);

	}
}