
#pragma once

#include "Tracer.h"

class PathTracer : public Tracer {
protected:
	Color getColor(Ray &ray, float x, float y, int depth);
	
public:
	PathTracer(Camera* cam) { camera = cam; }
};