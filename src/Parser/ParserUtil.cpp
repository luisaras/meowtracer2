#include "ParserUtil.h"

Vec3 parseVec3(Value &vec_js) {
  Array arr = vec_js.getArray();
  float x = arr[0].getReal();
  float y = arr[1].getReal();
  float z = arr[2].getReal();
  return Vec3(x, y, z);
}

Matrix4 parseTransform(json_spirit::Value &xform_js) {
	Array arr = xform_js.getArray();
	Matrix4 transform = Matrix4::identity();
	for (uint i = 0; i < arr.size(); i++) {
		Object xform_obj = arr[i].getObject();
		if (xform_obj.count("PIVOT")) {
			Vec3 v = -parseVec3(xform_obj["PIVOT"]);
			Matrix4 t = Matrix4::translation(v);
			transform = transform * t;
		} 
		if (xform_obj.count("SCALE")) {
			Vec3 v = parseVec3(xform_obj["SCALE"]);
			Matrix4 t = Matrix4::scaling(v);
			transform = transform * t;
		} 
		if (xform_obj.count("ROTATION")) {
			Vec3 v = parseVec3(xform_obj["ROTATION"]);
			Matrix4 t = Matrix4::rotation(v.x * DEG2RAD, v.y * DEG2RAD, v.z * DEG2RAD);
			transform = transform * t;
		} 
		if (xform_obj.count("TRANSLATION")) {
			Vec3 v = parseVec3(xform_obj["TRANSLATION"]);
			Matrix4 t = Matrix4::translation(v);
			transform = transform * t;
		} 
		if (xform_obj.count("MATRIX")) {
			Array t = xform_obj["MATRIX"].getArray();
			Matrix4 m;
			for (int j = 0; j < 16; j++) {
				m[j / 4][j % 4] = t[j].getReal();
			}
			transform = transform * m;
		}
	}
	return transform;
}