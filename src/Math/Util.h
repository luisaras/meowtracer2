#pragma once

#include "Vec2.h"
#include "Vec3.h"

#include <cstdlib>
#include <ctime>
#include <limits>
#include <cmath>

#define INF std::numeric_limits<float>::infinity()
#define PI2 6.28318530718 
#define PI 3.14159265359
#define DEG2RAD PI/180
#define RAD2DEG 180/PI
#define ERR 0.00001

Vec2 randomVec2();
Vec3 randomUnitVec3();
Vec3 angle2Vec3(float cost, float phi);
Vec3 GGX(const Vec2 &e, float r);
Vec3 GGX(float r);

float clamp(float x, float min, float max);
Color clampColor(Color& color);