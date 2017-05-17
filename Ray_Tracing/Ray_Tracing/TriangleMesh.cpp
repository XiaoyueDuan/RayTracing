#include"TriangleMesh.h"

bool TriangleMesh::intersect(const Vec3f &orig, const Vec3f &dir,
	float &tNear, int triIndex, Vec2f &uv)
{
	// 1. Use bounding box to judge roughly whether intersect first
	//if (!aabb.intersect(orig, dir))
	//	return false;

	// 2. The find out whether intersect and the intersect position
	bool isect = false;
	float t = kInfinity;
	Vec2f UV;
	for (int i = 0; i < triangleList.size(); ++i)
	{
		if (triangleList[i].intersect(orig, dir, t, UV) && t < tNear)
		{
			tNear = t;
			// comment next two lines with UV being subsituted by uv!!!!!!!!!!!!!!!!!!!!! 
			uv.x = UV.x;
			uv.y = UV.y;
			triIndex = i;
			isect = true;
		}
	}
	return isect;
}

void TriangleMesh::getSurfaceProperties(const Vec3f &hitPoint, const Vec3f &viewDirection,
	const int &triIndex, const Vec2f &uv,
	Vec3f &hitNormal, Vec2f &hitTextureCoordinates)
{
	if (triangleList[triIndex].getSmooth())
		triangleList[triIndex].getProperties(uv, hitNormal, hitTextureCoordinates);
	else
		triangleList[triIndex].getNormal(hitNormal);
}