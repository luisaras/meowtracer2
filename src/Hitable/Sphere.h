
#pragma once

#include "../Math/Matrix4.h"
#include "Hitable.h"

class Sphere : public Hitable {
private:
  Matrix4 invt;
  Matrix4 transform;
  Point3 center;
  float radius;
public:
	Sphere(Matrix4 &xform, Point3 p, float r);

	// Hitable
	RayHit hit(Ray &ray);
	Box hitBox();
	Color getTexture(Vec2& uv);
};