#include "r_common.h" 

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"
#include "dependencies/glm/gtx/quaternion.hpp"

void RotateTri(Tri* tri, glm::vec3 axis, float angle)
{
	glm::quat q = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
	tri->a.pos = glm::rotate(q, tri->a.pos);
	tri->b.pos = glm::rotate(q, tri->b.pos);
	tri->c.pos = glm::rotate(q, tri->c.pos);
}

void TranslateTri(Tri* tri, glm::vec3 t)
{
	tri->a.pos += t;
	tri->b.pos += t;
	tri->c.pos += t;
}

void TransformTri(Tri* tri, glm::mat4 modelMatrix)
{
	tri->a.pos = modelMatrix * glm::vec4(tri->a.pos, 1.0f);
	tri->b.pos = modelMatrix * glm::vec4(tri->b.pos, 1.0f);
	tri->c.pos = modelMatrix * glm::vec4(tri->c.pos, 1.0f);
}

Quad CreateQuad(glm::vec3 pos, float width, float height, glm::vec4 color)
{
	Quad result = {};
	Tri upperRight = { };
	Tri lowerLeft = { };

	float halfWidth =  width / 2.0f;
	float halfHeight = height / 2.0f;

	upperRight.a.pos = glm::vec3(-1.0f * halfWidth, 0.0f, 1.0f * halfHeight) + pos;
	upperRight.a.color = color;
	upperRight.a.bc = glm::vec3(1.0f, 0.0f, 0.0f);
	upperRight.b.pos = glm::vec3(1.0f * halfWidth, 0.0f, 1.0f * halfHeight) + pos;
	upperRight.b.color = color;
	upperRight.b.bc = glm::vec3(0.0f, 1.0f, 0.0f);
	upperRight.c.pos = glm::vec3(1.0f * halfWidth, 0.0f, -1.0f * halfHeight) + pos;
	upperRight.c.color = color;
	upperRight.c.bc = glm::vec3(0.0f, 0.0f, 1.0f);
	lowerLeft.a.pos = glm::vec3(1.0f * halfWidth, 0.0f, -1.0f * halfHeight) + pos;
	lowerLeft.a.color = color;
	lowerLeft.a.bc = glm::vec3(1.0f, 0.0f, 0.0f);
	lowerLeft.b.pos = glm::vec3(-1.0f * halfWidth, 0.0f, -1.0f * halfHeight) + pos;
	lowerLeft.b.color = color;
	lowerLeft.b.bc = glm::vec3(0.0f, 1.0f, 0.0f);
	lowerLeft.c.pos = glm::vec3(-1.0f * halfWidth, 0.0f, 1.0f * halfHeight) + pos;
	lowerLeft.c.color = color;
	lowerLeft.c.bc = glm::vec3(0.0f, 0.0f, 1.0f);

	result.a = upperRight;
	result.b = lowerLeft;

	return result;
}

void RotateQuad(Quad* quad, glm::vec3 axis, float angle)
{
	RotateTri(&quad->a, axis, angle);
	RotateTri(&quad->b, axis, angle);
}

void TranslateQuad(Quad* quad, glm::vec3 t)
{
	TranslateTri(&quad->a, t);
	TranslateTri(&quad->b, t);
}

void SetQuadColor(Quad* quad, glm::vec4 color)
{
	// Tri 1

	quad->a.vertices[0].color = color;
	quad->a.vertices[1].color = color;
	quad->a.vertices[2].color = color;

	// Tri 2

	quad->b.vertices[0].color = color;
	quad->b.vertices[1].color = color;
	quad->b.vertices[2].color = color;
}

//Quad front;
//Quad right;
//Quad back;
//Quad left;
//Quad top;
//Quad bottom;
Box CreateBox(glm::vec3 scale, glm::vec4 color)
{
	Box result = {};

	float halfWidth  = scale.x / 2.0f;
	float halfDepth  = scale.y / 2.0f;
	float halfHeight = scale.z / 2.0f;

	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

	Quad front =  CreateQuad(glm::vec3(0.0f), scale.x, scale.z, color);
	Quad right =  CreateQuad(glm::vec3(0.0f), scale.y, scale.z, color);
	Quad back =   CreateQuad(glm::vec3(0.0f), scale.x, scale.z, color);
	Quad left =   CreateQuad(glm::vec3(0.0f), scale.y, scale.z, color);
	Quad top =    CreateQuad(glm::vec3(0.0f), scale.x, scale.y, color);
	Quad bottom = CreateQuad(glm::vec3(0.0f), scale.x, scale.y, color);
	
	RotateQuad(&right, up, 90.0f);
	RotateQuad(&back, up, 180.0f);
	RotateQuad(&left, up, -90.0f);
	RotateQuad(&top, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	RotateQuad(&bottom, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);

	TranslateQuad(&front, glm::vec3(0.0f, -1.0f * halfDepth, 0.0f));
	TranslateQuad(&right, glm::vec3(1.0f * halfWidth, 0.0f, 0.0f));
	TranslateQuad(&back, glm::vec3(0.0f, 1.0f * halfDepth, 0.0f));
	TranslateQuad(&left, glm::vec3(-1.0f * halfWidth, 0.0f, 0.0f));
	TranslateQuad(&top, glm::vec3(0.0f, 0.0f, 1.0f * halfHeight));
	TranslateQuad(&bottom, glm::vec3(0.0, 0.0, -1.0f * halfHeight));

	result.front  = front;
	result.right  = right;
	result.back   = back;
	result.left   = left;
	result.top    = top;
	result.bottom = bottom;

	return result;
}

Box CreateBoxFromAABB(glm::vec3 mins, glm::vec3 maxs)
{
	float width  = abs(maxs.x - mins.x);
	float depth  = abs(maxs.y - mins.y);
	float height = abs(maxs.z - mins.z);
	glm::vec3 posFix = mins + glm::vec3(width / 2.0f, depth / 2.0f, height / 2.0f);
	Box result = CreateBox(glm::vec3(width, depth, height), glm::vec4(1.0, 1.0f, 1.0f, 1.0f));
	TranslateBox(&result, posFix);

	return result;
}

void TranslateBox(Box* box, glm::vec3 t)
{
	for (int i = 0; i < 6; i++) {
		Quad* q = &box->quads[i];
		TranslateQuad(q, t);
	}
}

void TransformBox(Box* box, glm::mat4 modelMatrix)
{
	for (int i = 0; i < 6; i++) {
		Quad* q = &box->quads[i];
		for (int j = 0; j < 2; j++) {
			TransformTri(&q->tris[j], modelMatrix);
		}
	}
}

