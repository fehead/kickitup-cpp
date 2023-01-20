#include "StageNormal.h"
#include "Context.h"
#include "../input/InputManager.h"
#include "../sound/SoundStore.h"
#include "../video/SDLSurfaceStore.h"
#include "../data/SongMgr.h"
#include "../video/Surface.h"
#include "../sound/Sound.h"
#include "../GameConfig.h"
#include <sstream>
#include <cmath>

using std::stringstream;
using std::abs;

StageNormal::StageNormal( Context & context )   :   m_context( context ),
    m_pBGM( 0 ),
    m_pBG( 0 ),
    m_pStepArrows( 0 ),
    m_pSelectedSong( 0 ),
    m_stepSpeed( 2 ),
    m_buttonState(eIN_ARROW_2P_MAX)
{
}

StageNormal::~StageNormal(void)
{
}

bool StageNormal::Initialize()
{
    // 예약만 한다.
    m_pBGM = g_pSoundStore->Order( "SelectedSong" );

    // 예약만 함.
    m_pBG = g_pSurfaceStore->Order( "SelectedTitleImg" );

    m_pBackArrows[0] = g_pSurfaceStore->Order( "arrows1" );
    m_pBackArrows[0]->Load( "images/arrow1.bmp" );
    m_pBackArrows[0]->SetColorKey();

    m_pBackArrows[1] = g_pSurfaceStore->Order( "arrows2" );
    m_pBackArrows[1]->Load( "images/arrow2.bmp" );
    m_pBackArrows[1]->SetColorKey();

    
    m_pStepArrows = g_pSurfaceStore->Order( "arrows" );
    m_pStepArrows->Load( "images/arrow.bmp" );
    m_pStepArrows->SetColorKey();

    m_pGaugeWaku = g_pSurfaceStore->Order( "gauge_waku" );
    m_pGaugeWaku->Load( "images/gauge_waku.bmp" );
    m_pGaugeWaku->SetColorKey();

    m_pGauge = g_pSurfaceStore->Order( "gauge" );
    m_pGauge->Load( "images/gauge.bmp" );
    m_pGauge->SetColorKey();

    
    for (int p = 0; p < 2; ++p) {
        for (int i = 0; i < eBA_Max; ++i) {
            // 버튼이 눌러질때 나타나는 AniMation x 위치.
            static const int    PUSHARROW_X_COORD[2][eBA_Max] = {
                { 25, 75, 125, 175, 225 },
                { 345, 395, 445, 495, 545 } };

            stringstream    tmp;
            tmp << "images/p_arrow" << ARRAY_TO_BUTON[i] << ".bmp";
            Surface* pSurface = g_pSurfaceStore->Order();
            pSurface->Load(tmp.str());
            pSurface->SetColorKey();

            m_aniPushArrows[p][i].setMaxFrames(9);
            m_aniPushArrows[p][i].setCurrentFrame(9);
            m_aniPushArrows[p][i].setFrameRate(30);
            m_aniPushArrows[p][i].setLoop(false);
            m_aniPushArrows[p][i].setSurface(pSurface);
            m_aniPushArrows[p][i].setCoord(Coord(PUSHARROW_X_COORD[p][i] + 2, 45));
            SDL_Rect    rt;
            rt.x = 72;
            rt.y = 0; rt.w = 72; rt.h = 70;
            m_aniPushArrows[p][i].setRect(rt);

            // Crash Animation
            tmp.str("");
            tmp << "images/c_arrow" << ARRAY_TO_BUTON[i] << ".bmp";
            pSurface = g_pSurfaceStore->Order();
            pSurface->Load(tmp.str());
            pSurface->SetColorKey();

            m_aniCrashArrows[p][i].setSurface(pSurface);
            m_aniCrashArrows[p][i].setMaxFrames(8);
            m_aniCrashArrows[p][i].setCurrentFrame(8);
            m_aniCrashArrows[p][i].setFrameRate(30);
            m_aniCrashArrows[p][i].setLoop(false);
            m_aniCrashArrows[p][i].setCoord(Coord(PUSHARROW_X_COORD[0][i], 43));
            rt.x = 80;
            rt.y = 0; rt.w = 80; rt.h = 80;
            m_aniCrashArrows[p][i].setRect(rt);
        }

        // 화살표 조각 구역 설정.
        // TODO: 2P
        for (int i = 0; i < eBA_Max; ++i) {
            // arrow.bmp 그림과 arrow1.bmp 그림순서를 맞추기위한 임시 배열.
            static const int arrawArray[5] = { 4, 0, 2, 1, 3 };

            int yIndex = arrawArray[i];
            SDL_Rect    rt = { 0, };
            rt.x = 0;
            rt.y = STEP_ARROW_SIZE * yIndex;
            rt.w = STEP_ARROW_SIZE;
            rt.h = STEP_ARROW_SIZE;

            m_AniStepArraws[p][i].setSurface(m_pStepArrows);
            m_AniStepArraws[p][i].setMaxFrames(6);
            m_AniStepArraws[p][i].setCurrentFrame(0);
            m_AniStepArraws[p][i].setFrameRate(100);
            m_AniStepArraws[p][i].setLoop(true);
            m_AniStepArraws[p][i].setRect(rt);
        }
    }

    return true;
}

void StageNormal::Destroy()
{
}


bool StageNormal::Think( unsigned long delta )
{
    // 판정.
    _judge();

    if( m_pBGM->IsPlaying() == false )
        SetQuitStage( true );

    m_playingTime = m_pBGM->GetPosition() - m_start[0];
    if( m_playingTime < 0 )
        m_playingTime = 0;

    m_dStepIdx = _getIndexByTime( m_playingTime );
    m_nStepIdx = static_cast<int>( m_dStepIdx ) + 1;

    // 기준 y값 - back arrow 위에서 끝이 나게 되어있다.
    m_y = static_cast<int>(BACK_ARROW_Y + (m_nStepIdx - m_dStepIdx) * m_distancePerStep );
    
    // 화면 전체에 출력하기 위해 추가되는 Step개수만큼 화면에서 빼준다. 0.5를 곱한이유는 사실 1배속이 0.5배속이다.
    m_y -= static_cast<int>(BACK_ARROW_Y * m_addedStep * m_stepSpeed * 0.5);

    using std::fill;
    fill( m_buttonState.begin(), m_buttonState.end(), false );    

    return true;
}

bool StageNormal::Render( unsigned long delta )
{
    _drawBackGround( delta );
    _drawBackArrow( m_playingTime );

    if( m_pSelectedSong->m_ksf[0].isEndStep( m_nStepIdx ) ) {
        SetQuitStage( true );
        return true;
    }

    for (int p = 0; p < 2; ++p) {
        for (int i = 0; i < 5; ++i) {
            if (m_aniPushArrows[p][i].isLoopEnd() == false) {
                m_aniPushArrows[p][i].OnAnimate(delta);
                m_aniPushArrows[p][i].OnRender();
            }
            if (m_aniCrashArrows[p][i].isLoopEnd() == false) {
                m_aniCrashArrows[p][i].OnAnimate(delta);
                m_aniCrashArrows[p][i].OnRender();
            }
            m_AniStepArraws[p][i].OnAnimate(delta);
        }

        int stepIndex = m_nStepIdx - m_addedStep;
        for (int i = 0; i < 48; ++i) {
            // static const int arrowX[] = { 30, 80, 132, 185, 235 };            
            static const int arrowX[2][5] = {
                { 30, 80, 132, 185, 235 },            
                { 350, 400, 452, 505, 555 }
            };
            
            if (m_stepData.size() < (i + stepIndex)) {
                break;
            }

            const char* pKsfData = m_stepData[i + stepIndex]->step;
            if (i == 0 && pKsfData[0] == '2')
                SetQuitStage(true);

            // drawing flowing steps.
            if (m_showStep[i + stepIndex]) {
                for (int j = 0; j < 5; ++j) {
                    if (pKsfData[5*p+j] == '1') {
                        m_AniStepArraws[p][j].setCoord(arrowX[p][j], m_y + static_cast<int>(i * m_distancePerStep));
                        m_AniStepArraws[p][j].OnRender();
                    }
                }
            }
        }
    }

    // 판정 관련 화면을 출력.
    _drawGauge();

    return true;
}


void StageNormal::GetIn()
{
    g_Input.RegisterObserver( "StageNormal", this );

    SetQuitStage( false );

    m_pSelectedSong = g_pSongMgr->GetSelectSong();
    m_showStep.assign( m_pSelectedSong->m_ksf[0].GetStepSize(), true );

    // setting step data
    _setStepData(&m_pSelectedSong->m_ksf[0]);

    // start time setting.
    for( int i = 0 ; i < 3 ; ++i )
        m_start[i] = m_pSelectedSong->m_ksf[0].GetStart( i );

    // get tick
    m_tick = m_pSelectedSong->m_ksf[0].GetTick();

    // set bpmpix
    m_bpm = m_pSelectedSong->m_ksf[0].GetBPM( 0 );


    // load Image.
    if( m_pSelectedSong->GetBGImgPath().length() )
        m_pBG->Load( m_pSelectedSong->GetBGImgPath() );
    else
        m_pBG = g_pSurfaceStore->Find( "bgImage" );

    // load sound.
    m_pBGM->Load( m_pSelectedSong->GetSongPath() );
    m_pBGM->Play();

    // 1step 시간(ms)
    m_stepGapTime = 60000.0/(m_bpm * m_tick);

    // 판정 시간.
    for( int i = ePointZone_Min ; i < ePointZone_Miss ; ++i ) {
        m_pointTimeZone[i] = static_cast<int>(m_stepGapTime / BACK_ARROW_Y * (i+1) * 10);
    }

    m_distancePerStep = BACK_ARROW_Y * m_stepSpeed * 0.5;  // 0.5를 곱한 이유는 1배속이 프로그램상 0.5배속이기 때문이다.

    // 추가되는 step개수는 가려서 잘 보이지 않는 스탭개수에서부터 스탭 백패널 까지의 개수이다. -60 ~ 55pixel 위치까지 step개수.
    m_addedStep = static_cast<int>((STEP_ARROW_SIZE + BACK_ARROW_Y) / m_distancePerStep);

    m_judgedIdx = 0;
    m_playingTime = 0;

    // 판정 개수 초기화
    std::fill( &m_judgePoint[0], &m_judgePoint[ePointZone_Max], 0 );
}

void StageNormal::GetOut()
{
    m_pBGM->Stop();
    
    _clearStepData();

    g_Input.DeleteObserver( "StageNormal" );
}


bool StageNormal::InputHandleMessage( const eInputName name, const eInputState is )
{
    if( is != eIS_PRESS )
        return false;

    if( eIN_ARROW_1P_MIN <= name && name < eIN_ARROW_1P_MAX ) {
        m_buttonState[name - eIN_ARROW_1P_MIN] = true;
        m_aniPushArrows[0][name - eIN_ARROW_1P_MIN].setCurrentFrame( 0 );
    }

    if (eIN_ARROW_2P_MIN <= name && name < eIN_ARROW_2P_MAX) {
        m_buttonState[name - eIN_ARROW_2P_MIN] = true;
        m_aniPushArrows[1][name- eIN_ARROW_2P_MIN].setCurrentFrame(0);
    }

    switch( static_cast<int>(name) )
    {
    case eIN_BACK:
        GoPreStage();
        break;
    }
    return true;
}

/// 이전 스테이지로 변경(SelectStage로 변경한다.)
void StageNormal::GoPreStage()
{
    m_context.SetState( m_context.GetStateSelect() );
}

/// 다음 스테이지로 변경(TitleStage로 변경한다.-임시)
void StageNormal::GoNextStage()
{
    // m_context.SetState( m_context.GetStateResult() );
	m_context.SetState( m_context.GetStateTitle() );
}


void StageNormal::_drawBackGround( unsigned int playTime ) const
{
    m_pBG->Blit( 0 , 0 );
}

/// 판정관련 데이터를 화면에 뿌린다.
void StageNormal::_drawGauge() const
{
    m_pGaugeWaku->Blit( 32, 0 );
    m_pGaugeWaku->Blit( 352, 0 );

    m_pGauge->Blit( 280, 20 );
    m_pGauge->Blit( 352, 20 );
}

// 화살표 백패널를 그린다.
void StageNormal::_drawBackArrow( unsigned int playTime ) const
{
    // 백패널의 반짝임을 계산 각 tick 마다 60ms시간만끔 반짝임을 준다.
    long tmp = playTime % static_cast<long>(m_bpm*m_tick);
    if( 0 < tmp && tmp < 60 ) {
        m_pBackArrows[1]->Blit( 32, 50 );
        m_pBackArrows[1]->Blit( 352, 50 );
    }
    else {
        m_pBackArrows[0]->Blit( 32, 50 );
        m_pBackArrows[0]->Blit( 352, 50 );
    }
}

// 음악시간으로 현재 index값을 찾는다.
double StageNormal::_getIndexByTime( unsigned int playTime ) const
{
	if( playTime <= 0)
		return 0;
    return playTime / m_stepGapTime;
}

// 음악시간으로 현재 index값을 찾는다.
int StageNormal::_getTimeByIndex( int stepIndex ) const
{
    if( m_playingTime <= 0)
        return 0;
    return static_cast<int>( stepIndex  * m_stepGapTime );
}

void StageNormal::_judge()
{

    if( m_playingTime == 0 )
        return;

    // 1. 검사할 index구간을 구한다.
    int judgeStartIdx = m_judgedIdx;
    int judgeEndIdx = static_cast<int>( _getIndexByTime( m_playingTime + m_pointTimeZone[ePointZone_Bad] ) );
    
    // 판정할것이 없으면 끝.
    if( judgeEndIdx <= judgeStartIdx)
        return;

    int lastJudgeIdx = m_judgedIdx;

    // 검사할 index의 구간을 판정
    for( int i = judgeStartIdx ; i <= judgeEndIdx ; ++i ) {
        // 검사할 index에 판정할 step이 있는지 검사한다.
        // 판정한 step이 있으면
        const char * currentStep = m_stepData[i]->step;
        if( strncmp( currentStep, "00000", 5 ) != 0 ) {
            // 해당 버튼이 눌러졌는가?
            // 해당 버튼이 눌러져 있으면.
            if( _isPress( currentStep ) ) {
                // 시간에 따라 판정을한다.
                ePointZone point = _getPoint( m_dStepIdx, i );
                
                ++m_judgePoint[point];

                if( point < ePointZone_Miss ) {
                    m_showStep[i] = false;
                    lastJudgeIdx = i + 1;
                    for( int j = 0 ; j < 5 ; ++j ) {
                        if( currentStep[j] == '1' ) {
                            m_aniCrashArrows[0][j].setCurrentFrame( 0 );
                        }
                    }
                }
            }
            // 해당 버튼이 눌러져 있지 않으면
            else {
                // MISS판정
                m_showStep[i] = true;
            }
        }
    }
    
    using std::max;
    int judgedTime = max( m_playingTime - 100, 0 );
    int lastJudgedStepByTime = static_cast<int>( _getIndexByTime( judgedTime ) );

    m_judgedIdx = max( lastJudgedStepByTime, lastJudgeIdx );    
}

// 해당 버튼이 눌러 졌는지 검사.
bool StageNormal::_isPress( const char * btnStr ) const
{    
    bool ret = false;
    for( int i = 0 ; i < m_buttonState.size(); ++i ) {
        if( btnStr[i] == '1' ) {
            if( m_buttonState[i] ) {
                ret = true;
            }
            else {
                ret = false;
                break;
            }
        }
    }

    return ret;
}


/** @brief  index 값으로 점수를 판정한다.
    @param  baseStepIndex       판정 기준 StepIndex
    @param  toJudgeStepIndex    판정할 stepIndex
*/
StageNormal::ePointZone
StageNormal::_getPoint( const double baseStepIndex, const int toJudgeStepIndex ) const
{
    double gapOfStep = abs( (m_nStepIdx - m_dStepIdx) + (toJudgeStepIndex - m_nStepIdx) );
    if( 1 < gapOfStep )  {
        return ePointZone_Miss;
    }

    double gapTime = gapOfStep * m_stepGapTime;

    for( int i = ePointZone_Min ; i < ePointZone_Miss ; ++i ) {
        if( gapTime <= m_pointTimeZone[i] ) {
            return static_cast<ePointZone>( i );
        }
    }

    return ePointZone_Miss;
}

/** @brief  set stepdata for Normal stage
*   @param  ksf step data
*/
void StageNormal::_setStepData(const Ksf* ksf)
{
    for (int i = 0; i < ksf->GetStepSize(); ++i) {
        StepData_t * sd = new StepData_t();
        const char * rawStepData = ksf->GetStep(i);
 
        // set 1p step data
        if (g_GameConfig.IsStarted(eP_1)) {            
            memcpy_s(&sd->step[0], 5, rawStepData, 5);
        }

        // set 2p step data
        if (g_GameConfig.IsStarted(eP_2)) {
            memcpy_s(&sd->step[5], 5, rawStepData, 5);
        }

        // exit data setting.
        if (sd->step[0] == '2') {
            memset(sd->step, '2', sizeof(sd->step));
        }

        m_stepData.push_back(sd);
    }
}

/** @brief  clear stepdata
*/
void StageNormal::_clearStepData()
{
    for (int i = 0; i < m_stepData.size(); ++i) {
        StepData_t* sd = m_stepData.at(i);
        delete sd;
    }

    m_stepData.clear();
}
