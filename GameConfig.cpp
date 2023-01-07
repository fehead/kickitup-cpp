#include "GameConfig.h"
#include <algorithm>
#include <iostream>
#include <fstream>

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
	m_stageBreak = true;
}

GameConfig::~GameConfig()
{
}


bool GameConfig::SaveToFile()
{	
	std::ofstream	cfgFile("kiu.cfg");
	if (!cfgFile.is_open())
		return false;
	
	cfgFile.write(reinterpret_cast<const char*>(&m_stageBreak), sizeof(m_stageBreak));	
	cfgFile.write(reinterpret_cast<const char*>(&m_judgePic), sizeof(m_judgePic));	
	cfgFile.write(reinterpret_cast<const char*>(&m_keySet), sizeof(m_keySet));
	cfgFile.write(reinterpret_cast<const char*>(&m_joySet), sizeof(m_joySet));
	cfgFile.write(reinterpret_cast<const char*>(&m_autoKey), sizeof(m_autoKey));

	cfgFile.close();

	return true;
}

bool GameConfig::LoadFromFile()
{
	std::ifstream	cfgFile("kiu.cfg");
	if (!cfgFile.is_open())
		return false;

	cfgFile.read(reinterpret_cast<char*>(&m_stageBreak), sizeof(m_stageBreak));
	cfgFile.read(reinterpret_cast<char*>(&m_judgePic), sizeof(m_judgePic));
	cfgFile.read(reinterpret_cast<char*>(&m_keySet), sizeof(m_keySet));
	cfgFile.read(reinterpret_cast<char*>(&m_joySet), sizeof(m_joySet));
	cfgFile.read(reinterpret_cast<char*>(&m_autoKey), sizeof(m_autoKey));

	cfgFile.close();

	return true;
}
