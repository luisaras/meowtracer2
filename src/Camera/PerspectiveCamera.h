
#pragma once

#include "Camera.h"
#include <iostream>

class PerspectiveCamera : public Camera {
protected:
	Point3 lens;
public:
	PerspectiveCamera(Matrix4& xform, Vec3& h, Vec3& v, Point3& pos, Point3& l, float ref, Color& a) : 
    	Camera(xform, h, v, pos, ref, a), lens(xform.transformPoint(l)) { }
	Ray getRay(float i, float j);
	float getDepth(Point3 p);
};