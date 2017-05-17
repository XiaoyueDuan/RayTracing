#include "FileLoader.h"

int main()
{
	//MTLLoader mtlLoader;
	//string mtlname = "../../models/scene_2_obj/scene02_modified.mtl";
	//mtlLoader.loadMTL(mtlname);

	ObjLoader objLoader;
	string objpath= "../../models/scene_1_obj/";
	string objname = "scene01.obj";
	objLoader.loadObj(objpath, objname);
	objLoader.scene;

	return 0;
}