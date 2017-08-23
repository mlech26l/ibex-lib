//============================================================================
//                                  I B E X                                   
// File        : ibex_Timer.h
// Author      : Jordan Ninin and Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#ifndef __IBEX_TIMER_H__
#define __IBEX_TIMER_H__

#include "ibex_Exception.h"

#ifndef _WIN32
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#else
/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */
struct mytimeval
  {
	long int tv_sec;		/* Seconds.  */
	long int tv_usec;	/* Microseconds.  */
  };
#endif // _MSC_VER

namespace ibex {

/** \ingroup tools
 *
 * \brief Timeout exception.
 *
 */
class TimeOutException : public Exception { };


/** \ingroup tools
 *
 * \brief Timer.
 */
class Timer {
 public:
	Timer();

	void start();
	void stop();
	void restart();
	double get_time();
	void check(double timeout);
private:
	  double start_time;
	  bool active;
};

/** \ingroup tools
 *
 * \brief Timer.
 */
class StaticTimer {
 public:
  static void start();

 private:
  friend class Timer;

  typedef double Time;

  static Time get_time();

  static Time real_lapse;
  static Time virtual_ulapse;
  static Time virtual_slapse;

 //private:
  static Time local_time;
  static Time real_time;
  static Time virtual_utime;
  static Time virtual_stime;
  static long resident_memory;
#ifndef _WIN32
  static struct rusage res;
#else
  static struct mytimeval tp;
#endif
};

} // end namespace ibex
#endif // __IBEX_TIMER_H__
