#ifndef __IMAGE__
#define __IMAGE__

#include "../Math/Vec3.h"
#include <png.h>
#include <string>

using std::string;

class Image {
private:
	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
public:
	int width, height;
	
	Image(int w, int h, png_byte type, png_byte depth, png_bytep *pointers) : 
		width(w), height(h), color_type(type), bit_depth(depth), row_pointers(pointers) {}
	Image(int w, int h, Color* colors);
	Image(string& file) { load(file); };
	Color getColor(int x, int y);
	bool save(string& file);
	bool load(string& file);
};

#endif