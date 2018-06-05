#ifndef __ORTCAMERA__
#define __ORTCAMERA__

#include "Camera.h"

class OrthogonalCamera : public Camera {
protected:
  Vec3 direction;
	Vec3 getRayDirection(float i, float j);
public:
	OrthogonalCamera(Matrix4 &xform, Vec3 &h, Vec3 &v, Point3 &pos, Vec3 dir) : 
    Camera(xform, h, v, pos), direction(xform.TransformVector(dir)) {}
	Ray getRay(float i, float j);
	float getDepth(Point3 p);
};

#endif