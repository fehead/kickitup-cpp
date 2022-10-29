/*
 * InputManager.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "InputManager.h"
#include "KeyboardDevice.h"
#include <SDL.h>

InputManager	g_Input;

InputManager::InputManager(void)	:	m_pKeyBoard( 0 )
{
}

InputManager::~InputManager(void)
{
	if( m_pKeyBoard )
		delete m_pKeyBoard;
}


// 초기화 함수.
bool InputManager::Initialize()
{
	// 디바이스 생성
	_createDevice();

	// 키 등록
	_registerKey();

	return true;
}

// 일정 시간 마다 업데이트가 호출됨.
void InputManager::Update( long delta )
{
	if( m_pKeyBoard )
		m_pKeyBoard->Update( delta );

	for( KeyIter i = m_registeredKeyList.begin() ; i != m_registeredKeyList.end() ; ++i ) {
		sRegisteredKey &	k = i->second;
		_sendKeyMessage( i->first, k );
	}
}

// 키입력이 필요한 옵저버를 등록한다.
void InputManager::RegisterObserver( const char * name, iInputHandler * pInputHandler )
{
	m_inputHandlermanager.RegisterObserver( name, pInputHandler );
}

// 키입력이 필요한 옵저버를 등록해제한다.
void InputManager::DeleteObserver( const char * name )
{
	m_inputHandlermanager.DeleteObserver( name );
}


// 입력 디바이스트 생성 함수.
bool InputManager::_createDevice()
{
	if( m_pKeyBoard )
		delete m_pKeyBoard;
	m_pKeyBoard = new KeyboardDevice;
	return true;
}

// 키를 등록한다.
void InputManager::_registerKey()
{
	m_registeredKeyList.clear();
	_registerKey( eIN_GO, SDL_SCANCODE_RETURN);
	_registerKey( eIN_GO, SDL_SCANCODE_KP_ENTER );
	_registerKey( eIN_BACK, SDL_SCANCODE_ESCAPE );
	_registerKey( eIN_CONFIG, SDL_SCANCODE_F2 );


	_registerKey( eIN_ARROW_1P_TOP_LEFT, SDL_SCANCODE_Q);
	_registerKey( eIN_ARROW_1P_TOP_RIGHT, SDL_SCANCODE_E);
	_registerKey( eIN_ARROW_1P_MIDLE, SDL_SCANCODE_S);
	_registerKey( eIN_ARROW_1P_BOTTOM_LEFT, SDL_SCANCODE_Z);
	_registerKey( eIN_ARROW_1P_BOTTOM_RIGHT, SDL_SCANCODE_C);

	_registerKey( eIN_ARROW_2P_TOP_LEFT, SDL_SCANCODE_KP_7);
	_registerKey( eIN_ARROW_2P_TOP_RIGHT, SDL_SCANCODE_KP_9);
	_registerKey( eIN_ARROW_2P_MIDLE, SDL_SCANCODE_KP_5);
	_registerKey( eIN_ARROW_2P_BOTTOM_LEFT, SDL_SCANCODE_KP_1);
	_registerKey( eIN_ARROW_2P_BOTTOM_RIGHT, SDL_SCANCODE_KP_3);

}

// 키값과 기능을 연결하여 등록.
//	kind : keyboard가 눌렸을때 나오는 키값.
void InputManager::_registerKey( eInputName name, int kind )
{
	sRegisteredKey	key = {0,};
	key.inputState = eIS_NONE;
	key.keyKind = kind;
	key.separate = false;

	for( KeyIter iter = m_registeredKeyList.begin() ;
		iter != m_registeredKeyList.end() ;
		++iter ) {
		if( iter->second.keyKind == kind ) {
			iter->second.separate = true;
			key.separate = true;
			break;
		}
	}

	m_registeredKeyList.insert( std::make_pair(name, key) );
}

// 해당 키입력이 있는지 확인후 키 매세지를 보냄
void InputManager::_sendKeyMessage( eInputName name, sRegisteredKey & key )
{
	int	prev = key.inputState;
	int	cur = eIS_NONE;
	int	state = 0;

	state = m_pKeyBoard->GetState( iInputDevice::KeyState, key.keyKind );

	if( state & eIS_MASK_STATE )
		cur = eIS_HOLD;

	key.inputState = cur;
	key.inputState |= ( (cur ^ prev) & eIS_MASK_STATE ) ? eIS_MASK_TRIGGER : eIS_NONE;

	if( key.inputState == eIS_PRESS || key.inputState == eIS_RELEASE )
		m_inputHandlermanager.SendMessage( name, static_cast<eInputState>(key.inputState) );
}
