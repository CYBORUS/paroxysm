#include <CGE/Engine.h>
#include <CGE/Module.h>

int main(int argc, char** argv)
{
    CGE::Engine e;
    CGE::Module m;
    e.run(&m);
    return 0;
}
