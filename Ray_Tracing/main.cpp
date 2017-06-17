#include "PathTracing.h"
#include "Sphere.h"
#include "ImageSaver.h"

int main(int argc, char *argv[])
{
#ifdef GI
	ObjLoader objLoader;
	string objpath= "../models/scene_3_obj/";
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
	render(option, objLoader.scene, pixels, lights,10);

	// Save images
	int dpi = 72;
	string saveFileName = "result/Secne1_Depth4_10pixel_1light.bmp";
	savebmp(saveFileName, option.width, option.height, dpi, pixels);
#else
	ObjLoader objLoader;
	string objpath = "../models/scene_4_obj/";
	string objname = "scene04.obj";
	objLoader.loadObj(objpath, objname);

	// Add 4 spheres
	Sphere sphere1(Vec3f(-3.4, 6, 3), 0.08);
	Sphere sphere2(Vec3f(0.1, 6, 3), 0.33);
	Sphere sphere3(Vec3f(3.7, 6, 3), 0.5);
	Sphere sphere4(Vec3f(7.3, 6, 3), 1.0);

	Material *m1 = nullptr;
	string sphere1materialName = "lambert2SG";
	Material *m2 = nullptr;
	string sphere2materialName = "lambert3SG";
	Material *m3 = nullptr;
	string sphere3materialName = "lambert4SG";
	Material *m4 = nullptr;
	string sphere4materialName = "lambert5SG";

	if (!objLoader.mtlLoder.findMaterial(sphere1materialName, m1) ||
		!objLoader.mtlLoder.findMaterial(sphere2materialName, m2) ||
		!objLoader.mtlLoder.findMaterial(sphere3materialName, m3) ||
		!objLoader.mtlLoder.findMaterial(sphere4materialName, m4))
	{
		cout << "No such material(main.cpp)!" << endl;
		return 0;
	}
	sphere1.material = m1;
	sphere2.material = m2;
	sphere3.material = m3;
	sphere4.material = m4;

	objLoader.scene.add(*(dynamic_cast<Object *> (&sphere1)));
	objLoader.scene.add(*(dynamic_cast<Object *> (&sphere2)));
	objLoader.scene.add(*(dynamic_cast<Object *> (&sphere3)));
	objLoader.scene.add(*(dynamic_cast<Object *> (&sphere4)));

	// Set opiton
	Options option;
	option.setCameraToWorldCoordinate(Vec3f(0, 10, 40), Vec3f(0, 2, 0), Vec3f(0, 1, 0));
	option.width = 600;
	option.height = 400;
	option.depth = 5;
	option.fov = 40;

	int n = option.width*option.height;
	Vec3f *pixels = new Vec3f[n];

	// Create Light Sources
	vector<Light *> lights;
	SphereLight l1(Vec3f(-3.4, 6, 3), 0.08);
	SphereLight l2(Vec3f(0.1, 6, 3), 0.33);
	SphereLight l3(Vec3f(3.7, 6, 3), 0.5);
	SphereLight l4(Vec3f(7.3, 6, 3), 1.00);
	lights.push_back((dynamic_cast<Light *>(&l1)));
	lights.push_back((dynamic_cast<Light *>(&l2)));
	lights.push_back((dynamic_cast<Light *>(&l3)));
	lights.push_back((dynamic_cast<Light *>(&l4)));

	// Rendering
	render(option, objLoader.scene, pixels, lights, 100);

	// Save images
	int dpi = 72;
	string saveFileName = "result/Secne2_Depth5_100pixel_1light.bmp";
	savebmp(saveFileName, option.width, option.height, dpi, pixels);
#endif 
	
	return 0;
}

