#ifndef IMAGE_H
#define IMAGE_H

#include "Graphics.h"

namespace CGE
{
    class Image
    {
        public:
            Image(const char* inFile = NULL);
            Image(int inWidth, int inHeight);
            Image(const Image& inImage);
            ~Image();

            inline bool isValid() { return !!mData; }
            void loadFile(const char* inFile);

            void blitOnto(Image& inImage) const;
            void loadIntoTexture(GLenum inTarget = GL_TEXTURE_2D) const;
            void setAsWindowIcon() const;
            int width() const;
            int height() const;

        private:
            void destroy();
            void findFormat();

            Surface mData;
            int mColors;
            GLenum mFormat;
    };
}

#endif
