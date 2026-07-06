/*
 * dsutil.h - SDL3 Port: Sound loading compatibility
 *
 * Original: DirectSound WAV loading from resources
 * Ported:   mpg123-based MP3 loading via Sound::LoadMP3
 */

#ifndef __DSUTIL_INCLUDE__
#define __DSUTIL_INCLUDE__

#include "sdl3_kick.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Load sound from MP3 file (decodes fully to PCM at load time) */
#define DSLoadSoundBuffer(pDS, lpName) \
    Sound::LoadMP3(lpName)

#define DSReloadSoundBuffer(pDSB, lpName)   true

#ifdef __cplusplus
}
#endif

#endif
