#pragma once

#include <cstdio>
class Log
{
public:
	Log(void);
	~Log(void);

	static void log(const char * format, ...);
};

