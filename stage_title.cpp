/*
 * stage_title.cpp - Title screen stage
 * Extracted from main.cpp
 */

#include "main.h"
#include "input.h"

extern Surface *GameTITLE, *g_cFont;
extern int ALPHA, inc;
extern uint32_t CKey_CFont;
extern bool Couple;
extern char First;
extern void ClearMode(void);

void StageTitle(void)
{
    Rect lRect;

    if(First==0)
    {
        ClearMode();
        g_p1.started = false;
        g_p2.started = false;
        First++;
        if(g_dsOpening)
            g_dsOpening->Play(0,0,0);
    }

    ReadGameInput();

    if(g_p1.pressedKey[5])  g_p1.started = true;
    if(g_p2.pressedKey[5])  g_p2.started = true;

    g_pDDSBack->BltFast(0,0,GameTITLE,NULL, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);

    if(g_p1.started || g_p2.started)
    {
        if(g_p1.started && g_p2.started)
            g_p2.pressedKey[0] = 3;
        else if(g_p1.started) {
            if(g_p1.pressedKey[5]) g_p2.pressedKey[0] = 3;
        }
        else if(g_p2.started) {
            if(g_p2.pressedKey[5]) g_p2.pressedKey[0] = 3;
        }
    }

    if(g_p2.pressedKey[0] == 3)
    {
        Couple = g_p1.started && g_p2.started;
        First = 0;
        if(g_dsOpening) g_dsOpening->Stop();
        g_p2.pressedKey[0] = 0;
        g_ProgramState = SELECTSONG;
    }

    /* "FREE PLAY!" banner */
    lRect = {0, 46, 220, 69};
    g_pDDSBack->BltFast(210, 450, g_cFont, &lRect, DDBLTFAST_SRCCOLORKEY);

    /* "PRESS CENTER BUTTON" 1P */
    if(!g_p1.started) {
        lRect = {0, 0, 220, 23};
        TransAlphaImproved(g_cFont, g_pDDSBack, 10, 450, lRect, ALPHA, CKey_CFont, 16);
    }
    /* "PRESS CENTER BUTTON" 2P */
    if(!g_p2.started) {
        lRect = {0, 0, 220, 23};
        TransAlphaImproved(g_cFont, g_pDDSBack, 410, 450, lRect, ALPHA, CKey_CFont, 16);
    }

    ALPHA += inc;
    if     (ALPHA > 256) { ALPHA = 256; inc = -20; }
    else if(ALPHA < 0)   { ALPHA = 0;   inc =  20; }

    Flipp();
}
