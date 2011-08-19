#include "StdAfx.h"
#include "FeTimer.h"

#ifndef WIN32
#include <cstring>
#include <cerrno>
#include <unistd.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#ifdef LEGACY_WIN32
#include <wspiapi.h>
#endif
#endif
#
#include <cmath>

#define NO_BUSY_WAITING
uint64_t CFeTimer::s_ullCPUFrequency = CFeTimer::readCPUFrequency();
#ifndef WIN32
pthread_mutex_t CFeTimer::m_EventLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CFeTimer::m_EventCond = PTHREAD_COND_INITIALIZER;
#else
pthread_mutex_t CFeTimer::m_EventLock = CreateMutex(NULL, false, NULL);
pthread_cond_t CFeTimer::m_EventCond = CreateEvent(NULL, false, false, NULL);
#endif

CFeTimer::CFeTimer():
m_ullSchedTime(),
	m_TickCond(),
	m_TickLock()
{
#ifndef WIN32
	pthread_mutex_init(&m_TickLock, NULL);
	pthread_cond_init(&m_TickCond, NULL);
#else
	m_TickLock = CreateMutex(NULL, false, NULL);
	m_TickCond = CreateEvent(NULL, false, false, NULL);
#endif
}

CFeTimer::~CFeTimer()
{
#ifndef WIN32
	pthread_mutex_destroy(&m_TickLock);
	pthread_cond_destroy(&m_TickCond);
#else
	CloseHandle(m_TickLock);
	CloseHandle(m_TickCond);
#endif
}

void CFeTimer::rdtsc(uint64_t &x)
{
#ifdef WIN32
	//HANDLE hCurThread = ::GetCurrentThread(); 
	//DWORD_PTR dwOldMask = ::SetThreadAffinityMask(hCurThread, 1); 
	BOOL ret = QueryPerformanceCounter((LARGE_INTEGER *)&x);
	//SetThreadAffinityMask(hCurThread, dwOldMask);

	if (!ret)
		x = getTime() * s_ullCPUFrequency;

#elif IA32
	uint32_t lval, hval;
	//asm volatile ("push %eax; push %ebx; push %ecx; push %edx");
	//asm volatile ("xor %eax, %eax; cpuid");
	asm volatile ("rdtsc" : "=a" (lval), "=d" (hval));
	//asm volatile ("pop %edx; pop %ecx; pop %ebx; pop %eax");
	x = hval;
	x = (x << 32) | lval;
#elif IA64
	asm ("mov %0=ar.itc" : "=r"(x) :: "memory");
#elif AMD64
	uint32_t lval, hval;
	asm ("rdtsc" : "=a" (lval), "=d" (hval));
	x = hval;
	x = (x << 32) | lval;
#else
	// use system call to read time clock for other archs
	timeval t;
	gettimeofday(&t, 0);
	x = (uint64_t)t.tv_sec * (uint64_t)1000000 + (uint64_t)t.tv_usec;
#endif
}

uint64_t CFeTimer::readCPUFrequency()
{
#ifdef WIN32
	int64_t ccf;
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&ccf))
		return ccf / 1000000;
	else
		return 1;
#elif IA32 || IA64 || AMD64
	uint64_t t1, t2;

	rdtsc(t1);
	timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 100000000;
	nanosleep(&ts, NULL);
	rdtsc(t2);

	// CPU clocks per microsecond
	return (t2 - t1) / 100000;
#else
	return 1;
#endif
}

uint64_t CFeTimer::getCPUFrequency()
{
	return s_ullCPUFrequency;
}

void CFeTimer::sleep(const uint64_t& interval)
{
	uint64_t t;
	rdtsc(t);

	// sleep next "interval" time
	sleepto(t + interval);
}

void CFeTimer::sleepto(const uint64_t& nexttime)
{
	// Use class member such that the method can be interrupted by others
	m_ullSchedTime = nexttime;

	uint64_t t;
	rdtsc(t);

	while (t < m_ullSchedTime)
	{
#ifndef NO_BUSY_WAITING
#ifdef IA32
		__asm__ volatile ("pause; rep; nop; nop; nop; nop; nop;");
#elif IA64
		__asm__ volatile ("nop 0; nop 0; nop 0; nop 0; nop 0;");
#elif AMD64
		__asm__ volatile ("nop; nop; nop; nop; nop;");
#endif
#else
#ifndef WIN32
		timeval now;
		timespec timeout;
		gettimeofday(&now, 0);
		if (now.tv_usec < 990000)
		{
			timeout.tv_sec = now.tv_sec;
			timeout.tv_nsec = (now.tv_usec + 10000) * 1000;
		}
		else
		{
			timeout.tv_sec = now.tv_sec + 1;
			timeout.tv_nsec = (now.tv_usec + 10000 - 1000000) * 1000;
		}
		pthread_mutex_lock(&m_TickLock);
		pthread_cond_timedwait(&m_TickCond, &m_TickLock, &timeout);
		pthread_mutex_unlock(&m_TickLock);
#else
		WaitForSingleObject(m_TickCond, 1);
#endif
#endif

		rdtsc(t);
	}
}

void CFeTimer::interrupt()
{
	// schedule the sleepto time to the current CCs, so that it will stop
	rdtsc(m_ullSchedTime);

	tick();
}

void CFeTimer::tick()
{
#ifndef WIN32
	pthread_cond_signal(&m_TickCond);
#else
	SetEvent(m_TickCond);
#endif
}

uint64_t CFeTimer::getTime()
{
	//For Cygwin and other systems without microsecond level resolution, uncomment the following three lines
	//uint64_t x;
	//rdtsc(x);
	//return x / s_ullCPUFrequency;

#ifndef WIN32
	timeval t;
	gettimeofday(&t, 0);
	return t.tv_sec * 1000000ULL + t.tv_usec;
#else
	LARGE_INTEGER ccf;
	HANDLE hCurThread = ::GetCurrentThread(); 
	DWORD_PTR dwOldMask = ::SetThreadAffinityMask(hCurThread, 1);
	if (QueryPerformanceFrequency(&ccf))
	{
		LARGE_INTEGER cc;
		if (QueryPerformanceCounter(&cc))
		{
			SetThreadAffinityMask(hCurThread, dwOldMask); 
			return (cc.QuadPart * 1000000ULL / ccf.QuadPart);
		}
	}

	SetThreadAffinityMask(hCurThread, dwOldMask); 
	return GetTickCount() * 1000ULL;
#endif
}

void CFeTimer::triggerEvent()
{
#ifndef WIN32
	pthread_cond_signal(&m_EventCond);
#else
	SetEvent(m_EventCond);
#endif
}

void CFeTimer::waitForEvent()
{
#ifndef WIN32
	timeval now;
	timespec timeout;
	gettimeofday(&now, 0);
	if (now.tv_usec < 990000)
	{
		timeout.tv_sec = now.tv_sec;
		timeout.tv_nsec = (now.tv_usec + 10000) * 1000;
	}
	else
	{
		timeout.tv_sec = now.tv_sec + 1;
		timeout.tv_nsec = (now.tv_usec + 10000 - 1000000) * 1000;
	}
	pthread_mutex_lock(&m_EventLock);
	pthread_cond_timedwait(&m_EventCond, &m_EventLock, &timeout);
	pthread_mutex_unlock(&m_EventLock);
#else
	WaitForSingleObject(m_EventCond, 1);
#endif
}

void CFeTimer::sleep()
{
#ifndef WIN32
	usleep(10);
#else
	Sleep(1);
#endif
}

typedef struct _TIMERPARM 
{
	CFeTimer* pTimer;
	WAITORTIMERCALLBACK callback;
	void* parm;
	unsigned long dueTime;
	unsigned int period;
}TIMERPARM;

void CFeTimer::CreateTimer( WAITORTIMERCALLBACK callback , void* parm , unsigned long dueTime , unsigned int period )
{
	unsigned int  uiID = 0;
	TIMERPARM* timeParm = new TIMERPARM;
	timeParm->pTimer = this;
	timeParm->callback = callback;
	timeParm->parm = parm;
	timeParm->dueTime = dueTime;
	timeParm->period = period;
	_beginthreadex( NULL , 0 , procThread , parm , 0 , &uiID );
	
}

unsigned int CFeTimer::procThread( void* parm )
{
	TIMERPARM* timeParm = (TIMERPARM*)parm;
	CFeTimer* pTimer = timeParm->pTimer;
	WAITORTIMERCALLBACK callback = timeParm->callback;
	void* timeparm = timeParm->parm;
	unsigned long dueTime = timeParm->dueTime;
	unsigned int period = timeParm->period;
	delete timeParm;

	while( 1 ){
		uint64_t next;
		CFeTimer::rdtsc(next);
		next += dueTime;
		pTimer->sleepto( next );
		if ( callback )
		{
			callback( timeparm , TRUE );
		}
		if ( period == 0 )
		{
			break;
		}
	}

	return 0;
}
