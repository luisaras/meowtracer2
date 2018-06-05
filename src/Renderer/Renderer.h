#ifndef __REND__
#define __REND__

#include "../Camera/Camera.h"

class Renderer {
protected:
	Camera* camera;
	Color tl, tr, bl, br;

	int sampleCount = 1;

	Color backgroundColor(float tCol, float tRow);
	virtual void preprocess() = 0;
	virtual Color getColor(Ray &initRay, float x, float y) = 0;

public:
	Color* render(int width, int height);
};

#endif