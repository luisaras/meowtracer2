#ifndef __PARSER__
#define __PARSER__

#include "../Math/Vec3.h"
#include "../Renderer/Renderer.h"
#include <string>

using std::string;

class Parser {
public:
	int colCount = 100;
	int rowCount = 100;
	Renderer* renderer;
	bool load(string& file);
};

#endif