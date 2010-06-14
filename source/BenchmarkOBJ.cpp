#include "BenchmarkOBJ.h"
#include "ModelStack.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

BenchmarkOBJ::BenchmarkOBJ()
{
}

BenchmarkOBJ::~BenchmarkOBJ()
{
}

void BenchmarkOBJ::run()
{
    const string message("loading tank #");
    for (int i = 1; i < 6; ++i)
    {
        stringstream a;
        a << i << "...";
        cout << a.str();
        cout.flush();
        stringstream b;
        b << "test" << i << ".obj";
        //ModelStack::load(b.str().c_str());
        ModelStack::load("bradley_body.c3m");
        cout << "done!" << endl;
    }
    ModelStack::unloadAll();
}
