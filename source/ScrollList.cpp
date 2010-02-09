#include "ScrollList.h"

ScrollList::ScrollList(string* inInfoPointer, float inWidth, float inHeight, int inID)
{
    mInfoPointer = inInfoPointer;
    mID = inID;
    mWidth = inWidth;
    mHeight = (int)(inHeight / LINE_HEIGHT) * LINE_HEIGHT;

    mSomeRand = mt19937(time(NULL));

    glGenTextures(1, &mNoImage);
}

ScrollList::~ScrollList()
{
    //dtor
}


void ScrollList::display()
{
    glDeleteTextures(1, &mNoImage);

    for (unsigned int i = 0; i < mImages.size(); ++i)
    {
        GLuint texture = mImages[i];
        glDeleteTextures(1, &texture);
    }
}

/***********************************
*   For adding list items with images next to them
************************************/
void ScrollList::addListItem(string inText, Surface inImage)
{
    GLuint texture;

    glGenTextures(1, &texture);

    mList.push_back(texture);
    mImages.push_back(texture);
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
    mList.push_back(texture);
    mImages.push_back(mNoImage);
}

void ScrollList::onMouseChange(float inX, float inY)
{
}
