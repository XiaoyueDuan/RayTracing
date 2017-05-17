#include "FileLoader.h"

int main()
{
	MTLLoader mtlLoader;
	string mtlname = "../../models/scene_2_obj/scene02_modified.mtl";
	mtlLoader.loadMTL(mtlname);

	return 0;
}