
#pragma once

#include "../Math/Vec3.h"
#include "../Image/Image.h"
#include <vector>
#include <cmath>

class Texture {
public:
	Texture() {}
  virtual Color getColor(float u, float v, Point3 &p) { return Color(1, 1, 1); }
  virtual ~Texture() {}
};

class CheckersTexture : public Texture {
private:
  Color color1, color2;
  int repeatx, repeaty;
public:
  CheckersTexture(Color &c1, Color &c2, int rx, int ry) : 
    color1(c1), color2(c2), repeatx(rx), repeaty(ry) {}
  Color getColor(float u, float v, Point3 &p);
};

class PerlinTexture : public Texture {
private:
  float scale;
public:
  PerlinTexture(float s) : scale(s) {}
  Color getColor(float u, float v, Point3 &p);
};

class ImageTexture : public Texture {
private:
  Image image;
public:
  ImageTexture(std::string &fileName) : image(fileName) {}
  Color getColor(float u, float v, Point3 &p);
};