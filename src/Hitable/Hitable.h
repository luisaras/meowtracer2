#ifndef __HITABLE__
#define __HITABLE__

#include "../Math/Vec2.h"
#include "../Math/Ray.h"
#include "../Material/Material.h"
#include "../Material/Texture.h"
#include "../Tree/Box.h"

struct RayHit;

class Hitable {
public:
	Material* material;
	Texture* texture;
	virtual RayHit hit(Ray& ray) = 0;
	virtual Box hitBox() = 0;
	virtual Color getTexture(Vec2& uv) = 0;
	virtual ~Hitable() {}
};

struct RayHit {
	Hitable* hitable = 0;
	float t = NAN;
	Vec3 point, normal;
	Vec2 uv;
};

#endif