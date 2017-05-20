#pragma once

#include "Object.h"

class Sphere : public Object
{
public:
	float radius;
	float radius2; // radius^2
	Vec3f center;
	Vec3f color;
	Material *material;

	Sphere(const Vec3f &c, const float &r) : radius(r), radius2(r*r), center(c),
											 color(Vec3f(1.0)) 
	{
		aabb.bounds[0] = c-r;
		aabb.bounds[1] = c+r;
	}

	// Ray-sphere intersection 
	//	orig: the ray origin
	//	dir:  the ray direction
	//	t(out): the distance from the ray origin to the intersection point
	bool intersect(const Vec3f &orig, const Vec3f &dir, float &tNear, int Index, Vec2f &uv, Object *&o)
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

		// if the ray intersect with the sphere, 
		// return true only if the t<tNear and t>kEpsilon(to avoid self-intersection)
		if (t0 < tNear && t0>kEpsilon)
		{
			tNear = t0;
			o = this;
			//o= * (dynamic_cast<Object *> (this));			
			return true;
		}
		return false;
	}
	
	// Set surface data such as normal and texture coordinates at a given point on the surface
	//	hitPoint: the point ont the surface we want to get data on
	//  viewDirection: ray direction
	//	hitNormal: the normal at hit point
	//	texture(out):the texture coordinates at hit point
	//	Color(out): the color of the sphere
	//	m(out): the Material pointer of the sphere
	void getSurfaceProperties(const Vec3f &hitPoint, const Vec3f &viewDirection,
		const int &triIndex, const Vec2f &uv,
		Vec3f &hitNormal, Vec2f &hitTextureCoordinates,
		Vec3f &Color, Material *&m)
	{
		// In this particular case, the normal is simular to a point on a unit sphere
		// centred around the origin. We can thus use the normal coordinates to compute
		// the spherical coordinates of hitPoint.
		hitNormal = hitPoint - center;
		hitNormal.normalize();

		// atan2 returns a value in the range [-pi, pi] and we need to remap it to range [0, 1]
		// acosf returns a value in the range [0, pi] and we also need to remap it to the range [0, 1]
		hitTextureCoordinates.x = (1 + atan2(hitNormal.z, hitNormal.x) / M_PI) * 0.5;
		hitTextureCoordinates.y = acosf(hitNormal.y) / M_PI;

		Color = color;
		m = material;
	}
};