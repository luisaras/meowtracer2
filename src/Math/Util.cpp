#include "Util.h"

Vec3 randomUnitVec3(float radius) {
  float phi = PI2 * rand() / RAND_MAX;
  float cost = 1.0 * rand() / RAND_MAX;
  float sint = sqrt(1 - cost*cost);
  float x = sint * cos(phi);
  float y = sint * sin(phi);
  return Vec3(x * radius, y * radius, cost * radius);
}