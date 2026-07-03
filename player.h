/*
 * player.h - Per-player state and logic
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "sdl3_kick.h"

struct Player {
    // Arrow input
    char  arrowState[10];
    uint32_t pressedKey[10];

    // Stats
    uint32_t perfect, great, good, bad, miss, maxCombo, combo;
    int32_t  score;
    int      gauge;
    char     judgement;
    uint32_t state;     // g_p1.state for animation
    bool     started;
    bool     alive;

    // Mode flags
    bool mirror, nonstep, synchro, union_, random, dMix;
    bool vanish, crazy, suddenR, randomS;

    // Speed
    int  speedBase;
    int  speed1, speed3, speed5, speed7, speed9;

    Player();
    void  clearModes();
    void  resetStats();
    char  judgeAnaly();
    bool  isDead() const { return gauge < -40; }
};

extern Player g_p1, g_p2;

#endif
