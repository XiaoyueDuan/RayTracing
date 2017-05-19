#include "PathTracing.h"

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

	render(option, objLoader.scene, pixels);

	return 0;
}

