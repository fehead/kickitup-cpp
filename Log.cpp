#include "Log.h"
#include <cstdarg>
#include <SDL_log.h>
#include <iostream>

#define	LOG_FILE "log.txt"

Log::Log(void)
{	
}

Log::~Log(void)
{	
}

void Log::log(const char* format, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, format);	
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, format, args);	
	va_end(args);	
	
	/*
	if (fp) {
		va_list args;
		va_start(args, format);
		vfprintf(fp, format, args);
		va_end(args);	
	}
	*/
}

