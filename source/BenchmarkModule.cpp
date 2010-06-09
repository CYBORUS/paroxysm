#include "BenchmarkModule.h"

BenchmarkModule::BenchmarkModule()
{
}

BenchmarkModule::~BenchmarkModule()
{
}

bool BenchmarkModule::onLoad()
{
    BenchmarkOBJ bobj;
    cout << "loading 5 tanks" << endl;
    bobj.start();
    bobj.stopAndWait();
    cout << "done!" << endl;
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
