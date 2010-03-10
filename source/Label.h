#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

#include "OGL.h"
#include "TextPic.h"
#include "Vector3D.h"


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
        void setText(const char* inText);

        void setFadeRate(float inFade);
        void fade();

        void display();

        void preProcessing(float inRange);
    protected:
        void buildLabel();

        Point2D<float> mTexDimensions;
        Point2D<float> mSizeRatios;

        float mFadeRate;

        float mRange;
        TextPic* mText;

        //GLuint mText;
        GLuint mList;

        int mFontSize;
        Vector3D<float> mColor;

        //string mLabelText;
};

#endif
