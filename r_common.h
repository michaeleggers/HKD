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


struct Tri {
	union {
		struct { Vertex a, b, c; };
		Vertex vertices[3];
	};
};

struct Quad {
	union {
		struct {
			Tri a;
			Tri b;
		};
		Tri		tris[2];
		Vertex  vertices[6];
	};
};

struct Box {
	union {
		struct {
			Quad front;
			Quad right;
			Quad back;
			Quad left;
			Quad top;
			Quad bottom;
		};
		Quad quads[6];
		Tri  tris[12];
	};
};

void RotateTri(Tri* tri, glm::vec3 axis, float angle);
void TranslateTri(Tri* tri, glm::vec3 t);
void TransformTri(Tri* tri, glm::mat4 modelMatrix);
Quad CreateQuad(glm::vec3 pos = glm::vec3(0, 0, 0), float width = 2.0f, float height = 2.0f, glm::vec4 color = glm::vec4(1, 0, 0, 1));
void RotateQuad(Quad* quad, glm::vec3 axis, float angle);
void TranslateQuad(Quad* quad, glm::vec3 t);
void SetQuadColor(Quad* quad, glm::vec4 color);
Box	 CreateBox(glm::vec3 scale = glm::vec3(1.0f), glm::vec4 color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
Box  CreateBoxFromAABB(glm::vec3 mins, glm::vec3 maxs);
void TranslateBox(Box* box, glm::vec3 t);
void TransformBox(Box* box, glm::mat4 modelMatrix);

#endif

