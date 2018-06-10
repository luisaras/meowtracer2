#include "Parser.h"
#include "../Renderer/RayTracer.h"
#include "../Reflection/BlinnPhong.h"
#include "../Reflection/CookTorrance.h"
#include "../Camera/PerspectiveCamera.h"
#include "../Hitable/Sphere.h"
#include "../Light/PointLight.h"

#include <random>

bool Parser::load(string& file) {
	colCount = rowCount = 1000;

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

	rayTracer->treeDepth = 10;
	rayTracer->rayDepth = 20;
	rayTracer->sampleCount = 8;
	

	// Material 0 - Lambertian
	{
		Material* mat = new Material();
		mat->kd = Color(0.8, 0.5, 0.8);
		mat->ks = Color(1, 0.8, 0.8) * 0.8;
		mat->shininess = 16;
		mat->type = 0;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 1 - Mirror
	{
		Material* mat = new Material();
		mat->kd = Color(0, 1, 1);
		mat->ks = Color(0, 0, 0);
		mat->type = 1;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 2 - Water
	{
		Material* mat = new Material();
		mat->kd = Color(1, 0, 1);
		mat->refraction = 1.330;
		mat->type = 2;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 3 - Air
	{
		Material* mat = new Material();
		mat->kd = Color(1, 0, 1);
		mat->refraction = 1 / 1.330;
		mat->type = 2;
		rayTracer->scene.materials.push_back(mat);
	}

	// Floor
	{
		Point3 center(0.5, -99.75, -3);
		Sphere* sphere = new Sphere(xform, center, 100);
		sphere->material = rayTracer->scene.materials[0];
		rayTracer->scene.hitables.push_back(sphere);
	}

	// Wall
	{
		Point3 center(0.5, -0.5, -104);
		Sphere* sphere = new Sphere(xform, center, 100);
		sphere->material = rayTracer->scene.materials[0];
		//rayTracer->scene.hitables.push_back(sphere);
	}

	// Opaque sphere
	{
		Point3 center(0, 0.5, -2);
		Sphere* sphere = new Sphere(xform, center, 0.25);
		sphere->material = rayTracer->scene.materials[1];
		rayTracer->scene.hitables.push_back(sphere);
	}

	// Metal sphere
	{
		Point3 center(0.5, 0.4, -2);
		Sphere* sphere = new Sphere(xform, center, 0.15);
		sphere->material = rayTracer->scene.materials[0];
		rayTracer->scene.hitables.push_back(sphere);
	}

	// Glass sphere
	{
		Point3 center(1, 0.5, -2);
		Sphere* sphere = new Sphere(xform, center, 0.25);
		sphere->material = rayTracer->scene.materials[2];
		rayTracer->scene.hitables.push_back(sphere);
	}

	// Anti-glass sphere
	{
		Point3 center(1, 0.525, -2);
		Sphere* sphere = new Sphere(xform, center, 0.225);
		sphere->material = rayTracer->scene.materials[3];
		rayTracer->scene.hitables.push_back(sphere);
	}

	std::default_random_engine generator (0);
	for (int i = 0; i < 0; i++) {
		Point3 center(1.5 * generator() / generator.max() - 0.25, 
			1.5 * generator() / generator.max() - 0.25,
			1.5 * generator() / generator.max() - 2.25);
		Sphere* sphere = new Sphere(xform, center, 0.25 * generator() / generator.max());
		sphere->material = rayTracer->scene.materials[0];
		rayTracer->scene.hitables.push_back(sphere);
	}

	//Light
	{
		Color color(1, 1, 1);
		Point3 origin(0.5, 1.25, -1);
		PointLight* light = new PointLight(xform, color, origin, 1);
		rayTracer->scene.lights.push_back(light);
	}
	rayTracer->scene.ambientColor = Color(0.05, 0.05, 0.1);

	renderer->preprocess();
	return true;
}