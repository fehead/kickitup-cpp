/*
 * audio.h - SDL3 Port: Audio manager (owning the sound objects)
 */

#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "sdl3_kick.h"

class AudioManager {
    Sound *_opening, *_dead, *_mode, *_cancel, *_move, *_beat, *_selSong;
    CMedia *_intro, *_songBg;

public:
    AudioManager();
    ~AudioManager();
    bool loadAll();
    void unloadAll();
};

extern AudioManager g_audio;

/* Globals owned by AudioManager — initialized by loadAll() */
extern Sound *g_dsOpening, *g_dsDead, *g_dsMode, *g_dsCancel;
extern Sound *g_dsMove, *g_dsBeat, *g_dsSelectSong;

#endif
