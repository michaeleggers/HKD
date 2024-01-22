#ifndef _RGL_H_
#define _RGL_H_

#include "irender.h"

class GLRender : public IRender {
public:
	virtual void Init(void)			override;
	virtual void Shutdown(void)		override;
	virtual void RegisterModel()	override;

private:
};

#endif
