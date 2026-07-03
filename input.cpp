/*
 * Input.cpp - SDL3 Port: Keyboard input processing
 *
 * Reads from the global rgKeyData[] buffer (filled by KIU_ReadKeyboard)
 * and populates g_p1.pressedKey[] / g_p2.pressedKey[] arrays.
 *
 * Key mapping (Pump It Up layout):
 *   1P: Q(7) W(9) S(5) Z(1) E(3)  —  up-left, up-right, center, down-left, down-right
 *   2P: KP7(7) KP9(9) KP5(5) KP1(1) KP3(3) — same layout on numpad
 */

#include "input.h"
#include "main.h"

/* Function pointers */
static void ReadKbd(void);

void (*ReadGameInput)(void)      = ReadKbd;
void (*ReadJoystickInput2)(void) = nullptr;

void ReadKeyboardInput(void) { ReadKbd(); }
void ReadFusionInput(void)   { ReadKbd(); }
void ReadKoinsInput(void)    { ReadKbd(); }
void ReadDDRInput(void)      { ReadKbd(); }

BOOL PickInputDevice(int N)   { (void)N; return TRUE; }
BOOL PickInputDevice2(int N)  { (void)N; return TRUE; }

static void ReadKbd(void)
{
    KIU_ReadKeyboard();  /* fills global rgKeyData[] */

    /* ── 2 Player (numpad) ── */
#define READ_KEY2P(idx, dik) do { \
    if (rgKeyData[dik] & 0x80) { \
        if (g_p2.arrowState[idx] == PRESS) g_p2.pressedKey[idx] = FALSE; \
        else { g_p2.pressedKey[idx] = TRUE; g_p2.arrowState[idx] = PRESS; } \
    } else { g_p2.arrowState[idx] = NORMAL; g_p2.pressedKey[idx] = FALSE; } \
} while(0)

    READ_KEY2P(1, DIK_NUMPAD1);
    READ_KEY2P(3, DIK_NUMPAD3);
    READ_KEY2P(5, DIK_NUMPAD5);
    READ_KEY2P(7, DIK_NUMPAD7);
    READ_KEY2P(9, DIK_NUMPAD9);

#undef READ_KEY2P

    /* ── 1 Player (QWESZC) ── */
#define READ_KEY1P(idx, dik) do { \
    if (rgKeyData[dik] & 0x80) { \
        if (g_p1.arrowState[idx] == PRESS) g_p1.pressedKey[idx] = FALSE; \
        else { g_p1.pressedKey[idx] = TRUE; g_p1.arrowState[idx] = PRESS; } \
    } else { g_p1.arrowState[idx] = NORMAL; g_p1.pressedKey[idx] = FALSE; } \
} while(0)

    READ_KEY1P(1, DIK_Z);
    READ_KEY1P(3, DIK_E);
    READ_KEY1P(5, DIK_S);
    READ_KEY1P(7, DIK_Q);
    READ_KEY1P(9, DIK_W);

#undef READ_KEY1P
}
