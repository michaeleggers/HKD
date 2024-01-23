#ifndef _RGL_H_
#define _RGL_H_

#include "irender.h"
#include "r_common.h"
#include "r_model.h"

#include <vector>

#include <SDL.h>
#include <glad/glad.h>

#include "r_gl_batch.h"

// Models for entities (Players, Monsters, Pickup Items...)
struct GLModel {
	GLuint				VAO, VBO;
	std::vector<Vertex> vertices;
	int					textureID;
};

class GLRender : public IRender {
public:
	virtual bool Init(void)													override;
	virtual void Shutdown(void)												override;
	virtual int  RegisterModel(HKD_Model* model)							override;
	virtual void Render(void)												override;

private:
	SDL_Window* m_Window;

	GLBatch* m_ModelBatch;
};

#endif
