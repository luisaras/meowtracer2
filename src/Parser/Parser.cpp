#include "Parser.h"
#include "../Renderer/RayTracer.h"
#include "../Reflection/BlinnPhong.h"
#include "../Reflection/CookTorrance.h"
#include "../Camera/PerspectiveCamera.h"
#include "../Hitable/Sphere.h"
#include "../Hitable/Triangle.h"
#include "../Light/PointLight.h"

#include <random>

void addCube(Scene& scene, Point3& center, Vec3& size, int mat) {
	vector<Point3> v;
	Vec3 half = size * 0.5;
	Point3 min = center - half;
	Point3 max = center + half;
	Box cube(min, max);
	cube.triangulate(v);
	for (uint i = 0; i < v.size(); i += 3) {
		Triangle* t = new Triangle(v[i], v[i + 1], v[i + 2]);
		t->norm[0] = t->norm[1] = t->norm[2] = t->calculateNormal();
		t->material = scene.materials[mat];
		scene.hitables.push_back(t);
	}
}

void addSphere(Scene& scene, Point3& center, float radius, int mat) {
	Matrix4 xform = Matrix4::identity();
	Sphere* sphere = new Sphere(xform, center, radius);
	sphere->material = scene.materials[mat];
	scene.hitables.push_back(sphere);
}

bool Parser::load(string& file) {
	colCount = 800;
	rowCount = 400;

	Matrix4 xform = Matrix4::identity();

	// Camera
	Vec3 h(1, 0, 0);
	Vec3 v(0, 0.5, 0);
	Point3 pos(0, 0.25, 0);
	Vec3 lens(0.5, 0.5, 1);
	float initRef = 1.00029;
	Camera* cam = new PerspectiveCamera(xform, h, v, pos, lens, initRef);

	// Renderer
	ReflectionModel* rm = new BlinnPhong();
	RayTracer* rayTracer = new RayTracer(cam, rm);
	renderer = rayTracer;
	renderer->tl = Color(1, 1, 1);
	renderer->tr = Color(1, 1, 1);
	renderer->bl = Color(0, 0, 1);
	renderer->br = Color(0, 0, 1);

	rayTracer->treeDepth = 10;
	rayTracer->rayDepth = 10;
	rayTracer->sampleCount = 8;

	// Material 0 - Blinn-Phong
	{
		Material* mat = new Material(BLINNPHONG);
		mat->kd = Color(0.8, 0.3, 0.8);
		mat->ks = Color(1, 0.8, 0.8) * 0.8;
		mat->shininess = 16;
		mat->reflectivity = 0.2;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 1 - Mirror
	{
		Material* mat = new Material(METAL);
		mat->kd = Color(0, 1, 1);
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 2 - Air
	{
		Material* mat = new Material(DIELECTRIC);
		mat->kd = Color(1, 1, 1);
		mat->refraction = initRef;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 3 - Water
	{
		Material* mat = new Material(DIELECTRIC);
		mat->kd = Color(1, 0.5, 1);
		mat->refraction = 1.330;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 4 - Glass
	{
		Material* mat = new Material(DIELECTRIC);
		mat->kd = Color(1, 1, 1);
		mat->refraction = 1.5;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 5 - Jelly
	{
		Material* mat = new Material(BEERS);
		mat->kd = Color(1, 1, 1);
		mat->refraction = 1.125;
		mat->absorb = Color(8.0, 8.0, 3.0);
		mat->reflectivity = 0.01;
		rayTracer->scene.materials.push_back(mat);
	}

	// Material 6 - Phong (wall)
	{
		Material* mat = new Material(BLINNPHONG);
		mat->kd = Color(0.5, 0.5, 0.5);
		mat->ks = Color(1, 0.8, 0.8) * 0.8;
		mat->shininess = 8;
		rayTracer->scene.materials.push_back(mat);
	}

	// Floor
	{
		Point3 center(0.5, -99.75, -3);
		addSphere(rayTracer->scene, center, 100, 6);
	}

	// Wall
	{
		Point3 center(0.5, -0.5, -104);
		//addSphere(rayTracer->scene, center, 100, 0);
	}

	// Metal sphere
	{
		Point3 center(0.1, 0.5, -2);
		addSphere(rayTracer->scene, center, 0.25, 1);
	}

	// Opaque sphere
	{
		Point3 center(0.5, 0.4, -2);
		addSphere(rayTracer->scene, center, 0.15, 0);
	}

	// Water sphere
	{
		Point3 center(0.9, 0.5, -2);
		addSphere(rayTracer->scene, center, 0.25, 3);
	}

	// Anti-water sphere
	{
		Point3 center(0.9, 0.525, -2);
		addSphere(rayTracer->scene, center, 0.225, 2);
	}

	// Glass sphere
	{
		Point3 center(1.3, 0.4, -2);
		addSphere(rayTracer->scene, center, 0.15, 4);
	}

	// Jelly Cube
	{
		Point3 center(-0.5, 0.35, -2.8);
		Vec3 size(0.2, 0.2, 0.2);
		addCube(rayTracer->scene, center, size, 5);
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