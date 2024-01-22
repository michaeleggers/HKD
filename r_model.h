#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>

#include <vector>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#include "iqm_loader.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 uv;
	glm::vec3 bc;
	glm::vec3 normal;
	glm::vec3 color;
	uint32_t  blendindices[4];
	glm::vec4 blendweights;
};

union Tri {
	struct {
		Vertex a, b, c;
	};
	Vertex vertices[3];
};

struct Model {
	std::vector<Tri> tris;
	int				 textureID;
};

Model CreateModelFromIQM(IQMModel* model);

#endif
