
#pragma once

#include "Light.h"

class SphereLight : public Light {
private:
  Point3 origin;
  Vec3 direction;
  float angle;
  float size;
public:
  SphereLight(Matrix4 &xform, Color &c, Point3 &o, Vec3 d, float a, float s) : 
    Light(c), origin(xform.transformPoint(o)), direction(-xform.transformVector(d)), angle(a), size(s) {}
  LightHit hit(Ray& ray, RayHit& rh);
};