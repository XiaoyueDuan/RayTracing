#pragma once
#include <limits>
#include "Geometry.h"

using namespace std;

#define M_PI 3.1415926

static const float kInfinity = numeric_limits<float>::max();
static const float kEpsilon = 1e-8;
static const Vec3f kDefaultBackgroundColor(0, 0, 0);

//  «∑Ò π”√OPENMP
//#define _USING_OPENMP

#include <random>
default_random_engine generator;
uniform_real_distribution<double> distribution(0.0, 1.0);