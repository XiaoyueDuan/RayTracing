#include "FileLoader.h"

int main()
{
	MTLLoader mtlLoader;
	string mtlname = "../../models/scene_2_obj/scene02_modified.mtl";
	mtlLoader.loadMTL(mtlname.c_str());

	return 0;
}