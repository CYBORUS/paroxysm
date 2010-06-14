#ifndef THREAD_H
#define THREAD_H

#include <SDL.h>

class Thread
{
    public:
        Thread();
        virtual ~Thread();

        bool isRunning();
        int getReturnStatus();
        void start(Uint32 inDelay = 0);
        void stop();
        void wait();
        void stopAndWait();
        virtual void run() = 0;

    protected:
        bool stopRequested();
        volatile int mReturnStatus;

    private:
        static int startThread(void* inData);
        volatile bool mRunning;
        volatile bool mStop;
        SDL_Thread* mThread;
};

inline bool Thread::isRunning()
{
    return mRunning;
}

/**
 *  Requests a stop in the thread but continues normal execution without
 *  waiting.
 */
inline void Thread::stop()
{
    mStop = true;
}

/**
 *  Waits for a thread to finish its run() function. WARNING: Needs to be called
 *  on a thread that will end eventually without any kind of stop() call.
 */
inline void Thread::wait()
{
    if (mRunning) SDL_WaitThread(mThread, NULL);
}

/**
 *  Both requests a stop in the thread and waits until the thread is no longer
 *  running. This is just an alternative to having to call stop() and wait()
 *  separately.
 */
inline void Thread::stopAndWait()
{
    stop();
    wait();
}

inline int Thread::getReturnStatus()
{
    return mReturnStatus;
}

inline bool Thread::stopRequested()
{
    return mStop;
}

#endif
