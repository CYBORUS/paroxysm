#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>
#include <cmath>
#include <iostream>

#define NUM_CHANNELS 32
#define MAX_VOLUME 128

namespace CGE
{
    class Sound
    {
        public:
            Sound();
            Sound(const char* inFile);
            ~Sound();

            void load(const char* inFile);
            void play();
            void play(float inVolume);
            void playFromPosition(int inAngle, int inDistance);

            void setVolume(float inVolume);
            void increaseVolume();
            void decreaseVolume();

        private:
            Mix_Chunk* mSound;

            static float mMasterVolume;
            static int mChannel;
    };
}

#endif
