#ifndef __REND__
#define __REND__

#include "../Camera/Camera.h"

class Renderer {
protected:

	Color backgroundColor(float tCol, float tRow);
	virtual Color getColor(Ray &initRay, float x, float y) = 0;

public:
	Camera* camera;
	Color tl, tr, bl, br;
	int sampleCount = 1;
	
	virtual void preprocess() = 0;
	Color* render(int width, int height);
};

#endif