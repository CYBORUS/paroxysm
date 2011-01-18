#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>

#define NUM_CHANNELS 32

namespace CGE
{
    typedef Mix_Chunk* SoundEffect;

    class Sound
    {
        public:
            Sound();
            virtual ~Sound();

            void load(const char* inFile);
            void play();

        protected:
            SoundEffect mSound;

            static int mChannel;

        private:
    };
}

#endif // SOUND_H
