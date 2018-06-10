
#pragma once

#include "Camera.h"

class OrthogonalCamera : public Camera {
protected:
	Vec3 direction;
public:
	OrthogonalCamera(Matrix4 &xform, Vec3 &h, Vec3 &v, Point3 &pos, Vec3 dir, float ref) : 
    Camera(xform, h, v, pos, ref), direction(xform.transformVector(dir)) {}
	Ray getRay(float i, float j);
	float getDepth(Point3 p);
};