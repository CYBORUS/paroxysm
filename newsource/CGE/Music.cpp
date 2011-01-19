#include "Music.h"

using namespace std;

namespace CGE
{
    Music::Music() : mBackgroundMusic(NULL)
    {
        mMusicVolume = 20;
        Mix_VolumeMusic(mMusicVolume);
    }

    Music::~Music()
    {
        if (Mix_PlayingMusic())
        {
            /* Stop the music from playing */
            Mix_HaltMusic();
        }

        Mix_FreeMusic(mBackgroundMusic); // safe to call on NULL value
    }


    void Music::increaseVolume()
    {
        mMusicVolume += 5;

        if (mMusicVolume > 128)
        {
            mMusicVolume = 128;
        }
        Mix_VolumeMusic(mMusicVolume);
    }


    void Music::decreaseVolume()
    {
        mMusicVolume -= 5;

        if (mMusicVolume < 0)
        {
            mMusicVolume = 0;
        }
        Mix_VolumeMusic(mMusicVolume);
    }

    void Music::setVolume(int inVolume)
    {
        mMusicVolume = inVolume;

        if (mMusicVolume > 128)
        {
            mMusicVolume = 128;
        }
        else if (mMusicVolume < 0)
        {
            mMusicVolume = 0;
        }

        Mix_VolumeMusic(mMusicVolume);
    }


    void Music::load(const char* inFile)
    {
        if ((mBackgroundMusic = Mix_LoadMUS(inFile)) == NULL)
        {
            cerr << Mix_GetError()  << " " << inFile << endl;
        }
    }

    void Music::play()
    {
        if (!Mix_PlayingMusic())
        {
            /* This begins playing the music - the first argument is a
               pointer to Mix_Music structure, and the second is how many
               times you want it to loop (use -1 for infinite, and 0 to
               have it just play once) */
            if ((Mix_PlayMusic(mBackgroundMusic, -1)) == -1)
            {
                cerr << Mix_GetError() << endl;
            }
        }
    }

    void Music::stop()
    {
        if (Mix_PlayingMusic())
        {
            /* Stop the music from playing */
            Mix_HaltMusic();
        }
    }

}
