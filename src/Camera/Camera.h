
#pragma once

#include "../Math/Ray.h"
#include "../Math/Matrix4.h"

class Camera {
protected:
	Vec3 horizontal;
	Vec3 vertical;
	Point3 position;

	Camera(Matrix4 &xform, Vec3 &h, Vec3 &v, Point3 &pos) : 
	    horizontal(xform.transformVector(h)), 
	    vertical(xform.transformVector(v)), 
	    position(xform.transformPoint(pos)) {}
public:
	virtual Ray getRay(float i, float j) = 0;
	virtual float getDepth(Point3 p) = 0;
	Point3 pixelPosition(float i, float j) { return position + horizontal * i + vertical * j; }
};