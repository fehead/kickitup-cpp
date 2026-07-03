/*
 * sdl3_kick.cpp - SDL3 + mpg123 implementation
 * Kick It Up SDL3 Port
 *
 * Implements all C++ wrapper classes declared in sdl3_kick.h
 * Replaces: ddutil.cpp, dsutil.cpp, Media.cpp, Input.cpp, and WinMain
 */

#include "sdl3_kick.h"
#include <SDL3_ttf/SDL_ttf.h>

/* ═══════════════════════════════════════════════════════════════════════
 * SDL3 Global State
 * ═══════════════════════════════════════════════════════════════════════ */
SDL_Window   *g_sdlWindow   = nullptr;
SDL_Renderer *g_sdlRenderer = nullptr;
int           g_screenWidth  = 640;
int           g_screenHeight = 480;
int           g_quitRequested = 0;

static SDL_AudioDeviceID g_audioDevice = 0;
static bool g_ttfAvailable = false;

/* ═══════════════════════════════════════════════════════════════════════
 * KIU Engine Init / Quit
 * ═══════════════════════════════════════════════════════════════════════ */

int KIU_Init(const char *title, int width, int height, int fullscreen)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 0;
    }

    g_screenWidth  = width;
    g_screenHeight = height;

    SDL_WindowFlags flags = SDL_WINDOW_HIDDEN;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    g_sdlWindow = SDL_CreateWindow(title, width, height, flags);
    if (!g_sdlWindow) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    g_sdlRenderer = SDL_CreateRenderer(g_sdlWindow, nullptr);
    if (!g_sdlRenderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_sdlWindow);
        SDL_Quit();
        return 0;
    }

    SDL_SetRenderDrawBlendMode(g_sdlRenderer, SDL_BLENDMODE_BLEND);
    SDL_ShowWindow(g_sdlWindow);

    mpg123_init();
    g_ttfAvailable = TTF_Init();

    return 1;
}

void KIU_Quit(void)
{
    SDL_DestroyRenderer(g_sdlRenderer);
    SDL_DestroyWindow(g_sdlWindow);
    mpg123_exit();
    SDL_Quit();
}

int KIU_PollEvents(void)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) {
            g_quitRequested = 1;
            return 0;
        }
    }
    return 1;
}

void KIU_Present(void)
{
    SDL_RenderPresent(g_sdlRenderer);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Surface Implementation
 * ═══════════════════════════════════════════════════════════════════════ */

Surface *Surface::LoadBitmap(const char *path, int dx, int dy)
{
    (void)dx; (void)dy;
    /* Load PNG/BMP via SDL3_image */
    SDL_Surface *s = IMG_Load(path);
    if (!s) {
        fprintf(stderr, "LoadBitmap(%s) failed: %s\n", path, SDL_GetError());
        return nullptr;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(g_sdlRenderer, s);
    int w = s->w, h = s->h;
    SDL_DestroySurface(s);

    if (!tex) return nullptr;

    Surface *surf = new Surface();
    surf->tex  = tex;
    surf->w    = w;
    surf->h    = h;
    surf->colorKey = RGB(255, 0, 255);
    surf->hasColorKey = true;

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    return surf;
}

void Surface::Release()
{
    if (tex) { SDL_DestroyTexture(tex); tex = nullptr; }
    delete this;
}

int Surface::BltFast(int x, int y,
                                     Surface *src,
                                     const Rect *srcRect,
                                     uint32_t flags)
{
    if (!src || !src->tex) return E_FAIL;
    (void)x; (void)y; (void)flags;

    SDL_FRect dst;
    dst.x = (float)x;
    dst.y = (float)y;

    SDL_FRect srcR;
    if (srcRect) {
        srcR.x = (float)srcRect->left;
        srcR.y = (float)srcRect->top;
        int rw = srcRect->right - srcRect->left;
        int rh = srcRect->bottom - srcRect->top;
        srcR.w = (float)(rw ? rw : src->w);
        srcR.h = (float)(rh ? rh : src->h);
        dst.w = srcR.w;
        dst.h = srcR.h;
    } else {
        srcR.x = 0; srcR.y = 0;
        srcR.w = (float)src->w;
        srcR.h = (float)src->h;
        dst.w  = (float)src->w;
        dst.h  = (float)src->h;
    }

    SDL_BlendMode savedBlend = SDL_BLENDMODE_BLEND;
    if (flags & DDBLTFAST_NOCOLORKEY) {
        SDL_GetTextureBlendMode(src->tex, &savedBlend);
        SDL_SetTextureBlendMode(src->tex, SDL_BLENDMODE_NONE);
    }

    SDL_RenderTexture(g_sdlRenderer, src->tex, &srcR, &dst);

    if (flags & DDBLTFAST_NOCOLORKEY) {
        SDL_SetTextureBlendMode(src->tex, savedBlend);
    }

    return DD_OK;
}

int Surface::Blt(const Rect *destRect,
                                 Surface *src,
                                 const Rect *srcRect,
                                 uint32_t flags,
                                 BlitFx *fx)
{
    /* Handle color fill */
    if (flags & DDBLT_COLORFILL) {
        if (fx) {
            uint8_t r = (uint8_t)((fx->dwFillColor >> 16) & 0xFF);
            uint8_t g = (uint8_t)((fx->dwFillColor >> 8) & 0xFF);
            uint8_t b = (uint8_t)(fx->dwFillColor & 0xFF);
            SDL_SetRenderDrawColor(g_sdlRenderer, r, g, b, 255);
            if (destRect) {
                SDL_FRect rct = { (float)destRect->left, (float)destRect->top,
                                  (float)(destRect->right - destRect->left),
                                  (float)(destRect->bottom - destRect->top) };
                SDL_RenderFillRect(g_sdlRenderer, &rct);
            } else {
                SDL_RenderClear(g_sdlRenderer);
            }
        }
        return DD_OK;
    }

    if (!src || !src->tex) return E_FAIL;

    SDL_FRect dst;
    if (destRect && (destRect->right > destRect->left || destRect->bottom > destRect->top)) {
        dst.x = (float)destRect->left;
        dst.y = (float)destRect->top;
        dst.w = (float)(destRect->right - destRect->left);
        dst.h = (float)(destRect->bottom - destRect->top);
    } else {
        dst.x = 0; dst.y = 0;
        dst.w = (float)g_screenWidth;
        dst.h = (float)g_screenHeight;
    }

    SDL_FRect srcR;
    if (srcRect && (srcRect->right > srcRect->left || srcRect->bottom > srcRect->top)) {
        srcR.x = (float)srcRect->left;
        srcR.y = (float)srcRect->top;
        srcR.w = (float)(srcRect->right - srcRect->left);
        srcR.h = (float)(srcRect->bottom - srcRect->top);
    } else {
        srcR.x = 0; srcR.y = 0;
        srcR.w = (float)src->w;
        srcR.h = (float)src->h;
    }

    SDL_RenderTexture(g_sdlRenderer, src->tex, &srcR, &dst);

    return DD_OK;
}

int Surface::SetColorKey(uint32_t flag, ColorKey *ck)
{
    (void)flag; (void)ck;
    hasColorKey = true;
    return DD_OK;
}

/* ═══════════════════════════════════════════════════════════════════════
 * GfxDevice Implementation
 * ═══════════════════════════════════════════════════════════════════════ */

int GfxDevice::CreateSurface(SurfaceDesc *d, Surface* *surf, void *u)
{
    (void)u;
    Surface *s = new Surface();
    s->w = (int)d->dwWidth;
    s->h = (int)d->dwHeight;
    s->tex = SDL_CreateTexture(g_sdlRenderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_TARGET,
                                s->w, s->h);
    if (!s->tex) { delete s; return E_FAIL; }
    SDL_SetTextureBlendMode(s->tex, SDL_BLENDMODE_BLEND);
    *surf = s;
    return DD_OK;
}

int GfxDevice::CreatePalette(uint32_t f, void *e, GfxDevicePalette **p, void *u)
{
    (void)f; (void)e; (void)u;
    *p = new GfxDevicePalette();
    return DD_OK;
}

void KIU_FillSurface(Surface* pdds, uint32_t color)
{
    (void)pdds;
    uint8_t r = (uint8_t)((color >> 16) & 0xFF);
    uint8_t g = (uint8_t)((color >> 8) & 0xFF);
    uint8_t b = (uint8_t)(color & 0xFF);
    SDL_SetRenderDrawColor(g_sdlRenderer, r, g, b, 255);
    SDL_RenderClear(g_sdlRenderer);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Sound Implementation (WAV effects via SDL3 audio)
 * ═══════════════════════════════════════════════════════════════════════ */

Sound::~Sound()
{
    if (stream) SDL_DestroyAudioStream(stream);
    if (data) SDL_free(data);
}

Sound *Sound::LoadWAV(const char *path)
{
    SDL_AudioSpec spec;
    Uint8 *buf;
    Uint32 len;

    if (!SDL_LoadWAV(path, &spec, &buf, &len)) {
        fprintf(stderr, "LoadWAV(%s) failed: %s\n", path, SDL_GetError());
        return nullptr;
    }

    Sound *snd = new Sound();
    snd->data   = buf;
    snd->length = len;

    snd->stream = SDL_CreateAudioStream(&spec, &spec);
    if (!snd->stream) {
        SDL_free(buf);
        delete snd;
        return nullptr;
    }
    return snd;
}

Sound *Sound::LoadMP3(const char *path)
{
    int err;
    mpg123_handle *mh = mpg123_new(nullptr, &err);
    if (!mh) {
        fprintf(stderr, "LoadMP3(%s): mpg123_new failed: %s\n", path, mpg123_plain_strerror(err));
        return nullptr;
    }
    if (mpg123_open(mh, path) != MPG123_OK) {
        fprintf(stderr, "LoadMP3(%s) failed: %s\n", path, mpg123_strerror(mh));
        mpg123_delete(mh);
        return nullptr;
    }

    long rate; int ch, enc;
    mpg123_getformat(mh, &rate, &ch, &enc);

    /* Decode entire MP3 to PCM buffer */
    size_t buf_size = 0, buf_cap = 65536;
    unsigned char *buf = (unsigned char *)malloc(buf_cap);
    size_t done;
    while (mpg123_read(mh, buf + buf_size, buf_cap - buf_size, &done) == MPG123_OK) {
        buf_size += done;
        if (buf_cap - buf_size < 4096) {
            buf_cap *= 2;
            buf = (unsigned char *)realloc(buf, buf_cap);
        }
    }
    /* Get any remaining data from final partial read */
    if (done > 0) buf_size += done;

    mpg123_close(mh);
    mpg123_delete(mh);

    if (buf_size == 0) { free(buf); return nullptr; }

    SDL_AudioSpec spec;
    SDL_memset(&spec, 0, sizeof(spec));
    spec.format   = SDL_AUDIO_S16;
    spec.channels = ch;
    spec.freq     = (int)rate;

    Sound *snd = new Sound();
    snd->data   = buf;
    snd->length = (Uint32)buf_size;
    snd->stream = SDL_CreateAudioStream(&spec, &spec);
    if (!snd->stream) {
        free(buf);
        delete snd;
        return nullptr;
    }
    return snd;
}

int Sound::Play(uint32_t reserved1, uint32_t reserved2, uint32_t flags)
{
    (void)reserved1; (void)reserved2;
    if (!data || !stream) return E_FAIL;

    looping = (flags & DSBPLAY_LOOPING) ? 1 : 0;

    if (!g_audioDevice) {
        SDL_AudioSpec spec;
        SDL_memset(&spec, 0, sizeof(spec));
        spec.format   = SDL_AUDIO_S16;
        spec.channels = 2;
        spec.freq     = 44100;
        g_audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
        if (!g_audioDevice) return E_FAIL;
    }

    SDL_ClearAudioStream(stream);
    SDL_PutAudioStreamData(stream, data, (int)length);
    SDL_BindAudioStream(g_audioDevice, stream);

    if (SDL_AudioDevicePaused(g_audioDevice))
        SDL_ResumeAudioDevice(g_audioDevice);

    /* For looping, we need to rebind; the callback handles refilling */
    /* We use a simpler approach: just send the data once for now */

    return DD_OK;
}

int Sound::Stop()
{
    looping = 0;
    if (stream) {
        SDL_ClearAudioStream(stream);
        SDL_UnbindAudioStream(stream);
    }
    return DD_OK;
}

int Sound::SetCurrentPosition(uint32_t pos)
{
    if (pos < length && stream) {
        SDL_ClearAudioStream(stream);
        SDL_PutAudioStreamData(stream, data + pos, (int)(length - pos));
    }
    return DD_OK;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CMedia Implementation (MP3 via mpg123)
 * ═══════════════════════════════════════════════════════════════════════ */

struct CMedia::Impl {
    mpg123_handle   *mh;
    long             rate;
    int              channels;
    State            state;
    SDL_AudioStream *stream;
    Uint8           *pcmData;
    Uint32           pcmLength;
    double           startPos;
    Uint64           startTick;
    bool             paused;
    double           pausedPos;
};

CMedia::CMedia()
{
    p = new Impl();
    p->mh       = nullptr;
    p->rate     = 44100;
    p->channels = 2;
    p->state    = Uninitialized;
    p->stream   = nullptr;
    p->pcmData   = nullptr;
    p->pcmLength = 0;
    p->startPos = 0.0;
    p->startTick = 0;
    p->paused    = false;
    p->pausedPos = 0.0;
}

CMedia::~CMedia()
{
    DeleteContents();
    delete p;
}

BOOL CMedia::CreateFilterGraph()
{
    /* mpg123 doesn't need filter graphs — just initialize the library.
     * Already done in KIU_Init. */
    return TRUE;
}

BOOL CMedia::RenderFile(char *szFile)
{
    int err;
    mpg123_handle *mh = mpg123_new(nullptr, &err);
    if (!mh) {
        fprintf(stderr, "mpg123_new failed: %s\n", mpg123_plain_strerror(err));
        return FALSE;
    }

    if (mpg123_open(mh, szFile) != MPG123_OK) {
        fprintf(stderr, "mpg123_open(%s) failed: %s\n", szFile, mpg123_strerror(mh));
        mpg123_delete(mh);
        return FALSE;
    }

    long rate; int ch, enc;
    mpg123_getformat(mh, &rate, &ch, &enc);

    DeleteContents();

    /* Decode entire MP3 to PCM buffer */
    size_t buf_size = 0, buf_cap = 262144;
    unsigned char *buf = (unsigned char *)malloc(buf_cap);
    size_t done;
    while (mpg123_read(mh, buf + buf_size, buf_cap - buf_size, &done) == MPG123_OK) {
        buf_size += done;
        if (buf_cap - buf_size < 4096) {
            buf_cap *= 2;
            buf = (unsigned char *)realloc(buf, buf_cap);
        }
    }
    if (done > 0) buf_size += done;

    mpg123_close(mh);
    mpg123_delete(mh);

    if (buf_size == 0) { free(buf); return FALSE; }

    p->mh        = nullptr; /* no longer needed — decoded to PCM */
    p->rate      = rate;
    p->channels  = ch;
    p->pcmData   = buf;
    p->pcmLength = (Uint32)buf_size;

    SDL_AudioSpec spec;
    SDL_memset(&spec, 0, sizeof(spec));
    spec.format   = SDL_AUDIO_S16;
    spec.channels = ch;
    spec.freq     = (int)rate;

    p->stream = SDL_CreateAudioStream(&spec, &spec);
    if (!p->stream) {
        free(buf);
        p->pcmData = nullptr;
        return FALSE;
    }

    return TRUE;
}

void CMedia::OpenMediaFile(char *szFile)
{
    RenderFile(szFile);
    ChangeStateTo(Stopped);
}

void CMedia::OnMediaPlay()
{
    if (!CanPlay() || !p->pcmData) return;

    if (!g_audioDevice) {
        SDL_AudioSpec spec;
        SDL_memset(&spec, 0, sizeof(spec));
        spec.format   = SDL_AUDIO_S16;
        spec.channels = p->channels;
        spec.freq     = (int)p->rate;
        g_audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
        if (!g_audioDevice) return;
    }

    SDL_ClearAudioStream(p->stream);
    SDL_PutAudioStreamData(p->stream, p->pcmData, (int)p->pcmLength);
    SDL_BindAudioStream(g_audioDevice, p->stream);

    if (SDL_AudioDevicePaused(g_audioDevice))
        SDL_ResumeAudioDevice(g_audioDevice);

    p->paused   = false;
    p->startTick = SDL_GetTicks();
    p->startPos  = 0.0;
    p->pausedPos = 0.0;

    ChangeStateTo(Playing);
}

void CMedia::OnMediaStop()
{
    if (!CanStop()) return;
    if (p->stream) {
        SDL_ClearAudioStream(p->stream);
        SDL_UnbindAudioStream(p->stream);
    }
    p->paused    = false;
    p->startPos  = 0.0;
    p->pausedPos = 0.0;
    ChangeStateTo(Stopped);
}

void CMedia::OnMediaPause()
{
    if (!CanPause()) return;
    p->paused    = true;
    p->pausedPos = GetCurrentPosition();
    ChangeStateTo(Paused);
}

void CMedia::OnMediaAbortStop()
{
    OnMediaStop();
}

double CMedia::GetCurrentPosition()
{
    if (!p->pcmData || !p->stream) return 0.0;
    if (p->paused) return p->pausedPos;

    /* Use SDL audio stream position: how many bytes have been consumed */
    int queued = SDL_GetAudioStreamQueued(p->stream);
    Uint32 played = p->pcmLength - (queued > 0 ? (Uint32)queued : 0);
    if (played > p->pcmLength) played = p->pcmLength;

    int bytesPerSec = (int)p->rate * p->channels * 2; /* S16 = 2 bytes/sample */
    if (bytesPerSec <= 0) return 0.0;
    return (double)played / (double)bytesPerSec;
}

void CMedia::ChangeStateTo(State newState) { p->state = newState; }
BOOL CMedia::CanPlay()  { return (p->state == Stopped || p->state == Paused); }
BOOL CMedia::CanStop()  { return (p->state == Playing || p->state == Paused); }
BOOL CMedia::CanPause() { return (p->state == Playing || p->state == Paused); }
void CMedia::DeleteContents()
{
    if (p->stream) { SDL_DestroyAudioStream(p->stream); p->stream = nullptr; }
    if (p->pcmData) { free(p->pcmData); p->pcmData = nullptr; p->pcmLength = 0; }
    p->state = Uninitialized;
}

/* ═══════════════════════════════════════════════════════════════════════
 * Keyboard input — fills rgKeyData[] buffer like DirectInput
 * ═══════════════════════════════════════════════════════════════════════ */

unsigned char rgKeyData[KEYBUFSIZE];
int g_cpdevFound = 0;  /* Stub for Input.cpp */

void KIU_ReadKeyboard(void)
{
    int numKeys;
    const bool *ks = SDL_GetKeyboardState(&numKeys);

    /* Clear the buffer */
    memset(rgKeyData, 0, sizeof(rgKeyData));

    /* Map each relevant DIK code to SDL scancode and read state */
    /* We only fill the entries that map to valid keys */
    struct { int dik; int sdl_sc; } mapping[] = {
        {DIK_ESCAPE,      SDL_SCANCODE_ESCAPE},
        {DIK_1,           SDL_SCANCODE_1},
        {DIK_2,           SDL_SCANCODE_2},
        {DIK_3,           SDL_SCANCODE_3},
        {DIK_4,           SDL_SCANCODE_4},
        {DIK_5,           SDL_SCANCODE_5},
        {DIK_6,           SDL_SCANCODE_6},
        {DIK_7,           SDL_SCANCODE_7},
        {DIK_8,           SDL_SCANCODE_8},
        {DIK_9,           SDL_SCANCODE_9},
        {DIK_0,           SDL_SCANCODE_0},
        {DIK_MINUS,       SDL_SCANCODE_MINUS},
        {DIK_EQUALS,      SDL_SCANCODE_EQUALS},
        {DIK_BACK,        SDL_SCANCODE_BACKSPACE},
        {DIK_TAB,         SDL_SCANCODE_TAB},
        {DIK_Q,           SDL_SCANCODE_Q},
        {DIK_W,           SDL_SCANCODE_W},
        {DIK_E,           SDL_SCANCODE_E},
        {DIK_R,           SDL_SCANCODE_R},
        {DIK_T,           SDL_SCANCODE_T},
        {DIK_Y,           SDL_SCANCODE_Y},
        {DIK_U,           SDL_SCANCODE_U},
        {DIK_I,           SDL_SCANCODE_I},
        {DIK_O,           SDL_SCANCODE_O},
        {DIK_P,           SDL_SCANCODE_P},
        {DIK_LBRACKET,    SDL_SCANCODE_LEFTBRACKET},
        {DIK_RBRACKET,    SDL_SCANCODE_RIGHTBRACKET},
        {DIK_RETURN,      SDL_SCANCODE_RETURN},
        {DIK_LCONTROL,    SDL_SCANCODE_LCTRL},
        {DIK_A,           SDL_SCANCODE_A},
        {DIK_S,           SDL_SCANCODE_S},
        {DIK_D,           SDL_SCANCODE_D},
        {DIK_F,           SDL_SCANCODE_F},
        {DIK_G,           SDL_SCANCODE_G},
        {DIK_H,           SDL_SCANCODE_H},
        {DIK_J,           SDL_SCANCODE_J},
        {DIK_K,           SDL_SCANCODE_K},
        {DIK_L,           SDL_SCANCODE_L},
        {DIK_SEMICOLON,   SDL_SCANCODE_SEMICOLON},
        {DIK_APOSTROPHE,  SDL_SCANCODE_APOSTROPHE},
        {DIK_GRAVE,       SDL_SCANCODE_GRAVE},
        {DIK_LSHIFT,      SDL_SCANCODE_LSHIFT},
        {DIK_BACKSLASH,   SDL_SCANCODE_BACKSLASH},
        {DIK_Z,           SDL_SCANCODE_Z},
        {DIK_X,           SDL_SCANCODE_X},
        {DIK_C,           SDL_SCANCODE_C},
        {DIK_V,           SDL_SCANCODE_V},
        {DIK_B,           SDL_SCANCODE_B},
        {DIK_N,           SDL_SCANCODE_N},
        {DIK_M,           SDL_SCANCODE_M},
        {DIK_COMMA,       SDL_SCANCODE_COMMA},
        {DIK_PERIOD,      SDL_SCANCODE_PERIOD},
        {DIK_SLASH,       SDL_SCANCODE_SLASH},
        {DIK_RSHIFT,      SDL_SCANCODE_RSHIFT},
        {DIK_LMENU,       SDL_SCANCODE_LALT},
        {DIK_SPACE,       SDL_SCANCODE_SPACE},
        {DIK_CAPITAL,     SDL_SCANCODE_CAPSLOCK},
        {DIK_F1,          SDL_SCANCODE_F1},
        {DIK_F2,          SDL_SCANCODE_F2},
        {DIK_F3,          SDL_SCANCODE_F3},
        {DIK_F4,          SDL_SCANCODE_F4},
        {DIK_F5,          SDL_SCANCODE_F5},
        {DIK_F6,          SDL_SCANCODE_F6},
        {DIK_F7,          SDL_SCANCODE_F7},
        {DIK_F8,          SDL_SCANCODE_F8},
        {DIK_F9,          SDL_SCANCODE_F9},
        {DIK_F10,         SDL_SCANCODE_F10},
        {DIK_F11,         SDL_SCANCODE_F11},
        {DIK_F12,         SDL_SCANCODE_F12},
        {DIK_NUMPAD7,     SDL_SCANCODE_KP_7},
        {DIK_NUMPAD8,     SDL_SCANCODE_KP_8},
        {DIK_NUMPAD9,     SDL_SCANCODE_KP_9},
        {DIK_NUMPAD4,     SDL_SCANCODE_KP_4},
        {DIK_NUMPAD5,     SDL_SCANCODE_KP_5},
        {DIK_NUMPAD6,     SDL_SCANCODE_KP_6},
        {DIK_NUMPAD1,     SDL_SCANCODE_KP_1},
        {DIK_NUMPAD2,     SDL_SCANCODE_KP_2},
        {DIK_NUMPAD3,     SDL_SCANCODE_KP_3},
        {DIK_NUMPAD0,     SDL_SCANCODE_KP_0},
        {DIK_DECIMAL,     SDL_SCANCODE_KP_PERIOD},
        {DIK_NUMPADENTER, SDL_SCANCODE_KP_ENTER},
        {DIK_RCONTROL,    SDL_SCANCODE_RCTRL},
        {DIK_RMENU,       SDL_SCANCODE_RALT},
        {DIK_HOME,        SDL_SCANCODE_HOME},
        {DIK_UP,          SDL_SCANCODE_UP},
        {DIK_PRIOR,       SDL_SCANCODE_PAGEUP},
        {DIK_LEFT,        SDL_SCANCODE_LEFT},
        {DIK_RIGHT,       SDL_SCANCODE_RIGHT},
        {DIK_END,         SDL_SCANCODE_END},
        {DIK_DOWN,        SDL_SCANCODE_DOWN},
        {DIK_NEXT,        SDL_SCANCODE_PAGEDOWN},
        {DIK_INSERT,      SDL_SCANCODE_INSERT},
        {DIK_DELETE,      SDL_SCANCODE_DELETE},
    };
    int nmap = sizeof(mapping) / sizeof(mapping[0]);

    for (int i = 0; i < nmap; i++) {
        int dik = mapping[i].dik;
        int sdl = mapping[i].sdl_sc;
        if (dik >= 0 && dik < KEYBUFSIZE && sdl >= 0 && sdl < numKeys) {
            rgKeyData[dik] = ks[sdl] ? 0x80 : 0;
        }
    }
}

int KIU_KeyStateDIK(int dik)
{
    if (dik < 0 || dik >= KEYBUFSIZE) return 0;
    return rgKeyData[dik];
}

/* ═══════════════════════════════════════════════════════════════════════
 * Debug Text Display
 * ═══════════════════════════════════════════════════════════════════════ */

static TTF_Font *g_debugFont = nullptr;

void KIU_DisplayMessage(int x, int y, const char *msg)
{
    if (!g_ttfAvailable) return;

    if (!g_debugFont) {
        g_debugFont = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 12.0f);
        if (!g_debugFont)
            g_debugFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12.0f);
        if (!g_debugFont) {
            g_ttfAvailable = false;
            return;
        }
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *textSurf = TTF_RenderText_Solid(g_debugFont, msg, 0, white);
    if (!textSurf) return;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(g_sdlRenderer, textSurf);
    SDL_FRect dst = { (float)x, (float)y, (float)textSurf->w, (float)textSurf->h };
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderTexture(g_sdlRenderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
    SDL_DestroySurface(textSurf);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Missing stubs
 * ═══════════════════════════════════════════════════════════════════════ */

void KIU_CleanupInput(void)
{
    /* Nothing to clean up for SDL3 input */
}

Color KIU_ColorMatch(Surface* pdds, Color rgb)
{
    if (rgb == CLR_INVALID)
        return RGB(255, 0, 255);
    return rgb;
}
