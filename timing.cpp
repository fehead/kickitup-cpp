/*
 * timing.cpp - TimingEngine implementation
 * Arrow positioning formulas extracted from KIU_STAGE
 */

#include "timing.h"
#include "player.h"

#define PUMP_SPRITE_Y 55

TimingEngine g_timing;

TimingEngine::TimingEngine() : _bpm(120.0), _tick(0), _cur(0),
    _maxSpeed(1), _minSpeed(1) {}

int TimingEngine::arrowY(int temp, int k, int tickDiv, int speed) const {
    int pixelY = k * PUMP_SPRITE_Y / tickDiv;
    return (temp + pixelY) * speed - PUMP_SPRITE_Y * (speed - 1);
}

int TimingEngine::judgeY(int temp, int k, int tickDiv) const {
    return arrowY(temp, k, tickDiv, _minSpeed);
}

int TimingEngine::arrowX(int baseX, int pixelOffset, int speed, int speedIndex) const {
    int spd = (&speed)[speedIndex]; // hack: access speed array by index
    return (pixelOffset - 1) * spd + baseX;
}

void TimingEngine::applySpeed(const Player &p) {
    int fast = p.speed1, slow = p.speed1;
    if (p.speed3 > fast) fast = p.speed3; if (p.speed3 < slow) slow = p.speed3;
    if (p.speed5 > fast) fast = p.speed5; if (p.speed5 < slow) slow = p.speed5;
    if (p.speed7 > fast) fast = p.speed7; if (p.speed7 < slow) slow = p.speed7;
    if (p.speed9 > fast) fast = p.speed9; if (p.speed9 < slow) slow = p.speed9;
    _maxSpeed = fast; _minSpeed = slow;
}
