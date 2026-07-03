/*
 * animator.cpp - Time-based animation
 */

#include "animator.h"
#include "sdl3_kick.h"  // timeGetTime

Animator::Animator(double speed)
    : _value(0), _speed(speed), _acc(0), _lastTime(0), _active(false), _dir(0) {}

void Animator::start(int direction) {
    _value = 0;
    _acc = 0;
    _dir = direction;
    _active = true;
    _lastTime = timeGetTime();
}

void Animator::stop() {
    _active = false;
    _value = 0;
    _acc = 0;
}

int Animator::update(double maxVal) {
    if (!_active) return 0;

    uint32_t now = timeGetTime();
    double dt = (_lastTime > 0) ? (now - _lastTime) / 1000.0 : 0.016;
    _lastTime = now;

    _acc += dt * _speed;
    int step = (int)_acc;
    _acc -= step;
    _value += step * _dir;

    /* Clamp and stop at bounds */
    if (_dir < 0 && _value <= -maxVal) { _value = -maxVal; }
    if (_dir > 0 && _value >=  maxVal) { stop(); return (int)_value; }

    /* Check if crossed 0 while going negative */
    if (_dir < 0 && _value >= 0) {
        _value = 0;
        stop();
    }

    return (int)_value;
}
