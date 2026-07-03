/*
 * ddutil.h - SDL3 Port: Bitmap loading compatibility
 *
 * Original: DirectDraw bitmap/palette loading
 * Ported:   Forwarders to KIU_* functions in sdl3_kick.cpp
 */

#ifndef __DDUTIL_INCLUDED__
#define __DDUTIL_INCLUDED__

#include "sdl3_kick.h"

#ifdef __cplusplus
extern "C" {
#endif

/* These forward to SDL3 implementations */
#define DDLoadBitmap(pdd, szBitmap, dx, dy) \
    Surface::LoadBitmap(szBitmap, dx, dy)

#define DDReLoadBitmap(pdds, szBitmap) \
    KIU_ReloadSurface(pdds, szBitmap)

#define DDSetColorKey(pdds, rgb) \
    ((Surface*)(pdds))->SetColorKey(0, nullptr)

#define DDColorMatch(pdds, rgb) \
    KIU_ColorMatch(pdds, rgb)

#define DDFillSurface(pdds, color) \
    KIU_FillSurface(pdds, color)

/* Palette stub */
#define DDLoadPalette(pdd, szBitmap)  ((Palette*)1)

#ifdef __cplusplus
}
#endif

#endif
