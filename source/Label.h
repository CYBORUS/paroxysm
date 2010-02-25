#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

#include <SDL_opengl.h>
#include "TextLayer.h"


#include <string>
#include <iostream>
#include <time.h>
#include <vector>
using namespace std;

class Label : public Widget
{
    public:
        Label(const char* inText, int inID);
        virtual ~Label();

        void setFontSize(int inFontSize);
        void setFontColor(float inRed, float inGreen, float inBlue, float inAlpha);

        void display();

        void preProcessing(float inRange);
    protected:
        GLuint mText;
        GLuint mList;

        int mFontSize;
        float mColor[4];

        string mLabelText;
};

#endif // LABEL_H
