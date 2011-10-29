#ifndef BENCHMARKOBJ_H
#define BENCHMARKOBJ_H

#include "Thread.h"

class BenchmarkOBJ : public Thread
{
    public:
        BenchmarkOBJ();
        virtual ~BenchmarkOBJ();

        virtual void run();
};

#endif
