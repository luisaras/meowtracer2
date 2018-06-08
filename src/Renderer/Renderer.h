#ifndef __REND__
#define __REND__

#include "../Camera/Camera.h"
#include <thread>
#include <mutex>

using std::thread;

class Renderer {
protected:

	std::mutex mutex;
	float progress;
	int progressi;
	int totalProgress;

	void updateProgress();
	void renderLines(Color* colors, int width, int height, int y1, int y2);
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