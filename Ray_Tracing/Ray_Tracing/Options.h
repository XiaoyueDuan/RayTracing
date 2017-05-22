#pragma once
#include "Geometry.h"
#include "Constant.h"

Matrix44f lookAt(const Vec3f& from, const Vec3f& to, Vec3f& tmp = Vec3f(0, 1, 0))
{
	// from-to or to-from???
	// Vec3f forward = (to-from).normalize();
	// Vec3f right = tmp.normalize().crossProduct(forward);
	// Vec3f up = right.crossProduct(forward);

	Vec3f forward = (from-to).normalize();
	Vec3f right = tmp.normalize().crossProduct(forward);
	Vec3f up = forward.crossProduct(right);

	Matrix44f camToWorld;

	camToWorld[0][0] = right.x;
	camToWorld[0][1] = right.y;
	camToWorld[0][2] = right.z;
	camToWorld[1][0] = up.x;
	camToWorld[1][1] = up.y;
	camToWorld[1][2] = up.z;
	camToWorld[2][0] = forward.x;
	camToWorld[2][1] = forward.y;
	camToWorld[2][2] = forward.z;

	camToWorld[3][0] = from.x;
	camToWorld[3][1] = from.y;
	camToWorld[3][2] = from.z;

	camToWorld[3][3] = 1;

	return camToWorld;
}

class Options
{
public:
	int width = 640;
	int height = 480;
	float fov = 90;
	Vec3f backgroundColor = kDefaultBackgroundColor;
	Matrix44f cameraToWorld;
	float bias = 0.0001;
	int depth = 2;

	void setCameraToWorldCoordinate(const Vec3f& from, const Vec3f& to, Vec3f& tmp = Vec3f(0, 1, 0))
	{
		 //from-to OR to-from???
		 Vec3f forward = (to-from).normalize();
		 Vec3f right = tmp.normalize().crossProduct(forward);
		 Vec3f up = right.crossProduct(forward);

		//Vec3f forward = (from - to).normalize();
		//Vec3f right = tmp.normalize().crossProduct(forward);
		//Vec3f up = forward.crossProduct(right);

		cameraToWorld[0][0] = right.x;
		cameraToWorld[0][1] = right.y;
		cameraToWorld[0][2] = right.z;
		cameraToWorld[1][0] = up.x;
		cameraToWorld[1][1] = up.y;
		cameraToWorld[1][2] = up.z;
		cameraToWorld[2][0] = forward.x;
		cameraToWorld[2][1] = forward.y;
		cameraToWorld[2][2] = forward.z;

		cameraToWorld[3][0] = from.x;
		cameraToWorld[3][1] = from.y;
		cameraToWorld[3][2] = from.z;

		cameraToWorld[3][3] = 1;
	}

	//void setCameraToWorldCoordinate(float eyeX, float eyeY, float eyeZ,
	//	float centerX, float centerY, float centerZ,
	//	float upX, float upY, float upZ)
	//{
	//	cameraToWorld=lookAt(Vec3f(eyeX, eyeY, eyeZ), Vec3f(centerX, centerY, centerZ), Vec3f(upX, upY, upZ));
	//}
};