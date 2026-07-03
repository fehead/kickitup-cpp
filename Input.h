/*
 * Input.h - SDL3 Port: Keyboard/Joystick input
 *
 * Original: DirectInput keyboard/joystick
 * Ported:   SDL3 keyboard via KIU_ReadKeyboard/rgKeyData
 */

#ifndef __INPUT_INCLUDED__
#define __INPUT_INCLUDED__

#include "sdl3_kick.h"

/* Input function pointers */
extern void (*ReadGameInput)(void);
extern void (*ReadJoystickInput2)(void);

void ReadKeyboardInput(void);
void ReadFusionInput(void);
void ReadKoinsInput(void);
void ReadDDRInput(void);

BOOL PickInputDevice(int N);
BOOL PickInputDevice2(int N);

/* Now just stubs that use KIU_ReadKeyboard */
#define InitKeyboard(pdi)          TRUE
#define InitDI(hInstance)          TRUE
#define CleanupInput()             KIU_CleanupInput()
#define ReacquireInput()           TRUE
#define ReacquireInput2()          TRUE

#endif
