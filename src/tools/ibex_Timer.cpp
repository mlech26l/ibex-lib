//============================================================================
//                                  I B E X                                   
// File        : ibex_StaticTimer.cpp
// Author      : ???? and Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Timer.h"

namespace ibex {


static int ____IGNORE___ = (StaticTimer::start(), 0);


Timer::Timer(): start_time(0.0), active(false) {
}


void Timer::start(){
	if (not(active)) {
		active = true;
		start_time = StaticTimer::get_time();
	}
}

void Timer::stop(){
	active = false;
	start_time = (StaticTimer::get_time() - start_time);
}
void Timer::restart() {
	start_time= StaticTimer::get_time();
	active =true;
}
double Timer::get_time() {
	if (active) {
		return (StaticTimer::get_time() - start_time);
	} else {
		return start_time;
	}
}
void Timer::check(double timeout) {
	if (get_time() >= timeout) throw TimeOutException();
}


	/////////////////////////////////////////////////////////////////////

#ifdef _WIN32  //_MSC_VER
// To get struct timeval.
#include <winsock2.h>
#include <sys/time.h>

//#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ui64
//#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#define DELTA_EPOCH_IN_MICROSECS_LOW 1216757760
#define DELTA_EPOCH_IN_MICROSECS_HIGH 2711190

/*
// Disable Visual Studio warnings about timezone declaration.
#pragma warning(disable : 6244) 

// Local declaration of timezone hides previous declaration in time.h.
struct timezone 
{
	int tz_minuteswest; // Minutes W of Greenwich.
	int tz_dsttime; // Type of DST correction.
};

// Restore the Visual Studio warnings previously disabled.
#pragma warning(default : 6244) 

*/

/*
Obtain the current time, expressed as seconds and microseconds since the Epoch, 
and store it in the timeval structure pointed to by tv (the accuracy should be
around 15 ms).

struct timeval* tv : (INOUT) Valid pointer to the structure that will receive 
the current time.
struct timezone* tz : (INOUT) Usually set to NULL.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
/*
int gettimeofday(struct timeval* tv, struct timezone* tz)
{
	FILETIME ft; // Will contain a 64-bit value representing the number of 100-nanosecond 
	// intervals since January 1, 1601 (UTC).
	ULONGLONG tmpres = 0;
	ULARGE_INTEGER li;
	ULARGE_INTEGER epoch;
#ifdef USE__TZSET
	static int tzflag;
#else
	TIME_ZONE_INFORMATION tz_winapi;
	int rez = 0;
#endif // USE__TZSET

	if (tv)
	{
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;

		// Converting file time to UNIX Epoch.
		tmpres = li.QuadPart/(ULONGLONG)10; // Convert into microseconds.
		//tmpres -= DELTA_EPOCH_IN_MICROSECS;
		epoch.LowPart = DELTA_EPOCH_IN_MICROSECS_LOW;
		epoch.HighPart = DELTA_EPOCH_IN_MICROSECS_HIGH;
		tmpres -= epoch.QuadPart;

		tv->tv_sec = (long)(tmpres/(ULONGLONG)1000000);
		tv->tv_usec = (long)(tmpres%(ULONGLONG)1000000);
	}

#ifdef USE__TZSET
	if (tz)
	{
		if (!tzflag)
		{
			_tzset();
			tzflag++;
		}
		tz->tz_minuteswest = _timezone/60;
		tz->tz_dsttime = _daylight;
	}
#else
	if (tz)
	{
		// _tzset(), do not work properly, so we use GetTimeZoneInformation.
		rez = GetTimeZoneInformation(&tz_winapi);
		tz->tz_dsttime = (rez == 2)?TRUE:FALSE;
		tz->tz_minuteswest = tz_winapi.Bias + ((rez == 2)?tz_winapi.DaylightBias:0);
	}
#endif // USE__TZSET

	return EXIT_SUCCESS;
}
*/

//inline int gettimeofday(struct timeval* tp, void* tz)
//{
//	struct _timeb timebuffer;
//
//	UNREFERENCED_PARAMETER(tz);
//
//	_ftime(&timebuffer);
//	tp->tv_sec = (long)timebuffer.time;
//	tp->tv_usec = timebuffer.millitm*1000;
//	return 0;
//}

int mygettimeofday(struct timeval* tv)
{
	FILETIME ft; // Will contain a 64-bit value representing the number of 100-nanosecond
	// intervals since January 1, 1601 (UTC).
	ULONGLONG tmpres = 0;
	ULARGE_INTEGER li;
	ULARGE_INTEGER epoch;

	if (tv)
	{
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;

		// Converting file time to UNIX Epoch.
		tmpres = li.QuadPart/(ULONGLONG)10; // Convert into microseconds.
		//tmpres -= DELTA_EPOCH_IN_MICROSECS;
		epoch.LowPart = DELTA_EPOCH_IN_MICROSECS_LOW;
		epoch.HighPart = DELTA_EPOCH_IN_MICROSECS_HIGH;
		tmpres -= epoch.QuadPart;

		tv->tv_sec = (long)(tmpres/(ULONGLONG)1000000);
		tv->tv_usec = (long)(tmpres%(ULONGLONG)1000000);
	}


	return EXIT_SUCCESS;
}

#endif // _WIN32     _MSC_VER

	/////////////////////////////////////////////////////////////////////


StaticTimer::Time StaticTimer::local_time = 0;
StaticTimer::Time StaticTimer::real_lapse;
StaticTimer::Time StaticTimer::virtual_ulapse;
StaticTimer::Time StaticTimer::virtual_slapse;
StaticTimer::Time StaticTimer::real_time;
StaticTimer::Time StaticTimer::virtual_utime;
StaticTimer::Time StaticTimer::virtual_stime;
long StaticTimer::resident_memory;

#ifndef _WIN32
struct rusage StaticTimer::res;
#else
struct timeval StaticTimer::tp;
#endif
/*
 *  The virtual time of day and the real time of day are calculated and
 *  stored for future use.  The future use consists of subtracting these
 *  values from similar values obtained at a later time to allow the user
 *  to get the amount of time used by the backtracking routine.
 */
void StaticTimer::start() {
#ifndef _WIN32
	//    res = std::clock();
	getrusage( RUSAGE_SELF, &res );

	virtual_utime = (Time) res.ru_utime.tv_sec +
			(Time) res.ru_utime.tv_usec / 1000000.0;
	virtual_stime = (Time) res.ru_stime.tv_sec +
			(Time) res.ru_stime.tv_usec / 1000000.0;

#else

	mygettimeofday( &tp);
	real_time =    (Time) tp.tv_sec +
			(Time) tp.tv_usec / 1000000.0;
#endif

}


StaticTimer::Time StaticTimer::get_time() {

#ifndef _WIN32

	getrusage( RUSAGE_SELF, &res );
	virtual_ulapse = (Time) res.ru_utime.tv_sec +
			(Time) res.ru_utime.tv_usec / 1000000.0
			- virtual_utime;
	virtual_slapse = (Time) res.ru_stime.tv_sec +
			(Time) res.ru_stime.tv_usec / 1000000.0
			- virtual_stime;
	resident_memory = res.ru_ixrss;
	if (resident_memory > 100000) ibex_error(" Timer: memory limit, out of resident memory "  );

	virtual_utime = (Time) res.ru_utime.tv_sec +
			(Time) res.ru_utime.tv_usec / 1000000.0;
	virtual_stime = (Time) res.ru_stime.tv_sec +
			(Time) res.ru_stime.tv_usec / 1000000.0;


	local_time += (virtual_ulapse + virtual_slapse);

#else
	mygettimeofday( &tp);
	real_lapse = (Time) tp.tv_sec +
			(Time) tp.tv_usec / 1000000.0
			- real_time;

	real_time =    (Time) tp.tv_sec +
			(Time) tp.tv_usec / 1000000.0;

	local_time += real_time;

#endif
	return local_time;
}




} // end namespace ibex
