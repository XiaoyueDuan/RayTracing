#pragma once

#include <limits>
#include <random>
#include "Geometry.h"

using namespace std;

#define M_PI 3.1415926

static const float kInfinity = numeric_limits<float>::max();
static const float kEpsilon = 1e-4;
static const Vec3f kDefaultBackgroundColor(0, 0, 0);

//  «∑Ò π”√OPENMP
//#define _USING_OPENMP

