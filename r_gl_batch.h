#ifndef _R_GL_BATCH_H_
#define _R_GL_BATCH_H_

#include <stdint.h>

#include <vector>

#include <glad/glad.h>

#include <string>

#include "r_common.h"
#include "r_gl_texture.h"


struct GLBatchDrawCmd {
	int		 offset;
	uint32_t numTris;	
	bool     cullFace;
};

class GLBatch {
public:
	GLBatch(uint32_t maxTris);

	GLBatchDrawCmd	Add(Tri* tris, uint32_t numTris, bool cullFace = true);
	void			Bind();
	void			Reset();
	void			Kill();

	uint32_t							TriCount();
	const std::vector<GLBatchDrawCmd>&	DrawCmds();
	
private:
	GLuint		m_VBO, m_VAO;

	uint32_t	m_MaxTris;
	uint32_t	m_NumTris;
	int			m_TriOffsetIndex;

	std::vector<GLBatchDrawCmd> m_DrawCmds;
};

#endif
