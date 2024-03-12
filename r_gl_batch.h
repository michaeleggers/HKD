#ifndef _R_GL_BATCH_H_
#define _R_GL_BATCH_H_

#include <stdint.h>

#include <vector>

#include <glad/glad.h>

#include <string>

#include "r_common.h"
#include "r_gl_texture.h"
#include "irender.h"


class GLBatch {
public:
	GLBatch(uint32_t maxTris);

	int				Add(Tri* tris, uint32_t numTris, bool cullFace = true, DrawMode drawMode = DRAW_MODE_SOLID);
	int				Add(Vertex* verts, uint32_t numVerts, bool cullFace = true, DrawMode drawMode = DRAW_MODE_LINES);
	void			Bind();
	void			Reset();
	void			Kill();

	uint32_t							VertCount();
	
private:
	GLuint		m_VBO, m_VAO;

	uint32_t	m_MaxVerts;
	uint32_t	m_NumVerts;
	int			m_VertOffsetIndex;	
};

#endif
