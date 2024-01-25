#ifndef _RGL_H_
#define _RGL_H_

#include "irender.h"

#include <vector>

#include <SDL.h>
#include <glad/glad.h>

#include "r_common.h"
#include "r_model.h"
#include "r_gl_batch.h"
#include "r_gl_shader.h"
#include "r_gl_texture_mgr.h"
#include "r_gl_texture.h"

struct GLMesh {
	int			triOffset, triCount; // Offsets into VBO of tris
	GLTexture*	texture;
};

// Models for entities (Players, Monsters, Pickup Items...)
struct GLModel {
	std::vector<GLMesh> meshes;
};

class GLRender : public IRender {
public:
	virtual bool Init(void)								override;
	virtual void Shutdown(void)							override;
	virtual int  RegisterModel(HKD_Model* model)		override;
	virtual std::vector<ITexture*> GetTextureHandles(int gpuModelHandle)	override;
	virtual void RenderBegin(void)						override;
	virtual void Render(void)							override;
	virtual void RenderEnd(void)						override;

	void InitShaders();

private:	
	SDL_Window*				m_Window;
	SDL_GLContext			m_SDL_GL_Conext;

	GLTextureManager*		m_TextureManager;

	GLBatch*				m_ModelBatch;
	Shader*					m_ModelShader;
	std::vector<GLModel>	m_Models;

	int						m_WindowWidth;
	int						m_WindowHeight;
};

#endif
