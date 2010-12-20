#include <CGE/Engine.h>
#include "TestModule.h"

int main(int argc, char** argv)
{
    CGE::Engine e;
    {
        TestModule tm;
        e.run(&tm);
    }
    return 0;
}
