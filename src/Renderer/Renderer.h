
#pragma once

#include "../Camera/Camera.h"
#include <thread>
#include <mutex>

using std::thread;

class Renderer {
protected:

	std::mutex printLock;
	std::mutex lineLock;
	bool* lineProgress;
	float progress;
	int progressi;
	int totalProgress;

	void updateProgress();
	void threadRender(Color* colors, int width, int height);
	void renderCell(Color* colors, int i, int j, int width, int height);

	Color backgroundColor(float tCol, float tRow);
	virtual Color getColor(Ray &initRay, float x, float y) = 0;

public:
	Camera* camera;
	Color tl, tr, bl, br;
	int sampleCount = 1;
	
	virtual void preprocess() = 0;
	Color* render(int width, int height);
};