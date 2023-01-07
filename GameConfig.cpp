#include "GameConfig.h"
#include <algorithm>

GameConfig	g_GameConfig;

GameConfig::GameConfig()
{
	std::fill( &m_bStart[0], &m_bStart[eP_Max], false );	

	for (int i = eP_Min; i < eP_Max; ++i) {
		std::fill(&m_autoKey[i][0], &m_autoKey[i][eBA_Max], false);		
	}
	
	m_keySet = 0;		// KEY SETTING NORMAL
	m_judgePic = true;	// JUDGEMENT PICTURE
	m_joySet = 0;		// DDR(0)
}

GameConfig::~GameConfig()
{
}
