/*
 * renderer.cpp - Unified rendering implementation
 */

#include "renderer.h"
#include "main.h"

extern Surface *GaugeWaku, *Gauge;
extern Surface *Score;
extern Surface *JudgeFont, *NumberFont, *ComboFont;

void Renderer::drawGauge(const Player &p, int xWaku, int xBar, int dir) {
    int curG = p.gauge;
    if (curG < 0) curG = 0;

    g_pDDSBack->BltFast(xWaku, 0, GaugeWaku, nullptr,
                        DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

    Rect sr = {0, 0, 6, 20};

    for (int i = 0; i < 7 && i <= curG; i++)
        g_pDDSBack->BltFast(xBar + dir * (i * 6), 20, Gauge, &sr,
                            DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    sr.left = 6; sr.right = 12;
    for (int i = 7; i < 21 && i <= curG; i++)
        g_pDDSBack->BltFast(xBar + dir * (i * 6), 20, Gauge, &sr,
                            DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    sr.left = 12; sr.right = 18;
    for (int i = 21; i < 42 && i <= curG; i++)
        g_pDDSBack->BltFast(xBar + dir * (i * 6), 20, Gauge, &sr,
                            DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

void Renderer::drawScore(const Player &p, int x, int y) {
    char buf[11];
    sprintf(buf, "%07d", p.score);
    for (int i = 0; buf[i]; i++) {
        int digit = buf[i] - '0';
        Rect sr = {(int16_t)(digit * 22), 0, (int16_t)(digit * 22 + 22), 35};
        g_pDDSBack->BltFast(x + i * 22, y, Score, &sr,
                            DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
    }
}

void Renderer::drawJudge(const Player &p, int x, int y) {
    char grade = p.judgeAnaly();
    const char *msg = "";
    switch (grade) {
    case 'S': msg = "S"; break;
    case 'A': msg = "A"; break;
    case 'B': msg = "B"; break;
    case 'C': msg = "C"; break;
    default:  msg = "F"; break;
    }
    DisplayMessage(x, y, (char *)msg);
}

void Renderer::blitArrow(int lane, int xOff, int temp, int k, int tickDiv,
                         int speed, Surface *arrowTex, const Rect *rects, int sta) {
    /* Y position formula from KIU_STAGE */
    int y = (temp + PUMP_SPRITE_Y * k / tickDiv) * speed - PUMP_SPRITE_Y * (speed - 1);
    ClpBlt(xOff, y, arrowTex, (Rect *)&rects[lane], DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}
