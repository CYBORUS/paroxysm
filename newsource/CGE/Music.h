#ifndef MUSIC_H
#define MUSIC_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <sstream>
#include <iostream>

namespace CGE
{
    typedef Mix_Music* BackgroundMusic;

    class Music
    {
        public:
            Music();
            virtual ~Music();

            void increaseVolume();
            void decreaseVolume();
            void setVolume(int inVolume);

            void load(const char* inFile);
            void play();
            void stop();

        protected:

            BackgroundMusic mBackgroundMusic;
            int mMusicVolume;
        private:
    };
}

#endif // MUSIC_H
