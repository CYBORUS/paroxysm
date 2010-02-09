#ifndef SCROLLLIST_H
#define SCROLLLIST_H

#include <SDL_opengl.h>
#include "Widget.h"
#include "dirent.h"
#include "DisplayEngine.h"
#include "TextLayer.h"

#include <boost/filesystem.hpp>
using namespace boost::filesystem;
#include <boost/random.hpp>
using namespace boost;


#include <string>
#include <iostream>
#include <time.h>
#include <vector>
using namespace std;


#define LINE_HEIGHT 20

class ScrollList : public Widget
{
    public:
        ScrollList(string* inInfoPointer, float inWidth, float inHeight, int inID);
        virtual ~ScrollList();

        void addListItem(string inText, Surface inImage);
        void addListItem(string inText);

        void onMouseChange(float inX, float inY);
        virtual void display();
    protected:
        //used to provide the creator of this widget
        //information about which item is currently selected
        string* mInfoPointer;

        float mWidth;
        float mHeight;

        vector<GLuint> mList;
        vector<GLuint> mImages;
        GLuint mNoImage; //used to see when a list item doesn't have an image

        mt19937 mSomeRand;

};

#endif // SCROLLLIST_H
