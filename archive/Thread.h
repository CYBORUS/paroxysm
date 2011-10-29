/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef THREAD_H
#define THREAD_H

#include <SDL.h>

class Thread
{
    public:
        Thread();
        virtual ~Thread();

        void start(Uint32 inDelay = 0);
        virtual void run() = 0;
        inline bool isRunning();
        inline void stop();
        inline void wait();
        inline void stopAndWait();

    protected:
        inline bool stopRequested();

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
 *  on a thread that will end eventually without any kind of stop() call (or
 *  where you have called it prior).
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

inline bool Thread::stopRequested()
{
    return mStop;
}

#endif
