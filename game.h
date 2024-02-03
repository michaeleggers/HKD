#ifndef _GAME_H_
#define _GAME_H_

#include <string>

#include "irender.h" 
#include "camera.h"
#include "hkd_interface.h"

class Game {
public:
	Game(std::string exePath, hkdInterface* interface, IRender* renderer);

	void Init();
	bool RunFrame(double dt);
	void Shutdown();

private:
	IRender*		m_Renderer;
	hkdInterface*	m_Interface;
	std::string		m_ExePath;

	HKD_Model		m_Model;
	HKD_Model		m_Model2;
	HKD_Model		m_Model3;

	Camera			m_Camera;
	Camera			m_FollowCamera;

	std::vector<HKD_Model*> m_Models;
	
	double			m_AccumTime;
};

#endif

