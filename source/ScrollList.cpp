#include "ScrollList.h"

ScrollList::ScrollList(string* inInfoPointer, float inWidth, float inHeight, int inID)
{
    mInfoPointer = inInfoPointer;
    mID = inID;
    mWidth = inWidth;
    mHeight = inHeight;//(int)(inHeight / LINE_HEIGHT) * LINE_HEIGHT;
    mSelectedItem = 0;
    mUpArrow = NULL;
    mDownArrow = NULL;

    if (!mText.loadFont("assets/misc/DejaVuSans.ttf", 24))
    {
        cerr << "failed to load font file." << endl;
    }

    mSomeRand = mt19937(time(NULL));

    glGenTextures(1, &mNoImage);
    glGenTextures(2, mArrows);

    mMipmapping = Config::get<bool>("mipmapping", false);


    mScrollList = glGenLists(2);
    mSelectionBox = mScrollList + 1;

}

ScrollList::~ScrollList()
{
    glDeleteTextures(1, &mNoImage);

    for (unsigned int i = 0; i < mImages.size(); ++i)
    {
        GLuint texture = mImages[i];
        glDeleteTextures(1, &texture);
    }

}

void ScrollList::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

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

    glGenTextures(1, &texture);
    mText.setText(inText.c_str());
    if (!DisplayEngine::loadTexture(mText.getTextImage(), texture))
    {
        cerr << "failed to load texture." << endl;
        exit(2);
    }

    Point2D<float> size;

    size.x = mText.getTextImage()->w;
    size.y = mText.getTextImage()->h;

    mList.push_back(texture);
    mListSizes.push_back(size);

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

    mListText.push_back(inText);


/*
    string temp = ".";

    if (is_directory(temp))
    {
        for (directory_iterator itr(temp); itr != directory_iterator(); ++itr)
        {
            cout << itr->path().filename() << ' ';
            if (is_regular_file(itr->status()))
            {
                cout << " [" << file_size(itr->path()) << ']';
            }
            cout << endl;
        }
    }
    else
    {
        cout << (exists(temp) ? "Found: " : "Not found: ") << temp << endl;
    }
*/
/*
    random_device device;
    double ent = device.entropy();
    unsigned int number = device();
    cout << "random number? " << number << " entropy: " << ent << endl;
    */
/*
    //someRand();
    //typedef boost::uniform_int<int> dist_t;
    typedef boost::normal_distribution<double> dist_t;
    //typedef uniform_real<double> dist_t;
    dist_t dist(100, 20);
    boost::variate_generator<boost::mt19937&, dist_t> rand_ (mSomeRand, dist);

    for (int i = 0; i < 20; ++i)
    {
        cout << "random number? " << (int)rand_() << endl;
    }
    cout << endl << endl;
    */
}


/*******************************************
*   For adding list items without images next to them
********************************************/
void ScrollList::addListItem(string inText)
{
    GLuint texture;

    glGenTextures(1, &texture);
    mText.setText(inText.c_str());
    if (!DisplayEngine::loadTexture(mText.getTextImage(), texture))
    {
        cerr << "failed to load texture." << endl;
        exit(2);
    }

    Point2D<float> size;

    size.x = mText.getTextImage()->w;
    size.y = mText.getTextImage()->h;

    mList.push_back(texture);
    mListSizes.push_back(size);

    size.x = 0;
    size.y = 0;

    mImages.push_back(mNoImage);
    mImageSizes.push_back(size);

    mListText.push_back(inText);
}

void ScrollList::setFontSize(int inSize)
{
    if (!mText.loadFont("assets/misc/DejaVuSans.ttf", inSize))
    {
        cerr << "failed to load font file." << endl;
    }
}


void ScrollList::setUpArrow(Surface inSurface)
{
    mUpArrow = inSurface;
    mPixelUpArrowBottom = mPixelUL.y + inSurface->h;
    DisplayEngine::loadTexture(mUpArrow, mArrows[0]);
}


void ScrollList::setDownArrow(Surface inSurface)
{
    mDownArrow = inSurface;
    mPixelDownArrowTop = mPixelLR.y - inSurface->h;
    DisplayEngine::loadTexture(mDownArrow, mArrows[1]);
}


void ScrollList::onMouseChange(int inX, int inY)
{
    switch(mMouseState)
    {
        case PRESS:
        {
            int startY = mPixelUL.y;
            bool found = false;

            for (int i = 0; startY < mPixelLR.y && !found; ++i)
            {
                if (startY + mListSizes[i].y >= inY)
                {
                    found = true;
                    mSelectedItem = i;
                    mSelectedItemStartY = startY;
                }
                else
                {
                    startY += (int)mListSizes[i].y;
                }
            }

            if (found)
            {
                setSelection();
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
                mSelectedItem = mList.size() - 1;
            }
            setSelection();
            break;
        }
        case SDLK_DOWN:
        {
            mSelectedItem = (mSelectedItem + 1) % mList.size();
            setSelection();
            break;
        }
        default:
        {
            break;
        }
    }
}


/*****************************************
*   overloaded to calculate all the internal
*   items in the list
*******************************************/
void ScrollList::findPixels(const Point2D<int>& inDisplay, float inRange)
{
    mDisplay = inDisplay;
    mRange = inRange;

    Point2D<int> center;
    center.x = inDisplay.x / 2;
    center.y = inDisplay.y / 2;

    float ratio = float(center.y) / inRange;

    mPixelUL.x = center.x + int((mLocation.x - (mSize.x / 2.0f)) * ratio);
    mPixelUL.y = center.y - int((mLocation.y + (mSize.y / 2.0f)) * ratio);
    mPixelLR.x = mPixelUL.x + int(mSize.x * ratio);
    mPixelLR.y = mPixelUL.y + int(mSize.y * ratio);

    buildScrollList();

    setSelection();
}


void ScrollList::buildScrollList()
{
    //set the top left corner in object space
    float startX = mLocation.x - (mSize.x / 2.0f);
    float startY = mLocation.y + (mSize.y / 2.0f);

    //need to find out how to deal with case that y > x
    float ratio = (float)mDisplay.x / (float)mDisplay.y;


    mPixelArrowWidth = (mUpArrow->w > mDownArrow->w) ? mUpArrow->w : mDownArrow->w;

    //mPixelListWidth = mPixelUL.x - mPixelLR.x - arrowWidth;
    //mListWidth = mSize.x - ((float)mPixelArrowWidth / (float)mDisplay.x * mRange * 2.0f);
    mArrowWidth = (float)mPixelArrowWidth / (float)mDisplay.x * mRange * 2.0f * ratio;
    //cerr << "mSize.x: " << mSize.x << " mListWidth: " << mListWidth << endl;

    if (glIsList(mScrollList))
    {
        glDeleteLists(mScrollList, 1);
    }

    glNewList(mScrollList, GL_COMPILE);
    {
        //we need to adjust the first listItem to put it back into
        //the box
        float nextTex = ((mListSizes[0].y + 1) * mRange * 2) / mDisplay.y;
        startY -= nextTex;

        glEnable(GL_SCISSOR_TEST);


        glEnable(GL_TEXTURE_2D);
        for (unsigned int i = 0; i < mList.size(); ++i)
        {
            float nextX = startX;

            //we want the height for the image and the text to be the same
            float texHeight = (mListSizes[i].y * mRange * 2) / (float)mDisplay.y;

            //the widths should be different
            float texWidth;

            nextTex = ((mListSizes[(i + 1) % mList.size()].y + 1) * mRange * 2) / mDisplay.y;

            if (mImages[i] != mNoImage)
            {
                texWidth = (mImageSizes[i].x * mRange * 2 * ratio) / mDisplay.x;
                glBindTexture(GL_TEXTURE_2D, mImages[i]);
                glBegin(GL_QUADS);
                {
                    glTexCoord2i(0, 1);
                    glVertex2f(nextX, startY);
                    glTexCoord2i(1, 1);
                    glVertex2f(nextX + texWidth, startY);
                    glTexCoord2i(1, 0);
                    glVertex2f(nextX + texWidth, startY + texHeight);
                    glTexCoord2i(0, 0);
                    glVertex2f(nextX, startY + texHeight);

                }
                glEnd();

                nextX += texWidth;
            }

            texWidth = (mListSizes[i].x * mRange * 2 * ratio) / mDisplay.x;

            glBindTexture(GL_TEXTURE_2D, mList[i]);
            glBegin(GL_QUADS);
            {
                glTexCoord2i(0, 1);
                glVertex2f(nextX, startY);
                glTexCoord2i(1, 1);
                glVertex2f(nextX + texWidth, startY);
                glTexCoord2i(1, 0);
                glVertex2f(nextX + texWidth, startY + texHeight);
                glTexCoord2i(0, 0);
                glVertex2f(nextX, startY + texHeight);

            }
            glEnd();
            startY -= nextTex;
        }

        glDisable(GL_TEXTURE_2D);

        glDisable(GL_SCISSOR_TEST);

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
    *mInfoPointer = mListText[mSelectedItem];

    if (glIsList(mSelectionBox))
    {
        glDeleteLists(mSelectionBox, 1);
    }

    float texHeight;
    float startX = mLocation.x - (mSize.x / 2.0f);
    float startY = mLocation.y + (mSize.y / 2.0f);

    for (int i = 0; i < mSelectedItem; ++i)
    {
        texHeight = ((mListSizes[i].y + 1) * mRange * 2) / mDisplay.y;
        startY -= texHeight;
    }
    texHeight = (mListSizes[mSelectedItem].y * mRange * 2) / mDisplay.y;

    glNewList(mSelectionBox, GL_COMPILE);
    {
        glEnable(GL_SCISSOR_TEST);

        glPushAttrib(GL_CURRENT_BIT);
        {
            glColor4f(0.0f, 0.2f, 0.4f, 0.8f);
            glRectf(startX, startY, startX + mSize.x, startY - texHeight);
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
