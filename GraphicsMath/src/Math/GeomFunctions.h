#pragma once

#include "Types.h"
#include "Operators.h"
#include "Functions.h"

namespace GM
{
	float IsPointInPlane(const Vector& normal, const Vector& center, const Vector& point)
	{
		// D = negative distance from the plane to Origin Plane
		//float D = -Vec3Magnitude(center - Vec3ProjectionOfV0OntoV1(P, perpendicularPlane, 0)));
		float D = Vec3Dot(-normal, center);
		return Vec3Dot(normal, point) + D;
	}
}