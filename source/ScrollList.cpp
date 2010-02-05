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
