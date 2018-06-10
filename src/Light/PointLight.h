
#pragma once

#include "Light.h"
#include "../Math/Matrix4.h"

class PointLight : public Light {
private:
	Point3 origin;
	float length;
public:
	PointLight(Matrix4 &xform, Color &c, Point3 &o, float l) : 
  		Light(c), origin(xform.transformPoint(o)), length(l) {}
	LightHit hit(Ray& ray, RayHit& rh);
};