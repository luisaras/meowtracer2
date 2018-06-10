#include "Texture.h"
#include "../Math/Perlin.h"

Color CheckersTexture::getColor(float u, float v, Point3 &p) {
  int x = floor(u * repeatx);
  int y = repeaty - floor(v * repeaty);
  x = ((x % repeatx) + repeatx) % repeatx;
  y = ((y % repeaty) + repeaty) % repeaty;
  if ((x + y) % 2)
    return color1;
  else
    return color2;
}

Color ImageTexture::getColor(float u, float v, Point3 &p) {
  int x = rint(u * image.width);
  int y = image.height - rint(v * image.height);
  x = ((x % image.width) + image.width) % image.width;
  y = ((y % image.height) + image.height) % image.height;
  return image.getColor(x % image.width, y % image.height);
}

Color PerlinTexture::getColor(float u, float v, Point3 &p) {
  return Vec3(1, 1, 1) * 0.5 * (1 + sin(scale*p.x + 5 * perlin(scale*p)));
}