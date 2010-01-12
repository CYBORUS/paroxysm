/**
 *  This file is part of CS490F.
 *
 *  CS490F is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  CS490F is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CS490F.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H


#include "Config.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <iostream>
using namespace std;

class SoundEngine
{
   public:
      static void setupAudio();

      static void playBackgroundMusic();
      static void stopBackgroundMusic();

      static void increaseMusicVolume();
      static void decreaseMusicVolume();

      static void tankFire(int inAngle, int inDistance);

      static void cleanup();

   private:
      static int mAudioRate;
      static Uint16 mAudioFormat;
      static int mNumChannels;
      static int mAudioBufferSize;

      static Mix_Music* mBackgroundMusic;
      static Mix_Chunk* mTankFire;
      static int mChannel;
      static int mMusicVolume;
};

#endif // SOUNDENGINE_H
