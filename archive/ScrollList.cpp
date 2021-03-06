/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScrollList.h"

ScrollList::ScrollList(string* inInfoPointer, int inID)
{
    mInfoPointer = inInfoPointer;
    mID = inID;
    //mWidth = inWidth;
    //mHeight = inHeight;//(int)(inHeight / LINE_HEIGHT) * LINE_HEIGHT;
    mSelectedItem = 0;
    mUpArrow = NULL;
    mDownArrow = NULL;

    //mSomeRand = mt19937(time(NULL));

    glGenTextures(1, &mNoImage);
    glGenTextures(2, mArrows);

    mMipmapping = Config::get<bool>("mipmapping", false);


    mScrollList = glGenLists(2);
    mSelectionBox = mScrollList + 1;

}

ScrollList::~ScrollList()
{
    glDeleteTextures(1, &mNoImage);

    for (size_t i = 0; i < mImages.size(); ++i)
    {
        GLuint texture = mImages[i];
        glDeleteTextures(1, &texture);
    }

    for (size_t i = 0; i < mText.size(); ++i)
    {
        delete mText[i];
    }

    glDeleteLists(mScrollList, 2);
}

void ScrollList::display()
{
    //glClear(GL_COLOR_BUFFER_BIT);

    //glScissor(mPixelUL.x, mDisplay.y - mPixelLR.y, mPixelLR.x - mPixelUL.x, mPixelLR.y - mPixelUL.y);
    glScissor(mPixelUL.x, mDisplay.y - mPixelLR.y, mPixelLR.x - mPixelUL.x - mPixelArrowWidth, mPixelLR.y - mPixelUL.y);
    if (glIsList(mScrollList))
    {
        glCallList(mSelectionBox);
        glCallList(mScrollList);
    }
}

/***********************************
*   For adding list items with images next to them
************************************/
void ScrollList::addListItem(string inText, Surface inImage)
{
    if (inImage == NULL)
    {
        addListItem(inText);
        return;
    }
    GLuint texture;

    TextPic* text = new TextPic();

    if (!text->loadFont("assets/misc/DejaVuSans.ttf", mFontSize))
    {
        cerr << "failed to load font file: " << TTF_GetError() << endl;
    }

    //glGenTextures(1, &texture);
    text->setText(inText.c_str());

    /*
    if (!DisplayEngine::loadTexture(mText.getTextImage(), texture))
    {
        cerr << "failed to load texture." << endl;
        exit(2);
    }
    */

    Point2D<float> size;

    mText.push_back(text);

    //size.x = mText.getTextSize().x;
    //size.y = mText.getTextSize().y;

    //mList.push_back(texture);
    //mListSizes.push_back(size);
    //mSizeRatios.push_back(mText.getRatio());

    size.x = inImage->w;
    size.y = inImage->h;

    glGenTextures(1, &texture);

    if (!DisplayEngine::loadTexture(inImage, texture))
    {
        cerr << "failed to load texture." << endl;
        exit(2);
    }

    mImages.push_back(texture);
    mImageSizes.push_back(size);
}


/*******************************************
*   For adding list items without images next to them
********************************************/
void ScrollList::addListItem(string inText)
{
    TextPic* text = new TextPic();

    if (!text->loadFont("assets/misc/DejaVuSans.ttf", mFontSize))
    {
        cerr << "failed to load font file: " << TTF_GetError() << endl;
    }

    //glGenTextures(1, &texture);
    text->setText(inText.c_str());

    /*
    if (!DisplayEngine::loadTexture(mText.getTextImage(), texture))
    {
        cerr << "failed to load texture." << endl;
        exit(2);
    }
    */

    Point2D<float> size;

/*
    size.x = mText.getTextSize().x;
    size.y = mText.getTextSize().y;

    mList.push_back(texture);
    mListSizes.push_back(size);
    mSizeRatios.push_back(mText.getRatio());
*/
    size.x = 0;
    size.y = 0;

    mImages.push_back(mNoImage);
    mImageSizes.push_back(size);

    //mListText.push_back(inText);

    mText.push_back(text);
}

void ScrollList::setFontSize(int inSize)
{
    mFontSize = inSize;
}


void ScrollList::setUpArrow(Surface inSurface)
{
    mUpArrow = inSurface;
    DisplayEngine::loadTexture(mUpArrow, mArrows[0]);
}


void ScrollList::setDownArrow(Surface inSurface)
{
    mDownArrow = inSurface;
    DisplayEngine::loadTexture(mDownArrow, mArrows[1]);
}


void ScrollList::onMouseChange(int inX, int inY)
{
    switch(mMouseState)
    {
        case PRESS:
        {
            //check to see that it's within the displayed list
            //(e.g. they didn't click over the arrows
            if (inX < (mPixelLR.x - mPixelArrowWidth))
            {
                Point2D<float> point;
                point.x = 0;
                point.y = mScrollStart;

                int startY = DisplayEngine::convert2DObjectToPixel(point, mDisplay, mRange).y;

                bool found = false;

                for (size_t i = 0; i < mText.size() &&  startY < mPixelLR.y && !found; ++i)
                {
                    if (startY + mText[i]->getTextSize().y >= inY)
                    {
                        found = true;
                        mSelectedItem = i;
                        mSelectedItemStartY = startY;
                    }
                    else
                    {
                        startY += mText[i]->getTextSize().y + 1;
                    }
                }

                if (found)
                {
                    setSelection();
                }
            }
            else if (inY < mPixelUpArrowBottom)
            {
                scrollUp();
            }
            else if (inY > mPixelDownArrowTop)
            {
                scrollDown();
            }
            break;
        }
        default:
        {
        }
    }
}


void ScrollList::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_UP:
        {
            --mSelectedItem;
            if (mSelectedItem < 0)
            {
                mSelectedItem = mText.size() - 1;
            }
            setSelection();
            break;
        }
        case SDLK_DOWN:
        {
            mSelectedItem = (mSelectedItem + 1) % mText.size();
            setSelection();
            break;
        }
        default:
        {
            break;
        }
    }
}


void ScrollList::scrollUp()
{
    mScrollStart -= (mSize.y / 2.0f);

    if (mScrollStart < (mLocation.y + (mSize.y / 2.0f)))
    {
        mScrollStart = mLocation.y + (mSize.y / 2.0f);
    }

    buildScrollList();
    setSelection();
}


void ScrollList::scrollDown()
{
    mScrollStart += (mSize.y / 2.0f);

    buildScrollList();
    setSelection();
}

/*****************************************
*   overloaded to calculate all the internal
*   items in the list
*******************************************/
void ScrollList::preProcessing(float inRange)
{
    mRange = inRange;

    mScrollStart = mLocation.y + (mSize.y / 2.0f);

    buildScrollList();

    setSelection();
}


void ScrollList::buildScrollList()
{
    //set the top left corner in object space
    float startX = mLocation.x - (mSize.x / 2.0f);
    float startY = mScrollStart;

    mPixelArrowWidth = (mUpArrow->w > mDownArrow->w) ? mUpArrow->w : mDownArrow->w;
    Point2D<int> point;

    Point2D<int> center;

    center.x = mDisplay.x / 2;
    center.y = mDisplay.y / 2;

    point.x = center.x + mPixelArrowWidth - 1;
    point.y = 0;

    //mPixelListWidth = mPixelUL.x - mPixelLR.x - arrowWidth;
    //mListWidth = mSize.x - ((float)mPixelArrowWidth / (float)mDisplay.x * mRange * 2.0f);

    //all we want is the width of the arrow in object space
    mArrowWidth = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).x;


    //setup the arrow dimensions
    if (mUpArrow != NULL)
    {
        mPixelUpArrowBottom = mPixelUL.y + mUpArrow->h;
    }

    if (mDownArrow != NULL)
    {
        mPixelDownArrowTop = mPixelLR.y - mDownArrow->h;
    }


/*
    if (glIsList(mScrollList))
    {
        glDeleteLists(mScrollList, 1);
    }
*/
    glNewList(mScrollList, GL_COMPILE);
    {
        float nextTex;
/*
        if (mText.size() > 0)
        {
            //we need to adjust the first listItem to put it back into
            //the box
            point.y = center.y - mText[0].getTextSize().y + 1;
            nextTex = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).y;
            startY -= nextTex;
        }
*/

        glEnable(GL_SCISSOR_TEST);

        for (size_t i = 0; i < mText.size(); ++i)
        {
            float nextX = startX;

            //we want the height for the image and the text to be the same
            point.y = center.y - mText[i]->getTextSize().y;
            float texHeight = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).y;

            //the widths should be different
            float texWidth;

            point.y = center.y - (mText[(i + 1) % mText.size()]->getTextSize().y) - 1;
            nextTex = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).y;

            if (mImages[i] != mNoImage)
            {
                point.x = center.x + (int)mImageSizes[i].x;
                texWidth = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).x;
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, mImages[i]);
                glBegin(GL_QUADS);
                {
                    glTexCoord2f(0, 1);
                    glVertex2f(nextX, startY);
                    glTexCoord2f(1, 1);
                    glVertex2f(nextX + texWidth, startY);
                    glTexCoord2f(1, 0);
                    glVertex2f(nextX + texWidth, startY + texHeight);
                    glTexCoord2i(0, 0);
                    glVertex2f(nextX, startY + texHeight);

                }
                glEnd();
                glDisable(GL_TEXTURE_2D);

                nextX += texWidth;
            }

            /*
            point.x = center.x + mText[i].getTextSize().x;
            texWidth = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).x;

            glBindTexture(GL_TEXTURE_2D, mList[i]);
            glBegin(GL_QUADS);
            {
                glTexCoord2f(0.0f, mSizeRatios[i].y);
                glVertex2f(nextX, startY);
                glTexCoord2f(mSizeRatios[i].x, mSizeRatios[i].y);
                glVertex2f(nextX + texWidth, startY);
                glTexCoord2f(mSizeRatios[i].x, 0);
                glVertex2f(nextX + texWidth, startY + texHeight);
                glTexCoord2i(0, 0);
                glVertex2f(nextX, startY + texHeight);

            }
            glEnd();
            */
            mText[i]->draw(nextX, startY, texHeight);
            startY -= nextTex;
        }

        glDisable(GL_SCISSOR_TEST);

        glEnable(GL_TEXTURE_2D);
        //display the up arrow
        point.x = 0;
        point.y = center.y - mUpArrow->h;

        Point2D<float> arrowHeight = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange);

        startX = mLocation.x + (mSize.x / 2.0) - mArrowWidth;
        startY = mLocation.y + (mSize.y / 2.0) - arrowHeight.y;

        glBindTexture(GL_TEXTURE_2D, mArrows[0]);
        glBegin(GL_QUADS);
        {
            glTexCoord2i(0, 1);
            glVertex2f(startX, startY);
            glTexCoord2i(1, 1);
            glVertex2f(startX + mArrowWidth, startY);
            glTexCoord2i(1, 0);
            glVertex2f(startX + mArrowWidth, startY + arrowHeight.y);
            glTexCoord2i(0, 0);
            glVertex2f(startX, startY + arrowHeight.y);
        }
        glEnd();

        //now display the down arrow
        point.x = 0;
        point.y = center.y - mDownArrow->h;

        arrowHeight = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange);

        startY = mLocation.y - (mSize.y / 2.0);

        glBindTexture(GL_TEXTURE_2D, mArrows[1]);
        glBegin(GL_QUADS);
        {
            glTexCoord2i(0, 1);
            glVertex2f(startX, startY);
            glTexCoord2i(1, 1);
            glVertex2f(startX + mArrowWidth, startY);
            glTexCoord2i(1, 0);
            glVertex2f(startX + mArrowWidth, startY + arrowHeight.y);
            glTexCoord2i(0, 0);
            glVertex2f(startX, startY + arrowHeight.y);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        {
            glVertex2f(mLocation.x - (mSize.x / 2.0), mLocation.y - (mSize.y / 2.0));
            glVertex2f(mLocation.x + (mSize.x / 2.0) - mArrowWidth, mLocation.y - (mSize.y / 2.0));
            glVertex2f(mLocation.x + (mSize.x / 2.0) - mArrowWidth, mLocation.y + (mSize.y / 2.0));
            glVertex2f(mLocation.x - (mSize.x / 2.0), mLocation.y + (mSize.y / 2.0));
        }
        glEnd();


    }
    glEndList();


}


void ScrollList::setSelection()
{
    if (mText.size() < 1)
    {
        return;
    }

    *mInfoPointer = mText[mSelectedItem]->getText();

/*
    if (glIsList(mSelectionBox))
    {
        glDeleteLists(mSelectionBox, 1);
    }
    */

    float texHeight;
    float startX = mLocation.x - (mSize.x / 2.0f);
    float startY = mScrollStart;

    Point2D<int> point;
    for (int i = 0; i < mSelectedItem; ++i)
    {
        point = mText[i]->getTextSize();
        point.y = (mDisplay.y / 2) - point.y - 1;
        texHeight = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).y;
        startY -= texHeight;
    }

    point = mText[mSelectedItem]->getTextSize();
    point.y = (mDisplay.y / 2) - point.y;
    texHeight = DisplayEngine::convert2DPixelToObject(point, mDisplay, mRange).y;

    glNewList(mSelectionBox, GL_COMPILE);
    {
        glEnable(GL_SCISSOR_TEST);

        glPushAttrib(GL_CURRENT_BIT);
        {
            glColor4f(0.0f, 0.2f, 0.4f, 0.8f);
            glRectf(startX + mSize.x, startY - texHeight, startX, startY);
        }
        glPopAttrib();

        glDisable(GL_SCISSOR_TEST);
    }
    glEndList();

}

bool ScrollList::canFocus()
{
    return true;
}
