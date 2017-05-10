#pragma once

#include "Geometry.h"

using namespace std;

class Object
{
public:
	Vec3f color;

	Object() {}

	// Method to compute the intersection of the object with a ray
	// Returns true if an intersection was found, false otherwise
	// See method implementation in children class for details
	virtual bool intersect(const Vec3f &orig, const Vec3f &dir, float &t) const = 0;

	// Method to compute the surface data such as normal and texture coordnates at the intersection point.
	// See method implementation in children class for details
	virtual void getSurfaceData(const Vec3f &hitPoint, 
								Vec3f &hitPointNormal) const = 0;
};