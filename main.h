/*
 * Main.h - Kick It Up SDL3 Port
 * Master game header
 *
 * Original: DirectDraw game definitions
 * Ported:   SDL3 definitions via sdl3_kick.h
 */

#ifndef __MAIN_INCLUDED__
#define __MAIN_INCLUDED__

#include "sdl3_kick.h"
#include "media.h"

#define NORMAL      0
#define PRESS       1

#define GAMETITLE   0
#define MODESELECT  1
#define SELECTSONG  2
#define STAGE1      3
#define DOUBLE      4
#define COUPLE      5
#define DEAD        6
#define CONFIG      7
#define RESULT      8
#define GAMEOVER    9

#define MODE_EASY   0
#define MODE_HARD   1
#define MODE_DOUBLE 2
#define MODE_CRAZY  3
#define MODE_REMIX  4

#define END         99

// #define TRUE        1
// #define FALSE       0

#define MAX_DATA    2048

#define NONE    0
#define PERFECT 1
#define GREAT   2
#define GOOD    3
#define BAD     4
#define MISS    5

#define ARROW_X 50

#define PUMP_SPRITE     50
#define PUMP_SPRITE_Y   55
#define PUMP_SPRITE_NEW 60

#define LP1_X   30
#define LP7_X   30+PUMP_SPRITE
#define LP5_X   32+PUMP_SPRITE*2
#define LP9_X   35+PUMP_SPRITE*3
#define LP3_X   35+PUMP_SPRITE*4

#define LP1_X1  350
#define LP7_X1  350+PUMP_SPRITE
#define LP5_X1  352+PUMP_SPRITE*2
#define LP9_X1  355+PUMP_SPRITE*3
#define LP3_X1  355+PUMP_SPRITE*4

#define JUDGE_SIZE_X    250
#define JUDGE_SIZE_Y    50

extern GfxDevice*           g_pDD;
extern Surface*    g_pDDSPrimary;
extern Surface*    g_pDDSBack;

extern void *hWnd;
extern bool  SongFlag;

#include "player.h"

extern int    MaxSpeed;
extern int    MinSpeed;

extern char   Data[MAX_DATA+1][14];
extern char   Data_Judge[MAX_DATA+1][14];
extern double Data_y[MAX_DATA+1];

extern char   Data1[MAX_DATA+1][14];
extern char   Data_Judge1[MAX_DATA+1][14];
extern double Data_y1[MAX_DATA+1];

extern char   Data_Double[MAX_DATA+1][14];
extern char   Data_Double_Judge[MAX_DATA+1][14];
extern double Data_Double_y[MAX_DATA+1];

extern char   ArrowState_Joy[10];
extern uint32_t PressedKey_Joy[10];

extern bool   Couple, Double, IntroFlag;
extern int    ALPHA, inc;
extern uint32_t  CKey_CFont;

extern CMedia *intro;
extern CMedia *song;

extern char   g_ProgramState;

void        DebugPrintf(char *str,...);

void        DisplayStageCount(uint32_t Count);
void        GameOver1(void);
void        DrawArrow1p(uint32_t cur);
void        DrawArrow2p(uint32_t cur);
void        DrawJudge1p(void);
void        DrawJudge2p(void);
void        DrawScore1p(void);
void        DrawScore2p(void);
void        DisplayMessage(int x, int y, char *message);
int     ClpBlt(int x ,int y ,Surface* ds,Rect *srect,uint32_t mode);
void        StageTitle(void);
int     RestoreAll(void);
void        Flipp(void);
void        ReleaseAllObjects(void);
void        UpdateFrame(void);
int     InitDD(void);
int     KLoadImage(void);
int     InitFail(void *hWnd, int hRet, const char *szError,...);
int     InitWin(void *hInstance, uint32_t Width, uint32_t Height, int nCmdShow);
int         main(int argc, char *argv[]);

#endif
