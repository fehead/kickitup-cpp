#ifndef _KICKITUP_STAGESELECT_H
#define _KICKITUP_STAGESELECT_H

#include "../common.h"
#include "Stage.h"
#include "../input/iInputHandler.h"
#include "../video/ZoomAnimation.h"
#include <deque>
using std::deque;

class Surface;
class Sound;
class SoundStore;
class SurfaceStore;
class InputStore;
class Input;
class Context;
class Song;
class ZoomAnimation;

class StageSelect	:	public Stage , public iInputHandler
{
private:
	Context &			m_context;

	Surface *	m_pBG;				// BackGround Image.
	Surface *	m_pStateComment;	// �ϴ� ���� ǥ��
	Surface *	m_pShiftLeft;		// �ϴ� ���� ��ư(�ٸ���ù�ư)
	Surface *	m_pShiftRight;		// �ϴ� ������ ��ư(�ٸ���ù�ư)
    Surface *   m_pModeIcon[ePM_Max];        // easy, hard, double crazy icon
	Sound *		m_pBGM;				// BackGround Music
    Sound *     m_pMove;            // shiftRight ShiftLeft Button�� �������� ���� �Ҹ�.

    Song *      m_pSongLeft;        // ���� �뷡
    Song *      m_pSongRight;       // ������ �뷡.
    Song *      m_pSongSelect;      // ���õ� �뷡.
    Sound *     m_pSoundIntro;      // ���õ� ����

    ZoomAnimation   m_buttonAni[4];    // ��ư

	deque<eInputName>	m_chatkeys[2];

public:
	StageSelect( Context & context );
	virtual ~StageSelect(void);

	virtual bool 		Initialize();
	virtual void 		Destroy();

	virtual bool		Think( unsigned long delta );
	virtual bool		Render( unsigned long delta );

	virtual void		GetIn();	// get in the stage.
	virtual void		GetOut();	// get out the stage.

	virtual bool		InputHandleMessage( const eInputName name, const eInputState is );

	virtual void		GoPreStage();	///< goto prev stage.
	virtual void		GoNextStage();	///< goto next stage.

private:
    void                _turnLeft();    ///< �������� ȭ���̵�
    void                _turnRight();   ///< ���������� ȭ���̵�
    void                _resetSelectSong(); ///< ���õȰ� ����
    void                _selectSong( const int direction );      ///< �� ����
    void                _selectRightSong();  ///< ������ �� ����
    void                _checkHiddenMode(); ///< check hidden mode.
};

#endif // _KICKITUP_STAGESELECT_H
