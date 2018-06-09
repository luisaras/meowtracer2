#ifndef __SLIGHT__
#define __SLIGHT__

#include "Light.h"
#include "../Math/Matrix4.h"

class SpotLight : public Light {
private:
  Point3 origin;
  Vec3 direction;
  float angle;
public:
  SpotLight(Matrix4 &xform, Color &c, Point3 &o, Vec3 d, float a) : 
    Light(c), origin(xform.TransformPoint(o)), direction(-xform.TransformVector(d)), angle(a) {
      direction = Vec3::Normalize(direction);
    }
    LightHit hit(Ray& ray, RayHit& rh);

};

#endif