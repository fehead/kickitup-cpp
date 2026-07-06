/*
 * input.h - SDL3 Port: Unified input manager
 */

#ifndef __INPUT_INCLUDED__
#define __INPUT_INCLUDED__

#include "sdl3_kick.h"
#include "player.h"

struct KeyMap { int dikIndex; int lane; };

class InputManager {
    
    static const KeyMap k1p[5], k2p[5];

    /* Edge-trigger state for special keys */
    uint8_t _prevF2, _prevEsc, _prevUp, _prevDown, _prevRet;

public:
    InputManager();

    
    void poll();

    /* Cleanup */
    void close();
};

extern InputManager g_input;

/* Backward-compat function pointer type */
extern void (*ReadGameInput)(void);
extern void (*ReadJoystickInput2)(void);
void ReadKeyboardInput(void);
void ReadFusionInput(void);
void ReadKoinsInput(void);
void ReadDDRInput(void);

bool PickInputDevice(int N);
bool PickInputDevice2(int N);

/* Stub macros for old DirectInput init calls */
#define InitKeyboard(pdi)   true
#define InitDI(hInstance)   true
#define CleanupInput()      KIU_CleanupInput()
#define ReacquireInput()    true
#define ReacquireInput2()   true

#endif
