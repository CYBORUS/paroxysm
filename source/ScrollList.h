#ifndef SCROLLLIST_H
#define SCROLLLIST_H

#include <SDL_opengl.h>
#include "Widget.h"
#include "DisplayEngine.h"
//#include "TextLayer.h"
#include "TextPic.h"

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

        void setFontSize(int inSize);
        void setUpArrow(Surface inSurface);
        void setDownArrow(Surface inSurface);

        void onMouseChange(int inX, int inY);
        void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void display();

        bool canFocus();

        void preProcessing(float inRange);
    protected:
        void buildScrollList();
        void setSelection();

        void scrollUp();
        void scrollDown();
        //used to provide the creator of this widget
        //information about which item is currently selected
        string* mInfoPointer;

        float mRange;

        float mScrollStart; //to record where the scrolling is currently

        int mSelectedItem;
        int mSelectedItemStartY;

        bool mMipmapping;

        //TextLayer mText;
        //TextPic mText;

        float mWidth;
        float mHeight;
        //float mListWidth;
        float mArrowWidth;
        int mPixelArrowWidth;
        GLuint mArrows[2];

        Surface mUpArrow;
        Surface mDownArrow;

        int mPixelUpArrowBottom; //the bottom of the up arrow
        int mPixelDownArrowTop; //the top of the down arrow

        vector<TextPic*> mText;

        int mFontSize;

        //vector<string> mListText;
        //vector<GLuint> mList;
        //vector<Point2D<float> > mListSizes;
        //vector<Point2D<float> > mSizeRatios;
        vector<GLuint> mImages;
        vector<Point2D<float> > mImageSizes;
        GLuint mNoImage; //used to see when a list item doesn't have an image
        GLuint mScrollList;
        GLuint mSelectionBox;

        //mt19937 mSomeRand;

};

#endif // SCROLLLIST_H
