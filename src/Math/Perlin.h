#ifndef __PERLIN__
#define __PERLIN__

#include "Vec3.h"

float perlin(const Vec3& p, int depth=7);
float noise(const Vec3& p);

#endif