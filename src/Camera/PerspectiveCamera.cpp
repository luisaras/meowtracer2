#include "PerspectiveCamera.h"

Ray PerspectiveCamera::getRay(float i, float j) {
  Vec3 origin = pixelPosition(i, j);
	return Ray(lens, origin - lens);
}

float PerspectiveCamera::getDepth(Point3 p) {
	return (p - lens).Length();
}