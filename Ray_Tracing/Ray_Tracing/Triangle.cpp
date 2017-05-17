#include"Triangle.h"

Triangle::Triangle(vector<Vec3f> &vs, vector<Vec3f> &vns, vector<Vec2f> &vts, bool s)
{
	// the sizes of vs, vns, vts are not 3
	if (vs.size() != 3 || vns.size() != 3 || vts.size() != 3)
	{
		cout << "Wrong Format.(Triangle class)" << endl;
		return;
	}

	// Initlize the three vertex of triangle
	// Loop three input vertex
	for (int i = 0; i < 3; ++i)
	{
		setV(vs[i], i);
		setVn(vns[i], i);
		setVt(vts[i], i);

		// Compare with vertex to find out max and min Position of the triangle
		bbox.modify(vs[i]);
	}

	// Calculate face normal
	normal = (v[1] - v[0]).crossProduct(v[2] - v[0]);
	normal.normalize();

	smooth = s;
}

bool Triangle::getNormal(Vec3f &hitNormal)
{
	if (smooth)
	{
		cout << "This triangle should be angular(Triangle Class)" << endl;
		return false;
	}

	hitNormal = normal;
	return true;

	/*hitNormal.x = normal.x;
	hitNormal.y = normal.y;
	hitNormal.z = normal.z;*/
}

bool Triangle::getProperties(const Vec2f &uv, Vec3f &hitNormal, Vec2f &hitTextureCoordinates)
{
	if (!smooth)
	{
		cout << "This triangle should be smooth(Triangle Class)" << endl;
		return false;
	}

	// face normal, linear interpoly
	hitNormal = (1 - uv.x - uv.y) * vn[0] + uv.x * vn[1] + uv.y * vn[2];
	// texture coordinates
	hitTextureCoordinates = (1 - uv.x - uv.y) * vt[0] + uv.x * vt[1] + uv.y * vt[2];

	return true;
}

bool Triangle::intersect(const Vec3f &orig, const Vec3f &dir,
	float &t, Vec2f &uv)
{
	// 1. Use bounding box to make a brief judgement about whether intersect or not
	if (!bbox.intersect(orig, dir))
		return false;

	// 2. Then calculate the possible intersect position
	Vec3f v0v1 = v[1] - v[0];
	Vec3f v0v2 = v[2] - v[0];
	Vec3f pvec = dir.crossProduct(v0v2);
	float det = v0v1.dotProduct(pvec);

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < kEpsilon) return false;

	float invDet = 1 / det;

	Vec3f tvec = orig - v[0];
	float u = tvec.dotProduct(pvec) * invDet;
	if (u < 0 || u > 1) return false;

	Vec3f qvec = tvec.crossProduct(v0v1);
	float v = dir.dotProduct(qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	uv.x = u;
	uv.y = v;

	t = v0v2.dotProduct(qvec) * invDet;
}