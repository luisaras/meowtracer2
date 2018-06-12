#pragma once

#include "../Renderer/Renderer.h"
#include <string>

using std::string;

class Parser {
private:
	bool parse(string& content);
public:
	int colCount = 100;
	int rowCount = 100;
	Renderer* renderer;
	bool load(string& file);
};