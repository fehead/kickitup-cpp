/*
 * gameplay.h - Shared state for KIU_STAGE gameplay
 */

#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "sdl3_kick.h"

struct GameplayState {
    /* Timing */
    int      arrowOffset;
    int      animFrame;
    double   fadeAlpha;
    uint32_t starttime, curtime;

    /* Arrow sprite rects */
    Rect rect1[7], rect3[7], rect5[7], rect7[7], rect9[7];
};

/* Per-player arrow drawing state */
struct ArrowAnim {
    int  arrowL[20], arrowR[20];
    int  carrowL[20], carrowR[20];
    uint8_t anim[5];          // s1,s3,s5,s7,s9
    uint32_t stepIndex[5];    // stat1,stat3,stat5,stat7,stat9
    uint32_t cur2;
    int  beat;
    bool crash[5];            // Crash1,3,5,7,9
    bool on[5];               // On1,3,5,7,9
    Rect pArr[5], cArr[5];
};

#endif
