#ifndef MUSIC_H
#define MUSIC_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <sstream>
#include <iostream>

namespace CGE
{
    class Music
    {
        public:
            Music();
            ~Music();

            void increaseVolume();
            void decreaseVolume();
            void setVolume(int inVolume);

            void load(const char* inFile);
            void play();
            void stop();

        private:
            Mix_Music* mBackgroundMusic;
            int mMusicVolume;
    };
}

#endif
