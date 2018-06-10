#include "Renderer.h"
#include <iostream>
#include <cmath>

#define NTHREAD 4

using std::cout;
using std::endl;
using std::flush;
using std::min;

Color* Renderer::render(int width, int height) {
	Color* colors = new Color[width * height];
	progress = progressi = 0;
	totalProgress = width * height;
	if (NTHREAD == 1) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				renderCell(colors, i, j, width, height);
				updateProgress();
			}
		}
	} else {
		thread threads[NTHREAD - 1];
		lineProgress = new bool[height];
		for (int i = 0; i < height; i++) {
			lineProgress[i] = false;
		}
		for (int i = 0; i < NTHREAD - 1; i++) {
			threads[i] = thread(&Renderer::threadRender, this, colors, width, height);
		}
		threadRender(colors, width, height);
		for (int i = 0; i < NTHREAD - 1; i++) {
			threads[i].join();
		}
		delete [] lineProgress;
	}
	cout << endl;
	return colors;
}

void Renderer::threadRender(Color* colors, int width, int height) {
	for (int j = 0; j < height; j++) {
		lineLock.lock();
		if (lineProgress[j]) {
			lineLock.unlock();
			continue;
		}
		lineProgress[j] = true;
		lineLock.unlock();
		for (int i = 0; i < width; i++) {
			renderCell(colors, i, j, width, height);
			printLock.lock();
			updateProgress();
			printLock.unlock();
		}
	}
}

void Renderer::renderCell(Color* colors, int i, int j, int width, int height) {
	Color c(0, 0, 0);
	if (sampleCount > 1) {
		for(int s = 0; s < sampleCount; s++) {
			float pixelx = 1.0 * rand() / RAND_MAX + i;
			float pixely = 1.0 * rand() / RAND_MAX + (height - j - 1);
			float x = pixelx / (width - 1);
			float y = pixely / (height - 1);
			Ray ray = camera->getRay(x, y);
			ray.normalize();
			c += getColor(ray, x, y) / sampleCount;
		}
	} else {
		float pixelx = 0.5 + i;
		float pixely = 0.5 + (height - j - 1);
		float x = pixelx / (width - 1);
		float y = pixely / (height - 1);
		Ray ray = camera->getRay(x, y);
		ray.normalize();
		c = getColor(ray, x, y) / sampleCount;
	}
	colors[j * width + i] = c;
}

void Renderer::updateProgress() {
	int newProgress = ceil(++progress * 100 / totalProgress);
	if (newProgress != progressi) {
		progressi = newProgress;
		cout << "\rProgress: " << progressi << '%' << flush; 
	}
}

Color Renderer::backgroundColor(float tCol, float tRow)  {
	Color top = Vec3::lerp(tl, tr, tCol);
	Color bottom = Vec3::lerp(bl, br, tCol);
	return Vec3::lerp(bottom, top, tRow);
}