#include <algorithm>
#include "FileLoader.h"
#include "Options.h"

Vec3f castRay(
	const Vec3f &orig, const Vec3f &dir,
	const Scene &scene,
	const Options &options,
	const int depth = 0)
{
	if (depth>options.maxDepth)
		return options.backgroundColor;

	Vec3f hitColor = options.backgroundColor;
	float tnear = kInfinity;
	Vec2f uv;
	int index = 0;
	Object hitObject;
	if (scene.intersect(orig, dir, tnear, index, uv, hitObject))
	{
		Vec3f hitPoint = orig + dir * tnear;
		Vec3f hitNormal;
		Vec2f hitTexCoordinates;
		hitObject.getSurfaceProperties(hitPoint, dir, index, uv, hitNormal, hitTexCoordinates);

		float NdotView = max(0.f, hitNormal.dotProduct(-dir));
		const int M = 10;
		float checker = (fmod(hitTexCoordinates.x * M, 1.0) > 0.5) ^ (fmod(hitTexCoordinates.y * M, 1.0) < 0.5);
		float c = 0.3 * (1 - checker) + 0.7 * checker;

		hitColor = c * NdotView; //Vec3f(uv.x, uv.y, 0);
	}
	return hitColor;
}

void render(
	const Options &options,
	const Scene &scene,
	Vec3f &pixels)
{
	float scale = tan(options.fov * 0.5*M_PI / 180);
	float imageAspectRatio = options.width / (float)options.height;
	Vec3f orig;
	options.cameraToWorld.multVecMatrix(Vec3f(0), orig);
	for (int j = 0; j < options.height; ++j) {
		for (int i = 0; i < options.width; ++i) {
			// generate primary ray direction
			float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
			float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;
			Vec3f dir;
			options.cameraToWorld.multDirMatrix(Vec3f(x, y, -1), dir);
			dir.normalize();
			pixels = castRay(orig, dir, scene, options, 0);
		}
	}
}

int main(int argc, char *argv[])
{
	//MTLLoader mtlLoader;
	//string mtlname = "../../models/scene_2_obj/scene02_modified.mtl";
	//mtlLoader.loadMTL(mtlname);

	ObjLoader objLoader;
	string objpath= "../../models/scene_1_obj/";
	string objname = "scene01.obj";
	objLoader.loadObj(objpath, objname);

	Options option;
	option.setCameraToWorldCoordinate(Vec3f(0, 0, 10), Vec3f(0, 0, 0), Vec3f(0, 1, 0));

	int n = option.width*option.height;
	Vec3f *pixels = new Vec3f[n];

	render(option, objLoader.scene, pixels[0]);

	return 0;
}

