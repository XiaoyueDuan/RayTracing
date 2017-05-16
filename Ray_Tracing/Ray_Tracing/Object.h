#pragma once

#include "Geometry.h"
#include "MaterialDataStructure.h"
#include "AABB.h"

using namespace std;

class Object
{
protected:
	Vec3f color;
	AABBox aabb;

public:
	Object() {}

	// Method to compute the intersection of the object with a ray
	// Returns true if an intersection was found, false otherwise
	// See method implementation in children class for details
	virtual bool intersect(const Vec3f &orig, const Vec3f &dir, float &t,
		int Index, Vec2f &uv) const
	{
		if (!aabb.intersect(orig, dir))
			return false;

		return true;
	}

	// Method to compute the surface data such as normal and texture coordnates at the intersection point.
	// See method implementation in children class for details
	virtual void getSurfaceProperties(const Vec3f &hitPoint, const Vec3f &viewDirection,
										const int &triIndex, const Vec2f &uv,
										Vec3f &hitNormal, Vec2f &hitTextureCoordinates) const = 0;

	virtual void add(Object &o)
	{
		aabb.modify(o.aabb.bounds[0]);
		aabb.modify(o.aabb.bounds[1]);
	}
};
