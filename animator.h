/*
 * animator.h - Time-based animation helper
 */

#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

#include <cstdint>

class Animator {
    double _value;
    double _speed;   // units per second
    double _acc;     // sub-pixel accumulator
    uint32_t _lastTime;
    bool _active;
    int _dir;        // -1 or +1

public:
    Animator(double speed = 60.0);

    void start(int direction);  // direction: -1 (forward) or +1 (backward)
    void stop();
    bool active() const { return _active; }
    int  value() const { return (int)_value; }

    
    int  update(double maxVal = 640.0);
};

#endif
