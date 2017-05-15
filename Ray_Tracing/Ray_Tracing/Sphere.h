#pragma once
#define M_PI 3.1415926

#include "Object.h"

class Sphere : public Object
{
public:
	float radius;
	float radius2; // radius^2
	Vec3f center;

	Sphere(const Vec3f &c, const float &r) : radius(r), radius2(r*r), center(c) {}
	// Ray-sphere intersection 
	//	orig: the ray origin
	//	dir:  the ray direction
	//	t(out): the distance from the ray origin to the intersection point
	bool intersect(const Vec3f &orig, const Vec3f &dir, float &t) const
	{
		float t0, t1; // solutions for t if the ray intersects

		// geometric solution
		Vec3f L = center - orig;
		float tca = L.dotProduct(dir);
		if (tca < 0) return false;
		float d2 = L.dotProduct(L) - tca * tca;
		if (d2 > radius2) return false;
		float thc = sqrt(radius2 - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		if (t0 > t1) swap(t0, t1);

		if (t0 < 0) 
		{
			t0 = t1; // if t0 is negative, let's use t1 instead
			if (t0 < 0) return false; // both t0 and t1 are negative
		}
		t = t0;
		return true;
	}
	
	// Set surface data such as normal and texture coordinates at a given point on the surface
	//	hitPoint: the point ont the surface we want to get data on
	//	hitPointNormal: the normal at hit point
	//	texture(out):the texture coordinates at hit point
	void getSurfaceData(const Vec3f &hitPoint,
						Vec3f &hitPointNormal) const
	{
		// In this particular case, the normal is simular to a point on a unit sphere
		// centred around the origin. We can thus use the normal coordinates to compute
		// the spherical coordinates of hitPoint.
		hitPointNormal = hitPoint - center;
		hitPointNormal.normalize();
	}
};