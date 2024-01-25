#ifndef _GAME_H_
#define _GAME_H_

#include <string>

#include "irender.h" 

class Game {
public:
	Game(std::string exePath, IRender* renderer);

	void Init();
	bool RunFrame();
	void Shutdown();

private:
	IRender*	m_Renderer;
	std::string m_ExePath;

	HKD_Model   m_Model;
};

#endif

