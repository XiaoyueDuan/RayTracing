#pragma once

#include "Geometry.h"
#include "MaterialDataStructure.h"
#include "AABB.h"

using namespace std;

class Object
{
protected:
	AABBox aabb;

public:
	Object() {}

	// Method to compute the intersection of the object with a ray
	// Returns true if an intersection was found, false otherwise
	// See method implementation in children class for details
	virtual bool intersect(const Vec3f &orig, const Vec3f &dir, float &t,
		int Index, Vec2f &uv, Object *&o)
	{
		if (!aabb.intersect(orig, dir))
			return false;

		return true;
	}

	// Method to compute the surface data such as normal and texture coordnates at the intersection point.
	// As well as the color and material (Only TriangleMesh and Sphere will have some attributes).
	// See method implementation in children class for details
	virtual void getSurfaceProperties(const Vec3f &hitPoint, const Vec3f &viewDirection,
		const int &triIndex, const Vec2f &uv,
		Vec3f &hitNormal, Vec2f &hitTextureCoordinates,
		Vec3f &Color, Material *&m) {}

	virtual void add(Object &o)
	{
		aabb.modify(o.aabb.bounds[0]);
		aabb.modify(o.aabb.bounds[1]);
	}
};

