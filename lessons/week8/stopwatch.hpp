#pragma once

#include "w32_funcs.hpp"

/*
    A simple stopwatch.
    This stopwatch is independent of wall clock time.  It sets a relative
    start position whenever you call 'reset()'.
    
    The only function it serves is to tell you the number of seconds since
    the reset() method was called.
*/



int64_t GetPerformanceFrequency()
{
	int64_t anum;
	int success = QueryPerformanceFrequency(&anum);
	if (success == 0) {
		return 0;   //, ffi.C.GetLastError(); 
    }

	return anum;
}

int64_t GetPerformanceCounter()
{
	int64_t pnum;
	int success = QueryPerformanceCounter(&pnum);
	if (success == 0) { 
		return 0; //--, ffi.C.GetLastError();
    }

	return pnum;
}

double GetCurrentTickTime()
{
    int64_t pnum;

	double frequency = 1.0f/GetPerformanceFrequency();
	double currentCount = GetPerformanceCounter();
	double seconds = (double)currentCount * frequency;

	return seconds;
}


class StopWatch
{
    double fStartTime;

public:
    StopWatch()
    {
        reset();
    }

    double seconds() const
    {
        double currentTime = GetCurrentTickTime();
        return currentTime - fStartTime;
    }

    double millis() const
    {
        return seconds() * 1000;
    }

    void reset()
    {
        fStartTime = GetCurrentTickTime();
    }
};

