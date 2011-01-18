#include "Music.h"

using namespace std;

namespace CGE
{
    Music::Music()
    {
        mMusicVolume = 100;
    }

    Music::~Music()
    {
        if (Mix_PlayingMusic())
        {
            /* Stop the music from playing */
            Mix_HaltMusic();
        }

        Mix_FreeMusic(mBackgroundMusic);
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
        stringstream music;
        music << "data/audio/" << inFile;
        //music << Config::get<string>("background music", inFile);


        /* Actually loads up the music */
        if ((mBackgroundMusic = Mix_LoadMUS(music.str().c_str())) == NULL)
        {
            cerr << Mix_GetError()  << " " << music.str() << endl;
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
