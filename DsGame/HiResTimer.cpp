#include "HiResTimer.h"
#include <SDL2/SDL.h>


 HiResTimer::HiResTimer() 
	 : frequency_(SDL_GetPerformanceFrequency())
 {
 }

double HiResTimer::now() const 
{
	return double(SDL_GetPerformanceCounter())/frequency_;
}
