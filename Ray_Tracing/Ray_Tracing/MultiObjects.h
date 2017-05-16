#pragma once
#include "Object.h"

class MultiObjects:public virtual Object
{
	// A derived class from Object. In the MultiObjects class, there will be 
	// an attribute which is a vector<Object> array
private:
	vector<Object> list;

public:
	virtual void add(Object &o)
	{
		Object::add(o);
		list.push_back(o);
	}

	virtual bool intersect(const Vec3f &orig, const Vec3f &dir,
		float &tNear, int index, Vec2f &uv) const
	{
		// 1. Use bounding box to judge roughly whether intersect first
		Object::intersect(orig, dir, tNear, index, uv);

		// 2. The find out whether intersect and the intersect position
		bool isect = false;
		float t = kInfinity;
		Vec2f UV;
		for (int i = 0; i < list.size(); ++i)
		{
			int j;
			if (list[i].intersect(orig, dir, t, j, UV) && t < tNear)
			{
				tNear = t;
				// comment next two lines with UV being subsituted by uv!!!!!!!!!!!!!!!!!!!!! 
				uv.x = UV.x;
				uv.y = UV.y;
				index = i;
				isect = true;
			}
		}
		return isect;
	}

	virtual void getSurfaceProperties(const Vec3f &hitPoint, const Vec3f &viewDirection,
		const int &index, const Vec2f &uv,
		Vec3f &hitNormal, Vec2f &hitTextureCoordinates) const
	{
		list[index].getSurfaceProperties(hitPoint, viewDirection, index, uv,
											hitNormal, hitTextureCoordinates);
	}
};