#pragma once


class Mutex
{
public:
	/**
	* Contructs a new simple mutex.
	*/
	Mutex();

	/**
	* Destructor
	*/
	~Mutex();

	/**
	* Locks the mutex.
	*/
	void lock();

	/**
	* Releases the mutex.
	*/
	void unlock();

private:
	class MutexImpl; //Ƕ����

	Mutex& operator=( const Mutex& );
	MutexImpl* m_mutex;
};
