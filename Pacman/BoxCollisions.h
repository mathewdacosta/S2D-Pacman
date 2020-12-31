#pragma once

#include "S2D/S2D.h"
using namespace S2D;

// Assorted functions for checking box collisions
namespace BoxCollisions
{
	/// <summary> Check whether two rectangles overlap, given the X and Y coordinates of their sides </summary>
	inline bool CheckBoxCollision(float left1, float right1, float top1, float bottom1, float left2, float right2, float top2, float bottom2)
	{
		if (left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2)
			return false;

		return true;
	}

	/// <summary> Check whether a Rect overlaps with another rectangle of the given coordinates </summary>
	inline bool CheckBoxCollision(Rect* rect1, float left2, float right2, float top2, float bottom2)
	{
		return CheckBoxCollision(
            rect1->X, rect1->X + rect1->Width, rect1->Y, rect1->Y + rect1->Height,
            left2, right2, top2, bottom2);	
	}

	/// <summary> Check whether two Rects overlap </summary>
	inline bool CheckBoxCollision(Rect* rect1, Rect* rect2)
	{
		return CheckBoxCollision(
            rect1->X, rect1->X + rect1->Width, rect1->Y, rect1->Y + rect1->Height,
            rect2->X, rect2->X + rect2->Width, rect2->Y, rect2->Y + rect2->Height);
	}
}
