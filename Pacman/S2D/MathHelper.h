// file:	MathHelper.h
//
// summary:	Declares the MathHelper Methods
//
// author: Paul Boocock, Staffordshire University

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#else
#define S2D_API __declspec(dllimport)
#endif

#include <cmath>

namespace S2D
{
	/// <summary>	Contains various mathematical methods. </summary>
	namespace MathHelper
	{
		/// <summary>	e. </summary>
		extern S2D_API const float E;
		/// <summary>	log 10 e. </summary>
		extern S2D_API const float Log10E;
		/// <summary>	log 2 e. </summary>
		extern S2D_API const float Log2E;
		/// <summary>	pi. </summary>
		extern S2D_API const float Pi;
		/// <summary>	pi over 2. </summary>
		extern S2D_API const float PiOver2;
		/// <summary>	pi over 4. </summary>
		extern S2D_API const float PiOver4;
		/// <summary>	two pi. </summary>
		extern S2D_API const float TwoPi;

		/// <summary>	Clamp the given value. </summary>
		///
		/// <param name="value">	The value. </param>
		/// <param name="min">  	The minimum. </param>
		/// <param name="max">  	The maximum. </param>
		///
		/// <returns>	A float. </returns>
		S2D_API float Clamp(float value, float min, float max);

		/// <summary>	Distances. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	A float. </returns>
		S2D_API float Distance(float value1, float value2);

		/// <summary>	Lerps. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		/// <param name="amount">	The amount. </param>
		///
		/// <returns>	A float. </returns>
		S2D_API float Lerp(float value1, float value2, float amount);

		/// <summary>	Determines the maximum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The maximum value. </returns>
		S2D_API float Max(float value1, float value2);

		/// <summary>	Determines the minimum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The minimum value. </returns>
		S2D_API float Min(float value1, float value2);

		/// <summary>	Determines the maximum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The maximum value. </returns>
		S2D_API int Max(int value1, int value2);

		/// <summary>	Determines the minimum of the given parameters. </summary>
		///
		/// <param name="value1">	The first value. </param>
		/// <param name="value2">	The second value. </param>
		///
		/// <returns>	The minimum value. </returns>
		S2D_API int Min(int value1, int value2);

		/// <summary>	Converts the radians to the degrees. </summary>
		///
		/// <param name="radians">	The radians. </param>
		///
		/// <returns>	radians as a float. </returns>
		S2D_API float ToDegrees(float radians);

		/// <summary>	Converts the degrees to the radians. </summary>
		///
		/// <param name="degrees">	The degrees. </param>
		///
		/// <returns>	degrees as a float. </returns>
		S2D_API float ToRadians(float degrees);

		/// <summary>	Wrap angle. </summary>
		///
		/// <param name="angle">	The angle. </param>
		///
		/// <returns>	A float. </returns>
		S2D_API float WrapAngle(float angle);

		/// <summary>	Rounds. </summary>
		///
		/// <param name="value">	The value. </param>
		///
		/// <returns>	A float. </returns>
		S2D_API float Round(float value);
	}
}

