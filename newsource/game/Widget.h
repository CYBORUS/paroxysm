#ifndef WIDGET_H
#define WIDGET_H

#include "SceneGraphNode.h"
#include <cstring>

class Widget : public SceneGraphNode
{
    public:
        typedef void (*Listener)(void*);

        struct EventPackage
        {
            Listener listener;
            void* data;

            inline void dispatch() { if (listener) listener(data); }

            EventPackage() { memset(this, 0, sizeof(EventPackage)); }
        };

        Widget(bool inCanHaveFocus = true);
        virtual ~Widget();

        virtual void display() = 0;

        inline bool canHaveFocus() { return mEnabled && mCanHaveFocus; }
        inline void enable(bool inEnabled = true)
        {
            mEnabled = inEnabled;
        }

        void onMouseIn();
        void onMouseOut();
        void onMouseDown();
        void onMouseUp();
        void onClick();
        void onFocus();
        void onBlur();

        void setMouseInListener(Listener inListener, void* inData);
        void setMouseOutListener(Listener inListener, void* inData);
        void setMouseDownListener(Listener inListener, void* inData);
        void setMouseUpListener(Listener inListener, void* inData);
        void setClickListener(Listener inListener, void* inData);
        void setFocusListener(Listener inListener, void* inData);
        void setBlurListener(Listener inListener, void* inData);

    protected:

    private:
        bool mEnabled;
        bool mCanHaveFocus;
        EventPackage mOnMouseIn;
        EventPackage mOnMouseOut;
        EventPackage mOnMouseDown;
        EventPackage mOnMouseUp;
        EventPackage mOnClick;
        EventPackage mOnFocus;
        EventPackage mOnBlur;
};

#endif
