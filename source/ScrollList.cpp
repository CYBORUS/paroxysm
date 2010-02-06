#include "ScrollList.h"

ScrollList::ScrollList(string* inInfoPointer, int inID)
{
    mInfoPointer = inInfoPointer;
    mID = inID;
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
    string temp = ".";

    if (is_directory(temp))
    {
        for (directory_iterator itr(temp); itr != directory_iterator(); ++itr)
        {
            cout << itr->path().filename() << ' ';
            if (is_regular_file(itr->status()))
            {
                cout << " [" << file_size(itr->path()) << ']' << endl;
            }
        }
    }
    else
    {
        cout << (exists(temp) ? "Found: " : "Not found: ") << temp << endl;
    }
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
