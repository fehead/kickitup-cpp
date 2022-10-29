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

void StageConfig::Destroy()
{
	m_pBG->Free();
}


bool StageConfig::Render(unsigned long delta)
{
	m_pBG->Blit(0, 0);
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
	default:
		ret = false;
		break;
	}
	return ret;
}