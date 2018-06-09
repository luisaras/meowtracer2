#ifndef __MUTIL__
#define __MUTIL__

#include "Vec2.h"
#include "Vec3.h"

#include <cstdlib>
#include <ctime>
#include <limits>
#include <cmath>

#define INF std::numeric_limits<float>::infinity()
#define PI2 6.28318530718 

Vec2 randomVec2();
Vec3 randomUnitVec3(float radius = 1);
Vec2 beckmannImportance(Vec2& e, float m2);

#endif