/*
 * KeyboardDevice.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "KeyboardDevice.h"
#include <cassert>
#include <SDL.h>

int		KeyboardDevice::GetState( const eKind kind, const uKey key )
{
	int	ret = 0;
	
	switch( kind ) {
		case KeyState:{			
			const Uint8 * keystates = SDL_GetKeyboardState( NULL );
			if( keystates )
				ret = static_cast<int>( keystates[key.key] );
			break;
		}
		default:
			assert( 0 );
			break;
	}
	return ret;
}
