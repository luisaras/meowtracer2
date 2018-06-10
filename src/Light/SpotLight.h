
#pragma once

#include "Light.h"
#include "../Math/Matrix4.h"

class SpotLight : public Light {
private:
  Point3 origin;
  Vec3 direction;
  float angle;
public:
  SpotLight(Matrix4 &xform, Color &c, Point3 &o, Vec3 d, float a) : 
    Light(c), origin(xform.transformPoint(o)), direction(-xform.transformVector(d)), angle(a) {
      direction = Vec3::normalize(direction);
    }
    LightHit hit(Ray& ray, RayHit& rh);

};