#ifndef _RGL_H_
#define _RGL_H_

#include "irender.h"

#include <SDL.h>
#include <glad/glad.h>

class GLRender : public IRender {
public:
	virtual bool Init(void)			override;
	virtual void Shutdown(void)		override;
	//virtual void RegisterModel()	override;
	virtual void Render(void)       override;

private:
	SDL_Window* m_Window;
};

#endif
