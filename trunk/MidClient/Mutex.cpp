#include "StdAfx.h"
#include ".\mutex.h"

class Mutex::MutexImpl
{
public:
	MutexImpl();
	~MutexImpl();
	void lock();
	void unlock();
private:
	MutexImpl( const MutexImpl& );
	MutexImpl& operator=( const MutexImpl& );

	CRITICAL_SECTION m_cs;
};

Mutex::MutexImpl::MutexImpl()
{
	InitializeCriticalSection( &m_cs );
}

Mutex::MutexImpl::~MutexImpl()
{
	DeleteCriticalSection( &m_cs );
}

void Mutex::MutexImpl::lock()
{
	EnterCriticalSection( &m_cs );
}

void Mutex::MutexImpl::unlock()
{
	LeaveCriticalSection( &m_cs );
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

void Mutex::unlock()
{
	m_mutex->unlock();
}