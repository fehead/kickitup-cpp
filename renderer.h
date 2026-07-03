/*
 * renderer.h - Unified rendering layer
 */

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "sdl3_kick.h"
#include "player.h"

class Renderer {
public:
    /* Gauge bar — unified 1P/2P */
    static void drawGauge(const Player &p, int xWaku, int xBar, int dir);

    /* Score display */
    static void drawScore(const Player &p, int x, int y);

    /* Judgement combo text */
    static void drawJudge(const Player &p, int x, int y);

    /* Arrow drawing — compute Y and call ClpBlt for one lane */
    static void blitArrow(int lane, int xOff, int temp, int k, int tickDiv,
                          int speed, Surface *arrowTex, const Rect *rects, int sta);

    /* Speed helpers */
    static int  maxSpeedOf(const Player &p) {
        int m = p.speed1;
        if (p.speed3 > m) m = p.speed3; if (p.speed5 > m) m = p.speed5;
        if (p.speed7 > m) m = p.speed7; if (p.speed9 > m) m = p.speed9;
        return m;
    }
    static int  minSpeedOf(const Player &p) {
        int m = p.speed1;
        if (p.speed3 < m) m = p.speed3; if (p.speed5 < m) m = p.speed5;
        if (p.speed7 < m) m = p.speed7; if (p.speed9 < m) m = p.speed9;
        return m;
    }
};

#endif
