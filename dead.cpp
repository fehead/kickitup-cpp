/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

*/

/* ddraw.h via main.h */
/* mmsystem.h replaced */
/* dsound.h via main.h */
#include "dead.h"
#include "main.h"
//#include "sound.h"
#include "dsutil.h"

extern	Surface*		DeadScreen;
extern	Surface*		GameOver;


extern	uint32_t	dwGameCount;

void Dead(void)
{
	static uint32_t	cur,last;
	static Rect	UpRect, DownRect;

	static int Firs;

	g_p1.score=g_p2.score=0;

	if(Firs==0)
	{
		UpRect.top=240;
		UpRect.left=0;
		UpRect.right=640;
		UpRect.bottom=240;

		DownRect.top=240;
		DownRect.left=0;
		DownRect.right=640;
		DownRect.bottom=240;

		last=cur=timeGetTime();
		
		Firs++;
	}

	if(Firs==1)
	{
		if(g_dsDead)g_dsDead->Play(0,0,0);

		if(UpRect.top==0)Firs++;
		else
		{
			UpRect.top-=40;
			DownRect.bottom+=40;
		}

		g_pDDSBack->BltFast(0,0,DeadScreen,&UpRect,DDBLTFAST_NOCOLORKEY);
		g_pDDSBack->BltFast(0,UpRect.top+240,DeadScreen,&DownRect,DDBLTFAST_NOCOLORKEY);
	}

	if(Firs==2)
	{
		g_pDDSBack->BltFast(0,0,DeadScreen,NULL,DDBLTFAST_NOCOLORKEY);

		cur=timeGetTime();
		if((cur-last)>2500)
		{
			Firs=0;
			g_p1.started=g_p2.started=FALSE;
			dwGameCount=0;

			g_ProgramState=GAMEOVER;
		}
	}

	Flipp();
}
	