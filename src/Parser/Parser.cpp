#include "Parser.h"
#include "../Renderer/RayTracer.h"
#include "../Reflection/BlinnPhong.h"
#include "../Camera/PerspectiveCamera.h"
#include "../Hitable/Sphere.h"
#include "../Light/PointLight.h"

bool Parser::load(string& file) {
	Matrix4 xform = Matrix4::Identity();

	// Camera
	Vec3 h(1, 0, 0);
	Vec3 v(0, 1, 0);
	Point3 pos(0, 0, 0);
	Vec3 lens(0.5, 0.5, 1);
	Camera* cam = new PerspectiveCamera(xform, h, v, pos, lens);

	// Renderer
	ReflectionModel* rm = new BlinnPhong();
	RayTracer* rayTracer = new RayTracer(cam, rm);
	renderer = rayTracer;
	renderer->tl = Color(1, 1, 1);
	renderer->tr = Color(1, 1, 1);
	renderer->bl = Color(0, 0, 1);
	renderer->br = Color(0, 0, 1);
	
	// Material
	{
		Material* mat = new Material();
		mat->kd = Color(1, 0, 1);
		rayTracer->scene.materials.push_back(mat);
	}

	// Sphere
	{
		Point3 center(0.5, 0.5, -2);
		Sphere* sphere = new Sphere(xform, center, 1);
		sphere->material = rayTracer->scene.materials[0];
		rayTracer->scene.hitables.push_back(sphere);
	}

	//Light
	{
		Color color(1, 1, 1);
		Point3 origin(0.5, 1.5, -1);
		PointLight* light = new PointLight(xform, color, origin, 1);
		rayTracer->scene.lights.push_back(light);
	}

	renderer->preprocess();
	return true;
}