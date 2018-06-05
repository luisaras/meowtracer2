#include "SphereLight.h"
#include "../Math/Util.h"

Vec3 SphereLight::getDirection(LightHit &lh) {
  Vec3 v = origin - lh.rayHit.point;
  v += randomUnitVec3(size);
  return v;
}