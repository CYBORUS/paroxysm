#include "ScrollList.h"

ScrollList::ScrollList(string* inInfoPointer, int inID)
{
    mInfoPointer = inInfoPointer;
    mID = inID;

    mSomeRand = mt19937(time(NULL));
}

ScrollList::~ScrollList()
{
    //dtor
}


void ScrollList::display()
{
}

/***********************************
*   For adding list items with images next to them
************************************/
void ScrollList::addListItem(string inText, Surface inImage)
{
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

    //someRand();
    //typedef boost::uniform_int<int> dist_t;
    typedef boost::normal_distribution<double> dist_t;
    //typedef uniform_real<double> dist_t;
    dist_t dist(100, 10);
    boost::variate_generator<boost::mt19937&, dist_t> rand_ (mSomeRand, dist);

    for (int i = 0; i < 20; ++i)
    {
        cout << "random number? " << (int)rand_() << endl;
    }
    cout << endl << endl;
}


/*******************************************
*   For adding list items without images next to them
********************************************/
void ScrollList::addListItem(string inText)
{
}

void ScrollList::onMouseChange(float inX, float inY)
{
}
