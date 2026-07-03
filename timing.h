/*
 * timing.h - BPM-based timing and arrow positioning engine
 */

#ifndef __TIMING_H__
#define __TIMING_H__

#include <cstdint>

struct Player;

class TimingEngine {
    double    _bpm;
    uint32_t  _tick;
    uint32_t  _cur;
    int       _maxSpeed, _minSpeed;

public:
    TimingEngine();

    void setBPM(double b)  { _bpm = b; }
    double bPM()      const { return _bpm; }
    uint32_t tick()   const { return _tick; }
    uint32_t cur()    const { return _cur; }
    int maxSpeed()    const { return _maxSpeed; }
    int minSpeed()    const { return _minSpeed; }
    void setMaxSpeed(int s) { _maxSpeed = s; }
    void setMinSpeed(int s) { _minSpeed = s; }
    void setTick(uint32_t t) { _tick = t; }
    void setCur(uint32_t c)  { _cur = c; }
    void advance();

    /* Compute arrow Y position (pixels) for a given lane speed.
       temp: current timing offset, k: sub-beat index, tickDiv: 2 or 4 */
    int arrowY(int temp, int k, int tickDiv, int speed) const;

    /* Judge-line Y position using MinSpeed */
    int judgeY(int temp, int k, int tickDiv) const;

    /* Arrow X position with optional per-lane pixel offset */
    int arrowX(int baseX, int pixelOffset, int speed, int speedIndex) const;

    /* Compute MaxSpeed/MinSpeed from player's lane speeds */
    void applySpeed(const Player &p);
};

extern TimingEngine g_timing;

#endif
