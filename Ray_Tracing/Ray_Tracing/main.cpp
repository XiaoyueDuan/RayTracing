#include "PathTracing.h"
#include "Sphere.h"
#include "ImageSaver.h"

int main(int argc, char *argv[])
{
	//MTLLoader mtlLoader;
	//string mtlname = "../../models/scene_2_obj/scene02_modified.mtl";
	//mtlLoader.loadMTL(mtlname);
	// Load Walls in scene03.obj
	ObjLoader objLoader;
	string objpath= "../../models/scene_3_obj/";
	string objname = "scene03.obj";
	objLoader.loadObj(objpath, objname);

	// Add two sphere
	Sphere sphere1(Vec3f(-2.88, 2, -2.5), 2);
	Sphere sphere2(Vec3f(2.88, 2, 2.5), 2);	

	Material *m1=nullptr;
	string sphere1materialName="sphere_mirror";
	Material *m2=nullptr;
	string sphere2materialName = "sphere_transparent";

	if (!objLoader.mtlLoder.findMaterial(sphere1materialName, m1)||
		!objLoader.mtlLoder.findMaterial(sphere2materialName, m2))
	{
		cout << "No such material(main.cpp)!" << endl;
		return 0;
	}
	sphere1.material = m1;
	sphere2.material = m2;

	objLoader.scene.add(*(dynamic_cast<Object *> (&sphere1)));
	objLoader.scene.add(*(dynamic_cast<Object *> (&sphere2)));
	
	// Set opiton
	Options option;
	option.setCameraToWorldCoordinate(Vec3f(0, 5, 30), Vec3f(0, 5, 0), Vec3f(0, 1, 0));
	option.width = 480;
	option.height = 400;
	option.depth = 4;
	option.fov = 25;

	int n = option.width*option.height;
	Vec3f *pixels = new Vec3f[n];

	// Create Light Sources
	vector<Light *> lights;
	AreaLight l1(Vec3f(0, 9.882, 0), Vec3f(1.0f), 1.0f,
		Vec3f(-1.05, 9.882, -1.05), Vec3f(1.05, 9.882, 1.05));
	lights.push_back((dynamic_cast<Light *>(&l1)));

	// Rendering
	render(option, objLoader.scene, pixels,lights);

	// Save images
	int dpi = 72;
	string saveFileName = "result/result_Depth2_100pixel_1light_test.bmp";
	savebmp(saveFileName, option.width, option.height, dpi, pixels);
	

	return 0;
}

