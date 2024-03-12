// This is the interface of the renderer. If you want to build a new backend (for example Apple Metal)
// then you have to implement against this interface.

#ifndef _IRENDER_H_
#define _IRENDER_H_

#include <vector>

#include "irender.h"
#include "r_itexture.h"
#include "r_model.h"
#include "camera.h"
#include "r_common.h"

enum DrawMode {
	DRAW_MODE_SOLID,
	DRAW_MODE_WIREFRAME,
	DRAW_MODE_LINES
};

struct GLBatchDrawCmd { // TODO: Rename
	int		 offset;
	uint32_t numVerts;
	bool     cullFace;
	DrawMode drawMode;
};

class IRender {
public:
	virtual bool Init(void)			= 0;
	virtual void Shutdown(void)		= 0;
	virtual int  RegisterModel(HKD_Model* model)	= 0;
	virtual std::vector<ITexture*> ModelTextures(int gpuModelHandle) = 0;
	virtual std::vector<ITexture*> Textures() = 0;
	virtual void ImDrawTris(Tri* tris, uint32_t numTris, bool cullFace = true, DrawMode drawMode = DRAW_MODE_SOLID) = 0;
	virtual void ImDrawVerts(Vertex* verts, uint32_t numVerts) = 0;
	virtual void ImDrawLines(Vertex* verts, uint32_t numVerts, bool close = false) = 0;
	virtual void RenderBegin(void) = 0;
	virtual void Render(Camera* camera, std::vector<HKD_Model>& models) = 0;
	virtual void RenderEnd(void) = 0;

private:

};

#endif
