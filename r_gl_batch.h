#ifndef _R_GL_BATCH_H_
#define _R_GL_BATCH_H_

#include <stdint.h>

#include <glad/glad.h>

#include "r_common.h"

class GLBatch {
public:
	GLBatch(uint32_t numTris);

	int     	Add(Tri* tris, uint32_t numTris);
	void		Bind();
	void		Kill();

	uint32_t	TriCount();
	
private:
	GLuint m_VBO, m_VAO;

	uint32_t m_NumTris;
	int		 m_TriOffsetIndex;
};

#endif
