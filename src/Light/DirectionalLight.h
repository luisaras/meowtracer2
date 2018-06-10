#ifndef __DLIGHT__
#define __DLIGHT__

#include "Light.h"

class DirectionalLight : public Light {
private:
	Vec3 direction;
public:
	DirectionalLight(Matrix4 &xform, Color &c, Vec3 &dir) : 
    Light(c), direction(-xform.transformVector(dir)) {
    	direction = Vec3::normalize(direction);
    }
	LightHit hit(Ray& ray, RayHit& rh);
};

#endif