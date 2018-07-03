#include "Parser.h"
#include "../Renderer/RayTracer.h"
#include "../Renderer/PathTracer.h"
#include "../Camera/PerspectiveCamera.h"
#include "../Camera/OrthogonalCamera.h"
#include "SceneParser.h"

#include <iostream>
#include <fstream>

using std::ifstream;

Camera* parseCamera(Value& cam_js) {
	Object cam_obj = cam_js.getObject();
	// Field of view
	float w = cam_obj["WIDTH"].getReal();
	float h = cam_obj["HEIGHT"].getReal();
	Vec3 hor(w, 0, 0);
	Vec3 ver(0, h, 0);
	// Position
	Point3 center = parseVec3(cam_obj["CENTER"]);
	Point3 corner = center - Vec3(w / 2, h / 2, 0);
	// Transform
	Matrix4 xform = cam_obj.count("TRANSFORM") ? 
		parseTransform(cam_obj["TRANSFORM"]) : 
		Matrix4::identity();
	// Refraction index
	float ref = cam_obj.count("REFRACTION") ? cam_obj["REFRACTION"].getReal() : 1;
	Vec3 ab = cam_obj.count("ABSORB") ? parseVec3(cam_obj["ABSORB"]) : Color(0, 0, 0);
	// Camera type
	string type = cam_obj["TYPE"].getString();
	if (type == "perspective") {
		// Perspective camera
		Point3 viewer = parseVec3(cam_obj["VIEWER"]);
		Point3 lens = center + viewer;
		return new PerspectiveCamera(xform, hor, ver, corner, lens, ref, ab);
	} else if (type == "parallel") {
		// Orthogonal camera
		Vec3 dir = cam_obj.count("DIRECTION") ? parseVec3(cam_obj["DIRECTION"]) : Vec3(0, 0, -1);
		return new OrthogonalCamera(xform, hor, ver, corner, dir, ref, ab);
	} else {
		cout << "Camera type not recognized: " << type << endl;
		return NULL;
	}
}

Tracer* parseRenderer(Value& renderer_js, Value& bg_js, Camera* cam) {
	Object renderer_obj = renderer_js.getObject();
	Object bg_obj = bg_js.getObject();
	// Algorithm
	string type = renderer_obj["TYPE"].getString();
	Tracer* rt = type == "raytracer" ? (Tracer*) new RayTracer(cam) : (Tracer*) new PathTracer(cam);
	// Background
	rt->tl = bg_obj.count("UPPER_LEFT") ? parseVec3(bg_obj["UPPER_LEFT"]) : Vec3(1, 1, 1);
	rt->tr = bg_obj.count("UPPER_RIGHT") ? parseVec3(bg_obj["UPPER_RIGHT"]) : Vec3(1, 1, 1);
	rt->bl = bg_obj.count("LOWER_LEFT") ? parseVec3(bg_obj["LOWER_LEFT"]) : Vec3(1, 1, 1);
	rt->br = bg_obj.count("LOWER_RIGHT") ? parseVec3(bg_obj["LOWER_RIGHT"]) : Vec3(1, 1, 1);
	// Renderer parameters
	if (renderer_obj.count("TREEDEPTH")) rt->treeDepth = renderer_obj["TREEDEPTH"].getInt();
	if (renderer_obj.count("RAYDEPTH")) rt->rayDepth = renderer_obj["RAYDEPTH"].getInt();
	if (renderer_obj.count("SPP")) rt->spp = renderer_obj["SPP"].getInt();
	if (renderer_obj.count("SPR")) rt->spr = renderer_obj["SPR"].getInt();
	if (renderer_obj.count("THREADS")) rt->threadCount = renderer_obj["THREADS"].getInt();
	return rt;
}

bool Parser::parse(string& content) {
	Value input_js;
	read(content, input_js);
	Object input_obj = input_js.getObject();
	colCount = input_obj["WIDTH"].getInt();
	rowCount = input_obj["HEIGHT"].getInt();
	Camera* camera = parseCamera(input_obj["CAMERA"]);
	if (!camera) return false;
	Tracer* tracer = parseRenderer(input_obj["RENDERER"], input_obj["BACKGROUND"], camera);
	renderer = tracer;
	return parseScene(input_obj, tracer->scene);
}

bool Parser::load(string& fileName) {
	ifstream file(fileName.c_str(), ifstream::in);
	if (file.is_open()) {
		string content;
		while(!file.eof()) {
			string input;
			getline(file, input);
			// Clear comments
			uint c = input.find_first_of("//");
			if (c != string::npos)
				input = input.substr(0, c);
			content += input;
		}
		if (parse(content)) {
			renderer->preprocess();
			return true;
		} else {
			cout << "Could not parse." << endl;
		}
	} else {
		cout << "File does not exist." << endl;
	}
	return false;
}