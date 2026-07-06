/*
 * sdl3_kick.h - SDL3 + mpg123 replacement for DirectX/Win32 API
 * Kick It Up SDL3 Port
 *
 * This is the ONLY header game files need to include.
 * It replaces windows.h, ddraw.h, dsound.h, dinput.h, dshow.h
 *
 * Uses C++ wrapper classes so the original ->BltFast(), ->Play(), etc.
 * member-call syntax compiles unchanged.
 */

#ifndef __SDL3_KICK_INCLUDED__
#define __SDL3_KICK_INCLUDED__

#include <SDL3/SDL.h>
#include <cstdint>
#include <SDL3_image/SDL_image.h>
#include <mpg123.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cctype>
#include <algorithm>
#define max std::max
#define min std::min


#define timeGetTime()   SDL_GetTicks()
#define MAX_PATH        260

typedef int   BOOL;
#ifndef TRUE
#define TRUE   1
#endif
#ifndef FALSE
#define FALSE  0
#endif

typedef unsigned UINT;

typedef uint32_t Color;
#define CLR_INVALID   ((Color)0xFFFFFFFF)
#define RGB(r,g,b)    ((Color)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)))

typedef struct {
    int32_t left, top, right, bottom;
} Rect;

typedef Rect* LPRECT;


#define DDBLTFAST_NOCOLORKEY   0x0000
#define DDBLTFAST_SRCCOLORKEY  0x0001
#define DDBLTFAST_WAIT         0x0010
#define DDBLT_WAIT             0x01000000
#define DDBLT_KEYSRC           0x00010000
#define DDBLT_COLORFILL        0x00000400

struct BlitFx {
    uint32_t dwSize;
    uint32_t dwFillColor;
};

typedef uint32_t ColorKey;
#define DDCKEY_SRCBLT  0
#define DD_OK          0
#define E_FAIL         ((int)0x80004005L)
#define SUCCEEDED(hr)  (((int)(hr)) >= 0)
#define FAILED(hr)     (((int)(hr)) < 0)


typedef struct {
    uint16_t  wFormatTag;
    uint16_t  nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t  nBlockAlign;
    uint16_t  wBitsPerSample;
    uint16_t  cbSize;
} WaveFmt;


#define mmioFOURCC(a,b,c,d) \
    ((uint32_t)(uint8_t)(a) | ((uint32_t)(uint8_t)(b) << 8) | \
     ((uint32_t)(uint8_t)(c) << 16) | ((uint32_t)(uint8_t)(d) << 24))





/* Forward declarations */
struct SurfaceDesc {
    uint32_t dwSize, dwFlags;
    uint32_t dwWidth, dwHeight;
    int32_t  lPitch;
    struct { uint32_t dwCaps; } ddsCaps;
    struct { uint32_t dwRGBBitCount; } ddpfPixelFormat;
    void *lpSurface;
};
#define DDSD_CAPS    0x1
#define DDSD_HEIGHT  0x2
#define DDSD_WIDTH   0x4
#define DDSCAPS_OFFSCREENPLAIN  0x40
#define HDC void*

class GfxDevicePalette {
public:
    int SetEntries(uint32_t f, uint32_t s, uint32_t c, void *e) { return DD_OK; }
};

#define DDPCAPS_8BIT  0x4

class Surface {
public:
    SDL_Texture *tex;
    int w, h;
    Color colorKey;
    bool hasColorKey;

    Surface() : tex(nullptr), w(0), h(0),
        colorKey(RGB(255,0,255)), hasColorKey(true) {}

    ~Surface() { if (tex) SDL_DestroyTexture(tex); }

    /* BltFast(x, y, srcSurface, srcRect, flags) */
    int BltFast(int x, int y, Surface *src,
                    const Rect *srcrect, uint32_t flags);

    /* Blt(destRect, srcSurface, srcRect, flags) */
    int Blt(const Rect *destRect, Surface *src,
                const Rect *srcRect, uint32_t flags, BlitFx *fx = nullptr);

    /* Color key */
    int SetColorKey(uint32_t flag, ColorKey *ck);
    Color GetColorKey() { return colorKey; }

    /* Stubs needed by original code */
    int GetDC(HDC *hdc) { *hdc = nullptr; return DD_OK; }
    int ReleaseDC(HDC hdc) { (void)hdc; return DD_OK; }
    int Restore() { return DD_OK; }
    int Lock(Rect *r, SurfaceDesc *d, uint32_t f, void *h) {
        (void)r; (void)d; (void)f; (void)h; return E_FAIL; }
    int Unlock(Rect *r) { (void)r; return DD_OK; }
    int GetSurfaceDesc(SurfaceDesc *d) {
        d->dwWidth = w; d->dwHeight = h; return DD_OK; }

    static Surface *LoadBitmap(const char *path, int dx, int dy);
    void Release();
    
    /* For compatibility: direct release function */
    void Release2() { Release(); }
};


/* GfxDevice stub */
class GfxDevice {
public:
    int CreateSurface(SurfaceDesc *d, Surface* *surf, void *u);
    int CreatePalette(uint32_t f, void *e, GfxDevicePalette **p, void *u);
    int SetCooperativeLevel(void *w, uint32_t f) { return DD_OK; }
    int SetDisplayMode(uint32_t w, uint32_t h, uint32_t bpp) { return DD_OK; }
    void Release() { delete this; }
};





class Sound {
public:
    Uint8  *data;
    Uint32  length;
    int     looping;
    SDL_AudioStream *stream;

    Sound() : data(nullptr), length(0), looping(0), stream(nullptr) {}
    ~Sound();

    int Play(uint32_t reserved1, uint32_t reserved2, uint32_t flags);
    int Stop();
    int SetCurrentPosition(uint32_t pos);
    int Restore() { return DD_OK; }
    void Release() { delete this; }

    static Sound *LoadWAV(const char *path);
    static Sound *LoadMP3(const char *path);
};


/* AudioDev stub */
class AudioDev {
public:
    int SetCooperativeLevel(void *w, uint32_t l) { return DD_OK; }
    void Release() { delete this; }
};

/* SoundDesc */
struct SoundDesc {
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwBufferBytes;
    WaveFmt *lpwfxFormat;
};
#define DSBCAPS_STATIC               0x80000
#define DSBCAPS_CTRLPAN              0x200000
#define DSBCAPS_CTRLVOLUME           0x20000
#define DSBCAPS_CTRLFREQUENCY        0x20
#define DSBCAPS_GETCURRENTPOSITION2  0x100000
#define DSBPLAY_LOOPING  1
#define DSBPLAY_TOEND    0






enum State { Uninitialized, Stopped, Paused, Playing };

class CMedia {
    struct Impl;
    Impl *p;
public:
    CMedia();
    ~CMedia();

    void OpenMediaFile(char *szFile);
    void OnMediaPlay();
    void OnMediaStop();
    void OnMediaPause();
    void OnMediaAbortStop();
    double GetCurrentPosition();
    void ChangeStateTo(State s);
    BOOL CanPlay();
    BOOL CanStop();
    BOOL CanPause();
    BOOL CreateFilterGraph();
    BOOL RenderFile(char *szFile);
    void DeleteContents();
};




extern SDL_Window    *g_sdlWindow;
extern SDL_Renderer  *g_sdlRenderer;
extern int            g_screenWidth;
extern int            g_screenHeight;
extern int            g_quitRequested;




int  KIU_Init(const char *title, int w, int h, int fullscreen);
void KIU_Quit(void);
int  KIU_PollEvents(void);
void KIU_Present(void);

void KIU_FillSurface(Surface* pdds, uint32_t color);
Color KIU_ColorMatch(Surface* pdds, Color rgb);
void KIU_DisplayMessage(int x, int y, const char *msg);
void KIU_ReadKeyboard(void);
void KIU_CleanupInput(void);
int  KIU_KeyStateDIK(int dik);


/* We keep the original hex values but remap inside KIU_KeyStateDIK */









#define KEYBUFSIZE    256


extern unsigned char rgKeyData[KEYBUFSIZE];

/* Input device types */
#define LPDIRECTINPUTDEVICE8   void*
#define LPDIRECTINPUT8         void*
#define DIDEVICEINSTANCE       void
#define LPCDIDEVICEINSTANCE    void*
#define DI_OK                  0
#define DIERR_INPUTLOST        1
#define DISCL_NONEXCLUSIVE     0x1
#define DISCL_FOREGROUND       0x2
#define DIEDFL_ATTACHEDONLY    0x1
#define DIEDFL_FORCEFEEDBACK   0x100
#define DI8DEVCLASS_GAMECTRL   0
#define GUID_SysKeyboard       ((void*)0)
#define c_dfDIKeyboard         ((void*)0)
#define c_dfDIJoystick         ((void*)0)
#define DIPROP_BUFFERSIZE      0
#define DIPH_DEVICE            0
#define DIPROP_RANGE           0
#define DIJOFS_X               0
#define DIPH_BYOFFSET          0
#define DIENUM_CONTINUE        1

/* DIK_* scan codes (subset used by Kick It Up) */
#define DIK_ESCAPE          0x01
#define DIK_1               0x02
#define DIK_2               0x03
#define DIK_3               0x04
#define DIK_4               0x05
#define DIK_5               0x06
#define DIK_6               0x07
#define DIK_7               0x08
#define DIK_8               0x09
#define DIK_9               0x0A
#define DIK_0               0x0B
#define DIK_MINUS           0x0C
#define DIK_EQUALS          0x0D
#define DIK_BACK            0x0E
#define DIK_TAB             0x0F
#define DIK_Q               0x10
#define DIK_W               0x11
#define DIK_E               0x12
#define DIK_R               0x13
#define DIK_T               0x14
#define DIK_Y               0x15
#define DIK_U               0x16
#define DIK_I               0x17
#define DIK_O               0x18
#define DIK_P               0x19
#define DIK_LBRACKET        0x1A
#define DIK_RBRACKET        0x1B
#define DIK_RETURN          0x1C
#define DIK_LCONTROL        0x1D
#define DIK_A               0x1E
#define DIK_S               0x1F
#define DIK_D               0x20
#define DIK_F               0x21
#define DIK_G               0x22
#define DIK_H               0x23
#define DIK_J               0x24
#define DIK_K               0x25
#define DIK_L               0x26
#define DIK_SEMICOLON       0x27
#define DIK_APOSTROPHE      0x28
#define DIK_GRAVE           0x29
#define DIK_LSHIFT          0x2A
#define DIK_BACKSLASH       0x2B
#define DIK_Z               0x2C
#define DIK_X               0x2D
#define DIK_C               0x2E
#define DIK_V               0x2F
#define DIK_B               0x30
#define DIK_N               0x31
#define DIK_M               0x32
#define DIK_COMMA           0x33
#define DIK_PERIOD          0x34
#define DIK_SLASH           0x35
#define DIK_RSHIFT          0x36
#define DIK_MULTIPLY        0x37
#define DIK_LMENU           0x38
#define DIK_SPACE           0x39
#define DIK_CAPITAL         0x3A
#define DIK_F1              0x3B
#define DIK_F2              0x3C
#define DIK_F3              0x3D
#define DIK_F4              0x3E
#define DIK_F5              0x3F
#define DIK_F6              0x40
#define DIK_F7              0x41
#define DIK_F8              0x42
#define DIK_F9              0x43
#define DIK_F10             0x44
#define DIK_F11             0x57
#define DIK_F12             0x58
#define DIK_NUMPAD7         0x47
#define DIK_NUMPAD8         0x48
#define DIK_NUMPAD9         0x49
#define DIK_NUMPAD4         0x4B
#define DIK_NUMPAD5         0x4C
#define DIK_NUMPAD6         0x4D
#define DIK_NUMPAD1         0x4F
#define DIK_NUMPAD2         0x50
#define DIK_NUMPAD3         0x51
#define DIK_NUMPAD0         0x52
#define DIK_DECIMAL         0x53
#define DIK_NUMPADENTER     0x9C
#define DIK_RCONTROL        0x9D
#define DIK_RMENU           0xB8
#define DIK_HOME            0xC7
#define DIK_UP              0xC8
#define DIK_PRIOR           0xC9  /* PageUp */
#define DIK_LEFT            0xCB
#define DIK_RIGHT           0xCD
#define DIK_END             0xCF
#define DIK_DOWN            0xD0
#define DIK_NEXT            0xD1  /* PageDown */
#define DIK_INSERT          0xD2
#define DIK_DELETE          0xD3

/* Input device pick stubs */
extern int g_cpdevFound;
#define MAX_DINPUT_DEVICES  10





#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

/* MessageBox stub */
#define MessageBox(h, msg, title, flags)  do { \
    fprintf(stderr, "%s: %s\n", (title)?(title):"", (msg)?(msg):""); \
} while(0)

/* PostQuitMessage stub */
#define PostQuitMessage(code)  do { g_quitRequested = 1; } while(0)

/* COM stubs */
#define CoInitialize(x)        ((void)0)
#define CoUninitialize()       ((void)0)
#define CoCreateInstance(a,b,c,d,e)  (E_FAIL)

/* MultiByteToWideChar stub */
#define MultiByteToWideChar(cp, flags, src, slen, dst, dlen)  0

/* String uppercase (MSVC _strupr) */
static inline char *_strupr(char *s) {
    for (char *p = s; *p; p++) if (*p >= 'a' && *p <= 'z') *p -= 32;
    return s;
}
#define _strdup  strdup

/* SetCurrentDirectory */
#define SetCurrentDirectory(path)  (chdir(path) == 0)

/* File access */
#define _access   access
#define access    access

/* WIN32_FIND_DATA / FindFirstFile / FindNextFile / FindClose */

struct WIN32_FIND_DATA {
    char cFileName[MAX_PATH];
    uint32_t dwFileAttributes;
};
#define FILE_ATTRIBUTE_DIRECTORY  0x10
#define INVALID_HANDLE_VALUE      ((DIR*)0)
typedef DIR* HANDLE;

/* Internal state for FindFirstFile/FindNextFile */
static inline HANDLE FindFirstFile(const char *pattern, WIN32_FIND_DATA *data) {
    (void)pattern;
    DIR *d = opendir(".");
    if (!d) return INVALID_HANDLE_VALUE;
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        strncpy(data->cFileName, entry->d_name, MAX_PATH - 1);
        data->cFileName[MAX_PATH - 1] = '\0';
        if (entry->d_type == DT_DIR) {
            data->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        } else {
            struct stat st;
            if (stat(entry->d_name, &st) == 0 && S_ISDIR(st.st_mode))
                data->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
            else
                data->dwFileAttributes = 0;
        }
        return (HANDLE)d;
    }
    closedir(d);
    return INVALID_HANDLE_VALUE;
}
static inline BOOL FindNextFile(HANDLE h, WIN32_FIND_DATA *data) {
    DIR *d = (DIR *)h;
    if (!d) return FALSE;
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        strncpy(data->cFileName, entry->d_name, MAX_PATH - 1);
        data->cFileName[MAX_PATH - 1] = '\0';
        if (entry->d_type == DT_DIR) {
            data->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        } else {
            struct stat st;
            if (stat(entry->d_name, &st) == 0 && S_ISDIR(st.st_mode))
                data->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
            else
                data->dwFileAttributes = 0;
        }
        return TRUE;
    }
    return FALSE;
}
static inline void FindClose(HANDLE h) { if (h && h != INVALID_HANDLE_VALUE) closedir((DIR*)h); }

/* GetFullPathName stub */
static inline uint32_t GetFullPathName(const char *file, uint32_t len, char *buf, char **part) {
    char cwd[MAX_PATH];
    getcwd(cwd, sizeof(cwd));
    snprintf(buf, len, "%s/%s", cwd, file);
    if (part) *part = NULL;
    return (uint32_t)strlen(buf);
}

/* OutputDebugString */
#define OutputDebugString(s)  fprintf(stderr, "%s", s)

/* _lopen / _lread / _lclose stubs (used in ddutil palette loading) */
#define _lopen(path, flags)   open(path, O_RDONLY)
#define _lread(fd, buf, sz)   (int)read(fd, buf, sz)
#define _lclose(fd)           close(fd)
#define OF_READ               0

#include <fcntl.h>

/* Additional stubs */
typedef const char* LPCTSTR;
typedef LPCTSTR LPCSTR;
typedef char* LPSTR;
typedef char* LPTSTR;
typedef char TCHAR;

#define DDLOCK_WAIT           0x1
#define DDERR_SURFACELOST     0x1
#define DDERR_WASSTILLDRAWING 0x2
#define DDSCL_EXCLUSIVE       0x1
#define DDSCL_FULLSCREEN      0x2
#define DDSCL_ALLOWREBOOT     0x4
#define DDSCL_NORMAL          0x8

#define MAKEINTRESOURCE(i)    ((const char*)(uintptr_t)(i))
#define WM_CLOSE              0x10
#define PostMessage(h, m, w, l)  do { g_quitRequested = 1; } while(0)
#define DestroyWindow(h)         ((void)0)
#define DefWindowProc(h, m, w, l) 0L
#define MB_OK                 0

/* Add lPitch to SurfaceDesc */
#define SurfaceDesc_HAS_LPITCH 1

/* TransAlphaImproved declaration */
extern "C" int TransAlphaImproved(Surface* src, Surface* dest,
    int32_t lDestX, int32_t lDestY, Rect srcRect, uint16_t ALPHA, uint32_t ColorKey, uint16_t BPP);

/* Add Release to GfxDevice */
/* Add Release to Sound */

/* VK_* virtual key codes */
#define VK_F2    0x71
#define VK_F12   0x7B
#define VK_UP    0x26
#define VK_DOWN  0x28
#define VK_LEFT       0x25
#define VK_RIGHT      0x27
#define VK_RETURN     0x0D
#define VK_ESCAPE     0x1B
#define WM_DESTROY     0x0002
#define WM_SETCURSOR   0x0020
#define SetCursor(c)   ((void)0)

#define WM_MOVE          0x0003
#define WM_ACTIVATEAPP   0x001C
#define WM_KEYDOWN       0x0100
#define GetClientRect(h, r)  ((void)0)
#define ClientToScreen(h, p) ((void)0)
#define WPARAM  unsigned long
#define LPARAM  long
#define POINT   void

#endif /* __SDL3_KICK_INCLUDED__ */
