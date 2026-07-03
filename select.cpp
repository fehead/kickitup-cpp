/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

  	2000/07/22 'Work for Opening Source'
			- Removed Motion-Video Support (for improve safety and speed)
			- Removed Xaudio player mp3 play (this required their own dlls)

			- Added Audio-Media play using DirectShow.
*/

#include <stdio.h>
/* windows.h */
/* direct.h */
#include <cstring>
#include <cstdio>
#include <ctime>
/* dsound.h */

#include "media.h"
#include "main.h"
#include "input.h"
#include "select.h"
#include "result.h"

#include "song.h"
//#include "sound.h"
#include "dsutil.h"

#define DISCSIZE_X	300
#define DISCSIZE_Y	200

#define	STILL_DRAWING	100

extern SONG					CSONG[512];
extern Surface*	 SelectBack;
extern Surface*	SelectFont;
extern Surface*	SongTitle;
extern	Surface*	NumberFont;

extern Surface*	SongBack;
extern Surface*	Background;
extern Surface*	ShiftLeft;
extern Surface*	ShiftRight;
extern	Surface*	ModeIcon;
extern	Surface*	g_cFont;
extern	Surface*	Diff;
extern	Surface*	DoubleIcon;
extern	Surface*	CrazyIcon;
extern	Surface*	HardIcon;
extern	Surface*	EasyIcon;

extern double				bpm,bpm2,bpm3;
extern int					start,start2,start3;
extern	int					tick;
extern	uint32_t					bunki,bunki2;

extern char					SongName[MAX_PATH+1];
extern char					SongName2[MAX_PATH+1];
extern char					SongName3[MAX_PATH+1];
extern char					Title[MAX_PATH+1];
extern	int					start1;

extern	char				GameMode;

extern bool Couple;
extern bool Double;
// 커플 데이터 끝


//extern CWAVE				*WavePrimary;
extern	AudioDev*		lpds;

extern Sound*			g_dsMode;
extern Sound*			g_dsCancel;
extern Sound*			g_dsMove;
extern Sound*			g_dsSelectSong;

extern char First;

uint32_t	startTimer, curTimer;

extern bool IntroFlag;

int	ClpBlt3(int x ,int y ,Surface* ds,Rect* srect,uint32_t mode)
{
	static Rect sRect;
	int	hRet;

	memcpy(&sRect,srect,sizeof(sRect));
	
	if(x>640 || y>480) return DD_OK;

	if(y+(srect->bottom-srect->top)>480)srect->bottom=srect->bottom-(y+(srect->bottom-srect->top)-480);
	if(y<0)
	{
		srect->top-=y;
		y=0;
	}

	if(x+(srect->right-srect->left)>640)srect->right=srect->right-(x+(srect->right-srect->left)-640);
	if(x<0)
	{
		srect->left-=x;
		x=0;
	}
	
	hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);

	memcpy(srect,&sRect,sizeof(sRect));
	
	return hRet;

}

void Read()
{
	HANDLE	hFind;
	uint32_t	Count=0;
	WIN32_FIND_DATA lpData;

	char* cPathStr;

	hFind=(HANDLE)FindFirstFile("*.*",&lpData);
	cPathStr=_strupr(_strdup(lpData.cFileName));
	if(strcmp(cPathStr,"SONG")!=0)
	{
		for(;;)
		{
			if(FindNextFile(hFind,&lpData)==FALSE)break;
			cPathStr=_strupr(_strdup(lpData.cFileName));
			if(strcmp(cPathStr,"SONG")==0)break;
		}
	}
	if(lpData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
	{
		SetCurrentDirectory(lpData.cFileName);
		hFind=FindFirstFile("*.*",&lpData);
		if(lpData.cFileName[0]!='.' && lpData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
		{
			SetCurrentDirectory(lpData.cFileName);

			if(access("Crazy_2.stf",04)==0)CSONG[Count].ReadCrazy_2_STF("Crazy_2.stf");
			else if(access("Crazy_2.ksf",04)==0)CSONG[Count].ReadCrazy_2_KSF("Crazy_2.ksf");

			if(access("Crazy_1.stf",04)==0)CSONG[Count].ReadCrazy_1_STF("Crazy_1.stf"),Count++;
			else if(access("Crazy_1.ksf",04)==0)CSONG[Count].ReadCrazy_1_KSF("Crazy_1.ksf"),Count++;

			if(access("Hard_2.stf",04)==0)CSONG[Count].ReadHard_2_STF("Hard_2.stf");
			else if(access("Hard_2.ksf",04)==0)CSONG[Count].ReadHard_2_KSF("Hard_2.ksf");

			if(access("Hard_1.stf",04)==0)CSONG[Count].ReadHard_1_STF("Hard_1.stf"),Count++;
			else if(access("Hard_1.ksf",04)==0)CSONG[Count].ReadHard_1_KSF("Hard_1.ksf"),Count++;

			if(access("Easy_2.stf",04)==0)CSONG[Count].ReadEasy_2_STF("Easy_2.stf");
			else if(access("Easy_2.ksf",04)==0)CSONG[Count].ReadEasy_2_KSF("Easy_2.ksf");

			if(access("Easy_1.stf",04)==0)CSONG[Count].ReadEasy_1_STF("Easy_1.stf"),Count++;
			else if(access("Easy_1.ksf",04)==0)CSONG[Count].ReadEasy_1_KSF("Easy_1.ksf"),Count++;

			if(access("Double.stf",04)==0)CSONG[Count].ReadDouble_STF("Double.stf"),Count++;
			else if(access("Double.ksf",04)==0)CSONG[Count].ReadDouble_KSF("Double.ksf");

			//if(CSONG[Count].bpm!=0)Count++;
			SetCurrentDirectory("../");
		}

		for(;;)
		{
			if(FindNextFile(hFind,&lpData)==FALSE)break;
			else
			{
				if(lpData.cFileName[0]!='.' && lpData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
				{
					SetCurrentDirectory(lpData.cFileName);

					if(access("Crazy_2.stf",04)==0)CSONG[Count].ReadCrazy_2_STF("Crazy_2.stf");
					else if(access("Crazy_2.ksf",04)==0)CSONG[Count].ReadCrazy_2_KSF("Crazy_2.ksf");

					if(access("Crazy_1.stf",04)==0)CSONG[Count].ReadCrazy_1_STF("Crazy_1.stf"),Count++;
					else if(access("Crazy_1.ksf",04)==0)CSONG[Count].ReadCrazy_1_KSF("Crazy_1.ksf"),Count++;

					if(access("Hard_2.stf",04)==0)CSONG[Count].ReadHard_2_STF("Hard_2.stf");
					else if(access("Hard_2.ksf",04)==0)CSONG[Count].ReadHard_2_KSF("Hard_2.ksf");

					if(access("Hard_1.stf",04)==0)CSONG[Count].ReadHard_1_STF("Hard_1.stf"),Count++;
					else if(access("Hard_1.ksf",04)==0)CSONG[Count].ReadHard_1_KSF("Hard_1.ksf"),Count++;

					if(access("Easy_2.stf",04)==0)CSONG[Count].ReadEasy_2_STF("Easy_2.stf");
					else if(access("Easy_2.ksf",04)==0)CSONG[Count].ReadEasy_2_KSF("Easy_2.ksf");

					if(access("Easy_1.stf",04)==0)CSONG[Count].ReadEasy_1_STF("Easy_1.stf"),Count++;
					else if(access("Easy_1.ksf",04)==0)CSONG[Count].ReadEasy_1_KSF("Easy_1.ksf"),Count++;

					if(access("Double.stf",04)==0)CSONG[Count].ReadDouble_STF("Double.stf"),Count++;
					else if(access("Double.ksf",04)==0)CSONG[Count].ReadDouble_KSF("Double.ksf"),Count++;
				
					//if(CSONG[Count].bpm!=0)Count++;
					SetCurrentDirectory("../");
				}
			}
		}
		SetCurrentDirectory("../");
	}
	FindClose(hFind);

	if(CSONG[0].bpm==0)
	{
		MessageBox(hWnd,"Song directory not found or No song data.","KICKITUP ERROR",MB_OK);
//		ReleaseAllObjects();

		PostQuitMessage(0);
	}
}

void SelectSong(void)
{
	uint32_t count,i;
	static uint32_t current;
	static uint32_t SelectCurrent;
	static int Selected, zoom,toggle,speed;

	Rect	lRect;
	int ModeTemp1p, ModeTemp2p;
	
	static	time_t t;
	
	static	int a,b,c;

	static	int iMove;

	Rect DiscSize,Screen;

	char s[50];

	if(First==0)
	{
		startTimer=timeGetTime();
		if(g_p1.started==FALSE)
		{
			g_p1.speedBase=1;
			g_p1.mirror=FALSE;
			g_p1.nonstep=FALSE;
			g_p1.synchro=FALSE;
			g_p1.union_=FALSE;
			g_p1.random=FALSE;
			g_p1.dMix=FALSE;
			g_p1.speed1=1;
			g_p1.speed3=1;
			g_p1.speed5=1;
			g_p1.speed7=1;
			g_p1.speed9=1;
			g_p1.vanish=FALSE;
			g_p1.suddenR=FALSE;
			g_p1.randomS=FALSE;

		}
		if(g_p2.started==FALSE)
		{
			g_p2.speedBase=1;
			g_p2.mirror=FALSE;
			g_p2.nonstep=FALSE;
			g_p2.union_=FALSE;
			g_p2.random=FALSE;
			g_p2.dMix=FALSE;
			g_p2.speed1=1;
			g_p2.speed3=1;
			g_p2.speed5=1;
			g_p2.speed7=1;
			g_p2.speed9=1;
			g_p2.vanish=FALSE;
			g_p1.suddenR=FALSE;
			g_p1.randomS=FALSE;
		}
		// paint the background black.
		DDFillSurface(g_pDDSPrimary,0);
		DDFillSurface(g_pDDSBack,0);
		
		// Draw BackGround as select image.
		g_pDDSBack->BltFast(0,0, SelectBack, NULL, DDBLTFAST_NOCOLORKEY);
		
		a=g_p1.started;b=g_p2.started;
		First++;

		if(g_dsSelectSong)
			g_dsSelectSong->Play(0,0,DSBPLAY_LOOPING);
	}

	DiscSize.top=0;
	DiscSize.left=0;
	DiscSize.right=300;
	DiscSize.bottom=200;

	for(count=0;;count++)
	{
		if(count!=0)
			CSONG[count].Prev=count-1;
		
		CSONG[count].Next=count+1;
		
		if(CSONG[count].bpm==0)
		{
			CSONG[count].Prev=0;
			count--;
			CSONG[count].Next=0;
			CSONG[0].Prev=count;
			break;
		}
	}

	if(speed==1) //일단은 변수를 이용합니다. 곧 타이머 형식으로 바꾸도록 합시다. 
	{
		speed=0;
		if(toggle==0)
		{
			if(zoom==10)
				toggle=1;
			else
				zoom++;
		}
		else if(toggle==1)
		{
			if(zoom==0)
				toggle=0;
			else
				zoom--;
		}
	}
	else
		speed++; 

	ReadGameInput();

	if(g_p1.pressedKey[5]==TRUE)
	{
		if(g_p1.started==FALSE)
		{
			g_p1.started=TRUE;
		}
	}
	if(g_p2.pressedKey[5]==TRUE)
	{
		if(g_p2.started==FALSE)
		{
			g_p2.started=TRUE;
		}
	}

	// Get 1Player hidden mode.
	ModeTemp1p=ScanHiddenMode1p();

	if(ModeTemp1p) {
		if(IntroFlag){
			intro->OnMediaStop();
			delete intro;
			IntroFlag=FALSE;
		}
	}

	switch(ModeTemp1p)
	{
		case HMODE_SUDDENR:
			g_p1.suddenR=TRUE;
			g_p1.vanish=FALSE;

			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_RANDOMS:
			g_p1.randomS=TRUE;
			g_p1.speedBase=1;

			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_2X:
			g_p1.speedBase=2;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_4X:
			g_p1.speedBase=4;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_8X:
			g_p1.speedBase=8;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_MIRROR:
			g_p1.mirror=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_NONSTEP:
			g_p1.nonstep=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_SYNCHRO:
			g_p1.synchro=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_UNION:
			g_p1.union_=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_RANDOM:
			g_p1.random=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_4DMIX:
			srand((unsigned) time(&t));

			g_p1.speed1=1+rand()%8;
			g_p1.speed3=1+rand()%8;
			g_p1.speed5=1+rand()%8;
			g_p1.speed7=1+rand()%8;
			g_p1.speed9=1+rand()%8;

			g_p1.dMix=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_VANISH:
			g_p1.vanish=TRUE;
			g_p2.suddenR=FALSE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
/*		case HMODE_NONSTOPDOUBLE:
			if(g_p1.started&&g_p2.started)break;
			Double=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;*/
		case HMODE_CANCEL:
			if(g_dsCancel)g_dsCancel->Play(0,0,0);
			g_p1.speedBase=1;
			g_p1.mirror=FALSE;
			g_p1.nonstep=FALSE;
			g_p1.synchro=FALSE;
			g_p1.union_=FALSE;
			g_p1.random=FALSE;
			g_p1.dMix=FALSE;
			g_p1.speed1=1;
			g_p1.speed3=1;
			g_p1.speed5=1;
			g_p1.speed7=1;
			g_p1.speed9=1;
			g_p1.vanish=FALSE;
			g_p1.suddenR=FALSE;
			g_p1.randomS=FALSE;
			Double=FALSE;
			break;
	default:
			break;
	}

	// Get 2Player hidden mode.
	ModeTemp2p=ScanHiddenMode2p();
	if(ModeTemp2p)if(IntroFlag){intro->OnMediaStop();delete intro;IntroFlag=FALSE;}
	
	switch(ModeTemp2p)
	{
		case HMODE_SUDDENR:
			g_p2.suddenR=TRUE;
			g_p2.vanish=FALSE;

			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_RANDOMS:
			g_p2.randomS=TRUE;
			g_p2.speedBase=1;

			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_2X:
			g_p2.speedBase=2;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_4X:
			g_p2.speedBase=4;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_8X:
			g_p2.speedBase=8;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_MIRROR:
			g_p2.mirror=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_NONSTEP:
			g_p2.nonstep=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_SYNCHRO:
			g_p1.synchro=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_UNION:
			g_p2.union_=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_RANDOM:
			g_p2.random=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_4DMIX:

			srand((unsigned) time(&t));

			g_p2.speed1 = 1+rand()%8;
			g_p2.speed3 = 1+rand()%8;
			g_p2.speed5 = 1+rand()%8;
			g_p2.speed7 = 1+rand()%8;
			g_p2.speed9 = 1+rand()%8;

			g_p2.dMix=TRUE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;
		case HMODE_VANISH:
			g_p2.vanish=TRUE;
			g_p2.suddenR=FALSE;
			if(g_dsMode){g_dsMode->Stop();g_dsMode->SetCurrentPosition(0);g_dsMode->Play(0,0,0);}
			break;

		case HMODE_CANCEL:
			if(g_dsCancel)g_dsCancel->Play(0,0,0);
			g_p2.speedBase=1;
			g_p2.mirror=FALSE;
			g_p2.nonstep=FALSE;
			g_p2.union_=FALSE;
			g_p2.random=FALSE;
			g_p2.dMix=FALSE;
			g_p2.speed1=1;
			g_p2.speed3=1;
			g_p2.speed5=1;
			g_p2.speed7=1;
			g_p2.speed9=1;
			g_p2.vanish=FALSE;
			g_p2.suddenR=FALSE;
			g_p2.randomS=FALSE;
			Double=FALSE;
			break;
	}

	// change screen to left.
	if((g_p1.started && g_p1.pressedKey[1]) || (g_p2.started && g_p2.pressedKey[1]))
	{
		if(IntroFlag){intro->OnMediaStop();delete intro;IntroFlag=FALSE;}
		if(g_dsMove){g_dsMove->Stop();g_dsMove->SetCurrentPosition(0);g_dsMove->Play(0,0,0);}

		Selected=0;

		iMove=1;
		current = CSONG[CSONG[current].Prev].Prev;
	}

	// change screen to right.
	if((g_p1.started && g_p1.pressedKey[3]) || (g_p2.started && g_p2.pressedKey[3]))
	{
		if(IntroFlag){intro->OnMediaStop();delete intro;IntroFlag=FALSE;}
		if(g_dsMove){g_dsMove->Stop();g_dsMove->SetCurrentPosition(0);g_dsMove->Play(0,0,0);}

		Selected=0;

		iMove=-1;
		current = CSONG[CSONG[current].Next].Next;
	}
	
	// select the left song.
	if((g_p1.started && g_p1.pressedKey[7]) || (g_p2.started && g_p2.pressedKey[7]))
	{
		// if 7button pressed twice then move to the next stage.
		if(Selected==7)
		{
			SelectCurrent=current;
			g_p1.pressedKey[0]=3;
		}
		// if 7button pressed once then select music.
		else
		{
			// if intro music is playing then stop.
			if(IntroFlag){
				intro->OnMediaStop();delete intro;IntroFlag=FALSE;
			}

			// if moving music is playing then stop.
			if(g_dsMove){
				g_dsMove->Stop();g_dsMove->SetCurrentPosition(0);g_dsMove->Play(0,0,0);
			}

			// start New intro music.
			if(access(CSONG[current].IntroWavPath,04)==0)
			{
				IntroFlag=TRUE;
				intro= new CMedia;
				intro->OpenMediaFile(CSONG[current].IntroWavPath);
				intro->OnMediaPlay();
			}
			else if(access(CSONG[current].IntroMp3Path,04)==0)
			{
				IntroFlag=TRUE;
				intro= new CMedia;
				intro->OpenMediaFile(CSONG[current].IntroMp3Path);
				intro->OnMediaPlay();
			}

			Selected=7;
		}
	}
	
	// select the right song.
	if((g_p1.started && g_p1.pressedKey[9]) || (g_p2.started && g_p2.pressedKey[9]))
	{
		// if 9button pressed twice then move to the next stage.
		if(Selected==9)
		{
			SelectCurrent=CSONG[current].Next;
			g_p1.pressedKey[0]=3;
		}
		// if 9button pressed once select the right song.
		else
		{
			// intro sound stop.
			if(IntroFlag){
				intro->OnMediaStop();delete intro;IntroFlag=FALSE;
			}

			// moving sound stop.
			if(g_dsMove){
				g_dsMove->Stop();g_dsMove->SetCurrentPosition(0);g_dsMove->Play(0,0,0);
			}
			
			// selected song intro sound start.
			if(access(CSONG[CSONG[current].Next].IntroWavPath,04)==0)
			{
				IntroFlag=TRUE;
				intro= new CMedia;
				intro->OpenMediaFile(CSONG[CSONG[current].Next].IntroWavPath);
				intro->OnMediaPlay();
			}
			else if(access(CSONG[CSONG[current].Next].IntroMp3Path,04)==0)
			{
				IntroFlag=TRUE;
				intro= new CMedia;
				intro->OpenMediaFile(CSONG[CSONG[current].Next].IntroMp3Path);
				intro->OnMediaPlay();
			}

			Selected=9;
		}
	}

	// Game Start( Change the Next Stage. )
	if(g_p1.pressedKey[0]==3)
	{
		g_p1.pressedKey[0]=0;

		// Game Mode setting.
		if(CSONG[SelectCurrent].HaveCrazy==TRUE)GameMode=MODE_CRAZY,Double=FALSE;
	   	else if(CSONG[SelectCurrent].HaveDouble==TRUE)GameMode=MODE_DOUBLE,Double=TRUE;
	   	else if(CSONG[SelectCurrent].HaveEasy==TRUE)GameMode=MODE_EASY,Double=FALSE;
	   	else if(CSONG[SelectCurrent].HaveHard==TRUE)GameMode=MODE_HARD,Double=FALSE;
		else return;

		// Couple mode setting.
		if(g_p1.started && g_p2.started)
		{
			Couple=TRUE;
			if(CSONG[SelectCurrent].HaveCouple==FALSE)
				g_p1.synchro=TRUE;
		} else
			Couple=FALSE;

		// Cancel sound restart.
		if(g_dsCancel)
		{
			g_dsCancel->Stop();
			g_dsCancel->Play(0,0,0);
		}


		// selected song background setting.
		if(SongBack)
		{
			SongBack->Release();
			SongBack=NULL;
		}

		SongBack = DDLoadBitmap(g_pDD,CSONG[SelectCurrent].BgImgPath,0,0);

		// selected song title setting.
		if(SongTitle)
		{
			SongTitle->Release();
			SongTitle=NULL;
		}
		
		SongTitle = DDLoadBitmap(g_pDD,CSONG[SelectCurrent].TitleImgPath,0,0);

		// if background is not set then background set title image.
		if(SongBack == NULL)
		{
			if(SongTitle)
				SongBack=DDLoadBitmap(g_pDD,CSONG[SelectCurrent].TitleImgPath,0,0);
			else	// default background image.
				SongBack=DDLoadBitmap(g_pDD, "images/back.png",0,0);
		}
		if(SongTitle == NULL)
		{
			SongTitle=DDLoadBitmap(g_pDD,"images/noDisc.png",0,0);
		}

		// draw title image.
		g_pDDSPrimary->BltFast(0,0,SongTitle,NULL,DDBLTFAST_NOCOLORKEY);

		// double mode
		if(GameMode == MODE_DOUBLE)
		{
			memcpy(&Data_Double, &CSONG[SelectCurrent].Data_Double, sizeof(CSONG[SelectCurrent].Data_Double));
		}
		// couple mode
		else if(Couple==TRUE)
		{
			switch(GameMode)
			{
				case MODE_CRAZY:
					if(g_p1.synchro)
					{
						memcpy(&Data,  &CSONG[SelectCurrent].Data_Crazy, sizeof(CSONG[SelectCurrent].Data_Crazy));
						memcpy(&Data1, &CSONG[SelectCurrent].Data_Crazy, sizeof(CSONG[SelectCurrent].Data_Crazy));

						for(i=0;i<MAX_DATA;i++)
						{
							Data1[i][5] = Data1[i][0];
							Data1[i][6] = Data1[i][1];
							Data1[i][7] = Data1[i][2];
							Data1[i][8] = Data1[i][3];
							Data1[i][9] = Data1[i][4];
						}
					}
					else
					{
						memcpy(&Data, &CSONG[SelectCurrent].Data_Crazy1, sizeof(CSONG[SelectCurrent].Data_Crazy1));
						memcpy(&Data1,&CSONG[SelectCurrent].Data_Crazy1, sizeof(CSONG[SelectCurrent].Data_Crazy1));
					}
					break;

				case MODE_EASY:
					if(g_p1.synchro)
					{
						memcpy(&Data, &CSONG[SelectCurrent].Data_Easy, sizeof(CSONG[SelectCurrent].Data_Easy));
						memcpy(&Data1,&CSONG[SelectCurrent].Data_Easy, sizeof(CSONG[SelectCurrent].Data_Easy));
						
						for(i=0;i<MAX_DATA;i++)
						{
							Data1[i][5] = Data1[i][0];
							Data1[i][6] = Data1[i][1];
							Data1[i][7] = Data1[i][2];
							Data1[i][8] = Data1[i][3];
							Data1[i][9] = Data1[i][4];
						}
					}
					else
					{
						memcpy(&Data, &CSONG[SelectCurrent].Data_Easy1, sizeof(CSONG[SelectCurrent].Data_Easy1));
						memcpy(&Data1,&CSONG[SelectCurrent].Data_Easy1, sizeof(CSONG[SelectCurrent].Data_Easy1));
					}
					break;

				case MODE_HARD:
					if(g_p1.synchro)
					{
						memcpy(&Data, &CSONG[SelectCurrent].Data_Hard, sizeof(CSONG[SelectCurrent].Data_Hard));
						memcpy(&Data1,&CSONG[SelectCurrent].Data_Hard, sizeof(CSONG[SelectCurrent].Data_Hard));
						
						for(i=0;i<MAX_DATA;i++)
						{
							Data1[i][5]=Data1[i][0];
							Data1[i][6]=Data1[i][1];
							Data1[i][7]=Data1[i][2];
							Data1[i][8]=Data1[i][3];
							Data1[i][9]=Data1[i][4];
						}
					}
					else
					{
						memcpy(&Data,&CSONG[SelectCurrent].Data_Hard1,sizeof(CSONG[SelectCurrent].Data_Hard1));
						memcpy(&Data1,&CSONG[SelectCurrent].Data_Hard1,sizeof(CSONG[SelectCurrent].Data_Hard1));
					}
					break;
			}
		}
		// single mode.
		else 
		{
			switch(GameMode)
			{
				case MODE_CRAZY:
					if(g_p1.started)
						memcpy(&Data, CSONG[SelectCurrent].Data_Crazy, sizeof(Data));
					else	// 2p play
					{
						memcpy(&Data, CSONG[SelectCurrent].Data_Crazy, sizeof(Data));
						memcpy(&Data1,CSONG[SelectCurrent].Data_Crazy, sizeof(Data));
						for(i=0;i<MAX_DATA;i++)
						{
							Data1[i][5] = Data[i][0];
							Data1[i][6] = Data[i][1];
							Data1[i][7] = Data[i][2];
							Data1[i][8] = Data[i][3];
							Data1[i][9] = Data[i][4];
						}
					}
					break;

				case MODE_EASY:
					if(g_p1.started)
						memcpy(&Data,CSONG[SelectCurrent].Data_Easy,sizeof(Data));
					else
					{
						memcpy(&Data,  CSONG[SelectCurrent].Data_Easy, sizeof(Data));
						memcpy(&Data1, CSONG[SelectCurrent].Data_Easy, sizeof(Data));
						for(i=0;i<MAX_DATA;i++)
						{
							Data1[i][5] = Data[i][0];
							Data1[i][6] = Data[i][1];
							Data1[i][7] = Data[i][2];
							Data1[i][8] = Data[i][3];
							Data1[i][9] = Data[i][4];
						}
					}
					break;

				case MODE_HARD:
					if(g_p1.started)
						memcpy(&Data,CSONG[SelectCurrent].Data_Hard,sizeof(Data));
					else	// 2p play
					{
						memcpy(&Data,  CSONG[SelectCurrent].Data_Hard, sizeof(Data));
						memcpy(&Data1, CSONG[SelectCurrent].Data_Hard, sizeof(Data));
						for(i=0;i<MAX_DATA;i++)
						{
							Data1[i][5] = Data[i][0];
							Data1[i][6] = Data[i][1];
							Data1[i][7] = Data[i][2];
							Data1[i][8] = Data[i][3];
							Data1[i][9] = Data[i][4];
						}
					}
					break;
			}
		}

		bpm=CSONG[SelectCurrent].bpm;
		bpm2=CSONG[SelectCurrent].bpm2;
		bpm3=CSONG[SelectCurrent].bpm3;
		bunki=CSONG[SelectCurrent].Bunki;
		bunki2=CSONG[SelectCurrent].Bunki2;
		
		switch(GameMode)
		{
			/*	idia
			Song a;
			a.setMode( GameMode );
			start = a.GetStart();
			start2 = a.GetStart2();
			..
			*/
			case MODE_CRAZY:
				start = CSONG[SelectCurrent].Crazy_Start;
				start2= CSONG[SelectCurrent].Crazy_Start2;
				start3= CSONG[SelectCurrent].Crazy_Start3;
				tick  = CSONG[SelectCurrent].Crazy_Tick;
				break;

			case MODE_HARD:
				start = CSONG[SelectCurrent].Hard_Start;
				start2= CSONG[SelectCurrent].Hard_Start2;
				start3= CSONG[SelectCurrent].Hard_Start3;
				tick  = CSONG[SelectCurrent].Hard_Tick;
				break;

			case MODE_EASY:
				start = CSONG[SelectCurrent].Easy_Start;
				start2= CSONG[SelectCurrent].Easy_Start2;
				start3= CSONG[SelectCurrent].Easy_Start3;
				tick  = CSONG[SelectCurrent].Easy_Tick;
				break;

			case MODE_DOUBLE:
				start  = CSONG[SelectCurrent].Double_Start;
				start2 = CSONG[SelectCurrent].Double_Start2;
				start3 = CSONG[SelectCurrent].Double_Start3;
				tick   = CSONG[SelectCurrent].Double_Tick;
				break;
				
			default:
				{
					char	buff[256] = { 0, };
					sprintf( buff, "MODE ERROR" __FILE__ " line:%d\n", __LINE__ );
					DisplayMessage(0, 0, buff );
				}		
				break;
		}

		strcpy(SongName,  CSONG[SelectCurrent].PlayWavPath);
		strcpy(SongName2, CSONG[SelectCurrent].PlayMp3Path);
		strcpy(SongName3, CSONG[SelectCurrent].PlayMpgPath);
		strcpy(Title,     CSONG[SelectCurrent].SongTitle);

		g_p1.judgement=NONE;
		g_p2.judgement=NONE;

		if(GameMode==MODE_DOUBLE)
			g_ProgramState=DOUBLE;
		else
			g_ProgramState=STAGE1;
		
		if(GameMode==MODE_DOUBLE)
		{
			if(g_p1.started && g_p2.started)
				g_p2.started=FALSE;
		}
		Selected=0;
		
		if(g_dsSelectSong)
			g_dsSelectSong->Stop();
		if(IntroFlag){
			intro->OnMediaStop();delete intro;IntroFlag=FALSE;
		}
		
		// open play song.
		if(access(SongName,04)==0)
		{
			SongFlag=TRUE;
			song=new CMedia;
			song->OpenMediaFile(SongName);
		}
		else if(access(SongName2,04)==0)
		{
			SongFlag=TRUE;
			song=new CMedia;
			song->OpenMediaFile(SongName2);
		}
		else
			SongFlag=FALSE;

		// paint background black.
		DDFillSurface(g_pDDSBack, 0);

		First=0;
		g_p1.combo=0;
		g_p2.combo=0;

		g_p1.perfect=0;
		g_p1.great=0;
		g_p1.good=0;
		g_p1.bad=0;
		g_p1.miss=0;
		g_p1.maxCombo=0;

		g_p2.perfect=0;
		g_p2.great=0;
		g_p2.good=0;
		g_p2.bad=0;
		g_p2.miss=0;
		g_p2.maxCombo=0;

		g_p1.state=0;
		g_p2.state=0;

		start1=0;

		g_p1.pressedKey[0]=0;
		g_p2.pressedKey[0]=0;
		SelectCurrent=0;

		return;
		
	}

	// Draw background select image.
	g_pDDSBack->BltFast(0, 0, SelectBack, NULL, DDBLTFAST_NOCOLORKEY);

	curTimer = timeGetTime();

	int i2;

	i2=(int)(curTimer-startTimer)/1000;
	sprintf(s,"%02d", (40 - i2));

	if(40 <= i2)
	{
		if(Selected == 7)
			SelectCurrent = current, g_p1.pressedKey[0]=3;
		else if(Selected == 9)
			SelectCurrent = CSONG[current].Next, g_p1.pressedKey[0]=3;
		else
			SelectCurrent = current,g_p1.pressedKey[0]=3;
	}
		
	DisplayNumber(560,8,s);
	
	// selected left song.
	if(Selected == 7)
	{
			Screen.top = 50 - zoom;
			Screen.bottom = 50 + DISCSIZE_Y + zoom;
			Screen.left = 10 - zoom;
			Screen.right = 10 + DISCSIZE_X + zoom;
	}
	// unselected left song.
	else
	{
		Screen.top = 50;
		Screen.bottom = 50 + DISCSIZE_Y;
		Screen.left = 10;
		Screen.right = 10 + DISCSIZE_X;
	}
	
	// change right screen.
	if(iMove<0)
	{
		if(iMove<=-640)
			iMove=0;
		else 
		{
			iMove-=8;

			ClpBlt3(10+iMove,50,CSONG[CSONG[CSONG[current].Prev].Prev].DiskImage, &DiscSize,DDBLTFAST_SRCCOLORKEY);
			ClpBlt3(650+iMove,50,CSONG[current].DiskImage, &DiscSize,DDBLTFAST_SRCCOLORKEY);
		}
	}
	// change left screen.
	else if(iMove>0)
	{
		if(iMove>=640)
			iMove=0;
		else
		{
			iMove+=8;
		
			ClpBlt3(-630+iMove,50,CSONG[current].DiskImage, &DiscSize, DDBLTFAST_SRCCOLORKEY);
			ClpBlt3(10+iMove,50,CSONG[CSONG[CSONG[current].Next].Next].DiskImage, &DiscSize, DDBLTFAST_SRCCOLORKEY);
		}
	}
	else if(iMove==0)
		g_pDDSBack->Blt(&Screen,CSONG[current].DiskImage, &DiscSize, DDBLT_KEYSRC,NULL);
	
	// selected right song.
	if(Selected == 9)
	{
		// zoom the diskImage.
		Screen.top = 50 - zoom;
		Screen.bottom = 50 + DISCSIZE_Y + zoom;
		Screen.left = 330 - zoom;
		Screen.right = 330 + DISCSIZE_X + zoom;
	}
	// unselected right song.
	else
	{
		// make normal size the diskImage.
		Screen.top = 50;
		Screen.bottom = 50 + DISCSIZE_Y;
		Screen.left = 330;
		Screen.right = 330 + DISCSIZE_X;
	}

	// draw left disk image.
	if(iMove<0)
	{
		if(iMove<=-640)
			iMove=0;
		else
		{
			iMove-=8;

			ClpBlt3(330+iMove,50,CSONG[CSONG[CSONG[CSONG[current].Prev].Prev].Next].DiskImage, &DiscSize,DDBLTFAST_SRCCOLORKEY);
			ClpBlt3(970+iMove,50,CSONG[CSONG[current].Next].DiskImage, &DiscSize,DDBLTFAST_SRCCOLORKEY);
		}
	}
	// draw right disk image.
	else if(iMove>0)
	{
		if(iMove>=640)
			iMove=0;
		else
		{
			iMove+=8;
		
			ClpBlt3(-310+iMove,50,CSONG[CSONG[current].Next].DiskImage, &DiscSize, DDBLTFAST_SRCCOLORKEY);
			ClpBlt3(330+iMove,50,CSONG[CSONG[CSONG[CSONG[current].Next].Next].Next].DiskImage, &DiscSize,DDBLTFAST_SRCCOLORKEY);
		}
	}
	else if(iMove==0)
		g_pDDSBack->Blt(&Screen,CSONG[CSONG[current].Next].DiskImage, &DiscSize, DDBLT_KEYSRC,NULL);

	// draw shiftleft image.
	g_pDDSBack->BltFast(0, 250, ShiftLeft, NULL, DDBLTFAST_SRCCOLORKEY);
	
	// draw shiftright image.
	g_pDDSBack->BltFast(320, 250,ShiftRight,NULL, DDBLTFAST_SRCCOLORKEY);

	// print song title to the screen.
	if(Selected == 7)
		DisplayMessage(200, 300, CSONG[current].SongTitle);
	else if(Selected==9)
		DisplayMessage(200, 300, CSONG[CSONG[current].Next].SongTitle);

	// draw 1p mode pictures.
	if(g_p1.mirror)
		DrawMode(0,200,HMODE_MIRROR);
	if(g_p1.nonstep)
		DrawMode(0,240,HMODE_NONSTEP);
	if(g_p1.synchro)
		DrawMode(0,280,HMODE_SYNCHRO);
	if(g_p1.union_)
		DrawMode(0,320,HMODE_UNION);
	if(g_p1.random)
		DrawMode(0,360,HMODE_RANDOM);
	if(g_p1.vanish)
		DrawMode(0,400,HMODE_VANISH);
	if(g_p1.speedBase>1)
		DrawMode(0,160,HMODE_2X);

	// draw 2p mode pictures.
	if(g_p2.mirror)
		DrawMode(600,200,HMODE_MIRROR);
	if(g_p2.nonstep)
		DrawMode(600,240,HMODE_NONSTEP);
	if(g_p2.union_)
		DrawMode(600,320,HMODE_UNION);
	if(g_p2.random)
		DrawMode(600,360,HMODE_RANDOM);
	if(g_p2.vanish)
		DrawMode(600,400,HMODE_VANISH);
	if(g_p2.speedBase>1)
		DrawMode(600,160,HMODE_2X);

	// Draw level icon to next to the title image.
	if(CSONG[current].HaveDouble && iMove==0 )
		g_pDDSBack->BltFast(0,50, DoubleIcon,NULL, DDBLTFAST_SRCCOLORKEY);
	if(CSONG[CSONG[current].Next].HaveDouble && iMove==0 )
		g_pDDSBack->BltFast(320,50, DoubleIcon, NULL, DDBLTFAST_SRCCOLORKEY);

	if(CSONG[current].HaveCrazy && iMove==0 )
		g_pDDSBack->BltFast(0,50, CrazyIcon,NULL, DDBLTFAST_SRCCOLORKEY);
	if(CSONG[CSONG[current].Next].HaveCrazy && iMove==0 )
		g_pDDSBack->BltFast(320,50, CrazyIcon, NULL, DDBLTFAST_SRCCOLORKEY);

	if(CSONG[current].HaveHard && iMove==0 )
		g_pDDSBack->BltFast(0,50, HardIcon,NULL, DDBLTFAST_SRCCOLORKEY);
	if(CSONG[CSONG[current].Next].HaveHard && iMove==0 )
		g_pDDSBack->BltFast(320,50, HardIcon, NULL, DDBLTFAST_SRCCOLORKEY);

	if(CSONG[current].HaveEasy && iMove==0 )
		g_pDDSBack->BltFast(0,50, EasyIcon,NULL, DDBLTFAST_SRCCOLORKEY);
	if(CSONG[CSONG[current].Next].HaveEasy && iMove==0 )
		g_pDDSBack->BltFast(320,50, EasyIcon, NULL, DDBLTFAST_SRCCOLORKEY);

	// Draw to screen "FREE PLAY!"
	lRect.top=46;
	lRect.left=0;
	lRect.right=220;
	lRect.bottom=69;

	g_pDDSBack->BltFast(210,450,g_cFont, &lRect, DDBLTFAST_SRCCOLORKEY);

	if(g_p1.started)
	{
		if(a==0)
		{
			a++;
			if(g_p1.started && g_p2.started)Couple=TRUE;
			else Couple=FALSE;
		}
	}
	if(g_p2.started)
	{
		if(b==0)
		{
			b++;
			if(g_p1.started && g_p2.started)Couple=TRUE;
			else Couple=FALSE;
		}
	}

	if(g_p1.started==FALSE)
	{

		// Draw "PRESS CENTER BUTTON"
		lRect.top=0;
		lRect.left=0;
		lRect.right=220;
		lRect.bottom=23;

		TransAlphaImproved(g_cFont, g_pDDSBack, 10, 450, lRect, ALPHA, CKey_CFont, 16);
	}

	if(g_p2.started==FALSE)
	{
		// Draw "PRESS CENTER BUTTON"
		lRect.top=0;
		lRect.left=0;
		lRect.right=220;
		lRect.bottom=23;

		TransAlphaImproved(g_cFont, g_pDDSBack, 410, 450, lRect, ALPHA, CKey_CFont, 16);
	}


	// Alpha 
	ALPHA += inc;
	if (ALPHA > 256)
	{
		ALPHA = 256;
		inc = -10;
	}
	else if (ALPHA < 0)
	{
		ALPHA = 0;
		inc = 10;
	}

	Flipp();

}

int	ScanHiddenMode1p(void)
{
	static char IntKey1p[10];
	int i;

	if(g_p1.pressedKey[1] || g_p1.pressedKey[3] || g_p1.pressedKey[5] || g_p1.pressedKey[7] || g_p1.pressedKey[9])
		for(i=0;i<7;i++)IntKey1p[i]=IntKey1p[i+1];
	
	if(g_p1.pressedKey[1])IntKey1p[7]='1';
	if(g_p1.pressedKey[3])IntKey1p[7]='3';
	if(g_p1.pressedKey[5])IntKey1p[7]='5';
	if(g_p1.pressedKey[7])IntKey1p[7]='7';
	if(g_p1.pressedKey[9])IntKey1p[7]='9';

	// 2배속 입니다.
	if(strcmp(IntKey1p,"55755595")==0)
	{
		IntKey1p[7]='0';
		return HMODE_2X;
	}
	// 4배속 입니다.
	else if(strcmp(IntKey1p,"55355755")==0)
	{
		IntKey1p[7]='0';
		return HMODE_4X;
	}
	// 8배속 입니다.
	else if(strcmp(IntKey1p,"55153555")==0)
	{
		IntKey1p[7]='0';
		return HMODE_8X;
	}
	// 배니쉬 입니다.
	else if(strcmp(IntKey1p,"55975315")==0)
	{
		IntKey1p[7]='0';
		return HMODE_VANISH;
	}
	// 미러모드 입니다.
	else if(strcmp(IntKey1p,"55159357")==0)
	{
		IntKey1p[7]='0';
		return HMODE_MIRROR;
	}
	// 랜덤모드 입니다.
	else if(strcmp(IntKey1p,"51535957")==0)
	{
		IntKey1p[7]='0';
		return HMODE_RANDOM;
	}
/*	// 커플모드 입니다.
	else if(strcmp(IntKey1p,"55979755")==0)
	{
		IntKey1p[7]='0';
		return HMODE_COUPLE;
	}
*/	// 싱크로 모드입니다.
	else if(strcmp(IntKey1p,"55797955")==0)
	{
		IntKey1p[7]='0';
		return HMODE_SYNCHRO;
	}
	// 논스텝모드 입니다.
	else if(strcmp(IntKey1p,"79579579")==0)
	{
		IntKey1p[7]='0';
		return HMODE_NONSTEP;
	}
/*	// 논스톱더블 입니다.
	else if(strcmp(IntKey1p,"17159395")==0)
	{
		IntKey1p[7]='0';
		return HMODE_NONSTOPDOUBLE;
	}*/
	// 유니온모드 입니다.
	else if(strcmp(IntKey1p,"13573159")==0)
	{
		IntKey1p[7]='0';
		return HMODE_UNION;
	}
/*	// 셀렉트올 입니다. 
	else if(strcmp(IntKey1p,"95197537")==0)
	{
		IntKey1p[7]='0';
		return HMODE_SELECTALL;
	}*/
	// 캔슬 입니다.
	else if(strcmp(IntKey1p,"55555555")==0)
	{
		IntKey1p[7]='0';
		return HMODE_CANCEL;
	}
	else if(strcmp(IntKey1p,"55955575")==0)
	{
		IntKey1p[7]='0';
		return HMODE_4DMIX;
	}
	else if(strcmp(IntKey1p,"79513579")==0)
	{
		IntKey1p[7]='0';
		return HMODE_SUDDENR;
	}
	else if(strcmp(IntKey1p,"17931793")==0)
	{
		IntKey1p[7]='0';
		return HMODE_RANDOMS;
	}
	else
		return HMODE_NONE;
}

int	ScanHiddenMode2p(void)
{
	static char IntKey2p[10];
	int i;

	if(g_p2.pressedKey[1] || g_p2.pressedKey[3] || g_p2.pressedKey[5] || g_p2.pressedKey[7] || g_p2.pressedKey[9])
		for(i=0;i<7;i++)IntKey2p[i]=IntKey2p[i+1];
	
	if(g_p2.pressedKey[1])IntKey2p[7]='1';
	if(g_p2.pressedKey[3])IntKey2p[7]='3';
	if(g_p2.pressedKey[5])IntKey2p[7]='5';
	if(g_p2.pressedKey[7])IntKey2p[7]='7';
	if(g_p2.pressedKey[9])IntKey2p[7]='9';

	if(strcmp(IntKey2p,"55755595")==0)
	{
		IntKey2p[7]='0';
		return HMODE_2X;
	} // 2배속 입니다.
	else if(strcmp(IntKey2p,"55355755")==0)
	{
		IntKey2p[7]='0';
		return HMODE_4X;
	} // 4배속 입니다.
	else if(strcmp(IntKey2p,"55153555")==0)
	{
		IntKey2p[7]='0';
		return HMODE_8X;
	} // 8배속 입니다.
	else if(strcmp(IntKey2p,"55975315")==0)
	{
		IntKey2p[7]='0';
		return HMODE_VANISH;
	} // 배니쉬 입니다.
	else if(strcmp(IntKey2p,"55159357")==0)
	{
		IntKey2p[7]='0';
		return HMODE_MIRROR;
	} // 미러모드 입니다.
	else if(strcmp(IntKey2p,"51535957")==0)
	{
		IntKey2p[7]='0';
		return HMODE_RANDOM;
	} // 랜덤모드 입니다.
/*	else if(strcmp(IntKey2p,"55979755")==0)
	{
		IntKey2p[7]='0';
		return HMODE_COUPLE;
	} // 커플모드 입니다.
*/	else if(strcmp(IntKey2p,"55797955")==0)
	{
		IntKey2p[7]='0';
		return HMODE_SYNCHRO;
	} // 싱크로 모드입니다.
	else if(strcmp(IntKey2p,"79579579")==0)
	{
		IntKey2p[7]='0';
		return HMODE_NONSTEP;
	} // 논스텝모드 입니다.
/*	else if(strcmp(IntKey2p,"17159395")==0)
	{
		IntKey2p[7]='0';
		return HMODE_NONSTOPDOUBLE;
	} // 논스톱더블 입니다.*/
	else if(strcmp(IntKey2p,"13573159")==0)
	{
		IntKey2p[7]='0';
		return HMODE_UNION;
	} // 유니온모드 입니다.
/*	else if(strcmp(IntKey2p,"95197537")==0)
	{
		IntKey2p[7]='0';
		return HMODE_SELECTALL;
	} // 셀렉트올 입니다. */
	else if(strcmp(IntKey2p,"55555555")==0)
	{
		IntKey2p[7]='0';
		return HMODE_CANCEL;
	} // 캔슬 입니다.

	else if(strcmp(IntKey2p,"55955575")==0)
	{
		IntKey2p[7]='0';
		return HMODE_4DMIX;
	}
	else if(strcmp(IntKey2p,"79513579")==0)
	{
		IntKey2p[7]='0';
		return HMODE_SUDDENR;
	}
	else if(strcmp(IntKey2p,"17931793")==0)
	{
		IntKey2p[7]='0';
		return HMODE_RANDOMS;
	}
	else return HMODE_NONE;
}

void DrawMode(int x, int y, int Mode)
{
	Rect	modeRect;

	switch(Mode)
	{
		case HMODE_2X:
		case HMODE_4X:
		case HMODE_8X:
			modeRect.top=0;
			modeRect.left=0;
			modeRect.right=37;
			modeRect.bottom=37;
			break;
		case HMODE_VANISH:
			modeRect.top=0;
			modeRect.left=37;
			modeRect.right=37*2;
			modeRect.bottom=37;
			break;
		case HMODE_MIRROR:
			modeRect.top=0;
			modeRect.left=37*2;
			modeRect.right=37*3;
			modeRect.bottom=37;
			break;
		case HMODE_RANDOM:
			modeRect.top=0;
			modeRect.left=37*3;
			modeRect.right=37*4;
			modeRect.bottom=37;
			break;
		case HMODE_COUPLE:
			modeRect.top=0;
			modeRect.left=37*4;
			modeRect.right=37*5;
			modeRect.bottom=37;
			break;
		case HMODE_SYNCHRO:
			modeRect.top=0;
			modeRect.left=37*5;
			modeRect.right=37*6;
			modeRect.bottom=37;
			break;
		case HMODE_NONSTEP:
			modeRect.top=0;
			modeRect.left=37*6;
			modeRect.right=37*7;
			modeRect.bottom=37;
			break;
		case HMODE_UNION:
			modeRect.top=0;
			modeRect.left=37*7;
			modeRect.right=37*8;
			modeRect.bottom=37;
			break;
		default:
			modeRect.top=0;
			modeRect.left=0;
			modeRect.right=0;
			modeRect.bottom=0;
			break;
	}

	g_pDDSBack->BltFast(x,y, ModeIcon, &modeRect, DDBLTFAST_SRCCOLORKEY);
}
