#include "ScrollList.h"

ScrollList::ScrollList(string* inInfoPointer, float inWidth, float inHeight, int inID)
{
    mInfoPointer = inInfoPointer;
    mID = inID;
    mWidth = inWidth;
    mHeight = (int)(inHeight / LINE_HEIGHT) * LINE_HEIGHT;

    if (!mText.loadFont("assets/misc/DejaVuSans.ttf", 24))
    {
        cerr << "failed to load font file." << endl;
    }

    mSomeRand = mt19937(time(NULL));

    glGenTextures(1, &mNoImage);

    mMipmapping = Config::get<bool>("mipmapping", false);

/*
    GLint nOfColors = DisplayEngine::mDisplay->format->BytesPerPixel;
    if (nOfColors == 4)
    {
        if (DisplayEngine::mDisplay->format->Rmask == 0x000000ff)
            mFormat = GL_RGBA;
        else
            mFormat = GL_BGRA;
    }
    else if (nOfColors == 3)
    {
        if (DisplayEngine::mDisplay->format->Rmask == 0x000000ff)
            mFormat = GL_RGB;
        else
            mFormat = GL_BGR;
    }
    else
    {
        cerr << "failed to load texture -- not true color\n";
    }
*/
    mDisplayList = glGenLists(1);

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

bool something = true;
void ScrollList::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (glIsList(mDisplayList))
    {
        glCallList(mDisplayList);
    }
    else
    {
        cerr << "it's not a list" << endl;
    }


/*
    glPolygonMode(GL_FRONT, GL_LINE);

    glRectf(mLocation.x - (mSize.x / 2.0), mLocation.y - (mSize.y / 2.0),
            mLocation.x + (mSize.x / 2.0), mLocation.y + (mSize.y / 2.0));
*/
    if (something)
    {
        cerr << "size: " << mSize.x << "," << mSize.y << " \nlocation: " <<
                mLocation.x << "," << mLocation.y << endl;
        something = false;

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
}


bool ScrollList::loadTextureString(Surface inSurface, GLuint inTexture)
{
    if (inSurface == NULL) return false;
    glBindTexture(GL_TEXTURE_2D, inTexture);

    if (mMipmapping)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
    }

    GLint nOfColors = inSurface->format->BytesPerPixel;
    GLenum tFormat = GL_RGBA;
    if (nOfColors == 4)
    {
        if (inSurface->format->Rmask == 0x000000ff)
            tFormat = GL_RGBA;
        else
            tFormat = GL_BGRA;
    }
    else if (nOfColors == 3)
    {
        if (inSurface->format->Rmask == 0x000000ff)
            tFormat = GL_RGB;
        else
            tFormat = GL_BGR;
    }
    else
    {
        cerr << "failed to load texture -- not true color\n";
        SDL_FreeSurface(inSurface);
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, inSurface->w, inSurface->h,
        0, tFormat, GL_UNSIGNED_BYTE, inSurface->pixels);

    if (mMipmapping)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    return true;

}

void ScrollList::onMouseChange(float inX, float inY)
{
}

/*****************************************
*   overloaded to calculate all the internal
*   items in the list
*******************************************/
void ScrollList::findPixels(const Point2D<int>& inDisplay, float inRange)
{
    Point2D<int> center;
    center.x = inDisplay.x / 2;
    center.y = inDisplay.y / 2;

    float ratio = float(center.y) / inRange;

    mPixelUL.x = center.x + int((mLocation.x - (mSize.x / 2.0f)) * ratio);
    mPixelUL.y = center.y - int((mLocation.y + (mSize.y / 2.0f)) * ratio);
    mPixelLR.x = mPixelUL.x + int(mSize.x * ratio);
    mPixelLR.y = mPixelUL.y + int(mSize.y * ratio);

    float startX = mLocation.x - (mSize.x / 2.0f);
    float startY = mLocation.y + (mSize.y / 2.0f);

    if (glIsList(mDisplayList))
    {
        glDeleteLists(1, mDisplayList);
    }

    glNewList(mDisplayList, GL_COMPILE);
    {
        //we need to adjust the first listItem to put it back into
        //the box
        float nextTex = ((mListSizes[0].y + 1) * inRange * 2) / inDisplay.y;
        startY -= nextTex;

        glEnable(GL_TEXTURE_2D);
        for (unsigned int i = 0; i < mList.size(); ++i)
        {
            float nextX = startX;

            //we want the height for the image and the text to be the same
            float texHeight = (mListSizes[i].y * inRange * 2) / inDisplay.y;

            //the widths should be different
            float texWidth;

            nextTex = ((mListSizes[i].y + 1) * inRange * 2) / inDisplay.y;

            if (mImages[i] != mNoImage)
            {
                //texHeight = (mImageSizes[i].y * inRange * 2) / inDisplay.y;
                texWidth = (mImageSizes[i].x * inRange * 2) / inDisplay.x;
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

            texWidth = (mListSizes[i].x * inRange * 2) / inDisplay.x;

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

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_STRIP);
        {
            glVertex2f(mLocation.x - (mSize.x / 2.0), mLocation.y - (mSize.y / 2.0));
            glVertex2f(mLocation.x + (mSize.x / 2.0), mLocation.y - (mSize.y / 2.0));
            glVertex2f(mLocation.x + (mSize.x / 2.0), mLocation.y + (mSize.y / 2.0));
            glVertex2f(mLocation.x - (mSize.x / 2.0), mLocation.y + (mSize.y / 2.0));
            glVertex2f(mLocation.x - (mSize.x / 2.0), mLocation.y - (mSize.y / 2.0));
        }
        glEnd();


    }
    glEndList();

}
