#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include "MaterialDataStructure.h"
#include "Object.h"
#include"Group.h"
#include"TriangleMesh.h"
#include"Scene.h"

using namespace std;

class MTLLoader
{
public:
	vector<Material> materialList;

	bool loadMTL(const char * path);
	bool findMaterial(string materialName, Material *m);
};

class ObjLoader
{
private:
	vector<Vec3f> vertexList;
	vector<Vec3f> vertexNormalList;
	vector<Vec2f> vertexTextureList;

	template<typename Out>
	void split(const string &s, char delim, Out result);
	vector<int> split(const string &s, char delim);

public:
	bool loadObj(const char * path, const char * fileName);
};