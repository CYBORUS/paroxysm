#include "Thread.h"

Thread::Thread() : mReturnStatus(0), mStop(false), mRunning(false)
{
}

Thread::~Thread()
{
}

void Thread::start()
{
    mThread = SDL_CreateThread(startThread, this);
}

int Thread::startThread(void* inData)
{
    Thread* t = (Thread*)inData;
    t->mRunning = true;
    t->run();
    t->mRunning = false;
    return t->mReturnStatus;
}
