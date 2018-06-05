#ifndef __SCENE__
#define __SCENE__

#include "../Material/Material.h"
#include "../Material/Texture.h"
#include "../Hitable/Hitable.h"
#include "../Light/Light.h"

#include <vector>

using std::vector;

struct Scene {

	vector<Hitable*> hitables;
	vector<Light*> lights;
	vector<Material*> materials;
	vector<Texture*> textures;
	Color ambientColor;

	~Scene() {
		for (uint i = 0; i < hitables.size(); i++)
			delete hitables[i];
		for (uint i = 0; i < lights.size(); i++)
			delete lights[i];
		for (uint i = 0; i < materials.size(); i++)
			delete materials[i];
		for (uint i = 0; i < textures.size(); i++)
			delete textures[i];
	}

};

#endif