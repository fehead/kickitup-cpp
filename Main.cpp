#include "common.h"
#include "KickItUp.h"
#include <SDL.h>

int main( int argc, char * argv[] )
{
	KickItUp	kickItUp;
	
	// Initialize the game
	if( !kickItUp.Initialize() )
		return 1;

	// Run the game
	kickItUp.Run();

	// Shutdown the game
	kickItUp.Destroy();

	return 0;
}
