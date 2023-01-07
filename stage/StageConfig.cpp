#include "StageConfig.h"
#include "../KickItUp.h"
#include "../video/Surface.h"
#include "../sound/Sound.h"
#include "../video/SurfaceStore.h"
#include "../sound/SoundStore.h"
#include "../input/InputManager.h"
#include "Context.h"
#include "../GameConfig.h"

StageConfig::StageConfig(Context& context) : m_context(context), m_pBG(0), m_pSmallFont(0)
	, m_curSelect(0)
{
}

StageConfig::~StageConfig(void)
{
}

bool StageConfig::Initialize()
{
	// loading image.
	m_pBG = g_pSurfaceStore->Order("config_back");	
	m_pBG->CreateRGB(0, 0, 0, 0);	// black background

	m_pSmallFont = g_pSurfaceStore->Order("s_font");
	if (!m_pSmallFont->Load("images/s_font.bmp")) {
		return false;
	}
	
	return true;
}

void StageConfig::Destroy()
{	
	m_pBG->Free();
}


bool StageConfig::Render(unsigned long delta)
{
#define	START_X		0
#define	FONT_SIZE_Y	16	

	m_pBG->Blit(0, 0);	// draw background.
	_drawMessage(40, START_X, "STAGE BREAK");
	_drawMessage(500, START_X, g_GameConfig.GetStageBreak() ? "ON" : "OFF");
	_drawMessage(40, START_X + FONT_SIZE_Y * 1, "JUDGEMENT PICTURE"); // 1
	_drawMessage(500, START_X + FONT_SIZE_Y * 1, g_GameConfig.GetJudgePic() ? "ON" : "OFF");
	_drawMessage(40, START_X + FONT_SIZE_Y * 2, "KEY SETTING : "); // 2
	_drawMessage(500, START_X + FONT_SIZE_Y * 2, g_GameConfig.GetKeySetStr());

	_drawMessage(40, START_X + FONT_SIZE_Y * 3, "JOY SETTING : "); // 3	 "DDR" or 
	_drawMessage(500, START_X + FONT_SIZE_Y * 3, g_GameConfig.GetJoySetStr());

	_drawMessage(40, START_X + FONT_SIZE_Y * 4, "");
	_drawMessage(40, START_X + FONT_SIZE_Y * 5, "");

	_drawMessage(40, START_X + FONT_SIZE_Y * 6, "AUTO KEY FOR 1P"); // 6

	_drawMessage(324, START_X + FONT_SIZE_Y * 7, "DOWN LEFT :"); // 7
	_drawMessage(500, START_X + FONT_SIZE_Y * 7, g_GameConfig.GetAutoKey(eP_1, eBA_1) ? "ON" : "OFF");

	_drawMessage(324, START_X + FONT_SIZE_Y * 8, "UP LEFT :"); // 8
	_drawMessage(500, START_X + FONT_SIZE_Y * 8, g_GameConfig.GetAutoKey(eP_1, eBA_7) ? "ON" : "OFF");

	_drawMessage(324, START_X + FONT_SIZE_Y * 9, "CENTER :"); // 9
	_drawMessage(500, START_X + FONT_SIZE_Y * 9, g_GameConfig.GetAutoKey(eP_1, eBA_5) ? "ON" : "OFF");
	
	_drawMessage(324, START_X + FONT_SIZE_Y * 10, "UP RIGHT :"); // 10
	_drawMessage(500, START_X + FONT_SIZE_Y * 10, g_GameConfig.GetAutoKey(eP_1, eBA_9) ? "ON" : "OFF");

	_drawMessage(324, START_X + FONT_SIZE_Y * 11, "DOWN RIGHT :"); // 11
	_drawMessage(500, START_X + FONT_SIZE_Y * 11, g_GameConfig.GetAutoKey(eP_1, eBA_3) ? "ON" : "OFF");

	_drawMessage(40, START_X + FONT_SIZE_Y * 12, "AUTO KEY FOR 2P");

	_drawMessage(324, START_X + FONT_SIZE_Y * 13, "DOWN LEFT :"); // 13
	_drawMessage(500, START_X + FONT_SIZE_Y * 13, g_GameConfig.GetAutoKey(eP_2, eBA_1) ? "ON" : "OFF");

	_drawMessage(324, START_X + FONT_SIZE_Y * 14, "UP LEFT :"); // 14
	_drawMessage(500, START_X + FONT_SIZE_Y * 14, g_GameConfig.GetAutoKey(eP_2, eBA_7) ? "ON" : "OFF");
	
	_drawMessage(324, START_X + FONT_SIZE_Y * 15, "CENTER :"); // 15
	_drawMessage(500, START_X + FONT_SIZE_Y * 15, g_GameConfig.GetAutoKey(eP_2, eBA_5) ? "ON" : "OFF");

	_drawMessage(324, START_X + FONT_SIZE_Y * 16, "UP RIGHT :"); // 16
	_drawMessage(500, START_X + FONT_SIZE_Y * 16, g_GameConfig.GetAutoKey(eP_2, eBA_9) ? "ON" : "OFF");
	
	_drawMessage(324, START_X + FONT_SIZE_Y * 17, "DOWN RIGHT :"); // 17
	_drawMessage(500, START_X + FONT_SIZE_Y * 17, g_GameConfig.GetAutoKey(eP_2, eBA_3) ? "ON" : "OFF");

	const int	idxArr[] = { 0, 1, 2, 3, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 };

	_drawMessage(0, START_X + idxArr[m_curSelect] * 16, ">");

	return true;
}

void StageConfig::GetIn()
{
	g_GameConfig.LoadFromFile();
	g_Input.RegisterObserver("StageConfig", this);
}

void StageConfig::GetOut()
{
	g_GameConfig.SaveToFile();
	g_Input.DeleteObserver("StageConfig");
}

void StageConfig::GoNextStage()
{
	m_context.SetState(m_context.GetStateTitle());
}

void StageConfig::GoPreStage()
{
	m_context.SetState(m_context.GetStateTitle());
}

bool StageConfig::InputHandleMessage(const eInputName name, const eInputState is)
{
	bool	ret = true;

	if (is != eIS_PRESS)
		return false;

	switch (name)
	{
	case eIN_BACK:
			GoPreStage();
		break;
	case eIN_DOWN:
		m_curSelect = min(m_curSelect + 1, MAX_CUR_SELECT - 1);
		break;
	case eIN_UP:
		m_curSelect = max(m_curSelect - 1, 0);
		break;
	case eIN_GO:
		_changeConfig();
		break;
	default:
		ret = false;
		break;
	}
	return ret;
}

bool StageConfig::_drawMessage(const int x, const int y, const char* message)
{
#define	FONT_WIDTH	8
#define	FONT_HEIGHT	16

	for (int i = 0; message[i] != NULL; i++) {	
		SDL_Rect sRect = { 0, };

		char ch = toupper(message[i]);
		sRect.x = FONT_WIDTH * (ch - ' ');
		sRect.y = 0;
		sRect.h = FONT_HEIGHT;
		sRect.w = FONT_WIDTH;

		m_pSmallFont->Blit(x + (FONT_WIDTH * i), y, &sRect);
	}

	return true;
}

void StageConfig::_changeConfig()
{
	const int	idxArr[] = { 0, 1, 2, 3, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 };
	switch (m_curSelect) {
	case 0:	// STAGE BREAK
		g_GameConfig.TogleStageBreak();
		break;
	case 1:		// JUDGEMENT PICTURE : ON OFF
		g_GameConfig.TogleJudgePic();
		break;
	case 2:		// KEY SETTING : NORMAL, FUSION		
		g_GameConfig.TogleKeySet();
		break;
	case 3:		// JOY SETTING : DDR
		g_GameConfig.TootleJoySet();
		break;
	case 4:		// 1p DOWN LEFT : ON OFF
		g_GameConfig.TotleAutoKey(eP_1, eBA_1);
		break;
	case 5:		// 1p UP LEFT : ON OFF
		g_GameConfig.TotleAutoKey(eP_1, eBA_7);
		break;
	case 6:		// 1p CENTER : ON OFF
		g_GameConfig.TotleAutoKey(eP_1, eBA_5);
		break;
	case 7:	// 1p UP RIGHT : ON OFF
		g_GameConfig.TotleAutoKey(eP_1, eBA_9);
		break;
	case 8:	// 1p DOWN RIGHT : ON OFF
		g_GameConfig.TotleAutoKey(eP_1, eBA_3);
		break;
	case 9:	// 2p DOWN LEFT : ON OFF
		g_GameConfig.TotleAutoKey(eP_2, eBA_1);
		break;
	case 10:	// 2p UP LEFT : ON OFF
		g_GameConfig.TotleAutoKey(eP_2, eBA_7);
		break;
	case 11:	// 2p CENTER : ON OFF
		g_GameConfig.TotleAutoKey(eP_2, eBA_5);
		break;
	case 12:	// 2p UP RIGHT : ON OFF
		g_GameConfig.TotleAutoKey(eP_2, eBA_9);
		break;
	case 13:	// 2p DOWN RIGHT : ON OFF
		g_GameConfig.TotleAutoKey(eP_2, eBA_3);
		break;
	default:
		break;
	}
}
