#include "StageConfig.h"
#include "../KickItUp.h"
#include "../video/Surface.h"
#include "../sound/Sound.h"
#include "../video/SurfaceStore.h"
#include "../sound/SoundStore.h"
#include "../input/InputManager.h"
#include "Context.h"
#include "../GameConfig.h"

StageConfig::StageConfig(Context& context) : m_context(context)
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
	return true;
}