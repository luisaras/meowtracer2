#include "SceneParser.h"
#include "HitableParser.h"
#include "../Light/DirectionalLight.h"
#include "../Light/PointLight.h"
#include "../Light/SphereLight.h"
#include "../Light/SpotLight.h"
#include "../Material/Texture.h"

Light* parseLight(Value& light_js) {
	Object light_obj = light_js.getObject();
	// Emitted color
	Color color = parseVec3(light_obj["COLOR"]);
	// Transform
	Matrix4 xform = light_obj.count("TRANSFORM") ? 
		parseTransform(light_obj["TRANSFORM"]) : 
		Matrix4::identity();
	// Type
	string type = light_obj["TYPE"].getString();
	if (type == "directional") {
		// Directional light
		Vec3 dir = parseVec3(light_obj["DIRECTION"]);
		dir = Vec3::normalize(dir);
		return new DirectionalLight(xform, color, dir);
	} else if (type == "point") {
		// Point light
		Point3 origin = parseVec3(light_obj["ORIGIN"]);
		float length = light_obj["LENGTH"].getReal();
		return new PointLight(xform, color, origin, length);
	} else if (type == "spot") {
		// Spot light
		Point3 origin = parseVec3(light_obj["ORIGIN"]);
		Vec3 dir = parseVec3(light_obj["DIRECTION"]);
		float angle = cos(light_obj["ANGLE"].getReal() * PI / 180.0);
		return new SpotLight(xform, color, origin, dir, angle);
	} else if (type == "sphere") {
		// Spheric light
		Point3 origin = parseVec3(light_obj["ORIGIN"]);
		Vec3 dir = parseVec3(light_obj["DIRECTION"]);
		float angle = cos(light_obj["ANGLE"].getReal() * PI / 180.0);
		float size = light_obj["SIZE"].getReal();
		return new SphereLight(xform, color, origin, dir, angle, size);
	} else {
		cout << "Light type not recognized: " << type << endl;
		return NULL;
	}
}

Material* parseMaterial(Value& mat_js) {
	Object mat_obj = mat_js.getObject();
	string type = mat_obj["TYPE"].getString();
	// Type
	MaterialType t;
	if (type == "blinn-phong")
		t = BLINNPHONG;
	else if (type == "cook-torrance")
		t = COOKTORRANCE;
	else if (type == "lambertian")
		t = LAMBERTIAN;
	else if (type == "metal")
		t = METAL;
	else if (type == "dielectric")
		t = DIELECTRIC;
	else if (type == "beers")
		t = BEERS;
	else {
		cout << "Material type not recognized: " << type << endl;
		return NULL;
	}
	Material* m = new Material(t);
	// Properties
	if (mat_obj.count("KA"))
		m->ka = parseVec3(mat_obj["KA"]);
	if (mat_obj.count("KD"))
		m->kd = parseVec3(mat_obj["KD"]);
	if (mat_obj.count("KS"))
		m->ks = parseVec3(mat_obj["KS"]);
	if (mat_obj.count("KE"))
		m->ke = parseVec3(mat_obj["KE"]);
	if (mat_obj.count("ABSORB"))
		m->absorb = parseVec3(mat_obj["ABSORB"]);
	if (mat_obj.count("SHINE"))
		m->shininess = mat_obj["SHINE"].getReal();
	if (mat_obj.count("ROUGH"))
		m->roughness = mat_obj["ROUGH"].getReal();
	if (mat_obj.count("FUZZ"))
		m->fuzz = mat_obj["FUZZ"].getReal();
	if (mat_obj.count("REFLECT"))
		m->reflectivity = mat_obj["REFLECT"].getReal();
	if (mat_obj.count("REFRACTION"))
		m->refraction = mat_obj["REFRACTION"].getReal();
	return m;
}

Texture* parseTexture(Value& tex_js) {
	Object tex_obj = tex_js.getObject();
	string type = tex_obj["TYPE"].getString();
	if (type == "checkers") {
		Color c1 = parseVec3(tex_obj["COLOR1"]);
		Color c2 = parseVec3(tex_obj["COLOR2"]);
		int rx = tex_obj["REPEATX"].getInt();
		int ry = tex_obj["REPEATY"].getInt();
		return new CheckersTexture(c1, c2, rx, ry);
	} else if (type == "image") {
		string fileName = "meow/" + tex_obj["FILE"].getString();
		return new ImageTexture(fileName);
	} else if (type == "perlin") {
		float scale = tex_obj["SCALE"].getReal();
		return new PerlinTexture(scale);
	} else {
		cout << "Texture type not recognized: " << type << endl;
		return NULL;
	}
}

bool parseScene(Object& input_obj, Scene& scene) {
	// Lights
	if (input_obj.count("LIGHTS")) {
		Array lights = input_obj["LIGHTS"].getArray();
		for (uint i = 0; i < lights.size(); i++) {
			Light* light = parseLight(lights[i]);
			if (!light) return false;
			scene.lights.push_back(light);
		}
	}
	scene.ambientColor = input_obj.count("AMBIENT") ? parseVec3(input_obj["AMBIENT"]) : Vec3(0, 0, 0);
	// Materials
	if (input_obj.count("MATERIALS")) {
		Array materials = input_obj["MATERIALS"].getArray();
		for (uint i = 0; i < materials.size(); i++) {
			Material* material = parseMaterial(materials[i]);
			if (!material) return false;
			scene.materials.push_back(material);
		}
	}
	// Texture
	if (input_obj.count("TEXTURES")) {
		Array textures = input_obj["TEXTURES"].getArray();
		for (uint i = 0; i < textures.size(); i++) {
			Texture* texture = parseTexture(textures[i]);
			if (!texture) return false;
			scene.textures.push_back(texture);
		}
	}
	// Hitables
	Array hitables = input_obj["OBJECTS"].getArray();
	return parseHitables(hitables, scene);
}