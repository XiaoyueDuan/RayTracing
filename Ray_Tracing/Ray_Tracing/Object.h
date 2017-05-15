#pragma once

#include "Geometry.h"
#include "MaterialDataStructure.h"
#include "AABB.h"

using namespace std;

class Object
{
public:
	Vec3f color;
	Material material;
	AABBox aabb;

	Object() {}

	// Method to compute the intersection of the object with a ray
	// Returns true if an intersection was found, false otherwise
	// See method implementation in children class for details
	virtual bool intersect(const Vec3f &orig, const Vec3f &dir, float &t,	
							int triIndex, Vec2f &uv) const = 0;

	// Method to compute the surface data such as normal and texture coordnates at the intersection point.
	// See method implementation in children class for details
	virtual void getSurfaceProperties(const Vec3f &hitPoint, const Vec3f &viewDirection,
										const int &triIndex, const Vec2f &uv,
										Vec3f &hitNormal, Vec2f &hitTextureCoordinates) const = 0;
};

