#pragma once
#include "Geometry.h"
#include "Constant.h"
#include "AABB.h"
#include <vector>
using namespace std;

class Triangle
{
private:
	Vec3f v[3];
	Vec3f vn[3];
	Vec2f vt[4];
	Vec3f normal;	
	bool smooth=false; // wether this triangle should be smoothy or not

public:
	AABBox bbox;
	Triangle(vector<Vec3f> &vs, vector<Vec3f> &vn, vector<Vec2f> &vt, bool s);
	
	bool intersect(const Vec3f &orig, const Vec3f &dir,
					float &t, Vec2f &uv);

	bool getNormal(Vec3f &hitNormal);
	bool getProperties(const Vec2f &uv, Vec3f &hitNormal, Vec2f &hitTextureCoordinates);

	bool setV(Vec3f vp, int index)
	{
		if (index < 0 || index>3)
			return false;
		v[index].x = vp.x;
		v[index].y = vp.y;
		v[index].z = vp.z;
	}

	bool setVn(Vec3f vnp, int index)
	{
		if (index < 0 || index>3)
			return false;
		vn[index].x = vnp.x;
		vn[index].y = vnp.y;
		vn[index].z = vnp.z;
	}

	bool setVt(Vec2f vtp, int index)
	{
		if (index < 0 || index>3)
			return false;
		vt[index].x = vtp.x;
		vt[index].y = vtp.y;
	}

	bool setSmooth(bool s) { smooth = s; }

	bool getSmooth() { return smooth; }
};

