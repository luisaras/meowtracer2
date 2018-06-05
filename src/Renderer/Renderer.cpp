#include "Renderer.h"
#include <iostream>

using std::cout;
using std::endl;
using std::flush;

Color* Renderer::render(int width, int height) {
	Color* colors = new Color[width * height];
	float progress = 0;
	int totalProgress = width * height;
	int progressi = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
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
			int newProgress = int(++progress * 100 / totalProgress);
			if (newProgress != progressi) {
				progressi = newProgress;
				cout << "\rProgress: " << progressi << '%' << flush; 
			}
			colors[j * width + i] = c;
		}
	}
	cout << endl;
	return colors;
}

Color Renderer::backgroundColor(float tCol, float tRow)  {
	Color top = Vec3::Lerp(tl, tr, tCol);
	Color bottom = Vec3::Lerp(bl, br, tCol);
	return Vec3::Lerp(top, bottom, tRow);
}