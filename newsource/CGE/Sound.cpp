#include "Sound.h"
#include "Exception.h"

namespace CGE
{
    int Sound::mChannel = 1;

    Sound::Sound() : mSound(NULL)
    {
    }

    Sound::Sound(const char* inFile) : mSound(NULL)
    {
        load(inFile);
    }

    Sound::~Sound()
    {
        Mix_FreeChunk(mSound); // safe to call on NULL value
    }

    void Sound::load(const char* inFile)
    {
        Mix_Volume(-1, 12);
        static const char* functionName = "Sound::load";

        if (!inFile || !*inFile)
            throw Exception(functionName, "invalid file");

        if ((mSound = Mix_LoadWAV(inFile)) == NULL)
            throw Exception(functionName, Mix_GetError());
    }

    void Sound::play()
    {
        static const char* functionName = "Sound::play";

        // We only want to throw an exception if there is a valid sound to be
        // played.
        if (!mSound) return;

        if (Mix_PlayChannel(mChannel, mSound, 0) == -1)
            throw Exception(functionName, Mix_GetError());

        mChannel = (mChannel + 1) % NUM_CHANNELS;
    }

    void Sound::play(int inVolume)
    {

    }


    void Sound::playFromPosition(int inAngle, int inDistance)
    {
        static const char* functionName = "Sound::playFromPosition";
        if (!mSound) return;

        if (!Mix_SetPosition(mChannel, inAngle, inDistance))
            throw Exception(functionName, Mix_GetError());

        if (Mix_PlayChannel(mChannel, mSound, 0) == -1)
            throw Exception(functionName, Mix_GetError());

        mChannel = (mChannel + 1) % NUM_CHANNELS;
    }

}
