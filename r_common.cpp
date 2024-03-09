#include "r_common.h" 

#include <math.h>

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

void SubdivTri(Tri* tri, Tri out_tris[])
{	
	glm::vec3 A = tri->a.pos;
	glm::vec3 B = tri->b.pos;
	glm::vec3 C = tri->c.pos;

	// Vectors from vert to vert in clockwise order

	glm::vec3 AB = B - A;
	glm::vec3 BC = C - B;
	glm::vec3 CA = A - C;

	// Midpoints between the points on half distance

	glm::vec3 mAB = A + 0.5f * AB;
	glm::vec3 mBC = B + 0.5f * BC;
	glm::vec3 mCA = C + 0.5f * CA;

	// Colors between points

	glm::vec4 cAB = 0.5f * tri->a.color + 0.5f * tri->b.color;
	glm::vec4 cBC = 0.5f * tri->b.color + 0.5f * tri->c.color;
	glm::vec4 cCA = 0.5f * tri->c.color + 0.5f * tri->a.color;

	// New tris

	Tri t1 = { .vertices = { {.pos = A}, {.pos = mAB}, {.pos = mCA} } };
	Tri t2 = { .vertices = { {.pos = mAB}, {.pos = B}, {.pos = mBC} } };
	Tri t3 = { .vertices = { {.pos = mBC}, {.pos = C}, {.pos = mCA} } };
	Tri t4 = { .vertices = { {.pos = mCA}, {.pos = mAB}, {.pos = mBC} } };
	t1.a.color = tri->a.color;
	t1.b.color = cAB;
	t1.c.color = cCA;
	t2.a.color = cAB;
	t2.b.color = tri->b.color;
	t2.c.color = cBC;
	t3.a.color = cBC;
	t3.b.color = tri->c.color;
	t3.c.color = cCA;
	t4.a.color = cCA;
	t4.b.color = cAB;
	t4.c.color = cBC;

	out_tris[0] = t1;
	out_tris[1] = t2;
	out_tris[2] = t3;
	out_tris[3] = t4;
}

void SubdivTri(Tri* tri, Tri out_tris[], uint32_t numIterations)
{
	// each iteration makes one tri to four tris -> 4 tris -> 16 tris and so on.
	// so each iteration will multiply the current tricount by 4.

	uint32_t maxTris = pow(4, numIterations);
	
	//SubdivTri(tri, out_tris);	

	Tri* tmp = (Tri*)malloc(maxTris * sizeof(Tri));
	out_tris[0] = *tri;
	tmp[0] = *tri;
	uint32_t numTris = 1;
	for (int i = 0; i < numIterations; i++) {		
		for (int j = 0; j < numTris; j++) {
			SubdivTri(&out_tris[j], tmp + j*4);
		}
		numTris <<= 2;
		memcpy(out_tris, tmp, numTris * sizeof(Tri));
	}
	free(tmp);
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

