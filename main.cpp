/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

	2000/07/22 'Work for Opening Source'
			- Removed Motion-Video Support (for improve safety and speed)
			- Removed Xaudio player mp3 play (this required their own dlls)

			- Added Audio-Media play using DirectShow.
	2000/07/23 'Patching'
			- Bpm changing bug fixed (bpm change was incorrected.)
*/
#include "main.h"
#include "player.h"
#include "renderer.h"

// Dshow ..
#include "media.h"

// Dshow ..

/* windows.h replaced by sdl3_kick.h */
/* ddraw.h replaced by sdl3_kick.h */
#include <stdio.h>
/* dsound.h replaced by sdl3_kick.h */
/* mmsystem.h replaced by SDL */
#include <time.h>
#include <cstdio>

#include "result.h"
#include "config.h"
#include "dead.h"
#include "double.h"
#include "ddutil.h"
#include "dsutil.h"
#include "song.h"
#include "select.h"
//#include "sound.h"
#include "input.h"
#include "resource.h"

#define VER_NUM	"0.4b"
char	TITLE[MAX_PATH];

/* MODESLCT stubs — mode selection images */
Surface*	ModeEasy   = NULL;
Surface*	ModeHard   = NULL;
Surface*	ModeDouble = NULL;
Surface*	ModeNonstop = NULL;
uint32_t				PressedKey[10] = {0};

#define	PRGNAME		"Kick It UP!"

// �Ϲ� �ϵ� ������ �κ�

// �������

// ���� ������ ����
// ���� ������ ��


char				SongName[MAX_PATH+1];
char				SongName2[MAX_PATH+1];
char				SongName3[MAX_PATH+1];

char				LogoTitleName[MAX_PATH];

double				bpm;
double				bpm2;
double				bpm3;

int					start,start2,start3;
uint32_t					bunki,bunki2;

int					tick;

char				Title[MAX_PATH+1];

char				g_ProgramState=GAMETITLE;

char				ArrowState_Joy[10];

uint32_t				PressedKey_Joy[10];

BOOL				g_bActive;



char				GameMode=MODE_HARD;
char Data[MAX_DATA+1][14];
char Data_Judge[MAX_DATA+1][14];
double Data_y[MAX_DATA+1];
char Data1[MAX_DATA+1][14];
char Data_Judge1[MAX_DATA+1][14];
double Data_y1[MAX_DATA+1];
char Data_Double[MAX_DATA+1][14];
char Data_Double_Judge[MAX_DATA+1][14];
double Data_Double_y[MAX_DATA+1];
bool Couple = false, Double = false;

// Couple now in player.h
// Double now in player.h

char				First;
int start1;


void *hWnd;
void *g_hInst;

GfxDevice* g_pDD                = NULL;
Surface*	g_pDDSPrimary = NULL;
Surface* g_pDDSBack    = NULL;

Surface*	GameTITLE		= NULL;
Surface*	Background		= NULL;
Surface*	SongTitle 		= NULL;
Surface*	SongBack 		= NULL;
Surface*	SelectBack		= NULL;
Surface*	JudgeFont		= NULL;
Surface*	NumberFont		= NULL;
Surface*	ComboFont		= NULL;
Surface* NoDISC			= NULL;
Surface*	ShiftLeft		= NULL;
Surface*	ShiftRight		= NULL;
Surface*	GaugeWaku		= NULL;
Surface* Gauge			= NULL;
Surface* Score			= NULL;
Surface* DeadScreen		= NULL;
Surface* GameOver		= NULL;
Surface* Logo			= NULL;
Surface*	Diff			= NULL;
Surface*	DoubleIcon		= NULL;
Surface*	CrazyIcon		= NULL;
Surface*	EasyIcon		= NULL;
Surface*	HardIcon		= NULL;

Surface*	SmallFont		= NULL;
Surface*	Arrow1			= NULL;
Surface*	Arrow2			= NULL;
Surface*	wArrow			= NULL;

Surface*	pArrow1			= NULL;
Surface*	pArrow3			= NULL;
Surface*	pArrow5			= NULL;
Surface*	pArrow7			= NULL;
Surface*	pArrow9			= NULL;

Surface*	cArrow1			= NULL;
Surface*	cArrow3			= NULL;
Surface*	cArrow5			= NULL;
Surface*	cArrow7			= NULL;
Surface*	cArrow9			= NULL;

Surface*	ModeIcon		= NULL;
Surface*	g_cFont			= NULL;

Surface*	ResultFont		= NULL;
Surface*	ResultBack		= NULL;
Surface*	StageCount		= NULL;

AudioDev*		lpds			= NULL;
Sound*	lpdsbd			= NULL;


SONG				CSONG[512];

bool	SongFlag;
bool	IntroFlag;




int		MaxSpeed;
int		MinSpeed;

int		JudgeArray[110];



int	ALPHA=0;
int	inc=20;
uint32_t	CKey_CFont;
uint32_t	CKey_Arr;



// Data of configuration
KIUCONFIG	KCFG;

uint32_t		dwGameCount;

BOOL debugflag=TRUE;
char g_szDebugName[MAX_PATH];

BOOL	g_fullscreen=FALSE;

CMedia *intro;
CMedia *song;

Rect                    g_rcViewport;           // Pos. & size to blt from
Rect                    g_rcScreen;             // Screen pos. for blt

void	DebugPrintf(char *str,...)
{
    if(debugflag)
	{
		char	szBuff[128];
		va_list va;
		FILE	*F;

		va_start(va, str);
		vsprintf(szBuff, str, va);
		F=fopen(g_szDebugName,"a");
		fprintf(F,szBuff);
		va_end(va);
		fclose(F);
	}
}

void	JudgementSet(void)
{
	int i;

	for(i=50;i<110;i++) JudgeArray[i] = BAD;
	for(i=57;i<104;i++) JudgeArray[i] = GOOD;
	for(i=64;i<96;i++)	JudgeArray[i] = GREAT;
	for(i=73;i<88;i++)	JudgeArray[i] = PERFECT;
}

void	DisplayStageCount(uint32_t Count)
{
	Rect	sssRect;
	
	sssRect.top=0;
	sssRect.left=Count*80;
	sssRect.right=sssRect.left+80;
	sssRect.bottom=46;

	g_pDDSBack->BltFast(280, 0, StageCount, &sssRect, DDBLTFAST_SRCCOLORKEY);
}



void ClearMode(void)
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
		g_p1.randomS=FALSE;
		g_p1.suddenR=FALSE;

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
		Double=FALSE;
		g_p2.randomS=FALSE;
		g_p2.suddenR=FALSE;
}

void	GameOver1(void)
{
	static int count;

	if(First==0)
	{
		count=0;
		First++;
	}
	count++;
	g_pDDSBack->BltFast(0,0, GameOver, NULL, DDBLTFAST_NOCOLORKEY);
	if(count==60)First=0, g_ProgramState=GAMETITLE;

	Flipp();
}

void DrawBackground(char Data[][14], uint32_t i, int temp)
{
	g_pDDSBack->BltFast(0,0,SongBack,NULL, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
}

void DrawScore1p(void) { Renderer::drawScore(g_p1, 20, 444); }
void DrawScore2p(void) { Renderer::drawScore(g_p2, 463, 444); }

void DrawGauge1p(void) { Renderer::drawGauge(g_p1, 32, 280, -1); }
void DrawGauge2p(void) { Renderer::drawGauge(g_p2, 352, 352, 1); }


void KIU_STAGE(void)
{
	static int temp;
	static uint32_t i;
	static uint32_t cur,last,sec;
	static uint32_t starttime, curtime;

	static Rect rect1[7],rect3[7],rect5[7],rect7[7],rect9[7];
	int k;
	uint32_t delta;

	static int sta;

	static double tail;

	static time_t t;

	static	int	hr;

	char s[50];

	double bpmpix=(PUMP_SPRITE_Y)*bpm/60000;

	DrawBackground(Data,i,temp);
	DisplayStageCount(dwGameCount);
	
	
	// Display debug msg.
	sprintf(s, "start1: %d", start);
	DisplayMessage(0,0,s);
	sprintf(s, "start2: %d", start2);
	DisplayMessage(0,16,s);
	sprintf(s, "start3: %d", start3);
	DisplayMessage(0,32,s);
	sprintf(s, "bpm 1:%g", bpm);
	DisplayMessage(0,48,s);
	sprintf(s, "bpm 2:%g", bpm2);
	DisplayMessage(0,64,s);
	sprintf(s, "bpm 3:%g", bpm3);
	DisplayMessage(0,80,s);
	sprintf(s, "bunki :%ld", bunki);
	DisplayMessage(0,96,s);
	sprintf(s, "bunki2:%ld", bunki2);
	DisplayMessage(0,112,s);

	if(start1==0)
	{
				if(g_p1.dMix==TRUE)
				{
					MinSpeed = Renderer::minSpeedOf(g_p1);
					MaxSpeed = Renderer::maxSpeedOf(g_p1);
				}
				else 		{
					MaxSpeed = MinSpeed = g_p1.speedBase;
					g_p1.speed1 = g_p1.speed3 = g_p1.speed5 = g_p1.speed7 = g_p1.speed9 = g_p1.speedBase;
				}

		if(g_p2.dMix)
		{
			MaxSpeed=MinSpeed=g_p2.speed1;

			MaxSpeed = max( MaxSpeed, g_p2.speed3);
			MaxSpeed = max( MaxSpeed, g_p2.speed5);
			MaxSpeed = max( MaxSpeed, g_p2.speed7);
			MaxSpeed = max( MaxSpeed, g_p2.speed9);
			
			MinSpeed = min( MinSpeed, g_p2.speed3);
			MinSpeed = min( MinSpeed, g_p2.speed5);
			MinSpeed = min( MinSpeed, g_p2.speed7);
			MinSpeed = min( MinSpeed, g_p2.speed9);
		}
		else 
		{
			MaxSpeed = MinSpeed = g_p2.speedBase;
			g_p2.speed1 = g_p2.speed3 = g_p2.speed5 = g_p2.speed7 = g_p2.speed9 = g_p2.speedBase;
		}

		for(sta=0;sta<6;sta++)
		{
			rect7[sta].top=0;
			rect7[sta].left=PUMP_SPRITE_NEW*sta;
			rect7[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect7[sta].bottom=PUMP_SPRITE_NEW;

			rect9[sta].top=PUMP_SPRITE_NEW;
			rect9[sta].left=PUMP_SPRITE_NEW*sta;
			rect9[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect9[sta].bottom=PUMP_SPRITE_NEW*2;

			rect5[sta].top=PUMP_SPRITE_NEW*2;
			rect5[sta].left=PUMP_SPRITE_NEW*sta;
			rect5[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect5[sta].bottom=PUMP_SPRITE_NEW*3;

			rect3[sta].top=PUMP_SPRITE_NEW*3;
			rect3[sta].left=PUMP_SPRITE_NEW*sta;
			rect3[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect3[sta].bottom=PUMP_SPRITE_NEW*4;

			rect1[sta].top=PUMP_SPRITE_NEW*4;
			rect1[sta].left=PUMP_SPRITE_NEW*sta;
			rect1[sta].right=PUMP_SPRITE_NEW+PUMP_SPRITE_NEW*sta;
			rect1[sta].bottom=PUMP_SPRITE_NEW*5;
		}
		sta=0;

		g_p1.gauge=10;
		g_p2.gauge=10;

		if(g_p1.random)
		{
			srand((unsigned) time(&t));
			
			for(i=0;i<MAX_DATA;i++)
			{
				Data[MAX_DATA][0]=Data[i][0];
				Data[MAX_DATA][1]=Data[i][1];
				Data[MAX_DATA][2]=Data[i][2];
				Data[MAX_DATA][3]=Data[i][3];
				Data[MAX_DATA][4]=Data[i][4];

				if(Data[i][0]=='2')break;
				
				Data[i][0]=Data[i][1]=Data[i][2]=Data[i][3]=Data[i][4]='0';

				if(Data[MAX_DATA][0]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][1]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][2]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][3]=='1')Data[i][rand()%5]='1';
				if(Data[MAX_DATA][4]=='1')Data[i][rand()%5]='1';
			}
		}
		
		if(g_p2.random)
		{
			if(!g_p1.random)srand((unsigned) time(&t));
			
			for(i=0;i<MAX_DATA;i++)
			{
				Data1[MAX_DATA][5]=Data1[i][5];
				Data1[MAX_DATA][6]=Data1[i][6];
				Data1[MAX_DATA][7]=Data1[i][7];
				Data1[MAX_DATA][8]=Data1[i][8];
				Data1[MAX_DATA][9]=Data1[i][9];

				if(Data1[i][0]=='2')break;
				
				Data1[i][5]=Data1[i][6]=Data1[i][7]=Data1[i][8]=Data1[i][9]='0';

				if(Data1[MAX_DATA][5]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][6]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][7]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][8]=='1')Data1[i][5+rand()%5]='1';
				if(Data1[MAX_DATA][9]=='1')Data1[i][5+rand()%5]='1';
			}
		}

		if(g_p1.mirror)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				Data[MAX_DATA][0]=Data[i][0];
				Data[MAX_DATA][1]=Data[i][1];
				Data[MAX_DATA][2]=Data[i][2];
				Data[MAX_DATA][3]=Data[i][3];
				Data[MAX_DATA][4]=Data[i][4];

				if(Data[i][0]=='2')break;

				if(Data[MAX_DATA][0]=='1')Data[i][3]='1'; else Data[i][3]='0'; 
				if(Data[MAX_DATA][1]=='1')Data[i][4]='1'; else Data[i][4]='0'; 
				if(Data[MAX_DATA][2]=='1')Data[i][2]='1'; else Data[i][2]='0'; 
				if(Data[MAX_DATA][3]=='1')Data[i][0]='1'; else Data[i][0]='0'; 
				if(Data[MAX_DATA][4]=='1')Data[i][1]='1'; else Data[i][1]='0'; 

			}
		}
		if(g_p2.mirror)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				Data1[MAX_DATA][5]=Data1[i][5];
				Data1[MAX_DATA][6]=Data1[i][6];
				Data1[MAX_DATA][7]=Data1[i][7];
				Data1[MAX_DATA][8]=Data1[i][8];
				Data1[MAX_DATA][9]=Data1[i][9];

				if(Data1[i][0]=='2')break;

				if(Data1[MAX_DATA][5]=='1')Data1[i][8]='1'; else Data1[i][8]='0'; 
				if(Data1[MAX_DATA][6]=='1')Data1[i][9]='1'; else Data1[i][9]='0'; 
				if(Data1[MAX_DATA][7]=='1')Data1[i][7]='1'; else Data1[i][7]='0'; 
				if(Data1[MAX_DATA][8]=='1')Data1[i][5]='1'; else Data1[i][5]='0'; 
				if(Data1[MAX_DATA][9]=='1')Data1[i][6]='1'; else Data1[i][6]='0'; 

			}
		}
		
		memcpy(&Data_Judge,&Data,sizeof(Data));
		memcpy(&Data_Judge1,&Data1,sizeof(Data));
	
		if(g_p1.nonstep)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				if(!(Data[i][0]=='0' && Data[i][1]=='0' && Data[i][2]=='0' && Data[i][3]=='0' && Data[i][4]=='0'))
				{
					i++;
					for(;i<MAX_DATA;i++)
					{
						if(Data[i][0]=='2')i=MAX_DATA;
						else
						{
							Data[i][0]='3';
							Data[i][1]='3';
							Data[i][2]='3';
							Data[i][3]='3';
							Data[i][4]='3';
						}
					}
				}
			}
		}
		
		if(g_p2.nonstep)
		{
			for(i=0;i<MAX_DATA;i++)
			{
				if(!(Data1[i][5]=='0' && Data1[i][6]=='0' && Data1[i][7]=='0' && Data1[i][8]=='0' && Data1[i][9]=='0'))
				{
					i++;
					for(;i<MAX_DATA;i++)
					{
						if(Data1[i][0]=='2')i=MAX_DATA;
						else
						{
							Data1[i][5]='3';
							Data1[i][6]='3';
							Data1[i][7]='3';
							Data1[i][8]='3';
							Data1[i][9]='3';
						}
					}
				}
			}
		}

		//FadeToSurface(SongBack);
		g_pDDSBack->BltFast(0,0, SongBack, NULL, DDBLTFAST_NOCOLORKEY);

		if(SongFlag==TRUE)
		{
			song->OnMediaPlay();
		}

		start*=10;
		start2*=10;start3*=10;
		bunki*=10;bunki2*=10;
		
		last=cur=timeGetTime();
		tail=0;
		i=0;
		temp=0;
		start1++;
		temp+=55;
		starttime=start;
		curtime=0;
	}
	
	cur=timeGetTime();        // 130/ 600000
	delta=cur-last;
	last=cur;

	if(g_p1.started)DrawArrow1p(i); //ȸ�� ȭ��ǥ�� ���մϴ�.
	if(g_p2.started)DrawArrow2p(i);

	start-=delta;

	if(bunki!=0 && (curtime >= bunki))
	{
		bunki=0;
		bpm=bpm2;
		bpmpix=(PUMP_SPRITE_Y)*bpm/60000;

		starttime=start2;

	}

	if(bunki==0 && bunki2!=0 && (curtime >= bunki2))
	{
		bunki2=0;
		bpm=bpm3;
		bpmpix=(PUMP_SPRITE_Y)*bpm/60000;

		starttime=start3;
	}

	if(start<=0)
	{
		start=0;
		if(SongFlag)
		{
			i=0;
			temp=+55;
			tail=0;

			curtime=(uint32_t)(song->GetCurrentPosition()*1000);

			if(curtime > starttime) 
			delta=(uint32_t)curtime-starttime;
			else delta=(uint32_t)curtime;
		}

		//1000 Tick�� 180/60 -> 1�ʿ� 64*(180/60)  �� 1 tick �� 64*(bpm/60)/1000
		temp-=(int)(delta*bpmpix);
		tail+=(double)((double)(delta*bpmpix)-(int)(delta*bpmpix));

		if(tail>=1)
		{
			temp-=(int)tail;
			tail-=(int)tail;
		}

		if(temp<-100)
		{
			while(temp<-100)
			{
				temp+=(PUMP_SPRITE_Y);

				i+=tick;
			}
		}

	}
	if(timeGetTime()-sec>50)
	{
		sec=timeGetTime();
		if(sta==5)sta=0;
		else sta++;
	}

	if(g_p1.started)
	for(k=0;k<48;k+=tick) 
	{
		if(Data[i][0]=='2' || Data[i+1][0]=='2' || Data[i+2][0]=='2' || Data[i+3][0]=='2')
		{
			k=48;
			if(SongFlag)
			{
				song->OnMediaStop();
				delete song;
				SongFlag=FALSE;
			}
			g_ProgramState=RESULT;

			First=0;
		}

		if(tick==2)
		{
			if(Data[i+k][0]=='1')
				ClpBlt(LP1_X,(temp+PUMP_SPRITE_Y*k/2)*g_p1.speed1-(PUMP_SPRITE_Y)*(g_p1.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][1]=='1')
				ClpBlt(LP7_X,(temp+PUMP_SPRITE_Y*k/2)*g_p1.speed7-(PUMP_SPRITE_Y)*(g_p1.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][2]=='1')
				ClpBlt(LP5_X,(temp+PUMP_SPRITE_Y*k/2)*g_p1.speed5-(PUMP_SPRITE_Y)*(g_p1.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][3]=='1')
				ClpBlt(LP9_X,(temp+PUMP_SPRITE_Y*k/2)*g_p1.speed9-(PUMP_SPRITE_Y)*(g_p1.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][4]=='1')
				ClpBlt(LP3_X,(temp+PUMP_SPRITE_Y*k/2)*g_p1.speed3-(PUMP_SPRITE_Y)*(g_p1.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			
			Data_y[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data[i+k+1][0]=='1')
				ClpBlt(LP1_X,(25+temp+PUMP_SPRITE_Y*k/2)*g_p1.speed1-(PUMP_SPRITE_Y)*(g_p1.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][1]=='1')
				ClpBlt(LP7_X,(25+temp+PUMP_SPRITE_Y*k/2)*g_p1.speed7-(PUMP_SPRITE_Y)*(g_p1.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][2]=='1')
				ClpBlt(LP5_X,(25+temp+PUMP_SPRITE_Y*k/2)*g_p1.speed5-(PUMP_SPRITE_Y)*(g_p1.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][3]=='1')
				ClpBlt(LP9_X,(25+temp+PUMP_SPRITE_Y*k/2)*g_p1.speed9-(PUMP_SPRITE_Y)*(g_p1.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][4]=='1')
				ClpBlt(LP3_X,(25+temp+PUMP_SPRITE_Y*k/2)*g_p1.speed3-(PUMP_SPRITE_Y)*(g_p1.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+1]=(25+temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(g_p1.suddenR)
			{
				if(Data_y[i+k]>240 && Data_y[i+k]<290)
				{
					Data[MAX_DATA][0]=Data[i+k][0];
					Data[MAX_DATA][1]=Data[i+k][1];
					Data[MAX_DATA][2]=Data[i+k][2];
					Data[MAX_DATA][3]=Data[i+k][3];
					Data[MAX_DATA][4]=Data[i+k][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k][0]=Data[i+k][1]=Data[i+k][2]=Data[i+k][3]=Data[i+k][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k][rand()%5]='1';

						Data_Judge[i+k][0]=Data[i+k][0];
						Data_Judge[i+k][1]=Data[i+k][1];
						Data_Judge[i+k][2]=Data[i+k][2];
						Data_Judge[i+k][3]=Data[i+k][3];
						Data_Judge[i+k][4]=Data[i+k][4];
					}
				}

				if(Data_y[i+k+1]>240 && Data_y[i+k+1]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+1][0];
					Data[MAX_DATA][1]=Data[i+k+1][1];
					Data[MAX_DATA][2]=Data[i+k+1][2];
					Data[MAX_DATA][3]=Data[i+k+1][3];
					Data[MAX_DATA][4]=Data[i+k+1][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+1][0]=Data[i+k+1][1]=Data[i+k+1][2]=Data[i+k+1][3]=Data[i+k+1][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+1][rand()%5]='1';

						Data_Judge[i+k+1][0]=Data[i+k+1][0];
						Data_Judge[i+k+1][1]=Data[i+k+1][1];
						Data_Judge[i+k+1][2]=Data[i+k+1][2];
						Data_Judge[i+k+1][3]=Data[i+k+1][3];
						Data_Judge[i+k+1][4]=Data[i+k+1][4];
					}
				}
			}
		}	
		else if(tick==4)
		{
			if(Data[i+k][0]=='1')
				ClpBlt(LP1_X,(temp+PUMP_SPRITE_Y*k/4)*g_p1.speed1-(PUMP_SPRITE_Y)*(g_p1.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][1]=='1')
				ClpBlt(LP7_X,(temp+PUMP_SPRITE_Y*k/4)*g_p1.speed7-(PUMP_SPRITE_Y)*(g_p1.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][2]=='1')
				ClpBlt(LP5_X,(temp+PUMP_SPRITE_Y*k/4)*g_p1.speed5-(PUMP_SPRITE_Y)*(g_p1.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][3]=='1')
				ClpBlt(LP9_X,(temp+PUMP_SPRITE_Y*k/4)*g_p1.speed9-(PUMP_SPRITE_Y)*(g_p1.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k][4]=='1')
				ClpBlt(LP3_X,(temp+PUMP_SPRITE_Y*k/4)*g_p1.speed3-(PUMP_SPRITE_Y)*(g_p1.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data[i+k+1][0]=='1')
				ClpBlt(LP1_X,(12+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed1-(PUMP_SPRITE_Y)*(g_p1.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][1]=='1')
				ClpBlt(LP7_X,(12+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed7-(PUMP_SPRITE_Y)*(g_p1.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][2]=='1')
				ClpBlt(LP5_X,(12+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed5-(PUMP_SPRITE_Y)*(g_p1.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][3]=='1')
				ClpBlt(LP9_X,(12+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed9-(PUMP_SPRITE_Y)*(g_p1.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+1][4]=='1')
				ClpBlt(LP3_X,(12+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed3-(PUMP_SPRITE_Y)*(g_p1.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data[i+k+2][0]=='1')
				ClpBlt(LP1_X,(25+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed1-(PUMP_SPRITE_Y)*(g_p1.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][1]=='1')
				ClpBlt(LP7_X,(25+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed7-(PUMP_SPRITE_Y)*(g_p1.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][2]=='1')
				ClpBlt(LP5_X,(25+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed5-(PUMP_SPRITE_Y)*(g_p1.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][3]=='1')
				ClpBlt(LP9_X,(25+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed9-(PUMP_SPRITE_Y)*(g_p1.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+2][4]=='1')
				ClpBlt(LP3_X,(25+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed3-(PUMP_SPRITE_Y)*(g_p1.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data[i+k+3][0]=='1')
				ClpBlt(LP1_X,(38+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed1-(PUMP_SPRITE_Y)*(g_p1.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][1]=='1')
				ClpBlt(LP7_X,(38+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed7-(PUMP_SPRITE_Y)*(g_p1.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][2]=='1')
				ClpBlt(LP5_X,(38+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed5-(PUMP_SPRITE_Y)*(g_p1.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][3]=='1')
				ClpBlt(LP9_X,(38+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed9-(PUMP_SPRITE_Y)*(g_p1.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data[i+k+3][4]=='1')
				ClpBlt(LP3_X,(38+temp+PUMP_SPRITE_Y*k/4)*g_p1.speed3-(PUMP_SPRITE_Y)*(g_p1.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y[i+k+3]=(38+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(g_p1.suddenR)
			{
				if(Data_y[i+k]>240 && Data_y[i+k]<290)
				{
					Data[MAX_DATA][0]=Data[i+k][0];
					Data[MAX_DATA][1]=Data[i+k][1];
					Data[MAX_DATA][2]=Data[i+k][2];
					Data[MAX_DATA][3]=Data[i+k][3];
					Data[MAX_DATA][4]=Data[i+k][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k][0]=Data[i+k][1]=Data[i+k][2]=Data[i+k][3]=Data[i+k][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k][rand()%5]='1';

						Data_Judge[i+k][0]=Data[i+k][0];
						Data_Judge[i+k][1]=Data[i+k][1];
						Data_Judge[i+k][2]=Data[i+k][2];
						Data_Judge[i+k][3]=Data[i+k][3];
						Data_Judge[i+k][4]=Data[i+k][4];
					}
				}
				if(Data_y[i+k+1]>240 && Data_y[i+k+1]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+1][0];
					Data[MAX_DATA][1]=Data[i+k+1][1];
					Data[MAX_DATA][2]=Data[i+k+1][2];
					Data[MAX_DATA][3]=Data[i+k+1][3];
					Data[MAX_DATA][4]=Data[i+k+1][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+1][0]=Data[i+k+1][1]=Data[i+k+1][2]=Data[i+k+1][3]=Data[i+k+1][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+1][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+1][rand()%5]='1';

						Data_Judge[i+k+1][0]=Data[i+k+1][0];
						Data_Judge[i+k+1][1]=Data[i+k+1][1];
						Data_Judge[i+k+1][2]=Data[i+k+1][2];
						Data_Judge[i+k+1][3]=Data[i+k+1][3];
						Data_Judge[i+k+1][4]=Data[i+k+1][4];
					}
				}
				if(Data_y[i+k+2]>240 && Data_y[i+k+2]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+2][0];
					Data[MAX_DATA][1]=Data[i+k+2][1];
					Data[MAX_DATA][2]=Data[i+k+2][2];
					Data[MAX_DATA][3]=Data[i+k+2][3];
					Data[MAX_DATA][4]=Data[i+k+2][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+2][0]=Data[i+k+2][1]=Data[i+k+2][2]=Data[i+k+2][3]=Data[i+k+2][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+2][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+2][rand()%5]='1';

						Data_Judge[i+k+2][0]=Data[i+k+2][0];
						Data_Judge[i+k+2][1]=Data[i+k+2][1];
						Data_Judge[i+k+2][2]=Data[i+k+2][2];
						Data_Judge[i+k+2][3]=Data[i+k+2][3];
						Data_Judge[i+k+2][4]=Data[i+k+2][4];
					}
				}
				if(Data_y[i+k+3]>240 && Data_y[i+k+3]<290)
				{
					Data[MAX_DATA][0]=Data[i+k+3][0];
					Data[MAX_DATA][1]=Data[i+k+3][1];
					Data[MAX_DATA][2]=Data[i+k+3][2];
					Data[MAX_DATA][3]=Data[i+k+3][3];
					Data[MAX_DATA][4]=Data[i+k+3][4];

					if(Data[MAX_DATA][0]!='2')
					{
						Data[i+k+3][0]=Data[i+k+3][1]=Data[i+k+3][2]=Data[i+k+3][3]=Data[i+k+3][4]='0';

						if(Data[MAX_DATA][0]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][1]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][2]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][3]=='1')Data[i+k+3][rand()%5]='1';
						if(Data[MAX_DATA][4]=='1')Data[i+k+3][rand()%5]='1';

						Data_Judge[i+k+3][0]=Data[i+k+3][0];
						Data_Judge[i+k+3][1]=Data[i+k+3][1];
						Data_Judge[i+k+3][2]=Data[i+k+3][2];
						Data_Judge[i+k+3][3]=Data[i+k+3][3];
						Data_Judge[i+k+3][4]=Data[i+k+3][4];
					}
				}
			}
		}
	}

	if(g_p2.started)
	for(k=0;k<48;k+=tick) 
	{
		if(Data1[i][0]=='2' || Data1[i+1][0]=='2' || Data1[i+2][0]=='2' || Data1[i+3][0]=='2')
		{
			k=48;
			if(SongFlag)
			{
				song->OnMediaStop();
				delete song;
				SongFlag=FALSE;
			}
			g_ProgramState=RESULT;

			First=0;
		}

		if(tick==2)
		{
			if(Data1[i+k][5]=='1')
				ClpBlt(LP1_X1,(temp+PUMP_SPRITE_Y*k/2)*g_p2.speed1-(PUMP_SPRITE_Y)*(g_p2.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][6]=='1')
				ClpBlt(LP7_X1,(temp+PUMP_SPRITE_Y*k/2)*g_p2.speed7-(PUMP_SPRITE_Y)*(g_p2.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][7]=='1')
				ClpBlt(LP5_X1,(temp+PUMP_SPRITE_Y*k/2)*g_p2.speed5-(PUMP_SPRITE_Y)*(g_p2.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][8]=='1')
				ClpBlt(LP9_X1,(temp+PUMP_SPRITE_Y*k/2)*g_p2.speed9-(PUMP_SPRITE_Y)*(g_p2.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][9]=='1')
				ClpBlt(LP3_X1,(temp+PUMP_SPRITE_Y*k/2)*g_p2.speed3-(PUMP_SPRITE_Y)*(g_p2.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			
			Data_y1[i+k]=(temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
			
			if(Data1[i+k+1][5]=='1')
				ClpBlt(LP1_X1,(25+temp+PUMP_SPRITE_Y*k/2)*g_p2.speed1-(PUMP_SPRITE_Y)*(g_p2.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][6]=='1')
				ClpBlt(LP7_X1,(25+temp+PUMP_SPRITE_Y*k/2)*g_p2.speed7-(PUMP_SPRITE_Y)*(g_p2.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][7]=='1')
				ClpBlt(LP5_X1,(25+temp+PUMP_SPRITE_Y*k/2)*g_p2.speed5-(PUMP_SPRITE_Y)*(g_p2.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][8]=='1')
				ClpBlt(LP9_X1,(25+temp+PUMP_SPRITE_Y*k/2)*g_p2.speed9-(PUMP_SPRITE_Y)*(g_p2.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][9]=='1')
				ClpBlt(LP3_X1,(25+temp+PUMP_SPRITE_Y*k/2)*g_p2.speed3-(PUMP_SPRITE_Y)*(g_p2.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+1]=(25+temp+PUMP_SPRITE_Y*k/2)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(g_p2.suddenR)
			{
				if(Data_y1[i+k]>240 && Data_y1[i+k]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k][5];
					Data1[MAX_DATA][6]=Data1[i+k][6];
					Data1[MAX_DATA][7]=Data1[i+k][7];
					Data1[MAX_DATA][8]=Data1[i+k][8];
					Data1[MAX_DATA][9]=Data1[i+k][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k][5]=Data1[i+k][6]=Data1[i+k][7]=Data1[i+k][8]=Data1[i+k][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k][5 + rand()%5]='1';

						Data_Judge1[i+k][5]=Data1[i+k][5];
						Data_Judge1[i+k][6]=Data1[i+k][6];
						Data_Judge1[i+k][7]=Data1[i+k][7];
						Data_Judge1[i+k][8]=Data1[i+k][8];
						Data_Judge1[i+k][9]=Data1[i+k][9];
					}
				}
				if(Data_y1[i+k+1]>240 && Data_y1[i+k+1]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+1][5];
					Data1[MAX_DATA][6]=Data1[i+k+1][6];
					Data1[MAX_DATA][7]=Data1[i+k+1][7];
					Data1[MAX_DATA][8]=Data1[i+k+1][8];
					Data1[MAX_DATA][9]=Data1[i+k+1][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+1][5]=Data1[i+k+1][6]=Data1[i+k+1][7]=Data1[i+k+1][8]=Data1[i+k+1][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+1][5 + rand()%5]='1';

						Data_Judge1[i+k+1][5]=Data1[i+k+1][5];
						Data_Judge1[i+k+1][6]=Data1[i+k+1][6];
						Data_Judge1[i+k+1][7]=Data1[i+k+1][7];
						Data_Judge1[i+k+1][8]=Data1[i+k+1][8];
						Data_Judge1[i+k+1][9]=Data1[i+k+1][9];
					}
				}
			}
		}	
		else if(tick==4)
		{

			if(Data1[i+k][5]=='1')
				ClpBlt(LP1_X1,(temp+PUMP_SPRITE_Y*k/4)*g_p2.speed1-(PUMP_SPRITE_Y)*(g_p2.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][6]=='1')
				ClpBlt(LP7_X1,(temp+PUMP_SPRITE_Y*k/4)*g_p2.speed7-(PUMP_SPRITE_Y)*(g_p2.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][7]=='1')
				ClpBlt(LP5_X1,(temp+PUMP_SPRITE_Y*k/4)*g_p2.speed5-(PUMP_SPRITE_Y)*(g_p2.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][8]=='1')
				ClpBlt(LP9_X1,(temp+PUMP_SPRITE_Y*k/4)*g_p2.speed9-(PUMP_SPRITE_Y)*(g_p2.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k][9]=='1')
				ClpBlt(LP3_X1,(temp+PUMP_SPRITE_Y*k/4)*g_p2.speed3-(PUMP_SPRITE_Y)*(g_p2.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k]=(temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data1[i+k+1][5]=='1')
				ClpBlt(LP1_X1,(12+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed1-(PUMP_SPRITE_Y)*(g_p2.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][6]=='1')
				ClpBlt(LP7_X1,(12+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed7-(PUMP_SPRITE_Y)*(g_p2.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][7]=='1')
				ClpBlt(LP5_X1,(12+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed5-(PUMP_SPRITE_Y)*(g_p2.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][8]=='1')
				ClpBlt(LP9_X1,(12+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed9-(PUMP_SPRITE_Y)*(g_p2.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+1][9]=='1')
				ClpBlt(LP3_X1,(12+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed3-(PUMP_SPRITE_Y)*(g_p2.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+1]=(12+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(Data1[i+k+2][5]=='1')
				ClpBlt(LP1_X1,(25+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed1-(PUMP_SPRITE_Y)*(g_p2.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][6]=='1')
				ClpBlt(LP7_X1,(25+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed7-(PUMP_SPRITE_Y)*(g_p2.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][7]=='1')
				ClpBlt(LP5_X1,(25+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed5-(PUMP_SPRITE_Y)*(g_p2.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][8]=='1')
				ClpBlt(LP9_X1,(25+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed9-(PUMP_SPRITE_Y)*(g_p2.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+2][9]=='1')
				ClpBlt(LP3_X1,(25+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed3-(PUMP_SPRITE_Y)*(g_p2.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+2]=(25+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);
		
			if(Data1[i+k+3][5]=='1')
				ClpBlt(LP1_X1,(38+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed1-(PUMP_SPRITE_Y)*(g_p2.speed1-1),wArrow,&rect1[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][6]=='1')
				ClpBlt(LP7_X1,(38+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed7-(PUMP_SPRITE_Y)*(g_p2.speed7-1),wArrow,&rect7[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][7]=='1')
				ClpBlt(LP5_X1,(38+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed5-(PUMP_SPRITE_Y)*(g_p2.speed5-1),wArrow,&rect5[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][8]=='1')
				ClpBlt(LP9_X1,(38+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed9-(PUMP_SPRITE_Y)*(g_p2.speed9-1),wArrow,&rect9[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
			if(Data1[i+k+3][9]=='1')
				ClpBlt(LP3_X1,(38+temp+PUMP_SPRITE_Y*k/4)*g_p2.speed3-(PUMP_SPRITE_Y)*(g_p2.speed3-1),wArrow,&rect3[sta],DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );

			Data_y1[i+k+3]=(38+temp+PUMP_SPRITE_Y*k/4)*MinSpeed-(PUMP_SPRITE_Y)*(MinSpeed-1);

			if(g_p2.suddenR)
			{
				if(Data_y1[i+k]>240 && Data_y1[i+k]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k][5];
					Data1[MAX_DATA][6]=Data1[i+k][6];
					Data1[MAX_DATA][7]=Data1[i+k][7];
					Data1[MAX_DATA][8]=Data1[i+k][8];
					Data1[MAX_DATA][9]=Data1[i+k][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k][5]=Data1[i+k][6]=Data1[i+k][7]=Data1[i+k][8]=Data1[i+k][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k][5 + rand()%5]='1';

						Data_Judge1[i+k][5]=Data1[i+k][5];
						Data_Judge1[i+k][6]=Data1[i+k][6];
						Data_Judge1[i+k][7]=Data1[i+k][7];
						Data_Judge1[i+k][8]=Data1[i+k][8];
						Data_Judge1[i+k][9]=Data1[i+k][9];
					}
				}
				if(Data_y1[i+k+1]>240 && Data_y1[i+k+1]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+1][5];
					Data1[MAX_DATA][6]=Data1[i+k+1][6];
					Data1[MAX_DATA][7]=Data1[i+k+1][7];
					Data1[MAX_DATA][8]=Data1[i+k+1][8];
					Data1[MAX_DATA][9]=Data1[i+k+1][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+1][5]=Data1[i+k+1][6]=Data1[i+k+1][7]=Data1[i+k+1][8]=Data1[i+k+1][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+1][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+1][5 + rand()%5]='1';

						Data_Judge1[i+k+1][5]=Data1[i+k+1][5];
						Data_Judge1[i+k+1][6]=Data1[i+k+1][6];
						Data_Judge1[i+k+1][7]=Data1[i+k+1][7];
						Data_Judge1[i+k+1][8]=Data1[i+k+1][8];
						Data_Judge1[i+k+1][9]=Data1[i+k+1][9];
					}
				}
				if(Data_y1[i+k+2]>240 && Data_y1[i+k+2]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+2][5];
					Data1[MAX_DATA][6]=Data1[i+k+2][6];
					Data1[MAX_DATA][7]=Data1[i+k+2][7];
					Data1[MAX_DATA][8]=Data1[i+k+2][8];
					Data1[MAX_DATA][9]=Data1[i+k+2][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+2][5]=Data1[i+k+2][6]=Data1[i+k+2][7]=Data1[i+k+2][8]=Data1[i+k+2][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+2][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+2][5 + rand()%5]='1';

						Data_Judge1[i+k+2][5]=Data1[i+k+2][5];
						Data_Judge1[i+k+2][6]=Data1[i+k+2][6];
						Data_Judge1[i+k+2][7]=Data1[i+k+2][7];
						Data_Judge1[i+k+2][8]=Data1[i+k+2][8];
						Data_Judge1[i+k+2][9]=Data1[i+k+2][9];
					}
				}
				if(Data_y1[i+k+3]>240 && Data_y1[i+k+3]<290)
				{
					Data1[MAX_DATA][5]=Data1[i+k+3][5];
					Data1[MAX_DATA][6]=Data1[i+k+3][6];
					Data1[MAX_DATA][7]=Data1[i+k+3][7];
					Data1[MAX_DATA][8]=Data1[i+k+3][8];
					Data1[MAX_DATA][9]=Data1[i+k+3][9];

					if(Data1[MAX_DATA][0]!='2')
					{
						Data1[i+k+3][5]=Data1[i+k+3][6]=Data1[i+k+3][7]=Data1[i+k+3][8]=Data1[i+k+3][9]='0';

						if(Data1[MAX_DATA][5]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][6]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][7]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][8]=='1')Data1[i+k+3][5 + rand()%5]='1';
						if(Data1[MAX_DATA][9]=='1')Data1[i+k+3][5 + rand()%5]='1';

						Data_Judge1[i+k+3][5]=Data1[i+k+3][5];
						Data_Judge1[i+k+3][6]=Data1[i+k+3][6];
						Data_Judge1[i+k+3][7]=Data1[i+k+3][7];
						Data_Judge1[i+k+3][8]=Data1[i+k+3][8];
						Data_Judge1[i+k+3][9]=Data1[i+k+3][9];
					}
				}
			}
		}
	}

	if(g_p1.started)
	{
		if(KCFG.OptJudge)DrawJudge1p();
		DrawGauge1p();
		DrawScore1p();
	}
	if(g_p2.started)
	{
		if(KCFG.OptJudge)DrawJudge2p();
		DrawGauge2p();
		DrawScore2p();
	}

	if(g_p1.mirror)DrawMode(0,200,HMODE_MIRROR);
	if(g_p1.nonstep)DrawMode(0,240,HMODE_NONSTEP);
	if(g_p1.synchro)DrawMode(0,280,HMODE_SYNCHRO);
	if(g_p1.union_)DrawMode(0,320,HMODE_UNION);
	if(g_p1.random)DrawMode(0,360,HMODE_RANDOM);
	if(g_p1.vanish)DrawMode(0,400,HMODE_VANISH);

	if(g_p1.speedBase>1)DrawMode(0,160,HMODE_2X);

	if(g_p2.mirror)DrawMode(600,200,HMODE_MIRROR);
	if(g_p2.nonstep)DrawMode(600,240,HMODE_NONSTEP);
	if(g_p2.union_)DrawMode(600,320,HMODE_UNION);
	if(g_p2.random)DrawMode(600,360,HMODE_RANDOM);
	if(g_p2.vanish)DrawMode(600,400,HMODE_VANISH);

	if(g_p2.speedBase>1)DrawMode(600,160,HMODE_2X);

	Flipp();
}


void WaveSet_Loading(void)
{
	g_audio.loadAll();
}

void DisplayMessage(int x, int y, char * message)
{
#define FONT_SIZE	8
#define FONT_HEIGHT	16

	Rect sRect = { 0, };

	for(int i = 0 ; ; i++) {		
		if(message[i] == NULL)
			break;	
	
		char msg1 = toupper(message[i]);
		
		sRect.top = 0;
		sRect.bottom = FONT_HEIGHT;
		sRect.left = FONT_SIZE * (msg1-' ');
		sRect.right = sRect.left + FONT_SIZE;
		
		g_pDDSBack->BltFast(x, y, SmallFont, &sRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		x += FONT_SIZE;
	}
}

			
int	ClpBlt(int x ,int y ,Surface* ds,Rect* srect,uint32_t mode)
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

	if(g_ProgramState==DOUBLE)
	{
		if(g_p1.vanish || g_p2.vanish)
		{
			if(y<150)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			if(y<250)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-150)*2,CKey_Arr,16);
			if(y>250)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else if(g_p1.suddenR || g_p2.suddenR)
		{
			if(y<=100)hRet = g_pDDSBack->BltFast(x,y,ds,srect,mode);
			else if(y>100 && y<=200)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(200-y)*2,CKey_Arr,16);
			else if(y>200 && y<320)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			else if(y<420)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-320)*2,CKey_Arr,16);
			else if(y>420)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
	}
	else if(x<320)
	{
		if(g_p1.vanish)
		{
			if(y<150)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			if(y<250)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-150)*2,CKey_Arr,16);
			if(y>250)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else if(g_p1.suddenR)
		{
			if(y<=100)hRet = g_pDDSBack->BltFast(x,y,ds,srect,mode);
			else if(y>100 && y<=200)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(200-y)*2,CKey_Arr,16);
			else if(y>200 && y<320)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			else if(y<420)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-320)*2,CKey_Arr,16);
			else if(y>420)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
	}
	else if(x>320)
	{
		if(g_p2.vanish)
		{
			if(y<150)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			if(y<250)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-150)*2,CKey_Arr,16);
			if(y>250)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else if(g_p2.suddenR)
		{
			if(y<=100)hRet = g_pDDSBack->BltFast(x,y,ds,srect,mode);
			else if(y>100 && y<=200)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(200-y)*2,CKey_Arr,16);
			else if(y>200 && y<320)
			{
				memcpy(srect,&sRect,sizeof(sRect));
				return 0;
			}
			else if(y<420)TransAlphaImproved(ds,g_pDDSBack,x,y,sRect,(y-320)*2,CKey_Arr,16);
			else if(y>420)hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
		}
		else hRet=g_pDDSBack->BltFast(x,y,ds,srect,mode);
	}

	memcpy(srect,&sRect,sizeof(sRect));
	
	return hRet;

}


void DrawJudge1p(void)
{
	static uint32_t cur, last;
	static char LastJudge;

	char chCombo1p[255];

	static uint32_t sec,delta;

	Rect rRect, cRect, destRect;
	int Loop;

	if(g_p1.judgement)
	{
		g_p1.state=0;
	}
	else if(g_p1.state)
	{
		g_p1.judgement=LastJudge;

		delta=timeGetTime()-sec;
		if(delta>16)
		{
			sec=timeGetTime();
			if(g_p1.state>=40)
			{
				g_p1.state=0;
				g_p1.judgement=NONE;
			}
			else
			{
				if(delta >16 && delta <32)g_p1.state+=2;
				else if(delta >=32)g_p1.state+=4;
			}
		}
	}

	switch(g_p1.judgement)
	{
		case NONE:LastJudge=NONE;
			break;
		case PERFECT:
			LastJudge=PERFECT;
			rRect.top=0;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y;
			if(g_p1.state==0)g_p1.state++;
			break;
		case GREAT:
			LastJudge=GREAT;
			rRect.top=JUDGE_SIZE_Y;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*2;
			if(g_p1.state==0)g_p1.state++;
			break;
		case GOOD:
			LastJudge=GOOD;
			rRect.top=JUDGE_SIZE_Y*2;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*3;
			if(g_p1.state==0)g_p1.state++;
			break;
		case BAD:
			LastJudge=BAD;
			rRect.top=JUDGE_SIZE_Y*3;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*4;
			if(g_p1.state==0)g_p1.state++;
			break;
		case MISS:
			LastJudge=MISS;
			rRect.top=JUDGE_SIZE_Y*4;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*5;
			if(g_p1.state==0)g_p1.state++;
			break;
	}

	if(g_p1.state>15)
	{
		destRect.top=200;
		destRect.left=40;
		destRect.right=40+JUDGE_SIZE_X;
		destRect.bottom=200+JUDGE_SIZE_Y;
	}
	else
	{
		destRect.top=200-30+(g_p1.state*2);
		destRect.left=40-60+(g_p1.state*4);
		destRect.right=40+JUDGE_SIZE_X+60-(g_p1.state*4);
		destRect.bottom=200+JUDGE_SIZE_Y+30-(g_p1.state*2);
	}

	if(g_p1.judgement)
	{
		//g_pDDSBack->BltFast(40,200+g_p1.state/2,JudgeFont,&rRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		
		g_pDDSBack->Blt(&destRect, JudgeFont, &rRect,DDBLT_WAIT | DDBLT_KEYSRC , NULL);

		/* �޺� ��º� �Դϴ�. */
		if((g_p1.judgement==PERFECT || g_p1.judgement==GREAT) && g_p1.combo>3)
		{
			sprintf(chCombo1p,"%03d",g_p1.combo);
			
			for(Loop=0;;Loop++)
			{
				if(chCombo1p[Loop]==NULL)break;
				chCombo1p[Loop]-=48;
				cRect.left=chCombo1p[Loop]*50;
				cRect.right=cRect.left+50;
				cRect.top=0;
				cRect.bottom=65;
				if(g_p1.state>10)g_pDDSBack->BltFast(80+Loop*50,250+g_p1.state*2-g_p1.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(80+Loop*50,250+g_p1.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);

				cRect.left=0;
				cRect.right=150;
				cRect.top=65;
				cRect.bottom=100;
				
				if(g_p1.state>10)g_pDDSBack->BltFast(80,320+g_p1.state*2-g_p1.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(80,320+g_p1.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
			}/* ������� */
		}
	}

	g_p1.judgement=NONE;
}

void DrawJudge2p(void)
{
	static uint32_t cur, last;
	static char LastJudge;

	char chCombo2p[255];

	static uint32_t sec,delta;

	Rect rRect, cRect, destRect;
	int Loop;

	if(g_p2.judgement)
	{
		g_p2.state=0;
	}
	else if(g_p2.state)
	{
		g_p2.judgement=LastJudge;

		delta=timeGetTime()-sec;
		
		if(delta>16)
		{
			sec=timeGetTime();
			if(g_p2.state>=40)
			{
				g_p2.state=0;
				g_p2.judgement=NONE;
			}
			else
			{
				if(delta >16 && delta <32)g_p2.state+=2;
				else if(delta >=32)g_p2.state+=4;
			}
		}
	}

	switch(g_p2.judgement)
	{
		case NONE:LastJudge=NONE;
			break;
		case PERFECT:
			LastJudge=PERFECT;
			rRect.top=0;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y;
			if(g_p2.state==0)g_p2.state++;
			break;
		case GREAT:
			LastJudge=GREAT;
			rRect.top=JUDGE_SIZE_Y;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*2;
			if(g_p2.state==0)g_p2.state++;
			break;
		case GOOD:
			LastJudge=GOOD;
			rRect.top=JUDGE_SIZE_Y*2;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*3;
			if(g_p2.state==0)g_p2.state++;
			break;
		case BAD:
			LastJudge=BAD;
			rRect.top=JUDGE_SIZE_Y*3;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*4;
			if(g_p2.state==0)g_p2.state++;
			break;
		case MISS:
			LastJudge=MISS;
			rRect.top=JUDGE_SIZE_Y*4;
			rRect.right=JUDGE_SIZE_X;
			rRect.left=0;
			rRect.bottom=JUDGE_SIZE_Y*5;
			if(g_p2.state==0)g_p2.state++;
			break;
	}

	if(g_p2.state>15)
	{
		destRect.top=200;
		destRect.left=350;
		destRect.right=350+JUDGE_SIZE_X;
		destRect.bottom=200+JUDGE_SIZE_Y;
	}
	else
	{
		destRect.top=200-30+(g_p2.state*2);
		destRect.left=350-60+(g_p2.state*4);
		destRect.right=350+JUDGE_SIZE_X+60-(g_p2.state*4);
		destRect.bottom=200+JUDGE_SIZE_Y+30-(g_p2.state*2);
	}

	if(g_p2.judgement)
	{
//		g_pDDSBack->BltFast(400,200+dwState22/2,JudgeFont,&rRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

		g_pDDSBack->Blt(&destRect, JudgeFont, &rRect,DDBLT_WAIT | DDBLT_KEYSRC , NULL);

		/* �޺� ��º� �Դϴ�. */
		if((g_p2.judgement==PERFECT || g_p2.judgement==GREAT) && g_p2.combo>3)
		{
			sprintf(chCombo2p,"%03d",g_p2.combo);
			
			for(Loop=0;;Loop++)
			{
				if(chCombo2p[Loop]==NULL)break;
				chCombo2p[Loop]-=48;
				cRect.left=chCombo2p[Loop]*50;
				cRect.right=cRect.left+50;
				cRect.top=0;
				cRect.bottom=65;
//				g_pDDSBack->BltFast(380+Loop*30+dwState22*2,250,NumberFont,&cRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				if(g_p2.state>10)g_pDDSBack->BltFast(400+Loop*50,250+g_p2.state*2-g_p2.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(400+Loop*50,250+g_p2.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);

				cRect.left=0;
				cRect.right=150;
				cRect.top=65;
				cRect.bottom=100;
				
				if(g_p2.state>10)g_pDDSBack->BltFast(400,320+g_p2.state*2-g_p2.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
				else g_pDDSBack->BltFast(400,320+g_p2.state*2,ComboFont,&cRect, DDBLTFAST_SRCCOLORKEY);
			}/* ������� */
		}
	}

	g_p2.judgement=NONE;
}


void DrawArrow1p(uint32_t cur)
{
	static int arrow_l[20]={0,0,72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648};
	static int arrow_r[20]={72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648,720,720};

	static int Carrow_l[20]={0,0,80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640};
	static int Carrow_r[20]={80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640,720,720};

	static uint8_t s1,s3,s5,s7,s9;
	static uint32_t stat1,stat3,stat5,stat7,stat9;
	static uint32_t cur2;
	static int beat;

	static BOOL Crash1, Crash3, Crash5, Crash7, Crash9;
	static BOOL	On1, On3, On5, On7, On9;

	static Rect pArr1,pArr3,pArr5,pArr7,pArr9;
	static Rect cArr1,cArr3,cArr5,cArr7,cArr9;


	uint8_t JudgeTemp=0;
	uint8_t	count;

	if(cur2!=cur)
	{
		if(g_p1.randomS == TRUE)
		{
			g_p1.speed1 = g_p1.speed3 = g_p1.speed5 = g_p1.speed7 = g_p1.speed9 = 1 + rand() % 8 ;
		}
		cur2=cur;
		beat=4;
	}
	else 
	{
		beat--;
		if(beat<=0)beat=0;
	}

	ReadGameInput();
// ���� ��ư ���� �κ� 
	if(KCFG.auto1_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][0]=='1')
				{
					Data_Judge[cur+count][0]='0';
					stat1=cur+count;
					s1=1;
					Crash1=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto7_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][1]=='1')
				{
					Data_Judge[cur+count][1]='0';
					stat7=cur+count;
					s7=1;
					Crash7=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto5_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][2]=='1')
				{
					Data_Judge[cur+count][2]='0';
					stat5=cur+count;
					s5=1;
					Crash5=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][3]=='1')
				{
					Data_Judge[cur+count][3]='0';
					stat9=cur+count;
					s9=1;
					Crash9=TRUE;
					break;
				}
			}
		}
	}

	if(KCFG.auto3_1p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y[cur+count]<50)
			{
				if(Data_Judge[cur+count][4]=='1')
				{
					Data_Judge[cur+count][4]='0';
					stat3=cur+count;
					s3=1;
					Crash3=TRUE;
					break;
				}
			}
			
		}
	}

	if(s1 || (g_p1.pressedKey[1]==TRUE) )
	{
		if(s1==20)
		{
			s1=0;
			Crash1=FALSE;
		}
		else
		{
			s1++;
		}
		if(g_p1.pressedKey[1]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p1.speed1 < Data_y[cur+count] && 
				ZONE_D*g_p1.speed1 > Data_y[cur+count]  )
			{
				if( PERFECT_ZONE_U*g_p1.speed1 < Data_y[cur+count] &&
					PERFECT_ZONE_D*g_p1.speed1 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p1.speed1 < Data_y[cur+count] && 
						 GREAT_ZONE_D*g_p1.speed1 > Data_y[cur+count]  )
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p1.speed1 < Data_y[cur+count] &&
					     GOOD_ZONE_D*g_p1.speed1 > Data_y[cur+count] )
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p1.speed1 < Data_y[cur+count] &&
						 BAD_ZONE_D*g_p1.speed1 > Data_y[cur+count] )
				{
					if(Data_Judge[cur+count][0]=='1')
					{
						Data_Judge[cur+count][0]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s3 || (g_p1.pressedKey[3]==TRUE))
	{
		if(s3==20)
		{
			s3=0;
			Crash3=FALSE;
		}
		else
		{
			s3++;
		}
		if(g_p1.pressedKey[3]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p1.speed3 < Data_y[cur+count] &&
				ZONE_D*g_p1.speed3 > Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*g_p1.speed3 < Data_y[cur+count] &&
				    PERFECT_ZONE_D*g_p1.speed3 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p1.speed3<Data_y[cur+count] &&
					     GREAT_ZONE_D*g_p1.speed3>Data_y[cur+count] )
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p1.speed3 < Data_y[cur+count] &&
						 GOOD_ZONE_D*g_p1.speed3 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p1.speed3<Data_y[cur+count] &&
						 BAD_ZONE_D*g_p1.speed3>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][4]=='1')
					{
						Data_Judge[cur+count][4]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s5 || (g_p1.pressedKey[5]==TRUE))
	{
		if(s5==20)
		{
			s5=0;
			Crash5=FALSE;
		}
		else
		{
			s5++;
		}
		if(g_p1.pressedKey[5]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p1.speed5<Data_y[cur+count] &&
				ZONE_D*g_p1.speed5>Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*g_p1.speed5<Data_y[cur+count] &&
					PERFECT_ZONE_D*g_p1.speed5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p1.speed5<Data_y[cur+count] &&
					     GREAT_ZONE_D*g_p1.speed5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p1.speed5<Data_y[cur+count] &&
					     GOOD_ZONE_D*g_p1.speed5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p1.speed5<Data_y[cur+count] &&
					     BAD_ZONE_D*g_p1.speed5>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][2]=='1')
					{
						Data_Judge[cur+count][2]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
				
			} 
		}

	}

	if(s7 || (g_p1.pressedKey[7]==TRUE) )
	{
		if(s7==20)
		{
			s7=0;
			Crash7=FALSE;
		}
		else
		{
			s7++;
		}
		if(g_p1.pressedKey[7]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p1.speed7 < Data_y[cur+count] &&
				ZONE_D*g_p1.speed7 > Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*g_p1.speed7 < Data_y[cur+count] &&
					PERFECT_ZONE_D*g_p1.speed7 > Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p1.speed7<Data_y[cur+count] &&
					     GREAT_ZONE_D*g_p1.speed7>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p1.speed7<Data_y[cur+count] &&
					     GOOD_ZONE_D*g_p1.speed7>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p1.speed7<Data_y[cur+count] &&
					     BAD_ZONE_D*g_p1.speed7>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][1]=='1')
					{
						Data_Judge[cur+count][1]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}

			}
		}
	}

	if(s9 || (g_p1.pressedKey[9]==TRUE))
	{
		if(s9==20)
		{

			s9=0;
			Crash9=FALSE;
		}
		else
		{
			s9++;
		}
		if(g_p1.pressedKey[9]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p1.speed9<Data_y[cur+count] &&
				ZONE_D*g_p1.speed9>Data_y[cur+count])
			{
				if( PERFECT_ZONE_U*g_p1.speed9<Data_y[cur+count] &&
					PERFECT_ZONE_D*g_p1.speed9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p1.speed9<Data_y[cur+count] &&
					     GREAT_ZONE_D*g_p1.speed9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p1.speed9<Data_y[cur+count] &&
					     GOOD_ZONE_D*g_p1.speed9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p1.speed9<Data_y[cur+count] &&
					     BAD_ZONE_D*g_p1.speed9>Data_y[cur+count])
				{
					if(Data_Judge[cur+count][3]=='1')
					{
						Data_Judge[cur+count][3]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}

	}
	
	g_p1.judgement=JudgeTemp;
	
	// �̽�ó���Դϴ�.
	for(count=0;count<10;count++)
	if(Data_y[cur+count] < ZONE_U
		&& (Data_Judge[cur+count][0]!='0' || Data_Judge[cur+count][1]!='0' || Data_Judge[cur+count][2]!='0' || Data_Judge[cur+count][3]!='0' || Data_Judge[cur+count][4]!='0')
		&& (Data[cur+count][0]!='0' || Data[cur+count][1]!='0' || Data[cur+count][2]!='0' || Data[cur+count][3]!='0' || Data[cur+count][4]!='0' ))
	{
		if(Data[cur+count][0]=='2')break;
		Data_Judge[cur+count][0]=Data_Judge[cur+count][1]=Data_Judge[cur+count][2]=Data_Judge[cur+count][3]=Data_Judge[cur+count][4]='0';
		g_p1.judgement=MISS;
		g_p1.combo=0;
	}


	if(Crash1)
	{
		if(s1==2)if(Data_Judge[stat1][0]=='0' && Data_Judge[stat1][1]=='0' && Data_Judge[stat1][2]=='0' && Data_Judge[stat1][3]=='0' && Data_Judge[stat1][4]=='0' )
		{
			Data[stat1][0]=Data[stat1][1]=Data[stat1][2]=Data[stat1][3]=Data[stat1][4]='0';
			//g_p1.judgement=PERFECT;
			g_p1.judgement=JudgeTemp;
		}
		else g_p1.judgement=NONE;
	}
	if(Crash7)
	{
		if(s7==2)if(Data_Judge[stat7][0]=='0' && Data_Judge[stat7][1]=='0' && Data_Judge[stat7][2]=='0' && Data_Judge[stat7][3]=='0' && Data_Judge[stat7][4]=='0' )
		{
			Data[stat7][0]=Data[stat7][1]=Data[stat7][2]=Data[stat7][3]=Data[stat7][4]='0';
			//g_p1.judgement=PERFECT;
			g_p1.judgement=JudgeTemp;
		}
		else g_p1.judgement=NONE;
	}
	if(Crash5)
	{
		if(s5==2)if(Data_Judge[stat5][0]=='0' && Data_Judge[stat5][1]=='0' && Data_Judge[stat5][2]=='0' && Data_Judge[stat5][3]=='0' && Data_Judge[stat5][4]=='0' )
		{
			Data[stat5][0]=Data[stat5][1]=Data[stat5][2]=Data[stat5][3]=Data[stat5][4]='0';
			//g_p1.judgement=PERFECT;
			g_p1.judgement=JudgeTemp;
		}
		else g_p1.judgement=NONE;
	}
	if(Crash9)
	{
		if(s9==2)if(Data_Judge[stat9][0]=='0' && Data_Judge[stat9][1]=='0' && Data_Judge[stat9][2]=='0' && Data_Judge[stat9][3]=='0' && Data_Judge[stat9][4]=='0' )
		{
			Data[stat9][0]=Data[stat9][1]=Data[stat9][2]=Data[stat9][3]=Data[stat9][4]='0';
			//g_p1.judgement=PERFECT;
			g_p1.judgement=JudgeTemp;
		}
		else g_p1.judgement=NONE;
	}
	if(Crash3)
	{
		if(s3==2)if(Data_Judge[stat3][0]=='0' && Data_Judge[stat3][1]=='0' && Data_Judge[stat3][2]=='0' && Data_Judge[stat3][3]=='0' && Data_Judge[stat3][4]=='0' )
		{
			Data[stat3][0]=Data[stat3][1]=Data[stat3][2]=Data[stat3][3]=Data[stat3][4]='0';
			//g_p1.judgement=PERFECT;
			g_p1.judgement=JudgeTemp;
		}
		else g_p1.judgement=NONE;
	}

	if(g_p1.judgement==PERFECT || g_p1.judgement==GREAT)
	{
		if(g_dsBeat)
		{
			g_dsBeat->Stop();
			g_dsBeat->SetCurrentPosition(0);
			g_dsBeat->Play(0,0,0);
		}
		if(g_p1.judgement==PERFECT)g_p1.perfect++;
		if(g_p1.judgement==GREAT)g_p1.great++;

		g_p1.combo++;
		if(g_p1.combo>g_p1.maxCombo)g_p1.maxCombo=g_p1.combo;
		if(g_p1.combo>10)
		{
			if(g_p1.gauge<0)g_p1.gauge=1;
			else g_p1.gauge++;
			if(g_p1.gauge>42)g_p1.gauge=41;
		}
		Crash1=On1;
		Crash3=On3;
		Crash5=On5;
		Crash7=On7;
		Crash9=On9;

		On1=On3=On5=On7=On9=FALSE;
	}
	else if(g_p1.judgement==GOOD || g_p1.judgement==BAD || g_p1.judgement==MISS)
	{
		if(g_p1.judgement==GOOD)g_p1.good++;
		else if(g_p1.judgement==BAD)g_p1.bad++;
		else if(g_p1.judgement==MISS)g_p1.miss++;
		g_p1.combo=0;
	}

	if(g_p1.combo)
	{
		if(g_p1.judgement==PERFECT)g_p1.score+=2000;
		else if(g_p1.judgement==GREAT)g_p1.score+=1500;
	}
	else
	{
		if(g_p1.judgement==PERFECT)g_p1.score+=1000;
		else if(g_p1.judgement==GREAT)g_p1.score+=500;
	}

	if(g_p1.judgement==GOOD)g_p1.score+=100;
	else if(g_p1.judgement==BAD)
	{
		g_p1.score-=700;
		g_p1.gauge-=2;
	}
	else if(g_p1.judgement==MISS)
	{
		g_p1.score-=1000;
		g_p1.gauge-=5;
	}

	if(KCFG.bcDead)
	{
		if(Couple)
		{
			if(g_p1.gauge<-40 && g_p2.gauge<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
		else
		{
			if(g_p1.gauge<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
	}

	if(g_p1.score<0)g_p1.score=0;

	if (beat) g_pDDSBack->BltFast(32,50,Arrow2,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else g_pDDSBack->BltFast(32,50,Arrow1,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	pArr1.top=0;
	pArr1.left=arrow_l[s1];
	pArr1.right=arrow_r[s1];
	pArr1.bottom=70;

	pArr3.top=0;
	pArr3.left=arrow_l[s3];
	pArr3.right=arrow_r[s3];
	pArr3.bottom=70;

	pArr5.top=0;
	pArr5.left=arrow_l[s5];
	pArr5.right=arrow_r[s5];
	pArr5.bottom=70;

	pArr7.top=0;
	pArr7.left=arrow_l[s7];
	pArr7.right=arrow_r[s7];
	pArr7.bottom=70;

	pArr9.top=0;
	pArr9.left=arrow_l[s9];
	pArr9.right=arrow_r[s9];
	pArr9.bottom=70;

	cArr1.top=0;
	cArr1.left=Carrow_l[s1];
	cArr1.right=Carrow_r[s1];
	cArr1.bottom=80;

	cArr3.top=0;
	cArr3.left=Carrow_l[s3];
	cArr3.right=Carrow_r[s3];
	cArr3.bottom=80;

	cArr5.top=0;
	cArr5.left=Carrow_l[s5];
	cArr5.right=Carrow_r[s5];
	cArr5.bottom=80;

	cArr7.top=0;
	cArr7.left=Carrow_l[s7];
	cArr7.right=Carrow_r[s7];
	cArr7.bottom=80;

	cArr9.top=0;
	cArr9.left=Carrow_l[s9];
	cArr9.right=Carrow_r[s9];
	cArr9.bottom=80;

	if(Crash1)g_pDDSBack->BltFast(25,43,cArrow1,&cArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s1)g_pDDSBack->BltFast(27,45,pArrow1,&pArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash7)g_pDDSBack->BltFast(75,43,cArrow7,&cArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s7)g_pDDSBack->BltFast(77,45,pArrow7,&pArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash5)g_pDDSBack->BltFast(125,43,cArrow5,&cArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s5)g_pDDSBack->BltFast(127,45,pArrow5,&pArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	if(Crash9)g_pDDSBack->BltFast(175,43,cArrow9,&cArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s9)g_pDDSBack->BltFast(177,45,pArrow9,&pArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash3)g_pDDSBack->BltFast(225,43,cArrow3,&cArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s3)g_pDDSBack->BltFast(227,45,pArrow3,&pArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

void DrawArrow2p(uint32_t cur)
{
	static int arrow_l[20]={0,0,72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648};
	static int arrow_r[20]={72,72,144,144,216,216,288,288,360,360,432,432,504,504,576,576,648,648,720,720};

	static int Carrow_l[20]={0,0,80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640};
	static int Carrow_r[20]={80,80,160,160,240,240,320,320,400,400,480,480,560,560,640,640,720,720};

	static uint8_t s1,s3,s5,s7,s9;
	static uint32_t stat1,stat3,stat5,stat7,stat9;
	static uint32_t cur2;
	static int beat;

	static BOOL Crash1, Crash3, Crash5, Crash7, Crash9;
	static BOOL	On1, On3, On5, On7, On9;

	static Rect pArr1,pArr3,pArr5,pArr7,pArr9;
	static Rect cArr1,cArr3,cArr5,cArr7,cArr9;


	uint8_t JudgeTemp=0;
	uint8_t	count;

	
	if(g_p2.started==TRUE && g_p1.started==FALSE)ReadGameInput();
	if(cur2!=cur)
	{
		if(g_p2.randomS == TRUE)
		{
			g_p2.speed1 = g_p2.speed3 = g_p2.speed5 = g_p2.speed7 = g_p2.speed9 = 1 + rand() % 8 ;
		}
		cur2=cur;
		beat=4;
	}
	else 
	{
		beat--;
		if(beat<=0)beat=0;
	}
// ���� ��ư ���� �κ� 
	if(KCFG.auto1_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][5]=='1')
				{
					Data_Judge1[cur+count][5]='0';
					stat1=cur+count;
					s1=1;
					Crash1=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto7_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][6]=='1')
				{
					Data_Judge1[cur+count][6]='0';
					stat7=cur+count;
					s7=1;
					Crash7=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto5_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][7]=='1')
				{
					Data_Judge1[cur+count][7]='0';
					stat5=cur+count;
					s5=1;
					Crash5=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto9_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][8]=='1')
				{
					Data_Judge1[cur+count][8]='0';
					stat9=cur+count;
					s9=1;
					Crash9=TRUE;
					break;
				}
			}
			
		}
	}

	if(KCFG.auto3_2p==TRUE)
	{
		for(count=0;count<10;count++)
		{
			if(Data_y1[cur+count]<50)
			{
				if(Data_Judge1[cur+count][9]=='1')
				{
					Data_Judge1[cur+count][9]='0';
					stat3=cur+count;
					s3=1;
					Crash3=TRUE;
					break;
				}
			}
			
		}
	}

	if(s1 || (g_p2.pressedKey[1]==TRUE) )
	{
		if(s1==20)
		{
			s1=0;
			Crash1=FALSE;
		}
		else
		{
			s1++;
		}
		if(g_p2.pressedKey[1]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p2.speed1 < Data_y1[cur+count] && 
				ZONE_D*g_p2.speed1 > Data_y1[cur+count]  )
			{
				if( PERFECT_ZONE_U*g_p2.speed1 < Data_y1[cur+count] &&
					PERFECT_ZONE_D*g_p2.speed1 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p2.speed1 < Data_y1[cur+count] && 
						 GREAT_ZONE_D*g_p2.speed1 > Data_y1[cur+count]  )
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						On1=TRUE;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							Crash1=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p2.speed1 < Data_y1[cur+count] &&
					     GOOD_ZONE_D*g_p2.speed1 > Data_y1[cur+count] )
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p2.speed1 < Data_y1[cur+count] &&
						 BAD_ZONE_D*g_p2.speed1 > Data_y1[cur+count] )
				{
					if(Data_Judge1[cur+count][5]=='1')
					{
						Data_Judge1[cur+count][5]='0';
						stat1=cur+count;
						s1=1;
						if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s3 || (g_p2.pressedKey[3]==TRUE))
	{
		if(s3==20)
		{
			s3=0;
			Crash3=FALSE;
		}
		else
		{
			s3++;
		}
		if(g_p2.pressedKey[3]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p2.speed3 < Data_y1[cur+count] &&
				ZONE_D*g_p2.speed3 > Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*g_p2.speed3 < Data_y1[cur+count] &&
				    PERFECT_ZONE_D*g_p2.speed3 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p2.speed3<Data_y1[cur+count] &&
					     GREAT_ZONE_D*g_p2.speed3>Data_y1[cur+count] )
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						On3=TRUE;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							Crash3=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p2.speed3 < Data_y1[cur+count] &&
						 GOOD_ZONE_D*g_p2.speed3 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p2.speed3<Data_y1[cur+count] &&
						 BAD_ZONE_D*g_p2.speed3>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][9]=='1')
					{
						Data_Judge1[cur+count][9]='0';
						stat3=cur+count;
						s3=1;
						if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}
	}

	if(s5 || (g_p2.pressedKey[5]==TRUE))
	{
		if(s5==20)
		{
			s5=0;
			Crash5=FALSE;
		}
		else
		{
			s5++;
		}
		if(g_p2.pressedKey[5]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p2.speed5<Data_y1[cur+count] &&
				ZONE_D*g_p2.speed5>Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*g_p2.speed5<Data_y1[cur+count] &&
					PERFECT_ZONE_D*g_p2.speed5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p2.speed5<Data_y1[cur+count] &&
					     GREAT_ZONE_D*g_p2.speed5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						On5=TRUE;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							Crash5=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p2.speed5<Data_y1[cur+count] &&
					     GOOD_ZONE_D*g_p2.speed5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p2.speed5<Data_y1[cur+count] &&
					     BAD_ZONE_D*g_p2.speed5>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][7]=='1')
					{
						Data_Judge1[cur+count][7]='0';
						stat5=cur+count;
						s5=1;
						if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
				
			} 
		}

	}

	if(s7 || (g_p2.pressedKey[7]==TRUE) )
	{
		if(s7==20)
		{
			s7=0;
			Crash7=FALSE;
		}
		else
		{
			s7++;
		}
		if(g_p2.pressedKey[7]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p2.speed7 < Data_y1[cur+count] &&
				ZONE_D*g_p2.speed7 > Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*g_p2.speed7 < Data_y1[cur+count] &&
					PERFECT_ZONE_D*g_p2.speed7 > Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p2.speed7<Data_y1[cur+count] &&
					     GREAT_ZONE_D*g_p2.speed7>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						On7=TRUE;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							Crash7=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p2.speed7<Data_y1[cur+count] &&
					     GOOD_ZONE_D*g_p2.speed7>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p2.speed7<Data_y1[cur+count] &&
					     BAD_ZONE_D*g_p2.speed7>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][6]=='1')
					{
						Data_Judge1[cur+count][6]='0';
						stat7=cur+count;
						s7=1;
						if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}

			}
		}
	}

	if(s9 || (g_p2.pressedKey[9]==TRUE))
	{
		if(s9==20)
		{

			s9=0;
			Crash9=FALSE;
		}
		else
		{
			s9++;
		}
		if(g_p2.pressedKey[9]==TRUE)
		for(count=0;count<18;count++)
		{
			if( ZONE_U*g_p2.speed9<Data_y1[cur+count] &&
				ZONE_D*g_p2.speed9>Data_y1[cur+count])
			{
				if( PERFECT_ZONE_U*g_p2.speed9<Data_y1[cur+count] &&
					PERFECT_ZONE_D*g_p2.speed9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=PERFECT;
						}
						break;
					}
				}
				else if( GREAT_ZONE_U*g_p2.speed9<Data_y1[cur+count] &&
					     GREAT_ZONE_D*g_p2.speed9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						On9=TRUE;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							Crash9=TRUE;
							JudgeTemp=GREAT;
						}
						break;
					}
				}
				else if( GOOD_ZONE_U*g_p2.speed9<Data_y1[cur+count] &&
					     GOOD_ZONE_D*g_p2.speed9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							JudgeTemp=GOOD;
						}
						break;
					}
				}
				else if( BAD_ZONE_U*g_p2.speed9<Data_y1[cur+count] &&
					     BAD_ZONE_D*g_p2.speed9>Data_y1[cur+count])
				{
					if(Data_Judge1[cur+count][8]=='1')
					{
						Data_Judge1[cur+count][8]='0';
						stat9=cur+count;
						s9=1;
						if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
						{
							JudgeTemp=BAD;
						}
						break;
					}
				}
			}
		}

	}
	
	g_p2.judgement=JudgeTemp;
	
	// �̽�ó���Դϴ�.
	for(count=0;count<10;count++)
	if(Data_y1[cur+count] < 40
		&& (Data_Judge1[cur+count][5]!='0' || Data_Judge1[cur+count][6]!='0' || Data_Judge1[cur+count][7]!='0' || Data_Judge1[cur+count][8]!='0' || Data_Judge1[cur+count][9]!='0')
		&& (Data1[cur+count][5]!='0' || Data1[cur+count][6]!='0' || Data1[cur+count][7]!='0' || Data1[cur+count][8]!='0' || Data1[cur+count][9]!='0' ))
	{
		if(Data1[cur+count][5]=='2')break;
		Data_Judge1[cur+count][5]=Data_Judge1[cur+count][6]=Data_Judge1[cur+count][7]=Data_Judge1[cur+count][8]=Data_Judge1[cur+count][9]='0';
		g_p2.judgement=MISS;
		g_p2.combo=0;
	}

	if(Crash1)
	{
		if(s1==2)if(Data_Judge1[stat1][5]=='0' && Data_Judge1[stat1][6]=='0' && Data_Judge1[stat1][7]=='0' && Data_Judge1[stat1][8]=='0' && Data_Judge1[stat1][9]=='0' )
		{
			Data1[stat1][5]=Data1[stat1][6]=Data1[stat1][7]=Data1[stat1][8]=Data1[stat1][9]='0';
			//g_p2.judgement=PERFECT;
			g_p2.judgement=JudgeTemp;
		}
		else g_p2.judgement=NONE;
	}
	if(Crash7)
	{
		if(s7==2)if(Data_Judge1[stat7][5]=='0' && Data_Judge1[stat7][6]=='0' && Data_Judge1[stat7][7]=='0' && Data_Judge1[stat7][8]=='0' && Data_Judge1[stat7][9]=='0' )
		{
			Data1[stat7][5]=Data1[stat7][6]=Data1[stat7][7]=Data1[stat7][8]=Data1[stat7][9]='0';
			//g_p2.judgement=PERFECT;
			g_p2.judgement=JudgeTemp;
		}
		else g_p2.judgement=NONE;
	}
	if(Crash5)
	{
		if(s5==2)if(Data_Judge1[stat5][5]=='0' && Data_Judge1[stat5][6]=='0' && Data_Judge1[stat5][7]=='0' && Data_Judge1[stat5][8]=='0' && Data_Judge1[stat5][9]=='0' )
		{
			Data1[stat5][5]=Data1[stat5][6]=Data1[stat5][7]=Data1[stat5][8]=Data1[stat5][9]='0';
			//g_p2.judgement=PERFECT;
			g_p2.judgement=JudgeTemp;
		}
		else g_p2.judgement=NONE;
	}
	if(Crash9)
	{
		if(s9==2)if(Data_Judge1[stat9][5]=='0' && Data_Judge1[stat9][6]=='0' && Data_Judge1[stat9][7]=='0' && Data_Judge1[stat9][8]=='0' && Data_Judge1[stat9][9]=='0' )
		{
			Data1[stat9][5]=Data1[stat9][6]=Data1[stat9][7]=Data1[stat9][8]=Data1[stat9][9]='0';
			//g_p2.judgement=PERFECT;
			g_p2.judgement=JudgeTemp;
		}
		else g_p2.judgement=NONE;
	}
	if(Crash3)
	{
		if(s3==2)if(Data_Judge1[stat3][5]=='0' && Data_Judge1[stat3][6]=='0' && Data_Judge1[stat3][7]=='0' && Data_Judge1[stat3][8]=='0' && Data_Judge1[stat3][9]=='0' )
		{
			Data1[stat3][5]=Data1[stat3][6]=Data1[stat3][7]=Data1[stat3][8]=Data1[stat3][9]='0';
			//g_p2.judgement=PERFECT;
			g_p2.judgement=JudgeTemp;
		}
		else g_p2.judgement=NONE;
	}

	if(g_p2.judgement==PERFECT || g_p2.judgement==GREAT)
	{
		if(g_dsBeat)
		{
			g_dsBeat->Stop();
			g_dsBeat->SetCurrentPosition(0);
			g_dsBeat->Play(0,0,0);
		}
		g_p2.combo++;
		if(g_p2.judgement==PERFECT)g_p2.perfect++;
		else if(g_p2.judgement==GREAT)g_p2.great++;

		if(g_p2.combo>g_p2.maxCombo)g_p2.maxCombo=g_p2.combo;
		if(g_p2.combo>10)
		{
			if(g_p2.gauge<0)g_p2.gauge=1;
			else g_p2.gauge++;
			if(g_p2.gauge>42)g_p2.gauge=41;
		}
		Crash1=On1;
		Crash3=On3;
		Crash5=On5;
		Crash7=On7;
		Crash9=On9;

		On1=On3=On5=On7=On9=FALSE;
	}
	else if(g_p2.judgement==GOOD || g_p2.judgement==BAD || g_p2.judgement==MISS)
	{
		if(g_p2.judgement==GOOD)g_p2.good++;
		else if(g_p2.judgement==BAD)g_p2.bad++;
		else if(g_p2.judgement==MISS)g_p2.miss++;
		g_p2.combo=0;
	}

	if(g_p2.combo)
	{
		if(g_p2.judgement==PERFECT)g_p2.score+=2000;
		else if(g_p2.judgement==GREAT)g_p2.score+=1500;
	}
	else
	{
		if(g_p2.judgement==PERFECT)g_p2.score+=1000;
		else if(g_p2.judgement==GREAT)g_p2.score+=500;
	}

	if(g_p2.judgement==GOOD)g_p2.score+=100;
	else if(g_p2.judgement==BAD)
	{
		g_p2.score-=700;
		g_p2.gauge-=2;
	}
	else if(g_p2.judgement==MISS)
	{
		g_p2.score-=1000;
		g_p2.gauge-=5;
	}

	if(KCFG.bcDead)
	{
		if(Couple)
		{
			if(g_p1.gauge<-40 && g_p2.gauge<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
		else
		{
			if(g_p2.gauge<-40)
			{
				if(SongFlag)
				{
					song->OnMediaStop();
					delete song;
					SongFlag=FALSE;
				}
				g_ProgramState=DEAD;
			}
		}
	}

	if(g_p2.score<0)g_p2.score=0;

	if (beat) g_pDDSBack->BltFast(352,50,Arrow2,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else g_pDDSBack->BltFast(352,50,Arrow1,NULL,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	pArr1.top=0;
	pArr1.left=arrow_l[s1];
	pArr1.right=arrow_r[s1];
	pArr1.bottom=70;

	pArr3.top=0;
	pArr3.left=arrow_l[s3];
	pArr3.right=arrow_r[s3];
	pArr3.bottom=70;

	pArr5.top=0;
	pArr5.left=arrow_l[s5];
	pArr5.right=arrow_r[s5];
	pArr5.bottom=70;

	pArr7.top=0;
	pArr7.left=arrow_l[s7];
	pArr7.right=arrow_r[s7];
	pArr7.bottom=70;

	pArr9.top=0;
	pArr9.left=arrow_l[s9];
	pArr9.right=arrow_r[s9];
	pArr9.bottom=70;

	cArr1.top=0;
	cArr1.left=Carrow_l[s1];
	cArr1.right=Carrow_r[s1];
	cArr1.bottom=80;

	cArr3.top=0;
	cArr3.left=Carrow_l[s3];
	cArr3.right=Carrow_r[s3];
	cArr3.bottom=80;

	cArr5.top=0;
	cArr5.left=Carrow_l[s5];
	cArr5.right=Carrow_r[s5];
	cArr5.bottom=80;

	cArr7.top=0;
	cArr7.left=Carrow_l[s7];
	cArr7.right=Carrow_r[s7];
	cArr7.bottom=80;

	cArr9.top=0;
	cArr9.left=Carrow_l[s9];
	cArr9.right=Carrow_r[s9];
	cArr9.bottom=80;

	if(Crash1)g_pDDSBack->BltFast(345,43,cArrow1,&cArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s1)g_pDDSBack->BltFast(347,45,pArrow1,&pArr1,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash7)g_pDDSBack->BltFast(395,43,cArrow7,&cArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s7)g_pDDSBack->BltFast(397,45,pArrow7,&pArr7,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash5)g_pDDSBack->BltFast(445,43,cArrow5,&cArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s5)g_pDDSBack->BltFast(447,45,pArrow5,&pArr5,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	if(Crash9)g_pDDSBack->BltFast(495,43,cArrow9,&cArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s9)g_pDDSBack->BltFast(497,45,pArrow9,&pArr9,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	
	if(Crash3)g_pDDSBack->BltFast(545,43,cArrow3,&cArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else if(s3)g_pDDSBack->BltFast(547,45,pArrow3,&pArr3,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

int	RestoreAll(void)
{
	int	hRet;

	hRet=g_pDDSPrimary->Restore();
 
	return hRet;
}

void Flipp(void)
{
	KIU_Present();
}


BOOL InitDSound(void *hWnd, int Samples, int Bits, int nChannels)
{
	(void)hWnd; (void)Samples; (void)Bits; (void)nChannels;
	DisplayMessage(0,16,"Loading Music Intro File.. please Wait...");
	return TRUE;
}

void ReleaseDSound(void)
{
	int i;

	for(i=0;i<512;i++)
	{
		if(CSONG[i].Int_Song != NULL)
		{
			CSONG[i].Int_Song->Release();
			CSONG[i].Int_Song = NULL;
		}
	}

/*	if (lpdsbd != NULL)
	{
		lpdsbd->Release();
		lpdsbd = NULL;
	}
	if (g_DJVOICE_BUFFER != NULL)
	{
		g_DJVOICE_BUFFER->Release();
		g_DJVOICE_BUFFER = NULL;
	}
	if (g_PUMPSONG_BUFFER != NULL)
	{
		g_PUMPSONG_BUFFER->Release();
		g_PUMPSONG_BUFFER = NULL;
	}
	if (g_dsOpening != NULL)
	{
		g_dsOpening->Release();
		g_dsOpening = NULL;
	}
	if (g_Select_Song != NULL)
	{
		g_Select_Song->Release();
		g_Select_Song = NULL;
	}
	if (g_Cur_Song != NULL)
	{
		g_Cur_Song->Release();
		g_Cur_Song = NULL;
	}
	if (g_dsDead != NULL)
	{
		g_dsDead->Release();
		g_dsDead = NULL;
	}
*/
	if(lpds)lpds->Release();
}

void ReleaseAllObjects(void)
{
    if (g_pDD != NULL)
    {
        if (g_pDDSPrimary != NULL)
        {
            g_pDDSPrimary->Release();
            g_pDDSPrimary = NULL;

        }
		if (GameTITLE != NULL)
		{
			GameTITLE->Release();
			GameTITLE = NULL;
		}
		if (Background != NULL)
		{
			Background->Release();
			Background = NULL;
		}
		if (SongTitle != NULL)
		{
			SongTitle->Release();
			SongTitle = NULL;
		}

		if (SongBack != NULL)
		{
			SongBack->Release();
			SongBack = NULL;
		}
		if (SelectBack != NULL)
		{
			SelectBack->Release();
			SelectBack = NULL;
		}
		if (JudgeFont != NULL)
		{
			JudgeFont->Release();
			JudgeFont = NULL;
		}
		if (NumberFont != NULL)
		{
			NumberFont->Release();
			NumberFont = NULL;
		}
		if (ComboFont != NULL)
		{
			ComboFont->Release();
			ComboFont = NULL;
		}
		if (NoDISC != NULL)
		{
			NoDISC->Release();
			NoDISC = NULL;
		}
		if (ShiftLeft != NULL)
		{
			ShiftLeft->Release();
			ShiftRight = NULL;
		}
		if (ShiftRight != NULL)
		{
			ShiftRight->Release();
			ShiftRight = NULL;
		}
		if (GaugeWaku != NULL)
		{
			GaugeWaku->Release();
			GaugeWaku = NULL;
		}
		if (Gauge != NULL)
		{
			Gauge->Release();
			Gauge = NULL;
		}
		if (SmallFont != NULL)
		{
			SmallFont->Release();
			SmallFont = NULL;
		}
		if (Arrow1 != NULL)
		{
			Arrow1->Release();
			Arrow1 = NULL;
		}
		if (Arrow2 != NULL)
		{
			Arrow2->Release();
			Arrow2 = NULL;
		}
		if (wArrow != NULL)
		{
			wArrow->Release();
			wArrow = NULL;
		}
		if (pArrow1 != NULL)
		{
			pArrow1->Release();
			pArrow1 = NULL;
		}
		if (pArrow3 != NULL)
		{
			pArrow3->Release();
			pArrow3 = NULL;
		}
		if (pArrow5 != NULL)
		{
			pArrow5->Release();
			pArrow5 = NULL;
		}
		if (pArrow7 != NULL)
		{
			pArrow7->Release();
			pArrow7 = NULL;
		}
		if (pArrow9 != NULL)
		{
			pArrow9->Release();
			pArrow9 = NULL;
		}
		if(	cArrow1	!= NULL)
		{
			cArrow1->Release();
			cArrow1 = NULL;
		}
		if(	cArrow3	!= NULL)
		{
			cArrow3->Release();
			cArrow3 = NULL;
		}
		if(	cArrow5	!= NULL)
		{
			cArrow5->Release();
			cArrow5 = NULL;
		}
		if(	cArrow7	!= NULL)
		{
			cArrow7->Release();
			cArrow7 = NULL;
		}
		if(	cArrow9	!= NULL)
		{
			cArrow9->Release();
			cArrow9 = NULL;
		}
		if(	ModeIcon != NULL)
		{
			ModeIcon->Release();
			ModeIcon = NULL;
		}
		if( g_cFont != NULL)
		{
			g_cFont->Release();
			g_cFont = NULL;
		}
		if( ResultFont != NULL)
		{
			ResultFont->Release();
			ResultFont = NULL;
		}
		if( ResultBack != NULL)
		{
			ResultBack->Release();
			ResultBack = NULL;
		}
		if( StageCount != NULL)
		{
			StageCount->Release();
			StageCount = NULL;
		}
		if( Score != NULL)
		{
			Score->Release();
			Score = NULL;
		}
		if( DeadScreen != NULL)
		{
			DeadScreen->Release();
			DeadScreen = NULL;
		}
		if( GameOver != NULL)
		{
			GameOver->Release();
			GameOver=NULL;
		}
		if( Logo != NULL)
		{
			Logo->Release();
			Logo=NULL;
		}
		if( Diff != NULL)
		{
			Diff->Release();
			Diff=NULL;
		}
		if( DoubleIcon != NULL)
		{
			DoubleIcon->Release();
			DoubleIcon=NULL;
		}
		if( CrazyIcon != NULL)
		{
			CrazyIcon->Release();
			CrazyIcon=NULL;
		}
		if( EasyIcon != NULL)
		{
			EasyIcon->Release();
			EasyIcon=NULL;
		}
		if( HardIcon != NULL)
		{
			HardIcon->Release();
			HardIcon=NULL;
		}
        g_pDD->Release();
        g_pDD = NULL;

		ReleaseDSound();
		CleanupInput();
	}
}

void UpdateFrame(void)
{
	// FPS count start
	static uint32_t lastTime, fpsTime,framesRendered,fps;

	char	buff[50];
	uint32_t	cur = timeGetTime();
	uint32_t	deltaTime = cur - lastTime;
	lastTime = cur;

	fpsTime += deltaTime;

	++framesRendered;

	if(fpsTime>1000)
	{
		fps = framesRendered;
		framesRendered = 0;
		fpsTime = 0;
	}

	// FPS count & print end
	sprintf(buff,"FPS:%3d",fps);
	DisplayMessage(583,463,buff);

	switch(g_ProgramState)
	{
		case GAMETITLE:
			StageTitle();
			break;
		case SELECTSONG:
			SelectSong();
			break;
		case STAGE1:
			KIU_STAGE();
			break;
		case DOUBLE:
			KIU_STAGE_DOUBLE();
			break;
		case COUPLE:
			KIU_STAGE();
			break;
		case DEAD:
			Dead();
			break;
		case CONFIG:
			Configuration();
			break;
		case RESULT:
			Result();
			break;
		case GAMEOVER:
			GameOver1();
			break;
		case END:
			g_quitRequested = 1;
			break;
		default:
			break;
	}

}

long WindowProc(void *hWnd, unsigned message, unsigned long wParam, long lParam)
{
	switch (message)
    {
			case WM_MOVE:
				if(!g_fullscreen)
				{
					GetClientRect(hWnd, &g_rcViewport);
					GetClientRect(hWnd, &g_rcScreen);
					ClientToScreen(hWnd, (POINT*)&g_rcScreen.left);
					ClientToScreen(hWnd, (POINT*)&g_rcScreen.right);
				}
				break;
			/*case WM_PAINT:
				UpdateFrame();
				break;*/

			case WM_ACTIVATEAPP:
				g_bActive=wParam;
				break;

			case WM_KEYDOWN:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
				case VK_F12:
					g_quitRequested = 1;
					return 0L;
/* SelectSong, StageTitle���� �����մϴ�.
   ���Ŀ� �ٲ��־�� �մϴ�.
   */
				case VK_F2:
					First=0;
					switch(g_ProgramState)
					{
						case GAMETITLE:
							First=0;
							if(g_dsOpening)g_dsOpening->Stop();
							g_ProgramState=CONFIG;

							break;

						case SELECTSONG:
							g_ProgramState=CONFIG;
							if(IntroFlag){intro->OnMediaStop();delete intro;IntroFlag=FALSE;}
							if(g_dsSelectSong)g_dsSelectSong->Stop();
							First=0;
							break;

						case STAGE1:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							g_ProgramState=CONFIG;
							break;
						case DOUBLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							g_ProgramState=CONFIG;
							break;
						case COUPLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							g_ProgramState=CONFIG;
							break;
						case CONFIG:
							g_ProgramState=CONFIG;
							break;
					}
					break;

				case VK_UP:
					switch(g_ProgramState)
					{
						case GAMETITLE:
							g_p2.pressedKey[0]=8;
							break;
						case SELECTSONG:
							g_p2.pressedKey[0]=8;
							break;
						case CONFIG:
							g_p2.pressedKey[0]=8;
							break;
					}
					break;
				case VK_DOWN:
					switch(g_ProgramState)
					{
						case GAMETITLE:
							g_p2.pressedKey[0]=2;
							break;
						case SELECTSONG:
							g_p2.pressedKey[0]=2;
						case CONFIG:
							g_p2.pressedKey[0]=2;
							break;
					}
					break;

				case VK_LEFT:
					switch(g_ProgramState)
					{
						case SELECTSONG:
							g_p2.pressedKey[0]=4;
							break;
						case CONFIG:
							g_p2.pressedKey[0]=4;
							break;
					}
					break;
				case VK_RIGHT:
					switch(g_ProgramState)
					{
						case SELECTSONG:
							g_p2.pressedKey[0]=6;
							break;
						case CONFIG:
							g_p2.pressedKey[0]=6;
							break;
					}
					break;

				case VK_RETURN:
					switch(g_ProgramState)
					{
						case SELECTSONG:
							g_p2.pressedKey[0]=3;
							break;
						case CONFIG:
							g_p2.pressedKey[0]=3;
							break;
					}
					break;

/* ������� �Դϴ�. */

				case VK_ESCAPE:
				{
					switch(g_ProgramState)
					{
						case GAMETITLE:
							CFGWrite();
							g_quitRequested = 1;
							return 0L;
							break;

						case SELECTSONG:
							g_ProgramState=GAMETITLE;
							if(IntroFlag){intro->OnMediaStop();delete intro;IntroFlag=FALSE;}
							if(g_dsSelectSong)g_dsSelectSong->Stop();
							First=0;
							break;

						case STAGE1:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							First=0;
							g_ProgramState=RESULT;
							break;
						case DOUBLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							First=0;
							g_ProgramState=RESULT;
							break;
						case COUPLE:
							if(SongFlag)
							{
								song->OnMediaStop();
								delete song;
								SongFlag=FALSE;
							}
							First=0;
							g_ProgramState=RESULT;
							break;
						case CONFIG:
							First=0;
							CFGWrite();
							g_ProgramState=GAMETITLE;
							break;
						case RESULT:
							First=0;
							g_ProgramState=SELECTSONG;
							break;
					}
				}

			}
            break;

			case WM_DESTROY:
				ReleaseAllObjects();
				PostQuitMessage(0);
				return 0L;
			case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
	            SetCursor(NULL);
            return TRUE;

    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int InitFail(void *hWnd, int hRet, const char *szError, ...)
{
	char                        szBuff[128];
	va_list                     vl;

	va_start(vl, szError);
	vsprintf(szBuff, szError, vl);
	ReleaseAllObjects();
	MessageBox(hWnd, szBuff, TITLE, MB_OK);
	DestroyWindow(hWnd);
	va_end(vl);
	return hRet;
}
int InitWin(void *hInstance, uint32_t Width, uint32_t Height, int nCmdShow)
{
	(void)hInstance; (void)Width; (void)Height; (void)nCmdShow;
	if (!KIU_Init("Kick It UP! SDL3", 640, 480, g_fullscreen ? 1 : 0))
		return E_FAIL;
	hWnd = (void*)g_sdlWindow;
	return DD_OK;
}


int InitDD(void)
{
	SurfaceDesc ddsd;
	int hRet;

	g_pDD = new GfxDevice();

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth = 640;
	ddsd.dwHeight = 480;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSBack, NULL);
	g_pDDSPrimary = g_pDDSBack;
	return hRet;
}


int KLoadImage(void)
{
	// Loading image here
	
	SmallFont=DDLoadBitmap(g_pDD,"images/sFont.png",0,0);
	if(SmallFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(SmallFont,CLR_INVALID);

	DisplayMessage(0,0,"Loading Image");

	GameTITLE = DDLoadBitmap(g_pDD,"images/title.png",0,0);
	if(GameTITLE == NULL)
	{
		MessageBox(hWnd,"Cannot Load GAMETITLE","ERROR",0);
		return FALSE;
	}

	Background = DDLoadBitmap(g_pDD,"images/back.png",0,0);
	if(Background==NULL)
	{
		MessageBox(hWnd,"Cannot Load Background.","ERROR",0);
		return FALSE;
	}

	SelectBack = DDLoadBitmap(g_pDD,"images/selectBack.png",0,0);
	if(SelectBack==NULL)
	{
		MessageBox(hWnd,"Cannot Load Select Background.","ERROR",0);
		return FALSE;
	}

	JudgeFont = DDLoadBitmap(g_pDD,"images/judgement.png",0,0);
	if(JudgeFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Judgement Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(JudgeFont,CLR_INVALID);
	
	NumberFont = DDLoadBitmap(g_pDD,"images/number.png",0,0);
	if(NumberFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Number Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(NumberFont,CLR_INVALID);

	ComboFont = DDLoadBitmap(g_pDD,"images/combo.png",0,0);
	if(ComboFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load Combo Font.","ERROR",0);
		return FALSE;
	}
	DDSetColorKey(ComboFont,CLR_INVALID);

	NoDISC=DDLoadBitmap(g_pDD,"images/noDisc.png",0,0);
	if(NoDISC==NULL)
	{
		MessageBox(hWnd,"Cannot Load NoDISC.png",0,0);
		return FALSE;
	}
	DDSetColorKey(NoDISC,CLR_INVALID);

	ShiftLeft=DDLoadBitmap(g_pDD,"images/shiftL.png",0,0);
	if(ShiftLeft==NULL)
	{
		MessageBox(hWnd,"Cannot Load Shiftl.png",0,0);
		return FALSE;
	}
	DDSetColorKey(ShiftLeft,CLR_INVALID);

	ShiftRight=DDLoadBitmap(g_pDD,"images/shiftR.png",0,0);
	if(ShiftRight==NULL)
	{
		MessageBox(hWnd,"Cannot Load Shiftr.png",0,0);
		return FALSE;
	}
	DDSetColorKey(ShiftRight,CLR_INVALID);

	GaugeWaku=DDLoadBitmap(g_pDD,"images/gaugeWaku.png",0,0);
	if(GaugeWaku==NULL)
	{
		MessageBox(hWnd,"Cannot Load gaugeWaku.png",0,0);
		return FALSE;
	}
	DDSetColorKey(GaugeWaku,CLR_INVALID);

	Gauge=DDLoadBitmap(g_pDD,"images/gauge.png",0,0);
	if(Gauge==NULL)
	{
		MessageBox(hWnd,"Cannot Load gauge.png",0,0);
		return FALSE;
	}
	DDSetColorKey(Gauge,CLR_INVALID);
	
	Arrow1=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_ARROW1),0,0);
	if(Arrow1==NULL)
	{
		MessageBox(hWnd,"Cannot Load Arrow1.png",0,0);
		return FALSE;
	}
	DDSetColorKey(Arrow1,CLR_INVALID);
	Arrow2=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_ARROW2),0,0);
	if(Arrow2==NULL)
	{
		MessageBox(hWnd,"Cannot Load arrow2.png",0,0);
		return FALSE;
	}
	DDSetColorKey(Arrow2,CLR_INVALID);

	wArrow=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(ID_ARROW),0,0);
	if(wArrow==NULL)
	{
		MessageBox(hWnd,"Cannot Load arrow.png",0,0);
		return FALSE;
	}
	DDSetColorKey(wArrow,CLR_INVALID);
	CKey_Arr=DDColorMatch(wArrow,CLR_INVALID);

	pArrow1=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW1),0,0);
	if(pArrow1==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow.png",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow1,CLR_INVALID);

	pArrow3=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW3),0,0);
	if(pArrow3==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow.png",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow3,CLR_INVALID);

	pArrow5=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW5),0,0);
	if(pArrow5==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow5.png",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow5,CLR_INVALID);

	pArrow7=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW7),0,0);
	if(pArrow7==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow7.png",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow7,CLR_INVALID);
	pArrow9=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_PARROW9),0,0);
	if(pArrow9==NULL)
	{
		MessageBox(hWnd,"Cannot Load pArrow9.png",0,0);
		return FALSE;
	}
	DDSetColorKey(pArrow9,CLR_INVALID);

	cArrow1=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW1),0,0);
	if(cArrow1==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow.png",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow1,CLR_INVALID);
	cArrow3=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW3),0,0);
	if(cArrow3==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow.png",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow3,CLR_INVALID);

	cArrow5=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW5),0,0);
	if(cArrow5==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow5.png",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow5,CLR_INVALID);

	cArrow7=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW7),0,0);
	if(cArrow7==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow7.png",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow7,CLR_INVALID);
	cArrow9=DDLoadBitmap(g_pDD,MAKEINTRESOURCE(IDB_CARROW9),0,0);
	if(cArrow9==NULL)
	{
		MessageBox(hWnd,"Cannot Load cArrow9.png",0,0);
		return FALSE;
	}
	DDSetColorKey(cArrow9,CLR_INVALID);
	ModeIcon=DDLoadBitmap(g_pDD, "images/modeIcon.png",0,0);
	if(ModeIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load modeIcon.png",0,0);
		return FALSE;
	}
	DDSetColorKey(ModeIcon, CLR_INVALID);
	g_cFont=DDLoadBitmap(g_pDD, "images/cFont.png",0,0);
	if(g_cFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load CFont.png",0,0);
		return	FALSE;
	}
	DDSetColorKey(g_cFont, CLR_INVALID);
	CKey_CFont=DDColorMatch(g_cFont,CLR_INVALID);

	ResultFont=DDLoadBitmap(g_pDD, "images/resFont.png",0,0);
	if(ResultFont==NULL)
	{
		MessageBox(hWnd,"Cannot Load resFont.png",0,0);
		return	FALSE;
	}
	DDSetColorKey(ResultFont, CLR_INVALID);
	ResultBack=DDLoadBitmap(g_pDD, "images/resBack.png",0,0);
	if(ResultBack==NULL)
	{
		MessageBox(hWnd,"Cannot Load Resback.png",0,0);
		return FALSE;
	}
	StageCount=DDLoadBitmap(g_pDD, "images/stageCount.png",0,0);
	if(StageCount==NULL)
	{
		MessageBox(hWnd,"Cannot Load stageCount.png",0,0);
		return FALSE;
	}
	DDSetColorKey(StageCount, CLR_INVALID);

	Score=DDLoadBitmap(g_pDD,"images/score.png",0,0);
	if(Score==NULL)
	{
		MessageBox(hWnd,"Cannot Load score.png",0,0);
		return FALSE;
	}
	DDSetColorKey(Score,CLR_INVALID);

	DeadScreen=DDLoadBitmap(g_pDD,"images/dead.png",0,0);
	if(DeadScreen==NULL)
	{
		MessageBox(hWnd,"Cannot Load DeadScreen.png",0,0);
		return FALSE;
	}
	GameOver=DDLoadBitmap(g_pDD,"images/gameOver.png",0,0);
	if(GameOver==NULL)
	{
		MessageBox(hWnd,"Cannot Load GameOver.png",0,0);
		return FALSE;
	}
	Logo=DDLoadBitmap(g_pDD,"images/logo.png",0,0);
	if(Logo==NULL)
	{
		MessageBox(hWnd,"Cannot Load logo.png",0,0);
		return FALSE;
	}
	DDSetColorKey(Logo,CLR_INVALID);
	Diff=DDLoadBitmap(g_pDD,"images/diff.png",0,0);
	if(Diff==NULL)
	{
		MessageBox(hWnd,"Cannot Load diff.png",0,0);
		return FALSE;
	}
	DDSetColorKey(Diff,CLR_INVALID);
	DoubleIcon=DDLoadBitmap(g_pDD,"images/doubleIcon.png",0,0);
	if(DoubleIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load doubleIcon.png",0,0);
		return FALSE;
	}
	DDSetColorKey(DoubleIcon,CLR_INVALID);
	CrazyIcon=DDLoadBitmap(g_pDD,"images/crazyIcon.png",0,0);
	if(CrazyIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load crazyIcon.png",0,0);
		return FALSE;
	}
	DDSetColorKey(CrazyIcon,CLR_INVALID);
	EasyIcon=DDLoadBitmap(g_pDD,"images/easyIcon.png",0,0);
	if(EasyIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load easyIcon.png",0,0);
		return FALSE;
	}
	DDSetColorKey(EasyIcon,CLR_INVALID);
	HardIcon=DDLoadBitmap(g_pDD,"images/hardIcon.png",0,0);
	if(HardIcon==NULL)
	{
		MessageBox(hWnd,"Cannot Load hardIcon.png",0,0);
		return FALSE;
	}
	DDSetColorKey(HardIcon,CLR_INVALID);

	return DD_OK;
}

int main(int argc, char *argv[])
{
	(void)argc; (void)argv;

	g_bActive = TRUE;

	if(FAILED(InitWin(NULL, 640, 480, 0)))return FALSE;

	if(FAILED(InitDD()))return FALSE;
	if(FAILED(KLoadImage()))return FALSE;

	Read();
 	if(FAILED(InitDSound(NULL,22050,8,2)))return FALSE;
	
	WaveSet_Loading();
 	if(FAILED(InitDI(NULL)))return FALSE;

	CFGInitialize();

	while (!g_quitRequested)
	{
		KIU_PollEvents();
		if (g_quitRequested) break;
		UpdateFrame();
	}

	g_audio.unloadAll();
		KIU_Quit();
		
		return 0;
}


int TransAlphaImproved(Surface* src, Surface* dest, 
				   int32_t lDestX, int32_t lDestY, Rect srcRect, uint16_t ALPHA, uint32_t ColorKey, uint16_t BPP)
{
	/* SDL3 port: Direct surface locking not supported.
	   Fall back to a simple opaque blit. */
	(void)ALPHA; (void)ColorKey; (void)BPP;
	if (!src || !dest) return E_FAIL;
	return dest->BltFast((int)lDestX, (int)lDestY, src, &srcRect,
	                      DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
} 
