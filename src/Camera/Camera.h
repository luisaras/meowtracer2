#ifndef __CAMERA__
#define __CAMERA__

#include "../Math/Ray.h"
#include "../Math/Matrix4.h"

class Camera {
protected:
	Point3 position;
	Vec3 horizontal;
	Vec3 vertical;

	Camera(Matrix4 &xform, Vec3 &h, Vec3 &v, Point3 &pos) : 
    horizontal(xform.TransformVector(h)), 
    vertical(xform.TransformVector(v)), 
    position(xform.TransformPoint(pos)) {}
public:
	virtual Ray getRay(float i, float j) = 0;
	virtual float getDepth(Point3 p) = 0;
	Point3 pixelPosition(float i, float j) { return position + horizontal * i + vertical * j; }
};

#endif