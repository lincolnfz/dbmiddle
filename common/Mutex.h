#pragma once


    /**
     * @brief A simple implementation of mutex as a wrapper around a pthread mutex
     * or a win32 critical section.
     *
     * If you locked a mutex you MUST unlock it within the same thread.
     *
     * @author Jakob Schroeter <js@camaya.net>
     * @since 0.9
     */
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
         * Tries to lock the mutex.
         * @return @b True if the attempt was successful, @b false otherwise.
         * @note This function also returns @b true if mutex support is not available, ie. if gloox
         * is compiled without pthreads on non-Windows platforms. Make sure threads/mutexes are available
         * if your code relies on trylock().
         */
        bool trylock();

        /**
         * Releases the mutex.
         */
        void unlock();

      private:
        class MutexImpl;

        Mutex& operator=( const Mutex& );
        MutexImpl* m_mutex;

    };
