#ifndef _RGL_H_
#define _RGL_H_

#include "r_model.h"
#include "irender.h"

#include <vector>

#include <SDL.h>
#include <glad/glad.h>

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
	virtual int  RegisterModel(Tri* tris, uint32_t triCount, int textureID)	override;
	virtual void Render(void)												override;

private:
	SDL_Window* m_Window;
};

#endif
