#pragma once
#include "Geometry.h"
#include "Constant.h"

class Light
{
public:
	Vec3f color;
	float intensity;
	Vec3f centerPosition;

	Light(const Vec3f& centerPos, const Vec3f &c = 1, const float &i = 1) :centerPosition(centerPos), color(c), intensity(i) {}
	virtual void illuminate(const Vec3f &P, Vec3f &lightDir, Vec3f &lightIntensity, float &distance){}	
};

class SphereLight : public Light
{
	float radius;
public:
	SphereLight(const Vec3f& centerPos, const Vec3f &c = 1, const float &i = 1,
		const float r=1.0f) : Light(centerPos, c, i) ,radius(r){}
	// P: is the shaded point
	void illuminate(const Vec3f &P, Vec3f &lightDir, Vec3f &lightIntensity, float &distance)
	{
		lightDir = (P - centerPosition);
		float r2 = lightDir.norm();
		distance = sqrt(r2)-radius;
		lightDir.normalize();		
		// avoid division by 0
		lightIntensity = color * intensity / (4 * M_PI * r2);
	}
};

class AreaLight : public Light
{
	// v1 and v2 are diagonal
	Vec3f v1;
	Vec3f v2;
	float length,width, height;
public:
	AreaLight(const Vec3f& centerPos, const Vec3f &c, const float &i,
		Vec3f &v1, Vec3f &v2) : Light(centerPos, c, i) 
	{
		this->v1 = v1;
		this->v2 = v2;
		length = v2.x - v1.x;
		height = v2.y - v1.y;
		width= v2.z - v1.z;
	}

	Vec3f randomSelectPoint()
	{
		default_random_engine generator;
		uniform_real_distribution<float> distribution(0.0, 1.0);

		float r1 = distribution(generator);
		float r2 = distribution(generator);
		float r3 = distribution(generator);
		return Vec3f(v1.x + r1*length, 
					v1.y+r2*height, 
					v1.z + r3*width);
	}

	// P: is the shaded point
	void illuminate(const Vec3f &P, Vec3f &lightDir, Vec3f &lightIntensity, float &distance)
	{
		lightDir = (P - randomSelectPoint());
		lightDir.normalize();
		float r2 = lightDir.norm();
		distance = sqrt(r2);		
		// avoid division by 0
		lightIntensity = color * intensity;
	}
};