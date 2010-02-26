#include "Label.h"

Label::Label(const char* inText, int inID)
{
    mID = inID;
    mFontSize = 24;
    mColor[0] = 1.0f;
    mColor[1] = 1.0f;
    mColor[2] = 1.0f;
    mColor[3] = 1.0f;

    glGenTextures(1, &mText);
    mList = glGenLists(1);

    mLabelText = inText;
}

Label::~Label()
{
    glDeleteTextures(1, &mText);
}


void Label::display()
{
    glScissor(mPixelUL.x, mDisplay.y - mPixelLR.y, mPixelLR.x - mPixelUL.x, mPixelLR.y - mPixelUL.y);

    glCallList(mList);


}

void Label::setFontSize(int inFontSize)
{
    mFontSize = inFontSize;

}

void Label::setFontColor(float inRed, float inGreen, float inBlue, float inAlpha)
{
    mColor[0] = inRed;
    mColor[1] = inGreen;
    mColor[2] = inBlue;
    mColor[3] = inAlpha;
}

void Label::setFadeRate(float inFade)
{
    mFadeRate = inFade;
}

void Label::fade()
{
    mColor[3] -= mFadeRate;

    buildLabel();
}


void Label::preProcessing(float inRange)
{
    TextLayer label;

    if (!label.loadFont("assets/misc/DejaVuSans.ttf", mFontSize))
    {
        cerr << "failed to load font file." << endl;
    }

    label.setColor(Uint8(mColor[0] * 255), Uint8(mColor[1] * 255), Uint8(mColor[2] * 255));
    label.setText(mLabelText);

    if (!DisplayEngine::loadTexture(label.getTextImage(), mText))
    {
        cerr << "failed to load texture." << endl;
        exit(2);
    }

    mSizeRatios = label.getRatio();

    Point2D<int> point;

    point.x = (mDisplay.x / 2) + label.getTextSize().x;
    point.y = (mDisplay.y / 2) - label.getTextSize().y;


    mTexDimensions = DisplayEngine::convert2DPixelToObject(point, mDisplay, inRange);

    buildLabel();
}


void Label::buildLabel()
{
    float startX = mLocation.x - (mSize.x / 2.0f);
    float startY = mLocation.y + (mSize.y / 2.0f) - mTexDimensions.y;

    glNewList(mList, GL_COMPILE);
    {
        glEnable(GL_SCISSOR_TEST);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, mText);

        glPushAttrib(GL_CURRENT_BIT);
        glColor4fv(mColor);

        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, mSizeRatios.y);
            glVertex2f(startX, startY);
            glTexCoord2f(mSizeRatios.x, mSizeRatios.y);
            glVertex2f(startX + mTexDimensions.x, startY);
            glTexCoord2f(mSizeRatios.x, 0);
            glVertex2f(startX + mTexDimensions.x, startY + mTexDimensions.y);
            glTexCoord2i(0, 0);
            glVertex2f(startX, startY + mTexDimensions.y);

        }
        glEnd();

        glPopAttrib();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_SCISSOR_TEST);
    }
    glEndList();
}
