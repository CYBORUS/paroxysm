#include "Thread.h"

Thread::Thread() : mReturnStatus(0), mRunning(false), mStop(false)
{
}

Thread::~Thread()
{
}

void Thread::start(Uint32 inDelay)
{
    mThread = SDL_CreateThread(startThread, this);
    SDL_Delay(inDelay);
}

int Thread::startThread(void* inData)
{
    Thread* t = (Thread*)inData;
    t->mRunning = true;
    t->run();
    t->mRunning = false;
    return t->mReturnStatus;
}
