#include "Widget.h"

Widget::Widget(bool inCanHaveFocus) : mEnabled(true),
    mCanHaveFocus(inCanHaveFocus)
{
}

Widget::~Widget()
{
}

void Widget::onMouseIn()
{
    mOnMouseIn.dispatch();
}

void Widget::onMouseOut()
{
    mOnMouseOut.dispatch();
}

void Widget::onMouseDown()
{
    mOnMouseDown.dispatch();
}

void Widget::onMouseUp()
{
    mOnMouseUp.dispatch();
}

void Widget::onClick()
{
    mOnClick.dispatch();
}

void Widget::onFocus()
{
    mOnFocus.dispatch();
}

void Widget::setMouseInListener(Listener inListener, void* inData)
{
    mOnMouseIn.listener = inListener;
    mOnMouseIn.data = inData;
}

void Widget::setMouseOutListener(Listener inListener, void* inData)
{
    mOnMouseOut.listener = inListener;
    mOnMouseOut.data = inData;
}

void Widget::setMouseDownListener(Listener inListener, void* inData)
{
    mOnMouseDown.listener = inListener;
    mOnMouseDown.data = inData;
}

void Widget::setMouseUpListener(Listener inListener, void* inData)
{
    mOnMouseUp.listener = inListener;
    mOnMouseUp.data = inData;
}

void Widget::setClickListener(Listener inListener, void* inData)
{
    mOnClick.listener = inListener;
    mOnClick.data = inData;
}

void Widget::setFocusListener(Listener inListener, void* inData)
{
    mOnFocus.listener = inListener;
    mOnFocus.data = inData;
}
