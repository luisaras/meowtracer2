#include "Parser.h"
#include "../Renderer/RayTracer.h"
#include "../Renderer/BlinnPhong.h"

bool Parser::load(string& file) {
	ReflectionModel* rm = new BlinnPhong();
	RayTracer* rayTracer = new RayTracer(rm);
	renderer = rayTracer;
}