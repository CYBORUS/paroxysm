#include "Sound.h"

#include <sstream>
#include <iostream>
using namespace std;

namespace CGE
{
    int Sound::mChannel = 1;


    Sound::Sound() : mSound(NULL)
    {
    }

    Sound::~Sound()
    {
        Mix_FreeChunk(mSound); // safe to call on NULL value
    }

    void Sound::load(const char* inFile)
    {
        if ((mSound = Mix_LoadWAV(inFile)) == NULL)
        {
            cerr << "something wrong: " << Mix_GetError() << endl;
        }
    }

    void Sound::play()
    {
        if (Mix_PlayChannel(mChannel, mSound, 0) == -1)
        {
            cerr << Mix_GetError() << endl;
        }
        mChannel = (mChannel + 1) % NUM_CHANNELS;
    }


    void Sound::playFromPosition(int inAngle, int inDistance)
    {
        if (!Mix_SetPosition(mChannel, inAngle, inDistance))
        {
            cerr << Mix_GetError() << endl;
        }
        if (Mix_PlayChannel(mChannel, mSound, 0) == -1)
        {
            cerr << Mix_GetError() << endl;
        }
        mChannel = (mChannel + 1) % NUM_CHANNELS;
    }

}
