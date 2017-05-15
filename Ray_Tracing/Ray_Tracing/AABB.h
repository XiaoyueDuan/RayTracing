#pragma once

#include "Geometry.h"
#include<cmath>

class AABBox
{
public:
	// bounds[0] is the minimal vertex(minX, minY, minZ)
	// bounds[1] is the maximum vertex(maxX, maxY, maxZ)
	Vec3f bounds[2];

	AABBox(){}
	AABBox(const Vec3f &b0, const Vec3f &b1) { bounds[0] = b0, bounds[1] = b1; }

	bool intersect(const Vec3f &orig, const Vec3f &dir) const
	{
		Vec3f invdir = 1 / dir;
		int sign[3];
		sign[0] = (invdir.x < 0);
		sign[1] = (invdir.y < 0);
		sign[2] = (invdir.z < 0);

		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (bounds[sign[0]].x - orig.x) * invdir.x;
		tmax = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
		tymin = (bounds[sign[1]].y - orig.y) * invdir.y;
		tymax = (bounds[1 - sign[1]].y - orig.y) * invdir.y;

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (bounds[sign[2]].z - orig.z) * invdir.z;
		tzmax = (bounds[1 - sign[2]].z - orig.z) * invdir.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		float t = tmin;

		if (t < 0) 
		{
			t = tmax;
			if (t < 0) return false;
		}
		return true;
	}	
};