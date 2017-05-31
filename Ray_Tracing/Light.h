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
		//random_device rd;  //Will be used to obtain a seed for the random number engine
		//mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()			
		////default_random_engine generator;
		//uniform_real_distribution<float> distribution(0.0, 1.0);
		//float r1 = 2*distribution(gen)-1;
		//float r2 = distribution(gen);

		//float sinTheta = sqrtf(1 - r1 * r1);
		//float phi = 2 * M_PI * r2;
		//float x = sinTheta * cosf(phi);
		//float z = sinTheta * sinf(phi);
		//Vec3f a(x, r1, z);
		//a *= radius;
		//a += centerPosition;
		//lightDir = (P - a);
		//distance = sqrt(lightDir.norm());
		//lightDir.normalize(); 
		//lightIntensity = color * intensity;

		lightDir = (P - centerPosition);
		float r2 = lightDir.norm();
		distance = sqrt(r2)-radius;
		lightDir.normalize();		
		// avoid division by 0
		lightIntensity = color * intensity;
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
		random_device rd;  //Will be used to obtain a seed for the random number engine
		mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()			
						   default_random_engine generator;
		uniform_real_distribution<float> distribution(0.0, 1.0);

		float r1 = distribution(gen);
		float r2 = distribution(gen);
		float r3 = distribution(gen);
		return Vec3f(v1.x + r1*length, 
					v1.y+r2*height, 
					v1.z + r3*width);
	}

	// P: is the shaded point
	void illuminate(const Vec3f &P, Vec3f &lightDir, Vec3f &lightIntensity, float &distance)
	{
		Vec3f lightPos = randomSelectPoint();
		lightDir = (P - lightPos);
		float r2 = lightDir.norm();
		distance = sqrt(r2);
		lightDir.normalize();			
		// avoid division by 0
		lightIntensity = color * intensity;
	}
};