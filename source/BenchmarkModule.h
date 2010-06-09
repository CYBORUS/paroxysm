#ifndef BENCHMARKMODULE_H
#define BENCHMARKMODULE_H

#include "Module.h"
#include "Label.h"
#include "BenchmarkOBJ.h"

class BenchmarkModule : public Module
{
    public:
        BenchmarkModule();
        virtual ~BenchmarkModule();

        /// module operation
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:
        Label* mTimer;
        Label* mStatus;
};

#endif
