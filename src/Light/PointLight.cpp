#include "PointLight.h"

Vec3 PointLight::getDirection(LightHit &lh) {
  return origin - lh.rayHit.point;
}