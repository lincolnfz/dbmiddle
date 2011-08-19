#include "StdAfx.h"
#include ".\mutex.h"

#if defined( _WIN32 ) && !defined( __SYMBIAN32__ )
# include <windows.h>
#endif

#ifdef _WIN32_WCE
# include <winbase.h>
#endif

#ifdef HAVE_PTHREAD
# include <pthread.h>
#endif

class Mutex::MutexImpl
{
public:
	MutexImpl();
	~MutexImpl();
	void lock();
	bool trylock();
	void unlock();
private:
	MutexImpl( const MutexImpl& );
	MutexImpl& operator=( const MutexImpl& );

#if defined( _WIN32 ) && !defined( __SYMBIAN32__ )
	CRITICAL_SECTION m_cs;
#elif defined( HAVE_PTHREAD )
	pthread_mutex_t m_mutex;
#endif

};

Mutex::MutexImpl::MutexImpl()
{
#if defined( _WIN32 ) && !defined( __SYMBIAN32__ )
	InitializeCriticalSection( &m_cs );
#elif defined( HAVE_PTHREAD )
	pthread_mutex_init( &m_mutex, 0 );
#endif
}

Mutex::MutexImpl::~MutexImpl()
{
#if defined( _WIN32 ) && !defined( __SYMBIAN32__ )
	DeleteCriticalSection( &m_cs );
#elif defined( HAVE_PTHREAD )
	pthread_mutex_destroy( &m_mutex );
#endif
}

void Mutex::MutexImpl::lock()
{
#if defined( _WIN32 ) && !defined( __SYMBIAN32__ )
	EnterCriticalSection( &m_cs );
#elif defined( HAVE_PTHREAD )
	pthread_mutex_lock( &m_mutex );
#endif
}

bool Mutex::MutexImpl::trylock()
{
#if defined( _WIN32 ) && !defined( __SYMBIAN32__ )
	return TryEnterCriticalSection( &m_cs ) ? true : false;
#elif defined( HAVE_PTHREAD )
	return !( pthread_mutex_trylock( &m_mutex ) );
#else
	return true;
#endif
}

void Mutex::MutexImpl::unlock()
{
#if defined( _WIN32 ) && !defined( __SYMBIAN32__ )
	LeaveCriticalSection( &m_cs );
#elif defined( HAVE_PTHREAD )
	pthread_mutex_unlock( &m_mutex );
#endif
}

Mutex::Mutex()
	: m_mutex( new MutexImpl() )
{
}

Mutex::~Mutex()
{
	delete m_mutex;
}

void Mutex::lock()
{
	m_mutex->lock();
}

bool Mutex::trylock()
{
	return m_mutex->trylock();
}

void Mutex::unlock()
{
	m_mutex->unlock();
}