/*
 * input.cpp - SDL3 Port: InputManager implementation
 */

#include "input.h"
#include "main.h"
#include "audio.h"

/* Externs for config/song handling in special keys */
extern CMedia *intro, *song;
extern char First;
extern int  g_quitRequested;
extern bool SongFlag, IntroFlag;


const KeyMap InputManager::k1p[5] = {
    {DIK_Z, 1}, {DIK_C, 3}, {DIK_S, 5}, {DIK_Q, 7}, {DIK_E, 9}
};
const KeyMap InputManager::k2p[5] = {
    {DIK_NUMPAD1, 1}, {DIK_NUMPAD3, 3}, {DIK_NUMPAD5, 5},
    {DIK_NUMPAD7, 7}, {DIK_NUMPAD9, 9}
};

#define EDGE(dik, prev) ({ \
    uint8_t c = (rgKeyData[dik] & 0x80) ? 1 : 0; \
    uint8_t r = c && !prev; prev = c; r; })


InputManager g_input;

InputManager::InputManager() {
    _prevF2 = _prevEsc = _prevUp = _prevDown = _prevRet = 0;
}

void InputManager::poll() {
    KIU_ReadKeyboard();

    /* Read arrow keys via lookup tables */
    for (int i = 0; i < 5; i++) {
        const KeyMap &m1 = k1p[i], &m2 = k2p[i];
        int lane = m1.lane;

        /* 1P */
        if (rgKeyData[m1.dikIndex] & 0x80) {
            if (g_p1.arrowState[lane] == PRESS)
                g_p1.pressedKey[lane] = false;
            else {
                g_p1.pressedKey[lane] = true;
                g_p1.arrowState[lane] = PRESS;
            }
        } else {
            g_p1.arrowState[lane] = NORMAL;
            g_p1.pressedKey[lane] = false;
        }

        /* 2P */
        if (rgKeyData[m2.dikIndex] & 0x80) {
            if (g_p2.arrowState[lane] == PRESS)
                g_p2.pressedKey[lane] = false;
            else {
                g_p2.pressedKey[lane] = true;
                g_p2.arrowState[lane] = PRESS;
            }
        } else {
            g_p2.arrowState[lane] = NORMAL;
            g_p2.pressedKey[lane] = false;
        }
    }

    

    /* F2: Toggle config */
    if (EDGE(DIK_F2, _prevF2)) {
        switch (g_ProgramState) {
        case GAMETITLE:
            if (g_dsOpening) g_dsOpening->Stop();
            g_ProgramState = CONFIG; break;
        case SELECTSONG:
            if (IntroFlag) { intro->OnMediaStop(); delete intro; IntroFlag = false; }
            if (g_dsSelectSong) g_dsSelectSong->Stop();
            g_ProgramState = CONFIG; break;
        case STAGE1: case DOUBLE: case COUPLE:
            if (SongFlag) { song->OnMediaStop(); delete song; SongFlag = false; }
            g_ProgramState = CONFIG; break;
        default: break;
        }
        First = 0;
    }

    /* ESC: Navigate back / quit */
    if (EDGE(DIK_ESCAPE, _prevEsc)) {
        First = 0;
        switch (g_ProgramState) {
        case GAMETITLE: g_quitRequested = 1; break;
        case SELECTSONG:
            if (IntroFlag) { intro->OnMediaStop(); delete intro; IntroFlag = false; }
            if (g_dsSelectSong) g_dsSelectSong->Stop();
            g_ProgramState = GAMETITLE; break;
        case STAGE1: case DOUBLE: case COUPLE:
            if (SongFlag) { song->OnMediaStop(); delete song; SongFlag = false; }
            g_ProgramState = RESULT; break;
        case CONFIG: g_ProgramState = GAMETITLE; break;
        case RESULT: g_ProgramState = SELECTSONG; break;
        default: break;
        }
    }

    /* UP/DOWN/RETURN for menus/config */
    if (EDGE(DIK_UP, _prevUp))     g_p2.pressedKey[0] = 8;
    if (EDGE(DIK_DOWN, _prevDown)) g_p2.pressedKey[0] = 2;
    if (EDGE(DIK_RETURN, _prevRet) && g_ProgramState == CONFIG)
        g_p2.pressedKey[0] = 3;
}

void InputManager::close() {
    KIU_CleanupInput();
}


static void compatPoll() { g_input.poll(); }

void (*ReadGameInput)(void)      = compatPoll;
void (*ReadJoystickInput2)(void) = nullptr;

void ReadKeyboardInput(void) { g_input.poll(); }
void ReadFusionInput(void)   { g_input.poll(); }
void ReadKoinsInput(void)    { g_input.poll(); }
void ReadDDRInput(void)      { g_input.poll(); }

bool PickInputDevice(int N)   { (void)N; return true; }
bool PickInputDevice2(int N)  { (void)N; return true; }

#undef EDGE
