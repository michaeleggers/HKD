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

#define VERT_POS_OFFSET			 0
#define VERT_UV_OFFSET			 (VERT_POS_OFFSET + sizeof(glm::vec3))
#define VERT_BC_OFFSET			 (VERT_UV_OFFSET + sizeof(glm::vec2))
#define VERT_NORMAL_OFFSET		 (VERT_BC_OFFSET + sizeof(glm::vec3))
#define VERT_COLOR_OFFSET		 (VERT_NORMAL_OFFSET + sizeof(glm::vec3))
#define VERT_BLENDINDICES_OFFSET (VERT_COLOR_OFFSET + sizeof(glm::vec4))
#define VERT_BLENDWEIGHTS_OFFSET (VERT_BLENDINDICES_OFFSET + 4*sizeof(uint32_t))

union Tri {
	struct {
		Vertex a, b, c;
	};
	Vertex vertices[3];
};

#endif

