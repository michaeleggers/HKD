// This is the interface of the renderer. If you want to build a new backend (for example Apple Metal)
// then you have to implement against this interface.

#ifndef _IRENDER_H_
#define _IRENDER_H_

#include <vector>

#include "irender.h"
#include "r_itexture.h"
#include "r_model.h"

class IRender {
public:
	virtual bool Init(void)			= 0;
	virtual void Shutdown(void)		= 0;
	virtual int  RegisterModel(HKD_Model* model)	= 0;
	virtual std::vector<ITexture*> ModelTextures(int gpuModelHandle) = 0;
	virtual void RenderBegin(void) = 0;
	virtual void Render(std::vector<HKD_Model*>& models) = 0;
	virtual void RenderEnd(void) = 0;

private:

};

#endif
