#ifndef _R_GL_BATCH_H_
#define _R_GL_BATCH_H_

#include <stdint.h>

#include <vector>

#include <glad/glad.h>

#include "r_common.h"

struct GLBatchDrawCmd {
	int		 offset;
	uint32_t numTris;
};

class GLBatch {
public:
	GLBatch(uint32_t maxTris);

	int     	Add(Tri* tris, uint32_t numTris);
	void		Bind();
	void		Kill();

	uint32_t							TriCount();
	const std::vector<GLBatchDrawCmd>&	DrawCmds();
	
private:
	GLuint m_VBO, m_VAO;

	uint32_t m_MaxTris;
	uint32_t m_NumTris;
	int		 m_TriOffsetIndex;

	std::vector<GLBatchDrawCmd> m_DrawCmds;
};

#endif
