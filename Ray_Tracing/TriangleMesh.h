#pragma once
#include "MultiObjects.h"
#include "Triangle.h"

class TriangleMesh: public MultiObjects
{
private:
	Vec3f color=1.0;
	vector<Triangle> triangleList;
	Material *material;
public:	
	TriangleMesh():color(Vec3f(1.0)) { }
	TriangleMesh(Material *m) :color(Vec3f(1.0))
	{ 
		material = m; 
	}

	bool intersect(const Vec3f &orig, const Vec3f &dir, 
					float &t, int &triIndex, Vec2f &uv, Object *&o, bool ShadowRay = false);

	void getSurfaceProperties(const Vec3f &hitPoint, const Vec3f &viewDirection,
								const int &triIndex, const Vec2f &uv,
								Vec3f &hitNormal, Vec2f &hitTextureCoordinates,
								Vec3f &Color, Material *&m);
	
	void addTriangle(Triangle &tri) 
	{ 
		triangleList.push_back(tri); 

		// Test whether to change the parameter of bounding box
		aabb.modify(tri.bbox.bounds[0]);
		aabb.modify(tri.bbox.bounds[1]);
	}

	void setMaterial(Material *m)
	{
		material = m;
	}
};
