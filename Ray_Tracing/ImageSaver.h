#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "Geometry.h"
#include "FileLoader.h" // why #include <string> is useless?
//struct RGBType {
//	double r;
//	double g;
//	double b;
//};

void savebmp(string filename, int w, int h, int dpi, Vec3f *data);