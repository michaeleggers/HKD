// This is the interface of the renderer. If you want to build a new backend (for example Apple Metal)
// then you have to implement against this interface.

#ifndef _IRENDER_H_
#define _IRENDER_H_

#include "irender.h"

#include "r_model.h"

class IRender {
public:
	virtual bool Init(void)			= 0;
	virtual void Shutdown(void)		= 0;
	virtual int  RegisterModel(Tri* tris, uint32_t triCount, int textureID)	= 0;
	virtual void Render(void) = 0;

private:

};

#endif
