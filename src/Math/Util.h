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
#define ERR 0.000001

Vec2 randomVec2();
Vec3 randomUnitVec3(float radius = 1);
Vec2 beckmannImportance(Vec2& e, float m2);

Vec3 reflect(const Vec3 &dir, const Vec3 &normal);
Vec3 refract(const Vec3 &dir, const Vec3 &normal, float ior);
float fresnel(const Vec3 &I, const Vec3 &N, float ior);
float schlick(float cos, float ref);