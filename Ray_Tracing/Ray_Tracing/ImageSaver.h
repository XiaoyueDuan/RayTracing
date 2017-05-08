#pragma once

#include <fstream>
#include <sstream>
#include <string>

struct RGBType {
	double r;
	double g;
	double b;
};

void savebmp(const char *filename, int w, int h, int dpi, RGBType *data) {};