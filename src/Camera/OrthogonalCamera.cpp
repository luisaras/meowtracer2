#include "OrthogonalCamera.h"

Ray OrthogonalCamera::getRay(float i, float j) { 
	return Ray(position + horizontal * i + vertical * j, direction, refraction); 
}

float OrthogonalCamera::getDepth(Point3 p) {
	return (position.z - p.z);
}