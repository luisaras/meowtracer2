#ifndef __PLIGHT__
#define __PLIGHT__

#include "Light.h"
#include "../Math/Matrix4.h"

class PointLight : public Light {
private:
  Point3 origin;
public:
  PointLight(Matrix4 &xform, Color &c, Point3 &o) : Light(c), origin(xform.TransformPoint(o)) {}
  Vec3 getDirection(LightHit &lh);
};

#endif