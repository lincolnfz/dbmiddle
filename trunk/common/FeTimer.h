#pragma once
/************************************************************************/
/* 时间计数器                                                            */
/************************************************************************/

#ifndef WIN32
#include <sys/time.h>
#include <sys/uio.h>
#include <pthread.h>
#else
#include <windows.h>
#endif
#include <stdint.h>
#include <cstdlib>

typedef void (__stdcall *WAITORTIMERCALLBACK)(	__in  PVOID lpParameter, __in  BOOLEAN TimerOrWaitFired	);

#ifdef WIN32
// Windows compability
typedef HANDLE pthread_t;
typedef HANDLE pthread_mutex_t;
typedef HANDLE pthread_cond_t;
#endif


////////////////////////////////////////////////////////////////////////////////

class CFeTimer
{
public:
	CFeTimer();
	~CFeTimer();
	void CreateTimer( WAITORTIMERCALLBACK callback , void* parm , unsigned long dueTime , unsigned period );
	void CancelTimer();

public:

	// Functionality:
	//    Sleep for "interval" CCs.
	// Parameters:
	//    0) [in] interval: CCs to sleep.
	// Returned value:
	//    None.

	void sleep(const uint64_t& interval);

	// Functionality:
	//    Seelp until CC "nexttime".
	// Parameters:
	//    0) [in] nexttime: next time the caller is waken up.
	// Returned value:
	//    None.

	void sleepto(const uint64_t& nexttime);

	// Functionality:
	//    Stop the sleep() or sleepto() methods.
	// Parameters:
	//    None.
	// Returned value:
	//    None.

	void interrupt();

	// Functionality:
	//    trigger the clock for a tick, for better granularity in no_busy_waiting timer.
	// Parameters:
	//    None.
	// Returned value:
	//    None.

	void tick();

public:

	// Functionality:
	//    Read the CPU clock cycle into x.
	// Parameters:
	//    0) [out] x: to record cpu clock cycles.
	// Returned value:
	//    None.

	static void rdtsc(uint64_t &x);

	// Functionality:
	//    return the CPU frequency.
	// Parameters:
	//    None.
	// Returned value:
	//    CPU frequency.

	static uint64_t getCPUFrequency();

	// Functionality:
	//    check the current time, 64bit, in microseconds.
	// Parameters:
	//    None.
	// Returned value:
	//    current time in microseconds.

	static uint64_t getTime();

	// Functionality:
	//    trigger an event such as new connection, close, new data, etc. for "select" call.
	// Parameters:
	//    None.
	// Returned value:
	//    None.

	static void triggerEvent();

	// Functionality:
	//    wait for an event to br triggered by "triggerEvent".
	// Parameters:
	//    None.
	// Returned value:
	//    None.

	static void waitForEvent();

	// Functionality:
	//    sleep for a short interval. exact sleep time does not matter
	// Parameters:
	//    None.
	// Returned value:
	//    None.

	static void sleep();

protected:
	static unsigned int __stdcall procThread( void* parm ); 

private:
	uint64_t m_ullSchedTime;             // next schedulled time

	pthread_cond_t m_TickCond;
	pthread_mutex_t m_TickLock;

	bool m_bCancle;

	static pthread_cond_t m_EventCond;
	static pthread_mutex_t m_EventLock;

private:
	static uint64_t s_ullCPUFrequency;	// CPU frequency : clock cycles per microsecond
	static uint64_t readCPUFrequency();
};
