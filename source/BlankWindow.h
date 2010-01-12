#ifndef _BLANKWINDOW_H_
#define _BLANKWINDOW_H_

#include "Module.h"

class BlankWindow : public Module
{
    public:
        /// module operation
        virtual bool onInit();
        virtual void onCleanup();
    private:
};

#endif // _BLANKWINDOW_H_
