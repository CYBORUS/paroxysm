#ifndef TOOLS_H
#define TOOLS_H

namespace CGE
{
    char* fileToBuffer(const char* inFile);

    template<class T>
    T linearInterpolate(const T& inMin, const T& inMax, double inT)
    {
        return (inMin * (1.0 - inT)) + (inMax * inT);
    }

}

#endif

