/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

*/

/* ddraw.h via main.h */

#include "main.h"
#include "input.h"
#include <stdio.h>
#include "ddutil.h"

extern	Surface*	ResultBack;
extern	Surface*	ResultFont;
extern	Surface*	Background;
extern	Surface*	NumberFont;

extern	char First;
extern bool Double;
extern	uint32_t dwGameCount;

int	ClpBlt2(int x ,int y ,Surface* ds,Rect* srect,uint32_t mode)
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

char JudgeAnaly1p(void)
{
	uint32_t	cTotal1p;

	cTotal1p=g_p1.perfect+g_p1.great+g_p1.good+g_p1.bad+g_p1.miss;

	if(g_p1.gauge<0)return	'F';

	if(g_p1.good==0 && g_p1.bad==0 && g_p1.miss==0)return	'S';

	if((((double)(g_p1.perfect+g_p1.great)/cTotal1p)*100)>95)return	'A';

	if((((double)(g_p1.perfect+g_p1.great)/(double)cTotal1p)*100)>80)return	'B';

	if((((double)(g_p1.perfect+g_p1.great)/(double)cTotal1p)*100)>60)return	'C';

	return	'F';
}

char JudgeAnaly2p(void)
{
	uint32_t	cTotal2p;

	cTotal2p=g_p2.perfect+g_p2.great+g_p2.good+g_p2.bad+g_p2.miss;

	if(g_p2.gauge<0)if(!Double)return	'F';

	if(g_p2.good==0 && g_p2.bad==0 && g_p2.miss==0)return	'S';

	if((((double)(g_p2.perfect+g_p2.great)/(double)cTotal2p)*100)>95)return	'A';

	if((((double)(g_p2.perfect+g_p2.great)/(double)cTotal2p)*100)>80)return	'B';

	if((((double)(g_p2.perfect+g_p2.great)/(double)cTotal2p)*100)>60)return	'C';

	return	'F';
}

void DisplayJudge(int x, int y, char s, uint32_t ColorKey)
{
	Rect	sRect;

	switch(s)
	{
		case	'A':
			sRect.top=2;
			sRect.left=7;
			sRect.right=153;
			sRect.bottom=220;

			break;
		case	'B':
			sRect.top=2;
			sRect.left=155;
			sRect.right=290;
			sRect.bottom=220;

			break;
		case	'C':
			sRect.top=2;
			sRect.left=295;
			sRect.right=438;
			sRect.bottom=220;

			break;
		case	'D':
			sRect.top=2;
			sRect.left=450;
			sRect.right=585;
			sRect.bottom=220;

			break;
		case	'F':
			sRect.top=225;
			sRect.left=127;
			sRect.right=225;
			sRect.bottom=443;

			break;
		case	'S':
			sRect.top=225;
			sRect.left=0;
			sRect.right=127;
			sRect.bottom=446;
			break;
	}

	TransAlphaImproved(ResultFont, g_pDDSBack, x, y, sRect, 150, ColorKey, 16);
}


void	DisplayNumber(int x, int y, char *message)
{
	int Loop;
	Rect	cRect;
	
	for(Loop=0;;Loop++)
	{
		if(message[Loop]==NULL)break;
		message[Loop]-=48;
		cRect.left=message[Loop]*30;
		cRect.right=cRect.left+30;
		cRect.top=0;
		cRect.bottom=38;

		g_pDDSBack->BltFast(x+Loop*30,y,NumberFont,&cRect,DDBLTFAST_SRCCOLORKEY);
	}/* 여기까지 */
}

void Result(void)
{
	Rect	sRect;

	static	int	FontColorkey;

	char	Number[10];
	
	static	int Count,Phase;

	static	uint32_t	Perfect, Great, Good, Bad, Miss;
	
	static	uint32_t		i;

	static	char	Judge1p,Judge2p;		// 1p, 2p --'S' 'A' 'B' 'C' 'F'

	if(First==0)
	{
		FontColorkey=DDColorMatch(ResultFont,CLR_INVALID);
		Phase=0;

		sRect.top=0;
		sRect.left=0;
		sRect.right=640;
		sRect.bottom=480;

		for(Count=480;Count>0;Count-=24)
		{
			ClpBlt2(Count,Count,ResultBack,&sRect,DDBLTFAST_NOCOLORKEY);
			Flipp();
		}
		Judge1p=JudgeAnaly1p();
		Judge2p=JudgeAnaly2p();

		First++;
	}

	ReadGameInput();
	
	if(g_p1.pressedKey[5])if(Phase!=7 && g_p1.started)Phase=6;
	if(g_p2.pressedKey[5])if(Phase!=7 && g_p2.started)Phase=6;

	g_pDDSBack->BltFast(0,0,ResultBack, NULL,DDBLTFAST_NOCOLORKEY);
	
	if(Phase>=0)
	{
		sRect.top=225;
		sRect.left=240;
		sRect.right=410;
		sRect.bottom=270;//Perfect
		
		if(Phase==0)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>0)g_pDDSBack->BltFast(235, 115, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 235,115, sRect, Count, FontColorkey, 16);
		//if(Phase>0)Count=0;
	}

	if(Phase>=1)
	{
		sRect.top=225;
		sRect.left=425;
		sRect.right=575;
		sRect.bottom=270;//Great
		
		if(Phase==1)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>1)g_pDDSBack->BltFast(253, 166, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 253,166,sRect,Count, FontColorkey, 16);
		//if(Phase>1)Count=0;
	}
	
	if(Phase>=2)
	{
		sRect.top=275;
		sRect.left=235;
		sRect.right=380;
		sRect.bottom=325;//Good
		
		if(Phase==2)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>2)g_pDDSBack->BltFast(250, 220, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 250,220,sRect,Count ,FontColorkey, 16);
		//if(Phase>2)Count=0;
	}

	if(Phase>=3)
	{
		sRect.top=275;
		sRect.left=430;
		sRect.right=515;
		sRect.bottom=320;//Bad
		
		if(Phase==3)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>3)g_pDDSBack->BltFast(275, 268, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 275,268,sRect,Count ,FontColorkey, 16);
		//if(Phase>3)Count=0;
	}

	if(Phase>=4)
	{
		sRect.top=328;
		sRect.left=240;
		sRect.right=350;
		sRect.bottom=370;//Miss
		
		if(Phase==4)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>4)g_pDDSBack->BltFast(263, 323, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 263,323,sRect,Count ,FontColorkey, 16);
		//if(Phase>4)Count=0;
	}

	if(Phase>=5)
	{
		sRect.top=380;
		sRect.left=242;
		sRect.right=510;
		sRect.bottom=425;//MAX Combo
		
		if(Phase==5)
		{
			if(Count>255)
			{
				Count=255;
			}
			else Count+=4;
		}
		if(Phase>5)g_pDDSBack->BltFast(190, 373, ResultFont, &sRect, DDBLTFAST_SRCCOLORKEY);
		else TransAlphaImproved(ResultFont, g_pDDSBack, 190,373,sRect,Count ,FontColorkey, 16);
		//if(Phase>5)Count=0;
	}

	if(Phase==0)
	{
		if(g_p1.started)
		{
			if(g_p1.perfect>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p1.perfect);
			DisplayNumber(80,120,Number);
		}
		if(g_p2.started)
		{
			if(g_p2.perfect>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p2.perfect);
			DisplayNumber(480,120,Number);
		}
		i++;
		if(g_p1.perfect<i && g_p2.perfect<i)
		{
			Phase=1;
			i=0;
			Count=0;
		}
	}
	if(Phase==1)
	{
		if(g_p1.started)
		{
			sprintf(Number, "%03d", g_p1.perfect);
			DisplayNumber(80,120, Number);
		
			if(g_p1.great>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p1.great);
			DisplayNumber(80,170,Number);
		}
		if(g_p2.started)
		{
			sprintf(Number, "%03d", g_p2.perfect);
			DisplayNumber(480,120, Number);

			if(g_p2.great>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p2.great);
			DisplayNumber(480,170,Number);
		}
		i++;
		if(g_p1.great<i && g_p2.great<i)
		{
			Phase=2;
			i=0;
			Count=0;
		}
	}
	if(Phase==2)
	{

		if(g_p1.started)
		{
			sprintf(Number, "%03d", g_p1.perfect);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",g_p1.great);
			DisplayNumber(80,170,Number);

			if(g_p1.good>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p1.good);
			DisplayNumber(80,220,Number);
		}
		if(g_p2.started)
		{
			sprintf(Number, "%03d", g_p2.perfect);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",g_p2.great);
			DisplayNumber(480,170,Number);

			if(g_p2.good>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p2.good);
			DisplayNumber(480,220,Number);
		}
		i++;
		if(g_p1.good<i && g_p2.good<i)
		{
			Phase=3;
			i=0;
			Count=0;
		}
	}

	if(Phase==3)
	{
		if(g_p1.started)
		{
			sprintf(Number, "%03d", g_p1.perfect);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",g_p1.great);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",g_p1.good);
			DisplayNumber(80,220,Number);

			if(g_p1.bad>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p1.bad);
			DisplayNumber(80,270,Number);
		}
		if(g_p2.started)
		{
			sprintf(Number, "%03d", g_p2.perfect);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",g_p2.great);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",g_p2.good);
			DisplayNumber(480,220,Number);

			if(g_p2.bad>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p2.bad);
			DisplayNumber(480,270,Number);
		}
		i++;
		if(g_p1.bad<i && g_p2.bad<i)
		{
			Phase=4;
			i=0;
			Count=0;
		}
	}

	if(Phase==4)
	{
		
		if(g_p1.started)
		{
			sprintf(Number, "%03d", g_p1.perfect);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",g_p1.great);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",g_p1.good);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",g_p1.bad);
			DisplayNumber(80,270,Number);
			
			if(g_p1.miss>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p1.miss);
			DisplayNumber(80,320,Number);
		}
		if(g_p2.started)
		{
			sprintf(Number, "%03d", g_p2.perfect);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",g_p2.great);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",g_p2.good);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",g_p2.bad);
			DisplayNumber(480,270,Number);

			if(g_p2.miss>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p2.miss);
			DisplayNumber(480,320,Number);
		}
		i++;
		if(g_p1.miss<i && g_p2.miss<i)
		{
			Phase=5;
			i=0;
			Count=0;
		}
	}
	if(Phase==5)
	{
		if(g_p1.started)
		{
			sprintf(Number, "%03d", g_p1.perfect);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",g_p1.great);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",g_p1.good);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",g_p1.bad);
			DisplayNumber(80,270,Number);
			sprintf(Number,"%03d",g_p1.miss);
			DisplayNumber(80,320,Number);
			
			if(g_p1.maxCombo>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p1.maxCombo);
			DisplayNumber(80,375,Number);
		}
		if(g_p2.started)
		{
			sprintf(Number, "%03d", g_p2.perfect);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",g_p2.great);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",g_p2.good);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",g_p2.bad);
			DisplayNumber(480,270,Number);
			sprintf(Number,"%03d",g_p2.miss);
			DisplayNumber(480,320,Number);

			if(g_p2.maxCombo>i)sprintf(Number,"%03d",i);
			else sprintf(Number,"%03d",g_p2.maxCombo);
			DisplayNumber(480,375,Number);
		}
		i++;
		if(g_p1.maxCombo<i && g_p2.maxCombo<i)
		{
			Phase=6;
			i=0;
			Count=0;
		}
	}

	if(Phase==6)
	{
		if(g_p1.started)
		{
			sprintf(Number, "%03d", g_p1.perfect);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",g_p1.great);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",g_p1.good);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",g_p1.bad);
			DisplayNumber(80,270,Number);
			sprintf(Number,"%03d",g_p1.miss);
			DisplayNumber(80,320,Number);
			sprintf(Number,"%03d",g_p1.maxCombo);
			DisplayNumber(80,375,Number);
		}
		if(g_p2.started)
		{
			sprintf(Number, "%03d", g_p2.perfect);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",g_p2.great);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",g_p2.good);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",g_p2.bad);
			DisplayNumber(480,270,Number);
			sprintf(Number,"%03d",g_p2.miss);
			DisplayNumber(480,320,Number);
			sprintf(Number,"%03d",g_p2.maxCombo);
			DisplayNumber(480,375,Number);
		}
		
		i++;

		if(i>=60)
		{
			Phase=7;
			i=0;
			Count=0;
		}
	}

	if(Phase==7)
	{
		if(g_p1.started)
		{
			sprintf(Number, "%03d", g_p1.perfect);
			DisplayNumber(80,120, Number);
			sprintf(Number,"%03d",g_p1.great);
			DisplayNumber(80,170,Number);
			sprintf(Number,"%03d",g_p1.good);
			DisplayNumber(80,220,Number);
			sprintf(Number,"%03d",g_p1.bad);
			DisplayNumber(80,270,Number);
			sprintf(Number,"%03d",g_p1.miss);
			DisplayNumber(80,320,Number);
			sprintf(Number,"%03d",g_p1.maxCombo);
			DisplayNumber(80,375,Number);
			DisplayJudge(70,140,Judge1p,FontColorkey);
		}
		if(g_p2.started)
		{
			sprintf(Number, "%03d", g_p2.perfect);
			DisplayNumber(480,120, Number);
			sprintf(Number,"%03d",g_p2.great);
			DisplayNumber(480,170,Number);
			sprintf(Number,"%03d",g_p2.good);
			DisplayNumber(480,220,Number);
			sprintf(Number,"%03d",g_p2.bad);
			DisplayNumber(480,270,Number);
			sprintf(Number,"%03d",g_p2.miss);
			DisplayNumber(480,320,Number);
			sprintf(Number,"%03d",g_p2.maxCombo);
			DisplayNumber(480,375,Number);
			DisplayJudge(470,140,Judge2p,FontColorkey);
		}

		i++;
		if(i==60)
		{
			First=0;
			// MaxState is 4
			if(dwGameCount==3 || (Judge1p=='F' && Judge2p=='F') ) {
				g_ProgramState=GAMEOVER;
				dwGameCount=0;
			}
			else
			{
				dwGameCount++;
				g_ProgramState=SELECTSONG;
			}
		}
	}

	Flipp();
}
