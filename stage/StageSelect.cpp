#include "StageSelect.h"
#include "../KickItUp.h"
#include "../video/Surface.h"
#include "../sound/Sound.h"
#include "../video/SurfaceStore.h"
#include "../sound/SoundStore.h"
#include "../input/InputManager.h"
#include "Context.h"
#include "../GameConfig.h"
#include "../data/SongMgr.h"

#include <algorithm>

StageSelect::StageSelect( Context & context )	:
	m_context( context ),
    m_pSongLeft( 0 ),
    m_pSongRight( 0 ),
    m_pSongSelect( 0 ),
    m_pSoundIntro( 0 )
{
}

StageSelect::~StageSelect(void)
{
}

// Initialize
bool StageSelect::Initialize()
{
	// loading image.
	m_pBG = g_pSurfaceStore->Order( "select_back" );
	m_pBG->Load( "images/select_back.bmp" );

	m_pShiftLeft = g_pSurfaceStore->Order( "shift_l" );
	m_pShiftLeft->Load( "images/shift_l.bmp" );
	m_pShiftLeft->SetColorKey();

	m_pShiftRight = g_pSurfaceStore->Order( "shift_r" );
	m_pShiftRight->Load( "images/shift_r.bmp" );
	m_pShiftRight->SetColorKey();

    m_pModeIcon[ePM_None] = 0;
    m_pModeIcon[ePM_Easy] = g_pSurfaceStore->Order( "easy_icon" );
    m_pModeIcon[ePM_Easy]->Load( "images/easy_icon.bmp" );
    m_pModeIcon[ePM_Hard] = g_pSurfaceStore->Order( "hard_icon" );
    m_pModeIcon[ePM_Hard]->Load( "images/hard_icon.bmp" );
    m_pModeIcon[ePM_Crazy] = g_pSurfaceStore->Order( "crazy_icon" );
    m_pModeIcon[ePM_Crazy]->Load( "images/crazy_icon.bmp" );
    m_pModeIcon[ePM_Double] = g_pSurfaceStore->Order( "double_icon" );
    m_pModeIcon[ePM_Double]->Load( "images/double_icon.bmp" );

    for( int i = ePM_Min ; i < ePM_Max ; ++i ) {
        m_pModeIcon[i]->SetColorKey();
	}

	// loading sound.
	m_pBGM = g_pSoundStore->Order( "music_select" );
	m_pBGM->Load( "wave/music_select.wav" );

    m_pMove = g_pSoundStore->Order( "move" );
    m_pMove->Load( "wave/move.wav" );

    // intro sound �Ҵ縸 ��.
    m_pSoundIntro = g_pSoundStore->Order( "intro" );

	m_pStateComment = g_pSurfaceStore->Order( "c_font" );
	assert( m_pStateComment );	// �̹� Ÿ��Ʋ ������������ �ε������Ƿ� Load���� ����.

    for( int i = 0 ; i < 4 ; ++i ) {
        m_buttonAni[i].setZoom( 1.0f, 1.15f );
        m_buttonAni[i].setFrameRate( 30 );
        m_buttonAni[i].setMaxFrames( 10 );
        m_buttonAni[i].setOscillate( true );
        m_buttonAni[i].setLoop( true );
        m_buttonAni[i].setPause( true );
    }

    m_buttonAni[2].setSurface( m_pShiftLeft );
    m_buttonAni[3].setSurface( m_pShiftRight );
    
    m_buttonAni[0].setCoord( 10, 50 );
    m_buttonAni[1].setCoord( 330, 50 );
    m_buttonAni[2].setCoord( 0, 250 );
    m_buttonAni[3].setCoord( 320, 250 );

	return true;
}

void StageSelect::Destroy()
{
}

// ȭ�鿡 ����ϱ��� �� �۾����� ó���Ѵ�.
bool StageSelect::Think(unsigned long delta )
{
	// ���� ��� üũ.
    _checkHiddenMode();

    // ���õ� �뷡 ������.
    if( m_pSongSelect )
        m_pSongSelect->GetDiscImg()->SetAlpha( g_pSurfaceStore->GetFlashAlphaValue() );

    for( int i = 0 ; i < 4 ; ++i ) {
        m_buttonAni[i].OnAnimate( delta );
    }

	return true;
}

// ȭ�鿡 ����� ����Ѵ�.
bool StageSelect::Render(unsigned long delta )
{
    // Draw BackGound.
    m_pBG->Blit( 0, 0 );

    // Draw left top
    if( m_pSongLeft ) {
        m_pSongLeft->GetDiscImg()->Blit( 10, 50 );
        m_pModeIcon[m_pSongLeft->GetPlayMode()]->Blit( 0, 50 );
    }

    // Draw right top
    if( m_pSongRight ) {
        m_pSongRight->GetDiscImg()->Blit( 330, 50 );
        m_pModeIcon[m_pSongRight->GetPlayMode()]->Blit( 320, 50 );
    }

	// Draw other left select song button.
	m_pShiftLeft->Blit( 0, 250 );

	// Draw other right select song button.
	m_pShiftRight->Blit( 320, 250 );

    for( int i = 0 ; i < 4 ; ++i ) {
        m_buttonAni[i].OnRender();
    }

	// Draw "FREE PLAY!"
	SDL_Rect sRect;
	sRect.x = 0;
	sRect.y = 46;
	sRect.w = 220;
	sRect.h = 69-46;
	m_pStateComment->SetAlpha ( 255 );
	m_pStateComment->Blit( 210, 450, &sRect );

	// Draw to screen "PRESS CENTER BUTTON"
	if( g_GameConfig.IsStarted( eP_1 ) == false ) {
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;

		m_pStateComment->SetAlpha( g_pSurfaceStore->GetFlashAlphaValue() );
		m_pStateComment->Blit( 10, 450, &sRect );
	}

	// Draw to screen "PRESS CENTER BUTTON"
	if( g_GameConfig.IsStarted( eP_2 ) == false ) {
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;

		m_pStateComment->SetAlpha( g_pSurfaceStore->GetFlashAlphaValue() );
		m_pStateComment->Blit( 410, 450, &sRect );
	}

	return true;
}

// get in the selectStage.
void StageSelect::GetIn()
{
	g_Input.RegisterObserver( "StageSelect", this );

    // LeftSong ���.
    m_pSongLeft = g_pSongMgr->GetLeftSong();

    // RightSong ���.
    m_pSongRight = g_pSongMgr->GetRightSong();

    _resetSelectSong();

	// ġƮŰ �ʱ�ȭ
	m_chatkeys[0].clear();
	m_chatkeys[1].clear();
}

// get out the selectStage.
void StageSelect::GetOut()
{
	m_pSoundIntro->Stop();
	m_pBGM->Stop();
	g_Input.DeleteObserver( "StageSelect" );
}

// process keyinput.
bool StageSelect::InputHandleMessage( const eInputName name, const eInputState is )
{
	bool	ret = true;

	if( is != eIS_PRESS )
		return false;

	switch( name )
	{
	case eIN_BACK:
		GoPreStage();
		break;

	case eIN_ARROW_1P_MIDLE:
		g_GameConfig.SetStart( eP_1 );
		break;

	case eIN_ARROW_2P_MIDLE:
		g_GameConfig.SetStart( eP_2 );
		break;

	// ���� �뷡�� �̵�
	case eIN_ARROW_1P_BOTTOM_LEFT:
        if( g_GameConfig.IsStarted( eP_1 ) )
            _turnLeft();
		break;
	case eIN_ARROW_2P_BOTTOM_LEFT:
        if( g_GameConfig.IsStarted( eP_2 ) )
            _turnLeft();
		break;

	// ���� �뷡�� �̵�
	case eIN_ARROW_1P_BOTTOM_RIGHT:
        if( g_GameConfig.IsStarted( eP_1 ) )
            _turnRight();
		break;

	case eIN_ARROW_2P_BOTTOM_RIGHT:
        if( g_GameConfig.IsStarted( eP_2 ) )
            _turnRight();
		break;

	// ���� �뷡 ����
	case eIN_ARROW_1P_TOP_LEFT:
        if( g_GameConfig.IsStarted( eP_1 ) )
            _selectSong( 0 );
		break;

	case eIN_ARROW_2P_TOP_LEFT:
        if( g_GameConfig.IsStarted( eP_2 ) )
            _selectSong( 0 );
       break;

	// ������ �뷡 ����
	case eIN_ARROW_1P_TOP_RIGHT:
        if( g_GameConfig.IsStarted( eP_1 ) )
            _selectSong( 1 );
		break;

	case eIN_ARROW_2P_TOP_RIGHT:
        if( g_GameConfig.IsStarted( eP_2 ) )
            _selectSong( 1 );
       break;

	default:
		ret = false;
		break;
	}

	// chatkey
	if( ret ) {
		if( eIN_ARROW_1P_MIN <= name && name < eIN_ARROW_1P_MAX ) {
			if( 8 <= m_chatkeys[0].size() )
				m_chatkeys[0].pop_front();
			m_chatkeys[0].push_back( name );
		}

		if( eIN_ARROW_2P_MIN <= name && name < eIN_ARROW_2P_MAX ) {
			if( 8 <= m_chatkeys[1].size() )
				m_chatkeys[1].pop_front();

			m_chatkeys[1].push_back( name );
		}
	}
	return ret;
}

// goto prev stage
void StageSelect::GoPreStage()
{
	m_context.SetState( m_context.GetStateTitle() );
}

// goto next stage.
void StageSelect::GoNextStage()
{
	// ���������� �̱۸������ �Ǻ��Ͽ� �������� gogo
	m_context.SetState( m_context.GetStateNormal() );
}

// ���� ȭ������.
void StageSelect::_turnLeft()
{
    g_pSongMgr->TurnLeft();
    _resetSelectSong();
    m_pMove->Play();
}

// ������ ȭ������.
void StageSelect::_turnRight()
{
    g_pSongMgr->TurnRight();
    _resetSelectSong();
    m_pMove->Play();
}

void StageSelect::_resetSelectSong()
{
    if( m_pSongSelect )
        m_pSongSelect->GetDiscImg()->SetAlpha( 255 );
    m_pSongSelect = 0;
    m_pSoundIntro->Stop();
    m_pSoundIntro->Free();
    m_pSongLeft = g_pSongMgr->GetLeftSong();
    m_pSongRight = g_pSongMgr->GetRightSong();
    m_pBGM->Play( true );

    for( int i = 0 ; i < 2 ; ++i ) {
        m_buttonAni[i].setCurZoom( 1.0f );
        m_buttonAni[i].setPause( true );
    }
    m_buttonAni[0].setSurface( m_pSongLeft->GetDiscImg() );
    m_buttonAni[1].setSurface( m_pSongRight->GetDiscImg() );
}


void StageSelect::_selectSong( const int direction )
{
    Song *  pNewSelectedSong = 0;
    if( direction == 0 ) {
        pNewSelectedSong = g_pSongMgr->GetLeftSong();
    }
    else {
        pNewSelectedSong = g_pSongMgr->GetRightSong();
    }

    // �̹� ���õǾ� ������ ���� ����������.
    if( m_pSongSelect == pNewSelectedSong ) {
        GoNextStage();
        return;
    }

    if( m_pSongSelect )
        m_pSongSelect->GetDiscImg()->SetAlpha( 255 );
    
    m_pSongSelect = pNewSelectedSong;

    m_pBGM->Stop();
    g_pSongMgr->SetSelectSong( m_pSongSelect );
    m_pSoundIntro->Stop();
    m_pSoundIntro->Free();
    m_pSoundIntro->Load( m_pSongSelect->GetIntroPath() );
    m_pSoundIntro->Play( true );

    for( int i = 0 ; i < 2 ; ++i ) {
        m_buttonAni[i].setCurZoom( 1.0f );
        m_buttonAni[i].setPause( true );
    }
    m_buttonAni[direction].setPause( false );
}


void StageSelect::_checkHiddenMode()
{
    // TODO:
	for(int i = 0 ; i < eP_Max ; ++i) {
		deque<eInputName> & chatkeys = m_chatkeys[i];
	    if( chatkeys.size() == 8 ) {
	    	char strKeyHistory[16] = {0, };
	    	std::copy(chatkeys.begin(), chatkeys.end(), strKeyHistory);

	    	// 2x Mode
	    	if ( strcmp ( strKeyHistory,"55755595" ) == 0 ) {
	    		g_GameConfig.SetSpeed((ePlayer)i, 2);
	    	}
	    	// 4x Mode
	    	else if ( strcmp ( strKeyHistory,"55355755" ) == 0 ) {
	    		g_GameConfig.SetSpeed((ePlayer)i, 4);
	    	}
	    	// 8x Mode
	    	else if ( strcmp ( strKeyHistory,"55153555" ) ==0 ) {
	    		g_GameConfig.SetSpeed((ePlayer)i, 8);
	    	}

	    }
	}
}



