#include "Renderer.h"
#include <iostream>
#include <cmath>

#define NTHREAD 1

using std::cout;
using std::endl;
using std::flush;
using std::min;

void Renderer::updateProgress() {
	mutex.lock();
	int newProgress = ceil(++progress * 100 / totalProgress);
	if (newProgress != progressi) {
		progressi = newProgress;
		cout << "\rProgress: " << progressi << '%' << flush; 
	}
	mutex.unlock();
}

Color* Renderer::render(int width, int height) {
	Color* colors = new Color[width * height];
	progress = progressi = 0;
	totalProgress = width * height;

	if (NTHREAD == 1) {
		renderLines(colors, width, height, 0, height);
	} else {
		thread threads[NTHREAD];
		int lineCount = ceil(height / NTHREAD);
		int y1 = 0, y2 = lineCount;
		for (int i = 0; i < NTHREAD; i++) {
			threads[i] = thread(&Renderer::renderLines, this, colors, width, height, y1, y2);
			y1 = y2;
			y2 = min(y1 + lineCount, height);
		}
		for (int i = 0; i < NTHREAD; i++) {
			threads[i].join();
		}
	}

	cout << endl;
	return colors;
}

void Renderer::renderLines(Color* colors, int width, int height, int y1, int y2) {
	for (int i = 0; i < width; i++) {
		for (int j = y1; j < y2; j++) {
			Color c(0, 0, 0);
			if (sampleCount > 1) 
				for(int s = 0; s < sampleCount; s++) {
					float pixelx = 1.0 * rand() / RAND_MAX + i;
					float pixely = 1.0 * rand() / RAND_MAX + (height - j - 1);
					float x = pixelx / (width - 1);
					float y = pixely / (height - 1);
					Ray ray = camera->getRay(x, y);
					ray.normalize();
					c += getColor(ray, x, y) / sampleCount;
				}
			else {
				float pixelx = 0.5 + i;
				float pixely = 0.5 + (height - j - 1);
				float x = pixelx / (width - 1);
				float y = pixely / (height - 1);
				Ray ray = camera->getRay(x, y);
				ray.normalize();
				c += getColor(ray, x, y) / sampleCount;
			}
			updateProgress();
			colors[j * width + i] = c;
		}
	}
}

Color Renderer::backgroundColor(float tCol, float tRow)  {
	Color top = Vec3::Lerp(tl, tr, tCol);
	Color bottom = Vec3::Lerp(bl, br, tCol);
	return Vec3::Lerp(bottom, top, tRow);
}