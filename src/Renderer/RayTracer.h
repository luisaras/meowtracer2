
#pragma once

#include "Tracer.h"

class RayTracer : public Tracer {
protected:
	Color getColor(Ray &ray, float x, float y, int depth);
	
public:
	RayTracer(Camera* cam) { camera = cam; }
};