#include "Mesh.h"
#define rad2Deg 180 / 3.14159265359

using std::stoi;
using std::stof;
using std::cout;
using std::endl;

template<typename T>
T& getElement(vector<T> &v, int i, int off = 0) {
	int n = v.size() - off;
	if (i < 0) {
		i = (-i - 1) % n;
		return v[n - 1 - i];
	} else {
		return v[i - off];
	}
}

Vec3 Mesh::getCoord(int i) { 
	if (vertices[i].coord == 0) {
		cout << "Null vertex coordinates." << endl;
		return Vec3(NAN, NAN, NAN);
	}
	return getElement(coords, vertices[i].coord, 1);
}

Vec3 Mesh::getNormal(int i) { return getElement(normals, vertices[i].normal); }

Vec2 Mesh::getTexUV(int i) { return getElement(texUVs, vertices[i].tex); }

Vec3 Mesh::getColor(int i) { return getElement(colors, vertices[i].color); }

void getIDs(const string& word, vector<int>& v) {
	string buff {""};
	for(auto c : word) {
		if (c != '/') {
			buff += c; 
		} else {
			v.push_back(buff == "" ? 0 : stoi(buff));
			buff = ""; 
		}
	}
	v.push_back(buff == "" ? 0 : stoi(buff));
	while (v.size() < 4)
		v.push_back(0);
}

void Mesh::clear() {
	coords.clear();
	normals.clear();
	colors.clear();
	texUVs.clear();
	vertices.clear();
	normals.push_back(Vec3(NAN, NAN, NAN));
	texUVs.push_back(Vec2(0, 0));
	colors.push_back(Vec3(0, 0, 0));
}

void Mesh::transform(Matrix4& xform) {
	for (uint i = 0; i < coords.size(); i++) {
		coords[i] = xform.transformPoint(coords[i]);
	}
	for (uint i = 0; i < normals.size(); i++) {
		normals[i] = xform.transformVector(normals[i]);
	}
}

void Mesh::getTriangles(vector<Triangle>& v, Material* mat) {
	Material* meshMat = mat == 0 ? material : mat;
	for(uint i = 0; i < vertices.size(); i += 3) {
		Vec3 v1 = getCoord(i);
		Vec3 v2 = getCoord(i+1);
		Vec3 v3 = getCoord(i+2);
		Triangle t(v1, v2, v3);
		if (vertices[i].normal == 0) {
			t.norm[0] = t.norm[1] = t.norm[2] = t.calculateNormal();
		} else {
			t.norm[0] = getNormal(i);
			t.norm[1] = getNormal(i+1);
			t.norm[2] = getNormal(i+2);
		}
		t.rad[0] = getColor(i);
		t.rad[1] = getColor(i+1);
		t.rad[2] = getColor(i+2);
		t.tex[0] = getTexUV(i);
		t.tex[1] = getTexUV(i+1);
		t.tex[3] = getTexUV(i+2);
		t.material = meshMat;
		v.push_back(t);
	}
}

bool Mesh::inTriangle(Vertex& v, Vertex& v1, Vertex& v2, Vertex& v3) {
	Vec3& point = getElement(coords, v.coord, 1);
	Vec3& tri1 = getElement(coords, v1.coord, 1);
	Vec3& tri2 = getElement(coords, v2.coord, 1);
	Vec3& tri3 = getElement(coords, v3.coord, 1);
	Triangle t(tri1, tri2, tri3);
	return t.isInside(point);
}

float Mesh::angleBetween(Vertex& v1, Vertex& v2, Vertex& v3) {
	Vec3& p1 = getElement(coords, v1.coord, 1);
	Vec3& p2 = getElement(coords, v2.coord, 1);
	Vec3& p3 = getElement(coords, v3.coord, 1);
	Vec3 a = p1 - p2;
	Vec3 b = p3 - p2;
	float angle = Vec3::dot(a, b);
	return angle / (a.length() * b.length()) * rad2Deg;
}

string Mesh::loadFace(vector<string> &words) {
	if (words.size() < 4)
		return "Not enough face vertices.";
	vector<Vertex> vert(0);
	for (uint i = 1; i < words.size(); i ++) {
		vector<int> ids;
		getIDs(words[i], ids);
		vert.push_back(Vertex(ids[0], ids[2], ids[1], ids[3]));
	}
	if (vert.size() == 3) {
		vertices.push_back(vert[0]);
		vertices.push_back(vert[1]);
		vertices.push_back(vert[2]);
		return "";
	}
	vector<Vertex> tvert = vert;
	while (true) {
		for(uint i = 0; i < tvert.size(); i++) {
			Vertex& prev = i == 0 ? tvert[tvert.size() - 1] : tvert[i - 1];
			Vertex& curr = tvert[i];
			Vertex& next = i == tvert.size() - 1 ? tvert[0] : tvert[i + 1];
			if (tvert.size() <= 4) {
				for (uint j = 0; j < tvert.size(); j++) {
					if (tvert[j].coord == curr.coord)
						vertices.push_back(tvert[j]);
					if (tvert[j].coord == prev.coord)
						vertices.push_back(tvert[j]);
					if (tvert[j].coord == next.coord)
						vertices.push_back(tvert[j]);
				}
				if (tvert.size() == 4) {
					int temp = 0;
					for (uint j = 0; j < tvert.size(); j++) {
						if (tvert[j].coord != curr.coord
								&& tvert[j].coord != prev.coord
								&& tvert[j].coord != next.coord) {
							temp = tvert[j].coord;
							break;
						}
					}
					for (uint j = 0; j < tvert.size(); j++) {
						if (tvert[j].coord == temp)
							vertices.push_back(tvert[j]);
						if (tvert[j].coord == prev.coord)
							vertices.push_back(tvert[j]);
						if (tvert[j].coord == next.coord)
							vertices.push_back(tvert[j]);
					}
				}
				tvert.clear();
				break;
			}
			// If Vertex is not an interior vertex
			float angle = angleBetween(prev, curr, next);
			if (angle <= 0 && angle >= 180)
				continue;
			// If any vertices are within this triangle
			bool inside = false;
			for (int j = 0; j < int(vert.size()); j++) {
				if (vert[j].coord != prev.coord &&
						vert[j].coord != curr.coord &&
						vert[j].coord != next.coord &&
						inTriangle(vert[j], prev, curr, next)) {
					inside = true;
					break;
				}
			}
			if (inside) continue;
			// Create a triangle from curr, prev, next
			for (uint j = 0; j < tvert.size(); j++) {
				if (tvert[j].coord == curr.coord)
					vertices.push_back(tvert[j]);
				if (tvert[j].coord == prev.coord)
					vertices.push_back(tvert[j]);
				if (tvert[j].coord == next.coord)
					vertices.push_back(tvert[j]);
			}
			tvert.erase(tvert.begin() + i);
			break;
		}
		if (tvert.size() == 0)
			break;
	}
	return "";
}