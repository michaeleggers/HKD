#ifndef _RCOMMON_H_
#define _RCOMMON_H_

#include <stdint.h>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

struct Vertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 bc;
	glm::vec3 normal;
	glm::vec4 color;
	uint32_t  blendindices[4];
	glm::vec4 blendweights;
};

union Tri {
	struct {
		Vertex a, b, c;
	};
	Vertex vertices[3];
};

#endif

