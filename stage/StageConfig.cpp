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
	_drawMessage(500, START_X, "ON");
	_drawMessage(40, START_X + FONT_SIZE_Y * 1, "JUDGEMENT PICTURE"); // 1
	_drawMessage(500, START_X + FONT_SIZE_Y * 1, "ON");
	_drawMessage(40, START_X + FONT_SIZE_Y * 2, "KEY SETTING : "); // 2
	_drawMessage(500, START_X + FONT_SIZE_Y * 2, "NORMAL");	

	_drawMessage(40, START_X + FONT_SIZE_Y * 3, "JOY SETTING : "); // 3
	_drawMessage(500, START_X + FONT_SIZE_Y * 3, "DDR");	

	_drawMessage(40, START_X + FONT_SIZE_Y * 4, "");
	_drawMessage(40, START_X + FONT_SIZE_Y * 5, "");

	_drawMessage(40, START_X + FONT_SIZE_Y * 6, "AUTO KEY FOR 1P"); // 6

	_drawMessage(324, START_X + FONT_SIZE_Y * 7, "DOWN LEFT :"); // 7
	_drawMessage(500, START_X + FONT_SIZE_Y * 7, "ON");	

	_drawMessage(324, START_X + FONT_SIZE_Y * 8, "UP LEFT :"); // 8
	_drawMessage(500, START_X + FONT_SIZE_Y * 8, "ON");	

	_drawMessage(324, START_X + FONT_SIZE_Y * 9, "CENTER :"); // 9
	_drawMessage(500, START_X + FONT_SIZE_Y * 9, "ON");
	
	_drawMessage(324, START_X + FONT_SIZE_Y * 10, "UP RIGHT :"); // 10
	_drawMessage(500, START_X + FONT_SIZE_Y * 10, "ON");	

	_drawMessage(324, START_X + FONT_SIZE_Y * 11, "DOWN RIGHT :"); // 11
	_drawMessage(500, START_X + FONT_SIZE_Y * 11, "ON");

	_drawMessage(40, START_X + FONT_SIZE_Y * 12, "AUTO KEY FOR 2P");

	_drawMessage(324, START_X + FONT_SIZE_Y * 13, "DOWN LEFT :"); // 13
	_drawMessage(500, START_X + FONT_SIZE_Y * 13, "ON");

	_drawMessage(324, START_X + FONT_SIZE_Y * 14, "UP LEFT :"); // 14
	_drawMessage(500, START_X + FONT_SIZE_Y * 14, "ON");
	
	_drawMessage(324, START_X + FONT_SIZE_Y * 15, "CENTER :"); // 15
	_drawMessage(500, START_X + FONT_SIZE_Y * 15, "ON");

	_drawMessage(324, START_X + FONT_SIZE_Y * 16, "UP RIGHT :"); // 16
	_drawMessage(500, START_X + FONT_SIZE_Y * 16, "ON");
	
	_drawMessage(324, START_X + FONT_SIZE_Y * 17, "DOWN RIGHT :"); // 17
	_drawMessage(500, START_X + FONT_SIZE_Y * 17, "ON");

	_drawMessage(0, START_X + m_curSelect * 16, ">");

	return true;
}

void StageConfig::GetIn()
{
	g_Input.RegisterObserver("StageConfig", this);
}

void StageConfig::GetOut()
{
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
		m_curSelect = min(m_curSelect + 1, MAX_CUR_SELECT);
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
}
