#ifndef TEXTURE_H
#define TEXTURE_H

#include "Graphics.h"

namespace CGE
{
    class Texture
    {
        public:
            Texture();
            virtual ~Texture();

            inline void bind()
            {
                glBindBuffer(mTarget, mHandle);
            }

        protected:
            GLtexture mHandle;
            GLenum mTarget;

    };
}

#endif
