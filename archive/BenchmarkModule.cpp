#include "BenchmarkModule.h"

#include "ModelStack.h"

BenchmarkModule::BenchmarkModule()
{
}

BenchmarkModule::~BenchmarkModule()
{
}

bool BenchmarkModule::onLoad()
{
    cout << "OBJ benchmark" << endl;
    Uint32 start = SDL_GetTicks();
    for (int i = 1; i < 6; ++i)
    {
        Uint32 start2 = SDL_GetTicks();
        stringstream a;
        a << i << "... ";
        cout << "loading tank #" << a.str();
        cout.flush();
        stringstream b;
        b << "test" << i << ".obj";
        ModelStack::load(b.str().c_str());
        cout << (SDL_GetTicks() - start2) << " ms" << endl;
    }
    cout << (SDL_GetTicks() - start) << " ms total" << endl;

    cout << "\nC3M benchmark" << endl;
    start = SDL_GetTicks();
    for (int i = 1; i < 6; ++i)
    {
        Uint32 start2 = SDL_GetTicks();
        stringstream a;
        a << i << "... ";
        cout << "loading tank #" << a.str();
        cout.flush();
        stringstream b;
        b << "test" << i << ".c3m";
        ModelStack::load(b.str().c_str());
        cout << (SDL_GetTicks() - start2) << " ms" << endl;
    }
    cout << (SDL_GetTicks() - start) << " ms total" << endl;

    ModelStack::unloadAll();

    return true;
}

void BenchmarkModule::onUnload()
{
}

void BenchmarkModule::onOpen()
{

}

void BenchmarkModule::onClose()
{

}

void BenchmarkModule::onFrame()
{

}

void BenchmarkModule::onLoop()
{
}
