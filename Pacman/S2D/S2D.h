// file:	S2D.h
//
// summary:	Declares the S2D Helper Methods
//
// author: Paul Boocock, Staffordshire University

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#else
#define S2D_API __declspec(dllimport)
#pragma comment(lib, "S2D.lib")
#endif

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include "GL\freeglut.h"

#include "Audio.h"
#include "Game.h"
#include "Graphics.h"
#include "Input.h"
#include "MathHelper.h"
#include "SoundEffect.h"
#include "SpriteBatch.h"
#include "Texture2D.h"

namespace S2D
{
	/// <summary>	Represents Color. </summary>
	class S2D_API Color {
		
	public:

		/// <summary>	Red Component. </summary>
		float R;

		/// <summary>	Green Component. </summary>
		float G;

		/// <summary>	Blue Component. </summary>
		float B;

		/// <summary>	Alpha Component. </summary>
		float A;

		/// <summary>Creates a Black Color Structure (R=0, G=0, B=0, A=1)</summary>
		Color();

		/// <summary>Creates a Color Class
		/// <param name="r">Red Color Attribute</param>
		/// <param name="g">Green Color Attribute</param>
		/// <param name="b">Blue Color Attribute</param>
		/// </summary>
		Color(float r, float g, float b);

		/// <summary>Creates a Color Class
		/// <param name="r">Red Color Attribute</param>
		/// <param name="g">Green Color Attribute</param>
		/// <param name="b">Blue Color Attribute</param>
		///  <param name="a">Alpha Attribute</param>
		/// </summary>
		Color(float r, float g, float b, float a);

		/// <summary>	Assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	A shallow copy of this object. </returns>
		Color & operator= (const Color &rhs);

		/// <summary>	Equality operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	true if the parameters are considered equivalent. </returns>
		bool operator== (const Color &rhs) const;

		/// <summary>	Inequality operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	true if the parameters are not considered equivalent. </returns>
		bool operator!=(const Color &other) const;

		/// <summary>	Black. </summary>
		static const Color* Black;
		/// <summary>	White. </summary>
		static const Color* White;
		/// <summary>	Red. </summary>
		static const Color* Red;
		/// <summary>	Green. </summary>
		static const Color* Green;
		/// <summary>	Blue. </summary>
		static const Color* Blue;
		/// <summary>	Yellow. </summary>
		static const Color* Yellow;
		/// <summary>	Cyan. </summary>
		static const Color* Cyan;
		/// <summary>	Magenta. </summary>
		static const Color* Magenta;
	};
	
	/// <summary>	Represents a 2D Vector. </summary>
	class S2D_API Vector2 {

	public:
		/// <summary>	The X coordinate. </summary>
		float X;
		/// <summary>	The Y coordinate. </summary>
		float Y;

		/// <summary>Creates a Zero Vector Structure (X=0, Y=0)</summary>
		Vector2();

		/// <summary>Creates a Vector2 Structure
		/// <param name="x">X Attribute</param>
		/// <param name="y">Y Attribute</param>
		/// </summary>
		Vector2(float x, float y);

		/// <summary>	Assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	A shallow copy of this object. </returns>
		Vector2 & operator= (const Vector2 &rhs);

		/// <summary>	Equality operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	true if the parameters are considered equivalent. </returns>
		bool operator== (const Vector2 &rhs) const;

		/// <summary>	Inequality operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	true if the parameters are not considered equivalent. </returns>
		bool operator!=(const Vector2 &other) const;

		/// <summary>	Addition assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	The result of the operation. </returns>
		Vector2 & operator+=(const Vector2 &rhs);

		/// <summary>	Subtraction assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	The result of the operation. </returns>
		Vector2 & operator-=(const Vector2 &rhs);

		/// <summary>	Division assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	The result of the operation. </returns>
		Vector2 & operator/=(const Vector2 &rhs);

		/// <summary>	Multiplication assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	The result of the operation. </returns>
		Vector2 & operator*=(const Vector2 &rhs);

		/// <summary>	Division assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	The result of the operation. </returns>
		Vector2 & operator/=(const float &rhs);

		/// <summary>	Multiplication assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	The result of the operation. </returns>
		Vector2 & operator*=(const float &rhs);

		/// <summary>	Addition operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		const Vector2 operator+(const Vector2 &other) const;

		/// <summary>	Negation operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		const Vector2 operator-(const Vector2 &other) const;

		/// <summary>	Multiplication operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		const Vector2 operator*(const Vector2 &other) const;

		/// <summary>	Division operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		const Vector2 operator/(const Vector2 &other) const;

		/// <summary>	Multiplication operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		const Vector2 operator*(const float &other) const;

		/// <summary>	Division operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		const Vector2 operator/(const float &other) const;

		/// <summary>	Gets the length. </summary>
		///
		/// <returns>	The Vector Length. </returns>
		float Length() const;

		/// <summary>	Gets the length squared. </summary>
		///
		/// <returns>	The Vector length squared. </returns>
		float LengthSquared() const;

		/// <summary>	Normalizes this Vector. </summary>
		void Normalize();

		/// <summary>	Clamp the given value. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="min">   	The minimum. </param>
		/// <param name="max">   	The maximum. </param>
		///
		/// <returns>	A Vector2. </returns>
		static Vector2 Clamp(const Vector2& value1, const Vector2& min, const Vector2& max);

		/// <summary>	Clamp the given value. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="min">   	The minimum. </param>
		/// <param name="max">   	The maximum. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Clamp(const Vector2& value1, const Vector2& min, const Vector2& max, Vector2& result);

		/// <summary>	Finds the distance between two Vectors. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The distance. </returns>
		static float Distance(const Vector2& value1, const Vector2& value2);

		/// <summary>	Finds the distance between two Vectors. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Distance(const Vector2& value1, const Vector2& value2, float& result);

		/// <summary>	Finds the distance between two Vectors squared. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	A float. </returns>
		static float DistanceSquared(const Vector2& value1, const Vector2& value2);

		/// <summary>	Finds the distance between two Vectors squared. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void DistanceSquared(const Vector2& value1, const Vector2& value2, float& result);

		/// <summary>	Performs the Dot Product between two vectors. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The Dot Product. </returns>
		static float Dot(const Vector2& value1, const Vector2& value2);

		/// <summary>	Performs the Dot Product between two vectors. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Dot(const Vector2& value1, const Vector2& value2, float& result);

		/// <summary>	Linearly Interpolates between two Vectors by the given amount. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="amount">	The amount. </param>
		///
		/// <returns>	A Vector2. </returns>
		static Vector2 Lerp(const Vector2& value1, const Vector2& value2, float amount);

		/// <summary>	Linearly Interpolates between two Vectors by the given amount. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="amount">	The amount. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Lerp(const Vector2& value1, const Vector2& value2, float amount, Vector2& result);

		/// <summary>	Determines the maximum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The maximum value. </returns>
		static Vector2 Max(const Vector2& value1, const Vector2& value2);

		/// <summary>	Determines the maximum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Max(const Vector2& value1, const Vector2& value2, Vector2& result);

		/// <summary>	Determines the minimum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The minimum value. </returns>
		static Vector2 Min(const Vector2& value1, const Vector2& value2);

		/// <summary>	Determines the minimum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Min(const Vector2& value1, const Vector2& value2, Vector2& result);

		/// <summary>	Negates the given value. </summary>
		///
		/// <param name="value">	The value. </param>
		///
		/// <returns>	A Vector2. </returns>
		static Vector2 Negate(const Vector2& value);

		/// <summary>	Negates the given value. </summary>
		///
		/// <param name="value"> 	The value. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Negate(const Vector2& value, Vector2& result);

		/// <summary>	Normalizes the given value. </summary>
		///
		/// <param name="value">	The value. </param>
		///
		/// <returns>	A Vector2. </returns>
		static Vector2 Normalize(const Vector2& value);

		/// <summary>	Normalizes the given value. </summary>
		///
		/// <param name="value"> 	The value. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Normalize(const Vector2& value, Vector2& result);

		/// <summary>	Reflects. </summary>
		///
		/// <param name="vector">	The vector. </param>
		/// <param name="normal">	The normal. </param>
		///
		/// <returns>	A Vector2. </returns>
		static Vector2 Reflect(const Vector2& vector, const Vector2& normal);

		/// <summary>	Reflects. </summary>
		///
		/// <param name="vector">	The vector. </param>
		/// <param name="normal">	The normal. </param>
		/// <param name="result">	[in,out] The result. </param>
		static void Reflect(const Vector2& vector, const Vector2& normal, Vector2& result);

		/// <summary>	Zero Vector. </summary>
		static const Vector2* Zero;
		/// <summary>	One Vector. </summary>
		static const Vector2* One;
		/// <summary>	UnitX Vector. </summary>
		static const Vector2* UnitX;
		/// <summary>	UnitY Vector. </summary>
		static const Vector2* UnitY;
	};

	/// <summary>	Represents a Rectangle. </summary>
	class S2D_API Rect {
	public:
		/// <summary>	The X coordinate. </summary>
		float X;
		/// <summary>	The Y coordinate. </summary>
		float Y;
		/// <summary>	The width. </summary>
		int Width;
		/// <summary>	The height. </summary>
		int Height;

		/// <summary>Creates a Zero Sized Rect Structure (X=0, Y=0, Width=0, Height=0)</summary>
		Rect();

		/// <summary>Creates a Colour  Structure
		/// <param name="x">X Attribute</param>
		/// <param name="y">Y Attribute</param>
		/// <param name="width">Width Attribute</param>
		/// <param name="height">Height Attribute</param>
		/// </summary>
		Rect(float x, float y, int width, int height);

		/// <summary>	Destructor. </summary>
		~Rect();

		/// <summary>	Assignment operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	A shallow copy of this object. </returns>
		Rect & operator= (const Rect &rhs);

		/// <summary>	Equality operator. </summary>
		///
		/// <param name="rhs">	The right hand side. </param>
		///
		/// <returns>	true if the parameters are considered equivalent. </returns>
		bool operator== (const Rect &rhs) const;

		/// <summary>	Inequality operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	true if the parameters are not considered equivalent. </returns>
		bool operator!=(const Rect &other) const;

		/// <summary>	An Empty Rectangle. </summary>
		static const Rect* Empty;

		/// <summary>	Gets the center. </summary>
		///
		/// <returns>	A Vector2. </returns>
		Vector2 Center() const;

		/// <summary>	Query if this object is empty. </summary>
		///
		/// <returns>	true if empty, false if not. </returns>
		bool IsEmpty() const;

		/// <summary>	Gets the top. </summary>
		///
		/// <returns>	A float. </returns>
		float Top() const;

		/// <summary>	Gets the bottom. </summary>
		///
		/// <returns>	A float. </returns>
		float Bottom() const;

		/// <summary>	Gets the left. </summary>
		///
		/// <returns>	A float. </returns>
		float Left() const;

		/// <summary>	Gets the right. </summary>
		///
		/// <returns>	A float. </returns>
		float Right() const;

		/// <summary>	Query if this object intersects the given rect. </summary>
		///
		/// <param name="rect">	The rectangle. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool Intersects(const Rect& rect) const;

		/// <summary>	Query if this object intersects the given rect. </summary>
		///
		/// <param name="rect">  	The rectangle. </param>
		/// <param name="result">	[in,out] The result. </param>
		void Intersects(const Rect& rect, bool& result) const;

		/// <summary>	Query if this rectangle contains the given vector. </summary>
		///
		/// <param name="vector">	The const Vector2&amp; to test for containment. </param>
		///
		/// <returns>	true if the object is in this collection, false if not. </returns>
		bool Contains(const Vector2& vector) const;

		/// <summary>	Query if this rectangle contains the given vector. </summary>
		///
		/// <param name="vector">	The vector. </param>
		/// <param name="result">	[in,out] The result. </param>
		void Contains(const Vector2& vector, bool& result) const;
	};
}