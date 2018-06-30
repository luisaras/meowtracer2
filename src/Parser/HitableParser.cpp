#include "HitableParser.h"
#include "../Hitable/Sphere.h"
#include "../Hitable/Triangle.h"

void addMesh(Object& hit_obj, Matrix4& xform, Material* mat, Texture* tex, bool shadows, Scene& scene) {
	// TODO
}

void addCube(Object& hit_obj, Matrix4& xform, Material* mat, Texture* tex, bool shadows, Scene& scene) {
	Vec3 size = parseVec3(hit_obj["SIZE"]);
	Vec3 center = parseVec3(hit_obj["CENTER"]);
	// Box
	Vec3 half = size * 0.5;
	Point3 min = center - half;
	Point3 max = center + half;
	Box cube(min, max);
	// Triangles
	vector<Point3> v;
	cube.triangulate(v);
	int id = scene.hitables.size();
	for (uint i = 0; i < v.size(); i += 3) {
		Point3 p0 = xform.transformPoint(v[i]);
		Point3 p1 = xform.transformPoint(v[i + 1]);
		Point3 p2 = xform.transformPoint(v[i + 2]);
		Triangle* t = new Triangle(id++, p0, p1, p2);
		t->norm[0] = t->norm[1] = t->norm[2] = t->calculateNormal();
		t->material = mat;
		t->texture = tex;
		t->castShadows = shadows;
		scene.hitables.push_back(t);
	}
}

Hitable* parsePrimitive(Object& hit_obj, string& type, Matrix4& xform, Material* mat, Texture* tex) {
	Hitable* hit = 0;
	if (type == "sphere") {
		// Sphere
		float r = hit_obj["RADIUS"].getReal();
		Vec3 center = parseVec3(hit_obj["CENTER"]);
		hit = new Sphere(0, xform, center, r);
	} else if (type == "triangle") {
		// Triangle
		Vec3 p0 = parseVec3(hit_obj["P0"]);
		Vec3 p1 = parseVec3(hit_obj["P1"]);
		Vec3 p2 = parseVec3(hit_obj["P2"]);
		p0 = xform.transformPoint(p0); p1 = xform.transformPoint(p1); p2 = xform.transformPoint(p2);
		Triangle* t = new Triangle(0, p0, p1, p2);
		Vec3 normal = hit_obj.count("NORMAL") ? parseVec3(hit_obj["NORMAL"]) : t->calculateNormal();
		t->norm[0] = t->norm[1] = t->norm[2] = normal;
		hit = t;
	} else {
		cout << "Object type not recognized: " << type << endl;
		return 0;
	}
	hit->material = mat;
	hit->texture = tex;
	return hit;
}

bool parseHitables(Array& hit_arr, Scene& scene) {
	for (uint i = 0; i < hit_arr.size(); i++) {
		Object hit_obj = hit_arr[i].getObject();
		// Transform
		Matrix4 xform = hit_obj.count("TRANSFORM") ? 
			parseTransform(hit_obj["TRANSFORM"]) : 
			Matrix4::identity();
		// Material
		Material* mat = scene.materials[hit_obj["MATERIAL"].getInt()];
		Texture* tex = hit_obj.count("TEXTURE") ? scene.textures[hit_obj["TEXTURE"].getInt()] : NULL;
		bool shadows = hit_obj.count("SHADOWS") ? hit_obj["SHADOWS"].getBool() : true;
		// Type
		string type = hit_obj["TYPE"].getString();
		if (type == "obj") {
			addMesh(hit_obj, xform, mat, tex, shadows, scene);
		} else if (type == "cube") {
			addCube(hit_obj, xform, mat, tex, shadows, scene);
		} else {
			Hitable* obj = parsePrimitive(hit_obj, type, xform, mat, tex);
			if (!obj) 
				return false;
			obj->id = scene.hitables.size();
			obj->castShadows = shadows;
			scene.hitables.push_back(obj);
		}
	}
	return true;
}