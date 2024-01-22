// This is the interface of the renderer. If you want to build a new backend (for example Apple Metal)
// then you have to implement against this interface.

#ifndef _IRENDER_H_
#define _IRENDER_H_

class IRender {
public:
	virtual void Init(void)			= 0;
	virtual void Shutdown(void)		= 0;
	virtual void RegisterModel()	= 0;

private:

};

#endif
