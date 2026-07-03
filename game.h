/*
 * game.h - Game state machine and flow control
 * Minimal dependencies — no include of main.h
 */

#ifndef __GAME_H__
#define __GAME_H__

/* Game states (matches original #defines) */
enum GameState {
    GS_TITLE = 0, GS_MODESELECT, GS_SELECTSONG, GS_STAGE1,
    GS_DOUBLE, GS_COUPLE, GS_DEAD, GS_CONFIG, GS_RESULT, GS_GAMEOVER,
    GS_END = 99
};

enum { GS_MODE_EASY = 0, GS_MODE_HARD, GS_MODE_DOUBLE, GS_MODE_CRAZY, GS_MODE_REMIX };

class Game {
    GameState _state;
    char      _mode;
    bool      _couple;
    bool      _double;
    bool      _songFlag;
    bool      _introFlag;
    bool      _active;

public:
    Game();
    GameState state()      const { return _state; }
    char      mode()       const { return _mode; }
    bool      isCouple()   const { return _couple; }
    bool      isDouble()   const { return _double; }
    bool      isCoOp()     const { return _couple || _double; }
    bool      isActive()   const { return _active; }
    bool      songFlag()   const { return _songFlag; }
    bool      introFlag()  const { return _introFlag; }
    void      setMode(char m)     { _mode = m; }
    void      startSolo()         { _couple = _double = false; }
    void      startCouple()       { _couple = true; _double = false; }
    void      startDouble()       { _double = true; _couple = false; }
    void      setSongFlag(bool f) { _songFlag = f; }
    void      setIntroFlag(bool f){ _introFlag = f; }
    void      setActive(bool f)   { _active = f; }
    void      enterState(GameState s) { _state = s; }
    void      update();  // defined in game.cpp (needs main.h)
};

extern Game g_game;

#endif
