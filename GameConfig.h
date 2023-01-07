#ifndef _KICKITUP_GAMECONFIG_H
#define _KICKITUP_GAMECONFIG_H

#include <algorithm>
#include "common.h"

using std::max;
using std::min;

enum	ePlayer
{
	eP_Min,
	eP_1 = eP_Min,
	eP_2,
	eP_Max
};

struct stStepSpeed {
	int		step;

public:
	stStepSpeed() : step(2) {
	}

	void set(const int speed = 1) {
		step = speed;
	}

	int getMax() const {
		int ret = step;
		return ret;
	}

	int getMin() const {
		int ret = step;
		return ret;
	}

	void setRandom()  {
		step = 1+rand() % 8;
	}
};

class GameConfig
{
private:
	bool		m_bStart[eP_Max];		// 1p 2p
	stStepSpeed	m_StepSpped[eP_Max];

	bool	m_stageBreak;		//< STAGE BREAK

	bool	m_autoKey[eP_Max][eBA_Max];	//< 1p, 2p auto key
	
	int		m_keySet;			//< KEY SETTING NORMAL, FUSION
	bool	m_judgePic;			//< JUDGEMENT PICTURE
	int		m_joySet;			//< DDR(0), KOINS(1)

public:
	GameConfig(void);
	~GameConfig(void);

	bool	IsStarted( const ePlayer p )	const	{	return m_bStart[p];	}
	void	SetStart( const ePlayer p, const bool bStart = true )	{	m_bStart[p] = bStart;	}
	void	SetSpeed( const ePlayer p, const int spped )	{	m_StepSpped[p].set(spped);	}

	bool SaveToFile();
	bool LoadFromFile();

	bool	TogleStageBreak() {
		m_stageBreak = !m_stageBreak;
		return m_stageBreak;
	}
	bool	GetStageBreak() {
		return m_stageBreak;
	}

	bool	TogleJudgePic() {
		m_judgePic = !m_judgePic;
		return m_judgePic;
	}
	bool	GetJudgePic() {
		return m_judgePic;
	}

	int TogleKeySet() {
		m_keySet = (m_keySet + 1) % 2;
		return m_keySet;
	}

	int GetKeySet() {
		return m_keySet;
	}

	const char * GetKeySetStr() {
		if (1 == m_keySet) {
			return "FUSION";
		}
		return "NORMAL";
	}

	int TootleJoySet() {
		m_joySet = (m_joySet + 1) % 2;
		return m_joySet;
	}

	int GetJoySet() {
		return m_joySet;
	}

	const char* GetJoySetStr() {
		if (1 == m_joySet) {
			return "KOINS";
		}		
		return "DDR";
	}

	bool TotleAutoKey(ePlayer p, eButtonToArray b) {		
		m_autoKey[p][b] = !m_autoKey[p][b];
		return m_autoKey[p][b];		
	}

	bool GetAutoKey(ePlayer p, eButtonToArray b) {		
		return m_autoKey[p][b];
	}
};
extern	GameConfig	g_GameConfig;

#endif // _KICKITUP_GAMECONFIG_H
