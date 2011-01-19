#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>

#define NUM_CHANNELS 32

namespace CGE
{
    class Sound
    {
        public:
            Sound();
            ~Sound();

            void load(const char* inFile);
            void play();
            void playFromPosition(int inAngle, int inDistance);

        protected:
            Mix_Chunk* mSound;

            static int mChannel;

        private:
    };
}

#endif
