#ifndef SCROLLLIST_H
#define SCROLLLIST_H

#include <SDL_opengl.h>
#include "Widget.h"
#include "dirent.h"
#include "DisplayEngine.h"

#include <boost/filesystem.hpp>
using namespace boost::filesystem;
#include <boost/random.hpp>
using namespace boost;
//using namespace boost::random;


#include <string>
#include <iostream>
#include <time.h>
using namespace std;



class ScrollList : public Widget
{
    public:
        ScrollList(string* inInfoPointer, int inID);
        virtual ~ScrollList();

        void addListItem(string inText, Surface inImage);
        void addListItem(string inText);

        void onMouseChange(float inX, float inY);
        virtual void display();
    protected:
        //used to provide the creator of this widget
        //information about which selection was clicked
        string* mInfoPointer;

        mt19937 mSomeRand;

};

#endif // SCROLLLIST_H
