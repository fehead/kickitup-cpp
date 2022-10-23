/*
 * SDLSurfaceStore.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "SDLSurfaceStore.h"
#include "SDLSurface.h"

SDLSurfaceStore::SDLSurfaceStore(void)	:	_pWindow( 0 ), _pSurface(0), _pRenderer(0)
{
}

SDLSurfaceStore::~SDLSurfaceStore(void)
{
}

// initilze sdl.
bool	SDLSurfaceStore::Initialize()
{

	_pWindow = SDL_CreateWindow("KickItUp ver 0.9",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_OPENGL);	

	if (_pWindow == 0)
		return false;

	// _pRenderer = SDL_CreateRenderer(_pWindow, -1, SDL_RENDERER_ACCELERATED);

	_pSurface = SDL_GetWindowSurface(_pWindow);

	if (_pSurface == 0)
		return false;

	return true;
}

// Destroy sdl.
void	SDLSurfaceStore::Destroy()
{
	SurfaceStore::Destroy();

	SDL_FreeSurface( _pSurface );

	if (_pWindow != 0) {
		SDL_DestroyWindow(_pWindow);
	}
}

// create sdl surface.
Surface *	SDLSurfaceStore::create()
{
	Surface * pSurface = new SDLSurface(_pSurface);
	return pSurface;
}

// flip the main surface.
bool	SDLSurfaceStore::Process( unsigned long delta )
{	
	SurfaceStore::Process( delta );
	// SDL_Flip( _pSurface );	
	// SDL_RenderPresent(_pRenderer);
	SDL_UpdateWindowSurface(_pWindow);
	return true;
}
